#include <rq/context.hpp>
#include <rq/generate.hpp>
#include <rq/utility.hpp>

#include <cstddef>

namespace rq {

void Generator::lookup(rq::SymbolTable &host,
                       llvm::ArrayRef<rq::PathComponent> path,
                       llvm::SmallPtrSetImpl<rq::Entity *> &candidates) {
  RQ_ASSERT(path.size() >= 1, "no names in path");
  rq::Module &module = rq::dereferencePtr(host.getModulePtr());
  const rq::PathComponent &front_component = path.front();
  for (rq::SymbolTable &scope : host.getInclusiveAscendingSubrange()) {
    rq::NodeListRef<rq::TableMember> list =
        scope.lookupList(front_component.getName());
    for (rq::TableMember &member : list) {
      if (!this->tryImplement(member)) {
        continue;
      }
      if (llvm::isa<rq::Route>(member)) {
        rq::Route &route = llvm::cast<rq::Route>(member);
        rq::SymbolicPath &route_symbolic_path =
            rq::dereferencePtr(route.getSymbolicPathPtr());
        rq::SymbolTable &route_start =
            rq::dereferencePtr(route.getDerivedRouteStartPtr());
        llvm::SmallPtrSet<rq::Route *, 1> found_redirects{&route};
        this->dfsSymbolicPath(module, route_start, path.drop_front(1),
                              found_redirects, {}, route, route_symbolic_path,
                              candidates);
      } else if (path.size() > 1) {
        this->dfsPath(module, member, path.drop_front(1), candidates);
      } else {
        candidates.insert(&member);
      }
    }
    if (!candidates.empty()) {
      return;
    }
  }
  if (path.size() == 1) {
    rq::Entity *builtin_ptr = this->tryLookupBuiltin(path.front().getName());
    if (builtin_ptr == nullptr) {
      return;
    }
    candidates.insert(builtin_ptr);
  }
}

void Generator::dfsPath(rq::Module &module, rq::TableMember &receiver,
                        llvm::ArrayRef<rq::PathComponent> path,
                        llvm::SmallPtrSetImpl<rq::Entity *> &candidates) {
  RQ_ASSERT(path.size() >= 1, "no names in path");
  if (!llvm::isa<rq::SymbolTable>(receiver)) {
    return;
  }
  rq::SymbolTable &table = llvm::cast<rq::SymbolTable>(receiver);
  const rq::PathComponent &front_component = path.front();
  rq::NodeListRef<rq::TableMember> list =
      table.lookupList(front_component.getName());
  for (rq::TableMember &member : list) {
    rq::Module &member_module = rq::dereferencePtr(member.getModulePtr());
    rq::ModifierFuseFlags member_flags = member.getModifierFuseFlags();
    if (member_module != module) {
      if (rq::getHasNone(member_flags, rq::ModifierFuseFlags::EXPORT)) {
        continue;
      }
    }
    if (!this->tryImplement(member)) {
      continue;
    }
    if (llvm::isa<rq::Route>(member)) {
      rq::Route &route = llvm::cast<rq::Route>(member);
      rq::SymbolicPath &route_symbolic_path =
          rq::dereferencePtr(route.getSymbolicPathPtr());
      rq::SymbolTable &route_start =
          rq::dereferencePtr(route.getDerivedRouteStartPtr());
      llvm::SmallPtrSet<rq::Route *, 1> found_redirects{&route};
      this->dfsSymbolicPath(module, route_start, path.drop_front(1),
                            found_redirects, {}, route, route_symbolic_path,
                            candidates);
    } else if (path.size() > 1) {
      this->dfsPath(module, member, path.drop_front(1), candidates);
    } else {
      candidates.insert(&member);
    }
  }
}

void Generator::dfsSymbolicPath(
    rq::Module &module, rq::SymbolTable &host,
    llvm::ArrayRef<rq::PathComponent> path,
    llvm::SmallPtrSetImpl<rq::Route *> &found_redirects,
    llvm::ArrayRef<rq::SymbolicPathSnapshot> snapshot_stack,
    rq::Route &redirect, rq::SymbolicPath &symbolic_path,
    llvm::SmallPtrSetImpl<rq::Entity *> &candidates) {
  rq::NodeListRef<rq::TableMember> list =
      host.lookupList(symbolic_path.getName());
  for (rq::TableMember &member : list) {
    rq::Module &member_module = rq::dereferencePtr(member.getModulePtr());
    rq::ModifierFuseFlags member_flags = member.getModifierFuseFlags();
    if (member_module != module) {
      if (rq::getHasNone(member_flags, rq::ModifierFuseFlags::EXPORT)) {
        continue;
      }
    }
    if (!this->tryImplement(member)) {
      continue;
    }
    if (llvm::isa<rq::Route>(member)) {
      rq::Route &route = llvm::cast<rq::Route>(member);
      if (found_redirects.contains(&route)) {
        continue;
      }
      found_redirects.insert(&route);
      llvm::SmallVector<rq::SymbolicPathSnapshot, 1> new_snapshot_stack{};
      new_snapshot_stack.reserve(snapshot_stack.size() + 1);
      new_snapshot_stack.append(snapshot_stack.begin(), snapshot_stack.end());
      new_snapshot_stack.emplace_back(redirect, symbolic_path);
      rq::SymbolicPath &route_symbolic_path =
          rq::dereferencePtr(route.getSymbolicPathPtr());
      rq::SymbolTable &route_start =
          rq::dereferencePtr(route.getDerivedRouteStartPtr());
      llvm::SmallPtrSet<rq::Route *, 1> found_redirects{&route};
      this->dfsSymbolicPath(module, route_start, path.drop_front(1),
                            found_redirects, new_snapshot_stack, route,
                            route_symbolic_path, candidates);
      continue;
    }
    if (symbolic_path.getNextPtr() == nullptr) {
      if (snapshot_stack.empty()) {
        if (path.empty()) {
          candidates.insert(&member);
          continue;
        }
      } else {
        rq::SymbolicPathSnapshot snapshot = snapshot_stack.back();
        this->dfsSymbolicPath(module, snapshot.getHost(), path, found_redirects,
                              snapshot_stack.drop_back(1), snapshot.getRoute(),
                              snapshot.getSymbolicPath(), candidates);
        continue;
      }
    }
    if (llvm::isa<rq::SymbolTable>(member)) {
      rq::SymbolTable &member_table = llvm::cast<rq::SymbolTable>(member);
      if (symbolic_path.getNextPtr() != nullptr) {
        rq::SymbolicPath &next_symbolic_path =
            rq::dereferencePtr(symbolic_path.getNextPtr());
        this->dfsSymbolicPath(module, member_table, path, found_redirects,
                              snapshot_stack, redirect, next_symbolic_path,
                              candidates);
      } else {
        this->dfsPath(module, member_table, path, candidates);
      }
    }
  }
}

[[nodiscard]] bool Generator::tryImplement(rq::TableMember &member) {
  if (llvm::isa<rq::LazyDeclaration>(member)) {
    rq::LazyDeclaration &lazy = llvm::cast<rq::LazyDeclaration>(member);
    if (lazy.getState() < rq::LazyState::IMPLEMENTED) {
      RQ_TODO_IMPLEMENTATION();
    }
  }
  return true;
}

[[nodiscard]] rq::Entity *Generator::tryLookupBuiltin(rq::Name name) {
  using K = rq::Keyword;
  const K keyword = this->getContext().getBuiltinKeyword(name);
  if (keyword == K::NONE) {
    const llvm::StringRef text = name.getText();
    const std::size_t digit_i = text.find_first_of("0123456789", 5);
    if (digit_i == llvm::StringRef::npos) {
      return nullptr;
    }
    const llvm::StringRef depth_text = text.drop_front(digit_i);
    unsigned depth = 0;
    if (depth_text.getAsInteger(10, depth) == true) {
      return nullptr;
    }
    if (depth == 0 || depth > rq::MAX_SCALED_PRIMITIVE_DEPTH) {
      return nullptr;
    }
    const llvm::StringRef prefix = text.take_front(digit_i);
    if (prefix == "SInt") {
      return &this->getContext().acquireScaledSIntType(depth);
    }
    if (prefix == "UInt") {
      return &this->getContext().acquireScaledUIntType(depth);
    }
    if (prefix == "FSInt") {
      return &this->getContext().acquireScaledFSIntType(depth);
    }
    if (prefix == "FUInt") {
      return &this->getContext().acquireScaledFUIntType(depth);
    }
    if (prefix == "LSInt") {
      return &this->getContext().acquireScaledLSIntType(depth);
    }
    if (prefix == "LUInt") {
      return &this->getContext().acquireScaledLUIntType(depth);
    }
    return nullptr;
  }
  switch (keyword) {
  case K::ARRAY_SUBSYMBOL:
    return &this->getContext().acquireArray();
  case K::REF_SUBSYMBOL:
    return &this->getContext().acquireRef();
  case K::PTR_SUBSYMBOL:
    return &this->getContext().acquirePtr();
  case K::SLICE_SUBSYMBOL:
    return &this->getContext().acquireSlice();
  case K::SPLIT_SUBSYMBOL:
    return &this->getContext().acquireSplit();
  case K::GENERIC_SUBSYMBOL:
    return &this->getContext().acquireGeneric();
  case K::INTEGER_LITERAL:
    return &this->getContext().acquireIntegerLiteralType();
  case K::FLOAT_LITERAL:
    return &this->getContext().acquireFloatLiteralType();
  case K::STRING_LITERAL:
    return &this->getContext().acquireStringLiteralType();
  case K::CODEUNIT_LITERAL:
    return &this->getContext().acquireCodeunitLiteralType();
  case K::INFERENCE:
    return &this->getContext().acquireInferenceType();
  case K::VOID:
    return &this->getContext().acquireVoidType();
  case K::NO_RETURN:
    return &this->getContext().acquireNoReturnType();
  case K::EXPRESSION:
    return &this->getContext().acquireExpressionType();
  case K::FSINT:
    return &this->getContext().acquireFittingFSIntType();
  case K::FUINT:
    return &this->getContext().acquireFittingFUIntType();
  case K::LSINT:
    return &this->getContext().acquireFittingLSIntType();
  case K::LUINT:
    return &this->getContext().acquireFittingLUIntType();
  case K::BOOL:
    return &this->getContext().acquireBooleanType();
  case K::HALF:
    return &this->getContext().acquireHalfType();
  case K::SINGLE:
    return &this->getContext().acquireSingleType();
  case K::DOUBLE:
    return &this->getContext().acquireDoubleType();
  case K::QUADRUPLE:
    return &this->getContext().acquireQuadrupleType();
  case K::SINT:
    return &this->getContext().acquireSIntType();
  case K::UINT:
    return &this->getContext().acquireUIntType();
  case K::SSIZE:
    return &this->getContext().acquireSSizeType();
  case K::USIZE:
    return &this->getContext().acquireUSizeType();
  case K::SINDEX:
    return &this->getContext().acquireSIndexType();
  case K::UINDEX:
    return &this->getContext().acquireUIndexType();
  case K::CHAR:
    return &this->getContext().acquireCharType();
  case K::BINARY16:
    return &this->getContext().acquireBinary16Type();
  case K::BINARY32:
    return &this->getContext().acquireBinary32Type();
  case K::BINARY64:
    return &this->getContext().acquireBinary64Type();
  case K::BINARY128:
    return &this->getContext().acquireBinary128Type();
  case K::BFLOAT16:
    return &this->getContext().acquireBFloat16Type();
  case K::ASCII:
    return &this->getContext().acquireAsciiType();
  case K::UTF8:
    return &this->getContext().acquireUtf8Type();
  case K::NO_MODIFIER:
    return &this->getContext().acquireNoModifier();
  case K::ANCHOR:
    return &this->getContext().acquireAnchorModifier();
  case K::INITS:
    return &this->getContext().acquireInitsModifier();
  case K::RESIDENT:
    return &this->getContext().acquireResidentModifier();
  case K::FLANK:
    return &this->getContext().acquireFlankModifier();
  case K::TRANSPARENT:
    return &this->getContext().acquireTransparentModifier();
  case K::OPAQUE:
    return &this->getContext().acquireOpaqueModifier();
  case K::PRIVATE:
    return &this->getContext().acquirePrivateModifier();
  case K::PUBLIC:
    return &this->getContext().acquirePublicModifier();
  case K::EXPORT:
    return &this->getContext().acquireExportModifier();
  case K::MUTABLE:
    return &this->getContext().acquireMutableModifier();
  case K::PARTIALLY_MUTABLE:
    return &this->getContext().acquirePartiallyMutableModifier();
  case K::CONSTANT:
    return &this->getContext().acquireConstantModifier();
  case K::EAGER:
    return &this->getContext().acquireEagerModifier();
  case K::LAZY:
    return &this->getContext().acquireLazyModifier();
  case K::POST:
    return &this->getContext().acquirePostModifier();
  case K::PRE:
    return &this->getContext().acquirePreModifier();
  case K::ALIAS:
    return &this->getContext().acquireAliasModifier();
  case K::ENUMERATOR:
    return &this->getContext().acquireEnumeratorModifier();
  case K::CONCEPT:
    return &this->getContext().acquireConceptModifier();
  case K::SPLIT:
    return &this->getContext().acquireSplitModifier();
  case K::RUNTIME:
    return &this->getContext().acquireRuntimeModifier();
  case K::COMTIME:
    return &this->getContext().acquireComtimeModifier();
  case K::ANYTIME:
    return &this->getContext().acquireAnytimeModifier();
  case K::PRESET:
    return &this->getContext().acquirePresetModifier();
  case K::SINGLETON:
    return &this->getContext().acquireSingletonModifier();
  case K::CAPTURE:
    return &this->getContext().acquireCaptureModifier();
  case K::STATELESS:
    return &this->getContext().acquireStatelessModifier();
  case K::LINKED:
    return &this->getContext().acquireLinkedModifier();
  case K::INLINE:
    return &this->getContext().acquireInlineModifier();
  case K::PURE:
    return &this->getContext().acquirePureModifier();
  case K::IMPURE:
    return &this->getContext().acquireImpureModifier();
  case K::STANDARD_MANGLE:
    return &this->getContext().acquireStandardMangleModifier();
  case K::MANGLE:
    return &this->getContext().acquireMangleModifier();
  case K::PAD:
    return &this->getContext().acquirePadModifier();
  case K::PACK:
    return &this->getContext().acquirePackModifier();
  case K::EQUIVOCAL:
    return &this->getContext().acquireEquivocalModifier();
  case K::LIKELY:
    return &this->getContext().acquireLikelyModifier();
  case K::UNLIKELY:
    return &this->getContext().acquireUnlikelyModifier();
  case K::SUPPORTED:
    return &this->getContext().acquireSupportedModifier();
  case K::DEPRECIATED:
    return &this->getContext().acquireDepreciatedModifier();
  case K::EXPERIMENTAL:
    return &this->getContext().acquireExperimentalModifier();
  case K::MOVEABLE:
    return &this->getContext().acquireMovableModifier();
  case K::COPYABLE:
    return &this->getContext().acquireCopyableModifier();
  case K::INVARIADIC:
    return &this->getContext().acquireInvariadicModifier();
  case K::VARIADIC:
    return &this->getContext().acquireVariadicModifier();
  case K::POLYADIC:
    return &this->getContext().acquirePolyadicModifier();
  case K::BEST_LOCATION:
    return &this->getContext().acquireBestLocationModifier();
  case K::LOCATION:
    return &this->getContext().acquireLocationModifier();
  case K::TEMPLATE:
    return &this->getContext().acquireTemplateModifier();
  case K::OVERLOAD:
    return &this->getContext().acquireOverloadModifier();
  case K::CONSTRAINT:
    return &this->getContext().acquireConstraintModifier();
  case K::MANUAL:
    return &this->getContext().acquireManualModifier();
  case K::AUTO:
    return &this->getContext().acquireAutoModifier();
  case K::DIRECT:
    return &this->getContext().acquireDirectModifier();
  case K::VIRTUAL:
    return &this->getContext().acquireVirtualModifier();
  case K::RANGER:
    return &this->getContext().acquireRangerModifier();
  case K::REQUIRE:
    return &this->getContext().acquireRequireModifier();
  case K::ENSURE:
    return &this->getContext().acquireEnsureModifier();
  case K::NO_MUT:
    return &this->getContext().acquireNoMutQualifier();
  case K::PARTIAL_MUT:
    return &this->getContext().acquirePartialMutQualifier();
  case K::MUT:
    return &this->getContext().acquireMutQualifier();
  case K::NO_VOLATILE:
    return &this->getContext().acquireNoVolatileQualifier();
  case K::VOLATILE:
    return &this->getContext().acquireVolatileQualifier();
  case K::NO_ATOMIC:
    return &this->getContext().acquireNoAtomicQualifier();
  case K::ATOMIC:
    return &this->getContext().acquireAtomicQualifier();
  case K::NO_NULL_TERMINATE:
    return &this->getContext().acquireNoNullTerminateQualifier();
  case K::NULL_TERMINATE:
    return &this->getContext().acquireNullTerminateQualifier();
  case K::NO_MARGIN:
    return &this->getContext().acquireNoMarginQualifier();
  case K::MARGIN:
    return &this->getContext().acquireMarginQualifier();
  case K::NO_DETACH:
    return &this->getContext().acquireNoDetachQualifier();
  case K::DETACH:
    return &this->getContext().acquireDetachQualifier();
  case K::NO_THROWS:
    return &this->getContext().acquireNoThrowsQualifier();
  case K::THROWS:
    return &this->getContext().acquireThrowsQualifier();
  case K::ANY_CONCEPT:
    return &this->getContext().acquireAnyConcept();
  case K::TYPE_CONCEPT:
    return &this->getContext().acquireTypeConcept();
  case K::COMTIME_CONCEPT:
    return &this->getContext().acquireComtimeConcept();
  case K::RUNTIME_CONCEPT:
    return &this->getContext().acquireRuntimeConcept();
  case K::ANYTIME_CONCEPT:
    return &this->getContext().acquireAnytimeConcept();
  case K::RANGE_CONCEPT:
    return &this->getContext().acquireRangeConcept();
  case K::ROUTE_CONCEPT:
    return &this->getContext().acquireRouteConcept();
  case K::FIGURATIVE_CONCEPT:
    return &this->getContext().acquireFigurativeConcept();
  case K::REFLECTION_CONCEPT:
    return &this->getContext().acquireReflectionConcept();
  case K::PRIMITIVE_CONCEPT:
    return &this->getContext().acquirePrimitiveConcept();
  case K::FITTING_PRIMITIVE_CONCEPT:
    return &this->getContext().acquireFittingPrimitiveConcept();
  case K::PLATFORM_PRIMITIVE_CONCEPT:
    return &this->getContext().acquirePlatformPrimitiveConcept();
  case K::STANDARD_PRIMITIVE_CONCEPT:
    return &this->getContext().acquireStandardPrimitiveConcept();
  case K::SCALED_PRIMITIVE_CONCEPT:
    return &this->getContext().acquireScaledPrimitiveConcept();
  case K::SIGNED_CONCEPT:
    return &this->getContext().acquireSignedConcept();
  case K::UNSIGNED_CONCEPT:
    return &this->getContext().acquireUnsignedConcept();
  case K::INT_CONCEPT:
    return &this->getContext().acquireIntConcept();
  case K::EXACT_INT_CONCEPT:
    return &this->getContext().acquireExactIntConcept();
  case K::FAST_INT_CONCEPT:
    return &this->getContext().acquireFastIntConcept();
  case K::LEAST_INT_CONCEPT:
    return &this->getContext().acquireLeastIntConcept();
  case K::FLOAT_CONCEPT:
    return &this->getContext().acquireFloatConcept();
  case K::BINARY_CONCEPT:
    return &this->getContext().acquireBinaryConcept();
  case K::BFLOAT_CONCEPT:
    return &this->getContext().acquireBFloatConcept();
  case K::CONCEPT_CONCEPT:
    return &this->getContext().acquireConceptConcept();
  case K::SUBSYMBOL_CONCEPT:
    return &this->getContext().acquireSubsymbolConcept();
  case K::ARRAY_CONCEPT:
    return &this->getContext().acquireArrayConcept();
  case K::REF_CONCEPT:
    return &this->getContext().acquireRefConcept();
  case K::PTR_CONCEPT:
    return &this->getContext().acquirePtrConcept();
  case K::SLICE_CONCEPT:
    return &this->getContext().acquireSliceConcept();
  case K::SPLIT_CONCEPT:
    return &this->getContext().acquireSplitConcept();
  case K::MODULE_CONCEPT:
    return &this->getContext().acquireModuleConcept();
  case K::IMPORT_CONCEPT:
    return &this->getContext().acquireImportConcept();
  case K::PORTAL_CONCEPT:
    return &this->getContext().acquirePortalConcept();
  case K::REALIZATION_CONCEPT:
    return &this->getContext().acquireRealizationConcept();
  case K::CONFORMITY_CONCEPT:
    return &this->getContext().acquireConformityConcept();
  case K::ADAPTION_CONCEPT:
    return &this->getContext().acquireAdaptionConcept();
  case K::TRIP_CONCEPT:
    return &this->getContext().acquireTripConcept();
  case K::JUXT_CONCEPT:
    return &this->getContext().acquireJuxtConcept();
  case K::SPECIALIZATION_SET_CONCEPT:
    return &this->getContext().acquireSpecializationSetConcept();
  case K::ADAPTER_SPECIALIZATION_SET_CONCEPT:
    return &this->getContext().acquireAdapterSpecializationSetConcept();
  case K::PROC_SPECIALIZATION_SET_CONCEPT:
    return &this->getContext().acquireProcSpecializationSetConcept();
  case K::ARITHMETIC_SEQUENCE_CONCEPT:
    return &this->getContext().acquireArithmeticSequenceConcept();
  case K::ARITHMETIC_INTERVAL_CONCEPT:
    return &this->getContext().acquireArithmeticIntervalConcept();
  case K::INFINITE_ARITHMETIC_SEQUENCE_CONCEPT:
    return &this->getContext().acquireInfiniteArithmeticSequenceConcept();
  case K::FINITE_ARITHMETIC_SEQUENCE_CONCEPT:
    return &this->getContext().acquireFiniteArithmeticSequenceConcept();
  case K::EAGER_DECLARATION_CONCEPT:
    return &this->getContext().acquireEagerDeclarationConcept();
  case K::ANCHOR_CONCEPT:
    return &this->getContext().acquireAnchorConcept();
  case K::ENUMERATOR_CONCEPT:
    return &this->getContext().acquireEnumeratorConcept();
  case K::VARIABLE_CONCEPT:
    return &this->getContext().acquireVariableConcept();
  case K::EAGER_VARIABLE_CONCEPT:
    return &this->getContext().acquireEagerVariableConcept();
  case K::POST_EAGER_VARIABLE_CONCEPT:
    return &this->getContext().acquirePostEagerVariableConcept();
  case K::PRE_EAGER_VARIABLE_CONCEPT:
    return &this->getContext().acquirePreEagerVariableConcept();
  case K::PARAMETER_CONCEPT:
    return &this->getContext().acquireParameterConcept();
  case K::SIGNATURE_CONCEPT:
    return &this->getContext().acquireSignatureConcept();
  case K::LAYOUT_CONCEPT:
    return &this->getContext().acquireLayoutConcept();
  case K::PLACEMENT_CONCEPT:
    return &this->getContext().acquirePlacementConcept();
  case K::COMPOSITION_CONCEPT:
    return &this->getContext().acquireCompositionConcept();
  case K::SYNONYM_CONCEPT:
    return &this->getContext().acquireSynonymConcept();
  case K::SYMBOL_TABLE_CONCEPT:
    return &this->getContext().acquireSymbolTableConcept();
  case K::NAMESPACE_CONCEPT:
    return &this->getContext().acquireNamespaceConcept();
  case K::EAGER_SCOPING_STATEMENT_CONCEPT:
    return &this->getContext().acquireEagerScopingStatementConcept();
  case K::IF_STATEMENT_CONCEPT:
    return &this->getContext().acquireIfStatementConcept();
  case K::ELSE_IF_STATEMENT_CONCEPT:
    return &this->getContext().acquireElseIfStatementConcept();
  case K::ELSE_STATEMENT_CONCEPT:
    return &this->getContext().acquireElseStatementConcept();
  case K::SWITCH_STATEMENT_CONCEPT:
    return &this->getContext().acquireSwitchStatementConcept();
  case K::CASE_STATEMENT_CONCEPT:
    return &this->getContext().acquireCaseStatementConcept();
  case K::FOR_STATEMENT_CONCEPT:
    return &this->getContext().acquireForStatementConcept();
  case K::WHILE_STATEMENT_CONCEPT:
    return &this->getContext().acquireWhileStatementConcept();
  case K::SCOPE_STATEMENT_CONCEPT:
    return &this->getContext().acquireScopeStatementConcept();
  case K::IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireImplementationConcept();
  case K::OVERLOAD_CONCEPT:
    return &this->getContext().acquireOverloadConcept();
  case K::SPECIALIZATION_CONCEPT:
    return &this->getContext().acquireSpecializationConcept();
  case K::TEMPLATE_CONCEPT:
    return &this->getContext().acquireTemplateConcept();
  case K::CLASS_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireClassImplementationConcept();
  case K::CLASS_OVERLOAD_CONCEPT:
    return &this->getContext().acquireClassOverloadConcept();
  case K::CLASS_SPECIALIZATION_CONCEPT:
    return &this->getContext().acquireClassSpecializationConcept();
  case K::CLASS_TEMPLATE_CONCEPT:
    return &this->getContext().acquireClassTemplateConcept();
  case K::ENUM_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireEnumImplementationConcept();
  case K::ENUM_OVERLOAD_CONCEPT:
    return &this->getContext().acquireEnumOverloadConcept();
  case K::INTERFACE_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireInterfaceImplementationConcept();
  case K::INTERFACE_OVERLOAD_CONCEPT:
    return &this->getContext().acquireInterfaceOverloadConcept();
  case K::INTERFACE_SPECIALIZATION_CONCEPT:
    return &this->getContext().acquireInterfaceSpecializationConcept();
  case K::ADAPTER_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireAdapterImplementationConcept();
  case K::ADAPTER_OVERLOAD_CONCEPT:
    return &this->getContext().acquireAdapterOverloadConcept();
  case K::ADAPTER_SPECIALIZATION_CONCEPT:
    return &this->getContext().acquireAdapterSpecializationConcept();
  case K::ADAPTER_TEMPLATE_CONCEPT:
    return &this->getContext().acquireAdapterTemplateConcept();
  case K::PROC_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireProcImplementationConcept();
  case K::PROC_OVERLOAD_CONCEPT:
    return &this->getContext().acquireProcOverloadConcept();
  case K::PROC_SPECIALIZATION_CONCEPT:
    return &this->getContext().acquireProcSpecializationConcept();
  case K::PROC_TEMPLATE_CONCEPT:
    return &this->getContext().acquireProcTemplateConcept();
  case K::LAZY_VARIABLE_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireLazyVariableImplementationConcept();
  case K::LAZY_VARIABLE_SPECIALIZATION_CONCEPT:
    return &this->getContext().acquireLazyVariableSpecializationConcept();
  case K::LAZY_VARIABLE_TEMPLATE_CONCEPT:
    return &this->getContext().acquireLazyVariableTemplateConcept();
  case K::ALIAS_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireAliasImplementationConcept();
  case K::ALIAS_OVERLOAD_CONCEPT:
    return &this->getContext().acquireAliasOverloadConcept();
  case K::ALIAS_SPECIALIZATION_CONCEPT:
    return &this->getContext().acquireAliasSpecializationConcept();
  case K::ALIAS_TEMPLATE_CONCEPT:
    return &this->getContext().acquireAliasTemplateConcept();
  case K::CONCEPT_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireConceptImplementationConcept();
  case K::VARIANT_IMPLEMENTATION_CONCEPT:
    return &this->getContext().acquireVariantImplementationConcept();
  case K::VARIANT_OVERLOAD_CONCEPT:
    return &this->getContext().acquireVariantOverloadConcept();
  case K::VARIANT_SPECIALIZATION_CONCEPT:
    return &this->getContext().acquireVariantSpecializationConcept();
  case K::VARIANT_TEMPLATE_CONCEPT:
    return &this->getContext().acquireVariantTemplateConcept();
  case K::ATTRIBUTE_CONCEPT:
    return &this->getContext().acquireAttributeConcept();
  case K::MODIFIER_CONCEPT:
    return &this->getContext().acquireModifierConcept();
  case K::ANCHOR_MODIFIER_CONCEPT:
    return &this->getContext().acquireAnchorModifierConcept();
  case K::CONTAINER_MODIFIER_CONCEPT:
    return &this->getContext().acquireContainerModifierConcept();
  case K::VISIBILITY_MODIFIER_CONCEPT:
    return &this->getContext().acquireVisibilityModifierConcept();
  case K::ACCESS_MODIFIER_CONCEPT:
    return &this->getContext().acquireAccessModifierConcept();
  case K::MUTABILITY_MODIFIER_CONCEPT:
    return &this->getContext().acquireMutabilityModifierConcept();
  case K::EVALUATION_TIME_MODIFIER_CONCEPT:
    return &this->getContext().acquireEvaluationTimeModifierConcept();
  case K::DIALECT_MODIFIER_CONCEPT:
    return &this->getContext().acquireDialectModifierConcept();
  case K::TENURE_MODIFIER_CONCEPT:
    return &this->getContext().acquireTenureModifierConcept();
  case K::INITIALIZATION_TIME_MODIFIER_CONCEPT:
    return &this->getContext().acquireInitializationTimeModifierConcept();
  case K::CLOSURE_MODIFIER_CONCEPT:
    return &this->getContext().acquireClosureModifierConcept();
  case K::LINKAGE_MODIFIER_CONCEPT:
    return &this->getContext().acquireLinkageModifierConcept();
  case K::SIDE_EFFECTS_MODIFIER_CONCEPT:
    return &this->getContext().acquireSideEffectsModifierConcept();
  case K::MANGLE_MODIFIER_CONCEPT:
    return &this->getContext().acquireMangleModifierConcept();
  case K::PACK_MODIFIER_CONCEPT:
    return &this->getContext().acquirePackModifierConcept();
  case K::BRANCH_TREND_MODIFIER_CONCEPT:
    return &this->getContext().acquireBranchTrendModifierConcept();
  case K::SUPPORT_NOTICE_MODIFIER_CONCEPT:
    return &this->getContext().acquireSupportNoticeModifierConcept();
  case K::MOVABLE_MODIFIER_CONCEPT:
    return &this->getContext().acquireMovableModifierConcept();
  case K::COPYABLE_MODIFIER_CONCEPT:
    return &this->getContext().acquireCopyableModifierConcept();
  case K::VARIADIC_MODIFIER_CONCEPT:
    return &this->getContext().acquireVariadicModifierConcept();
  case K::POLYADIC_MODIFIER_CONCEPT:
    return &this->getContext().acquirePolyadicModifierConcept();
  case K::OFFSET_MODIFIER_CONCEPT:
    return &this->getContext().acquireOffsetModifierConcept();
  case K::LAZY_DECLARATION_MODIFIER_CONCEPT:
    return &this->getContext().acquireLazyDeclarationModifierConcept();
  case K::CONSTRAINT_MODIFIER_CONCEPT:
    return &this->getContext().acquireConstraintModifierConcept();
  case K::DEDUCTION_MODIFIER_CONCEPT:
    return &this->getContext().acquireDeductionModifierConcept();
  case K::VIRTUALITY_MODIFIER_CONCEPT:
    return &this->getContext().acquireVirtualityModifierConcept();
  case K::RANGER_MODIFIER_CONCEPT:
    return &this->getContext().acquireRangerModifierConcept();
  case K::REQUIRE_MODIFIER_CONCEPT:
    return &this->getContext().acquireRequireModifierConcept();
  case K::ENSURE_MODIFIER_CONCEPT:
    return &this->getContext().acquireEnsureModifierConcept();
  case K::QUALIFIER_CONCEPT:
    return &this->getContext().acquireQualifierConcept();
  case K::MUT_QUALIFIER_CONCEPT:
    return &this->getContext().acquireMutQualifierConcept();
  case K::VOLATILE_QUALIFIER_CONCEPT:
    return &this->getContext().acquireVolatileQualifierConcept();
  case K::ATOMIC_QUALIFIER_CONCEPT:
    return &this->getContext().acquireAtomicQualifierConcept();
  case K::NULL_TERMINATE_QUALIFIER_CONCEPT:
    return &this->getContext().acquireNullTerminateQualifierConcept();
  case K::DETACH_QUALIFIER_CONCEPT:
    return &this->getContext().acquireDetachQualifierConcept();
  case K::MARGIN_QUALIFIER_CONCEPT:
    return &this->getContext().acquireMarginQualifierConcept();
  case K::THROWS_QUALIFIER_CONCEPT:
    return &this->getContext().acquireThrowsQualifierConcept();
  default:
    break;
  }
  return nullptr;
}

} // namespace rq
