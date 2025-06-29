// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

constexpr bool getHasDepth(requite::RootSymbolType type) {
  switch (type) {
  case requite::RootSymbolType::NONE:
    return false;
  case requite::RootSymbolType::INFERENCE:
    return false;
  case requite::RootSymbolType::VOID:
    return false;
  case requite::RootSymbolType::BOOLEAN:
    return false;
  case requite::RootSymbolType::WORD:
    return true;
  case requite::RootSymbolType::SIGNED_INTEGER:
    return true;
  case requite::RootSymbolType::UNSIGNED_INTEGER:
    return true;
  case requite::RootSymbolType::BINARY_HALF_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_SINGLE_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_DOUBLE_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_QUAD_FLOAT:
    return false;
  case requite::RootSymbolType::C_CHAR:
    return false;
  case requite::RootSymbolType::UTF8:
    return false;
  case requite::RootSymbolType::VARIADIC_ARGUMENTS:
    return false;
  case requite::RootSymbolType::NULL_:
    return false;
  case requite::RootSymbolType::SCOPE:
    return false;
  case requite::RootSymbolType::OBJECT:
    return false;
  case requite::RootSymbolType::TABLE:
    return false;
  case requite::RootSymbolType::ALIAS:
    return false;
  case requite::RootSymbolType::ORDERED_VARIABLE:
    return false;
  case requite::RootSymbolType::UNORDERED_VARIABLE:
    return false;
  case requite::RootSymbolType::SIGNATURE:
    return false;
  case requite::RootSymbolType::TUPLE:
    return false;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    return false;
  case requite::RootSymbolType::PROCEDURE:
    return false;
  case requite::RootSymbolType::NAMED_PROCEDURE_GROUP:
    return false;
  case requite::RootSymbolType::MODULE:
    return false;
  case requite::RootSymbolType::LABEL:
    return false;
  }
  REQUITE_UNREACHABLE();
}

constexpr bool getHasUserAttributeFlags(requite::RootSymbolType type) {
  switch (type) {
  case requite::RootSymbolType::NONE:
    return false;
  case requite::RootSymbolType::INFERENCE:
    return false;
  case requite::RootSymbolType::VOID:
    return false;
  case requite::RootSymbolType::BOOLEAN:
    return false;
  case requite::RootSymbolType::WORD:
    return false;
  case requite::RootSymbolType::SIGNED_INTEGER:
    return false;
  case requite::RootSymbolType::UNSIGNED_INTEGER:
    return false;
  case requite::RootSymbolType::BINARY_HALF_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_SINGLE_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_DOUBLE_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_QUAD_FLOAT:
    return false;
  case requite::RootSymbolType::C_CHAR:
    return false;
  case requite::RootSymbolType::UTF8:
    return false;
  case requite::RootSymbolType::VARIADIC_ARGUMENTS:
    return false;
  case requite::RootSymbolType::NULL_:
    return false;
  case requite::RootSymbolType::SCOPE:
    return false;
  case requite::RootSymbolType::OBJECT:
    return true;
  case requite::RootSymbolType::TABLE:
    return false;
  case requite::RootSymbolType::ALIAS:
    return true;
  case requite::RootSymbolType::ORDERED_VARIABLE:
    return false;
  case requite::RootSymbolType::UNORDERED_VARIABLE:
    return true;
  case requite::RootSymbolType::SIGNATURE:
    return false;
  case requite::RootSymbolType::TUPLE:
    return false;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    return false;
  case requite::RootSymbolType::PROCEDURE:
    return true;
  case requite::RootSymbolType::NAMED_PROCEDURE_GROUP:
    return false;
  case requite::RootSymbolType::MODULE:
    return false;
  case requite::RootSymbolType::LABEL:
    return false;
  }
  REQUITE_UNREACHABLE();
}

constexpr bool getHasCount(requite::SubSymbolType type) {
  switch (type) {
  case requite::SubSymbolType::NONE:
    return false;
  case requite::SubSymbolType::ARRAY:
    return true;
  case requite::SubSymbolType::REFERENCE:
    return false;
  case requite::SubSymbolType::STOLEN_REFERENCE:
    return false;
  case requite::SubSymbolType::FAT_POINTER:
    return false;
  case requite::SubSymbolType::POINTER:
    return false;
  }
  return false;
}

template <unsigned BUFFER_SIZE_PARAM>
llvm::StringRef
Symbol::getName(llvm::SmallString<BUFFER_SIZE_PARAM> &buffer) const {
  llvm::raw_svector_ostream ostream(buffer);
  for (const requite::SubSymbol &sub : this->getSubs()) {
    requite::AttributeFlags attributes = sub.getAttributeFlags();
    REQUITE_ASSERT(!attributes.getHasAttribute(requite::AttributeType::USER));
    if (attributes.getHasAttribute(requite::AttributeType::MUTABLE)) {
      ostream << "mutable ";
    }
    if (attributes.getHasAttribute(requite::AttributeType::CONSTANT)) {
      ostream << "constant ";
    }
    if (attributes.getHasAttribute(requite::AttributeType::VOLATILE)) {
      ostream << "volatile ";
    }
    if (attributes.getHasAttribute(requite::AttributeType::ATOMIC)) {
      ostream << "atomic ";
    }
    if (attributes.getHasAttribute(requite::AttributeType::NULL_TERMINATED)) {
      ostream << "null-terminated ";
    }
    REQUITE_ASSERT(
        !attributes.getHasAttribute(requite::AttributeType::TEMPLATE));
    REQUITE_ASSERT(
        !attributes.getHasAttribute(requite::AttributeType::PRIVATE));
    REQUITE_ASSERT(
        !attributes.getHasAttribute(requite::AttributeType::PROTECTED));
    REQUITE_ASSERT(!attributes.getHasAttribute(requite::AttributeType::EXPORT));
    REQUITE_ASSERT(
        !attributes.getHasAttribute(requite::AttributeType::EXTERNAL));
    REQUITE_ASSERT(
        !attributes.getHasAttribute(requite::AttributeType::NOT_FINAL));
    REQUITE_ASSERT(
        !attributes.getHasAttribute(requite::AttributeType::MAY_DISCARD));
    REQUITE_ASSERT(!attributes.getHasAttribute(requite::AttributeType::INLINE));
    REQUITE_ASSERT(
        !attributes.getHasAttribute(requite::AttributeType::MANGLED_NAME));
    REQUITE_ASSERT(!attributes.getHasAttribute(requite::AttributeType::LABEL));
    REQUITE_ASSERT(!attributes.getHasAttribute(requite::AttributeType::PACK));
    switch (const requite::SubSymbolType type = sub.getType()) {
    case requite::SubSymbolType::NONE:
      ostream << "invalid subtype of ";
      break;
    case requite::SubSymbolType::ARRAY:
      if (sub.getHasInferencedCount()) {
        ostream << "inferenced count array of ";
      } else {
        ostream << sub.getCount() << " count array of ";
      }
      break;
    case requite::SubSymbolType::REFERENCE:
      ostream << "reference to ";
      break;
    case requite::SubSymbolType::STOLEN_REFERENCE:
      ostream << "stolen reference to ";
      break;
    case requite::SubSymbolType::POINTER:
      ostream << "pointer to ";
      break;
    case requite::SubSymbolType::FAT_POINTER:
      ostream << "fat pointer to ";
      break;
    }
  }
  const requite::RootSymbol &root = this->getRoot();
  switch (const requite::RootSymbolType type = root.getType()) {
  case requite::RootSymbolType::NONE:
    ostream << "invalid type";
    break;
  case requite::RootSymbolType::INFERENCE:
    ostream << "inferrenced type";
    break;
  case requite::RootSymbolType::VOID:
    ostream << "void";
    break;
  case requite::RootSymbolType::VARIADIC_ARGUMENTS:
    ostream << "variadic arguments";
    break;
  case requite::RootSymbolType::NULL_:
    ostream << "null";
    break;
  case requite::RootSymbolType::BOOLEAN:
    ostream << "boolean";
    break;
  case requite::RootSymbolType::WORD:
    ostream << "word with depth " << root.getDepth();
    break;
  case requite::RootSymbolType::SIGNED_INTEGER:
    ostream << "signed integer with depth" << root.getDepth();
    break;
  case requite::RootSymbolType::UNSIGNED_INTEGER:
    ostream << "unsigned integer with depth" << root.getDepth();
    break;
  case requite::RootSymbolType::BINARY_HALF_FLOAT:
    ostream << "IEEE 754 binary half floating point";
    break;
  case requite::RootSymbolType::BINARY_SINGLE_FLOAT:
    ostream << "IEEE 754 binary single floating point";
    break;
  case requite::RootSymbolType::BINARY_DOUBLE_FLOAT:
    ostream << "IEEE 754 binary double floating point";
    break;
  case requite::RootSymbolType::BINARY_QUAD_FLOAT:
    ostream << "IEEE 754 binary quad floating point";
    break;
  case requite::RootSymbolType::C_CHAR:
    ostream << "C char";
    break;
  case requite::RootSymbolType::UTF8:
    ostream << "utf8 codeunit";
    break;
  case requite::RootSymbolType::SIGNATURE:
    ostream << "signature";
    break;
  case requite::RootSymbolType::TUPLE:
    ostream << "tuple";
    break;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    ostream << "anonymous object";
    break;
  case requite::RootSymbolType::SCOPE:
    ostream << "scope";
  case requite::RootSymbolType::OBJECT:
    ostream << "object";
    break;
  case requite::RootSymbolType::TABLE:
    ostream << "table";
    break;
  case requite::RootSymbolType::ORDERED_VARIABLE:
    ostream << "local variable";
    break;
  default:
    // TODO
    break;
  }
  return buffer;
}

} // namespace requite