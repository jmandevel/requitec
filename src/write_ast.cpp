// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/ast_writer.hpp>
#include <requite/codeunits.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/options.hpp>
#include <requite/source_location.hpp>
#include <requite/source_range.hpp>
#include <requite/utility.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FormatVariadic.h>
#include <llvm/Support/raw_ostream.h>

#include <string>

namespace requite {

bool Context::writeAst(const requite::Module &module,
                       llvm::StringRef out_path) {
  requite::AstWriter writer(*this);
  return writer.writeAst(module, out_path);
}

AstWriter::AstWriter(requite::Context &context)
    : _buffer(), _ostream(_buffer), _context_ref(context), _indentation(0) {}

requite::Context &AstWriter::getContext() { return _context_ref.get(); }

const requite::Context &AstWriter::getContext() const {
  return _context_ref.get();
}

llvm::raw_string_ostream &AstWriter::getOstream() { return _ostream; }

bool AstWriter::writeAst(const requite::Module &module,
                         llvm::StringRef out_path) {
  this->_buffer.clear();
  if (module.getHasExpression()) {
    for (const auto &expression :
         module.getExpression().getHorizontalSubrange()) {
      this->writeExpression(expression);
    }
  }
  std::error_code ec;
  llvm::raw_fd_ostream fout(out_path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->getContext().logMessage(
        llvm::Twine(
            "error: failed to open output file for writing\n\tpath: ")
            +
        llvm::Twine(out_path) + llvm::Twine("\n\treason: ") +
        llvm::Twine(ec.message()));
    return false;
  }
  fout << this->_buffer;
  return true;
}

void AstWriter::addIndentation() { this->_indentation++; }

void AstWriter::removeIndentation() { this->_indentation--; }

void AstWriter::writeIndentation() {
  for (unsigned i = 0; i < this->_indentation; i++) {
    this->getOstream() << "    ";
  }
}

void AstWriter::writeExpression(const requite::Expression &expression) {
  this->writeIndentation();
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::__INTEGER_LITERAL:
    [[fallthrough]];
  case requite::Opcode::__REAL_LITERAL: {
    this->getOstream() << expression.getSourceText();
    this->getOstream() << " // from ";
    this->getOstream() << requite::getName(opcode);
    this->writeExpressionLocationComment(expression);
  } break;
  case requite::Opcode::__STRING_LITERAL: {
    this->getOstream() << "\"";
    if (expression.getHasDataText()) {
      for (char c : expression.getDataText()) {
        this->getOstream() << requite::getIntermediateFileEscapeSequence(c);
      }
    } else {
      this->getOstream() << expression.getSourceText();
    }
    this->getOstream() << "\" // from ";
    this->getOstream() << requite::getName(requite::Opcode::__STRING_LITERAL);
    this->writeExpressionLocationComment(expression);
  } break;
  case requite::Opcode::__CODEUNIT_LITERAL: {
    this->getOstream() << "\'";
    if (expression.getHasDataText()) {
      for (char c : expression.getDataText()) {
        this->getOstream() << requite::getIntermediateFileEscapeSequence(c);
      }
    } else {
      this->getOstream() << expression.getSourceText();
    }
    this->getOstream() << "\' // from ";
    this->getOstream() << requite::getName(requite::Opcode::__CODEUNIT_LITERAL);
    this->writeExpressionLocationComment(expression);
  } break;
  case requite::Opcode::__IDENTIFIER_LITERAL: {
    this->getOstream() << "\\\"";
    for (char c : expression.getDataText()) {
      this->getOstream() << requite::getIntermediateFileEscapeSequence(c);
    }
    this->getOstream() << "\" // from ";
    this->getOstream() << requite::getName(
        requite::Opcode::__IDENTIFIER_LITERAL);
    this->writeExpressionLocationComment(expression);
  } break;
  default: {
    this->getOstream() << "[";
    this->getOstream() << requite::getName(expression.getOpcode());
    if (!expression.getHasBranch()) {
      this->getOstream() << "]";
      this->writeExpressionLocationComment(expression);
    } else {
      this->writeExpressionLocationComment(expression);
      {
        requite::AstWriterIndentLock lock(*this);
        for (const auto &branch : expression.getBranchSubrange()) {
          this->writeExpression(branch);
        }
      }
      this->writeIndentation();
      this->getOstream() << "]\n";
    }
  }
  }
}

void AstWriter::writeExpressionLocationComment(
    const requite::Expression &expression) {
  this->getOstream() << "                // ";
  if (expression.getHasSourceText()) {
    requite::SourceRange source_range =
        this->getContext().getSourceRange(expression);
    this->getOstream() << source_range.start.file << ":"
                       << source_range.start.line << ":"
                       << source_range.start.column;
    if (source_range.start != source_range.end) {
      this->getOstream() << "-" << source_range.end.line << ":"
                         << source_range.end.column;
    }
  }
  this->getOstream() << "\n";
}

AstWriterIndentLock::AstWriterIndentLock(AstWriter &writer) : _writer(writer) {
  _writer.get().addIndentation();
}

AstWriterIndentLock::~AstWriterIndentLock() {
  _writer.get().removeIndentation();
}

} // namespace requite