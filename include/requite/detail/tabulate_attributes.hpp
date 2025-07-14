#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/context.hpp>
#include <requite/user_symbol.hpp>

namespace requite {

template <requite::UserSymbolType TYPE_PARAM>
void Tabulator::tabulateAttributes(requite::AttributeFlags &out_flags,
                                   requite::Expression &expression) {
  for (requite::Expression &attribute : expression.getNextSubrange()) {
    const requite::Opcode opcode = attribute.getOpcode();
    const requite::AttributeType type = requite::getAttributeType(opcode);
    if (!requite::getIsValidUserSymbolAttribute<TYPE_PARAM>(
            this->getScope().getType(), type)) {
      this->getContext().logSourceMessage(attribute, requite::LogType::ERROR,
                             llvm::Twine(requite::getName(type)) +
                                 " attribute is not " +
                                 requite::getName(TYPE_PARAM) + " attribute");
      this->setNotOk();
    }
    if (type == requite::AttributeType::USER) {
      out_flags.addAttribute(type);
    } else {
      if (out_flags.getHasAttribute(type)) {
        this->getContext().logErrorDuplicateAttribute(attribute, type);
        this->setNotOk();
        continue;
      }
      out_flags.addAttribute(type);
    }
  }
}

} // namespace requite