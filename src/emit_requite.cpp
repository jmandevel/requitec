#include <rq/context.hpp>
#include <rq/emit_requite.hpp>
#include <rq/expressions.hpp>
#include <rq/options.hpp>
#include <rq/utility.hpp>

#include <llvm/Support/raw_ostream.h>

namespace rq {

void RequiteEmitter::emitIndent(unsigned indent) {
  for (unsigned i = 0; i < indent; i++) {
    this->_output << "  ";
  }
}

void RequiteEmitter::emitExpression(const rq::Expression &top,
                                    unsigned indent) {
  llvm::raw_ostream &fout = this->_output;
  if (!rq::getNoComment()) {
    this->emitIndent(indent);
    fout << "//";
    if (top.getIsInternal()) {
      fout << " " << top.getName();
    }
    if (!top.getHasSourceText()) {
      fout << " (no position)";
    } else if (top.getSourceTextLength() == 0) {
      rq::SourceLocation location =
          this->_context.getSourceLocation(top.getLlvmSourceBegin());
      fout << " " << location.file << ":" << location.line << ":"
           << location.column;
    } else {
      rq::SourceRange range = this->_context.getSourceRange(top);
      fout << " " << range.start.file << ":" << range.start.line << ":"
           << range.start.column << "-";
      if (range.start.file != range.end.file) {
        fout << range.end.file << ":";
      }
      fout << range.end.line << ":" << range.end.column;
    }
    if (top.getIsInserted()) {
      fout << " (inserted)";
    }
    if (top.getHasSituatorError()) {
      fout << " (situator error)";
    }
    fout << "\n";
  }
  this->emitIndent(indent);
  if (top.getIsLiteral()) {
    fout << top.getSourceText();
    return;
  }
  fout << "[" << top.getName();
  if (top.getHasBranch()) {
    fout << '\n';
    for (const rq::Expression &branch : top.getBranchSubrange()) {
      this->emitExpression(branch, indent + 1);
      if (branch.getIsStatement()) {
        fout << ";\n";
      } else if (branch.getIsTag()) {
        if (branch.getHasNext()) {
          fout << ",\n";
        } else {
          fout << "\n";
        }
      } else {
        fout << '\n';
      }
    }
    this->emitIndent(indent);
  }
  fout << "]";
}

void RequiteEmitter::emit(const rq::Module &module) {
  const rq::Expression *top_ptr = module.getExpressionPtr();
  if (top_ptr == nullptr) {
    return;
  }
  llvm::raw_ostream &fout = this->_output;
  const rq::Expression &top = rq::dereferencePtr(top_ptr);
  for (const rq::Expression &branch : top.getInclusiveNextSubrange()) {
    this->emitExpression(branch, 0);
    if (branch.getIsStatement()) {
      fout << ";\n";
    } else if (branch.getIsTag()) {
      if (branch.getHasNext()) {
        fout << ",\n";
      } else {
        fout << "\n";
      }
    } else {
      fout << "\n";
    }
  }
}

} // namespace rq
