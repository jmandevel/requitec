#pragma once

#include <rq/ast.hpp>
#include <rq/context.hpp>
#include <rq/static.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/Twine.h>

#include <functional>

namespace rq {

struct Context;
struct Module;

struct Situator final {
  using Self = rq::Situator;

  std::reference_wrapper<rq::Context> _context_ref;
  std::reference_wrapper<rq::StaticFrame> _static_frame_ref;
  bool _is_ok = true;

  Situator(rq::Context &context, rq::StaticFrame &static_frame)
      : _context_ref(context), _static_frame_ref(static_frame) {}
  Situator(const Self &) = delete;
  Situator(Self &&) = delete;
  ~Situator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticFrame &getStaticFrame() {
    return this->_static_frame_ref.get();
  }
  RQ_ALWAYS_INLINE void setNotOk() { this->_is_ok = false; }
  void logErrorNotAtLeastBranchCount(rq::Siatuation situation,
                                     rq::Expression &expression,
                                     unsigned count);
  void logErrorNotExactBranchCount(rq::Situation situation,
                                   rq::Expression &expression, unsigned count);
  void logErrorTooManyBranchCount(rq::Situation situation,
                                  rq::Expression &expression,
                                  unsigned max_count);
  void logErrorInvalidBranchSituation(
      rq::Situation situation, rq::Situation branch_situation,
      rq::Expression &branch, rq::Keyword outer_keyword,
      rq::Keyword branch_keyword, unsigned branch_i, llvm::Twine log_context);
  void situateModule(rq::Module &module);
  void situateTree(rq::Situation situation, rq::Expression &expression);
};

} // namespace rq