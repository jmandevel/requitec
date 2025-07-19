#pragma once

#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/label.hpp>

namespace requite {

template <requite::AttributeCategory CATEGORY_PARAM>
requite::AttributeFlags
Tabulator::tabulateAttributes(requite::Expression &expression) {
  requite::AttributeFlags flags;
  for (requite::Expression &attribute : expression.getNextSubrange()) {
    const requite::Opcode opcode = attribute.getOpcode();
    const requite::AttributeType type = requite::getAttributeType(opcode);
    if (!requite::getCanBeAttributeCategory<CATEGORY_PARAM>(type)) {
      this->getContext().logSourceMessage(
          attribute, requite::LogType::ERROR,
          llvm::Twine(requite::getName(type)) +
              requite::getErrorMessageEnding(CATEGORY_PARAM));
      this->setNotOk();
      continue;
    }
    if (type != requite::AttributeType::USER && flags.getHasAttribute(type)) {
      this->getContext().logErrorDuplicateAttribute(attribute, type);
      this->setNotOk();
      continue;
    }
    if constexpr (requite::getCanBeAttributeCategory<CATEGORY_PARAM>(
                      requite::AttributeType::LABEL)) {
      if (type == requite::AttributeType::LABEL) {
        requite::Expression &name_expression = attribute.getBranch();
        requite::Label &label = this->getContext().makeLabel();
        label.setAttributeExpression(attribute);
        label.setStatementExpression(expression);
        label.setContainingScope(this->getScope());
        llvm::StringRef name;
        if (!this->getContext().evaluateInstantName(name, name_expression)) {
          this->setNotOk();
        } else {
          label.setName(name);
        }
        flags.addAttribute(type);
      }
    }
    if constexpr (requite::getCanBeAttributeCategory<CATEGORY_PARAM>(
                      requite::AttributeType::USER)) {
      if (type == requite::AttributeType::USER) {
        this->tabulateExpression(attribute);
      }
    }
    if constexpr (requite::getCanBeAttributeCategory<CATEGORY_PARAM>(
                      requite::AttributeType::MANGLED_NAME)) {
      if (type == requite::AttributeType::MANGLED_NAME) {
        this->tabulateExpression(attribute.getBranch());
      }
    }
    flags.addAttribute(type);
  }
  return flags;
}

} // namespace requite