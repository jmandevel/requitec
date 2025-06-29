#include <requite/export_table.hpp>

namespace requite {

bool ExportTable::operator==(const Self& rhs) const {
    return this == &rhs;
}

bool ExportTable::operator!=(const Self& rhs) const {
    return this == &rhs;
}

llvm::StringMap<requite::RootSymbol> &ExportTable::getSymbolMap() {
    return this->_exported_symbol_map;
}

const llvm::StringMap<requite::RootSymbol> &ExportTable::getSymbolMap() const {
    return this->_exported_symbol_map;
}

}