#include <rq/entity.hpp>
#include <rq/context.hpp>

namespace rq {

void SymbolTableSymbol::tabulateNamedSymbol(rq::Context &context,
                                            llvm::StringRef name,
                                            rq::Symbol &symbol) {
  auto it = this->_named_values.find(name);
  if (it != this->_named_values.end()) {
    rq::BumpPtrList<rq::Symbol> &entry = it->second;
    rq::BumpPtrListNode<rq::Symbol> &node =
        context.allocateValue<rq::BumpPtrListNode<rq::Symbol>>(symbol, entry);
    entry = rq::BumpPtrList<rq::Symbol>(node);
  } else {
    this->_named_values.insert({name, rq::BumpPtrList<rq::Symbol>(symbol)});
  }
}

void SymbolTableSymbol::tabulateUnamedSymbol(rq::Context &context,
                                             rq::Symbol &symbol) {
  rq::BumpPtrList<rq::Symbol> &entry = this->_unamed_values;
  if (entry.getIsEmpty()) {
    entry = symbol;
    return;
  }
  rq::BumpPtrListNode<rq::Symbol> &node =
      context.allocateValue<rq::BumpPtrListNode<rq::Symbol>>(symbol, entry);
  entry = rq::BumpPtrList<rq::Symbol>(node);
}

} // namespace rq
