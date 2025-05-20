// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/attributes.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>

#include <cstddef>

namespace requite {

Attributes::Attributes(llvm::ArrayRef<requite::AttributeType> attributes) {
  for (requite::AttributeType attribute : attributes) {
    REQUITE_ASSERT(!this->getHasAttribute(attribute));
    this->addAttribute(attribute);
  }
}

requite::MakeAttributesResult
Attributes::makeAttributes(requite::Context &context,
                           requite::Expression &ascribe_expression) {
  requite::MakeAttributesResult result;
  std::ignore =
      ascribe_expression.walkBranch()
          .doUntilLast([&](requite::Expression &branch) {
            requite::AttributeType type =
                requite::getAttributeType(branch.getOpcode());
            REQUITE_ASSERT(type !=
                           requite::AttributeType::NONE); // insured by situator
            if (type == requite::AttributeType::LABEL) {
              requite::Expression &name_expression = branch.getBranch();
              llvm::StringRef name = name_expression.getDataText();
              result.label_names.push_back(name.str());
              // NOTE:
              //  if there are duplicate label names, this error is found when
              //  label instances are created.
            } else {
              if (type == requite::AttributeType::MANGLED_NAME) {
                if (result.mangled_name.empty()) {
                  llvm::StringRef mangled_name =
                      branch.getBranch().getDataText();
                  result.mangled_name = mangled_name;
                }
              }
              if (result.attributes.getHasAttribute(type)) {
                context.logSourceMessage(
                    branch, requite::LogType::ERROR,
                    llvm::Twine("duplicate attribute of type \"") +
                        requite::getName(type) + "\"");
                result.has_error = true;
                return;
              }
              result.attributes.addAttribute(type);
            }
          })
          .doLast([&](requite::Expression &branch) {
            result.last_expression_ptr = &branch;
          });
  return result;
}

void Attributes::logErrorDuplicateAttribute(requite::Context &context,
                                            requite::Expression &branch,
                                            requite::AttributeType type) {
  context.logSourceMessage(branch, requite::LogType::ERROR,
                           llvm::Twine("duplicate attribute found of type") +
                               requite::getName(type) + "");
}

void Attributes::clear() { this->_flags.reset(); }

bool Attributes::getHasAnyAttribute() const { return this->_flags.any(); }

void Attributes::addAttribute(requite::AttributeType type) {
  REQUITE_ASSERT(!this->getHasAttribute(type));
  this->_flags.set(static_cast<std::size_t>(type));
}

void Attributes::removeAttribute(requite::AttributeType type) {
  REQUITE_ASSERT(this->getHasAttribute(type));
  this->_flags.reset(static_cast<std::size_t>(type));
}

bool Attributes::getHasAttribute(requite::AttributeType type) const {
  return this->_flags.test(static_cast<std::size_t>(type));
}

bool Attributes::operator==(const requite::Attributes &other) const {
  return this->_flags == other._flags;
}

bool Attributes::operator!=(const requite::Attributes &other) const {
  return !(*this == other);
}

} // namespace requite