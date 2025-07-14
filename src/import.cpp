#include <requite/import.hpp>
#include <requite/assert.hpp>

namespace requite {

    bool Import::operator==(const Self& rhs) const {
        return this == &rhs;
    }

    bool Import::operator!=(const Self& rhs) const {
        return this != &rhs;
    }


bool Import::getHasExpression() const {
    return this->_expression_ptr != nullptr;
}

void Import::setExpression(requite::Expression& expression) {
    requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression& Import::getExpression() {
    return requite::getRef(this->_expression_ptr);
}

const requite::Expression& Import::getExpression() const {
    return requite::getRef(this->_expression_ptr);
}

bool Import::getHasModule() const {
    return this->_module_ptr != nullptr;
}

void Import::setModule(requite::Module& module) {
    requite::setSingleRef(this->_module_ptr, module);
}

requite::Module& Import::getModule() {
    return requite::getRef(this->_module_ptr);
}

const requite::Module &Import::getModule() const {
    return requite::getRef(this->_module_ptr);
}

requite::AttributeFlags &Import::getAttributeFlags() {
    return this->_attributes;
}

requite::AttributeFlags Import::getAttributeFlags() const {
    return this->_attributes;
}

bool Import::getHasNext() const {
    return this->_next_ptr != nullptr;
}

void Import::setNext(requite::Import& next) {
    requite::setSingleRef(this->_next_ptr, next);
}

requite::Import &Import::getNext() {
    return requite::getRef(this->_next_ptr);
}

const requite::Import &Import::getNext() const {
    return requite::getRef(this->_next_ptr);
}

}