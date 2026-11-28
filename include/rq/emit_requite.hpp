#pragma once

namespace llvm {
class raw_ostream;
}

namespace rq {

struct Context;
struct Expression;
struct Module;

struct RequiteEmitter final {
  rq::Context &_context;
  llvm::raw_ostream &_output;

  explicit RequiteEmitter(rq::Context &context, llvm::raw_ostream &output)
      : _context(context), _output(output) {}

  void emit(const rq::Module &module);
  void emitIndent(unsigned indent);
  void emitExpression(const rq::Expression &top, unsigned indent);
};

} // namespace rq
