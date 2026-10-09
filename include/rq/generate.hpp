#pragma once
#include <rq/entity.hpp>
#include <rq/utility.hpp>
#include <rq/symbols.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <optional>

namespace rq {

struct Statement final {
  using Self = rq::Statement;
};

struct RValue final {
  using Self = rq::RValue;
};

struct Generator final {
  // etc etc

  void
  lookupOuterMemberOf(rq::SymbolTable &host, rq::Expression &ex,
                      llvm::SmallVector<rq::Symbol *> &inout_candidate_ptrs);
  void
  lookupInnerMemberOf(rq::SymbolTable &host, rq::Expression &ex,
                      llvm::SmallVector<rq::Symbol *> &inout_candidate_ptrs);
  void lookupAlias(rq::Alias &alias,
                   llvm::SmallVector<rq::Symbol *> &inout_candidate_ptrs);
  [[nodiscard]] rq::Symbol* lookupPredefinedMacro(rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIdentifyOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateAsOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateOfOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateCastOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateBitwiseCastOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateSignatureCastOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateContentOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateAddressOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateSliceOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateProcedureAddressOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateRefOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateDataAddressOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateMoveOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateTakeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateEmplaceOf(rq::SymbolTable& host, rq::Expression &ex);
  [[nodiscard]] rq::RValue generateInvokeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateComposeOf(rq::SymbolTable &host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateDecomposeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::Statement generateForgetOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateInitOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateInplaceDestroyOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateInplaceInitOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateBakeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::Statement generateIgnoreOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateByteSizeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateBitDepthOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateElementCountOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateSnippetOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateNameOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateLineOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateColumnOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateHoldsOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateHasMemberOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateHasOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateGetOf(rq::SymbolTable& host, rq::Expression &ex);
  [[nodiscard]] rq::RValue generateSignatureOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateSynonymOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateAtOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateMainOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateDestroyOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateUnderlyingValueOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateUnderlyingTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateVariableOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateOverloadOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateOverloadRangeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateSpecializationRangeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateWeightOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateTemplateOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateTemplateRangeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateConstructorRangeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateResolveProcedureOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateResolveAdapterOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsRangeTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsPlacementTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsSignedTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsUnsignedTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsIntegerTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsFloatTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsBinaryTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsBFloatTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsStringTypeOf(rq::SymbolTable& host, rq::Expression& ex);
  [[nodiscard]] rq::RValue generateIsCodeunitTypeOf(rq::SymbolTable& host, rq::Expression& ex);
};

// TODO

} // namespace rq
