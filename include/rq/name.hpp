#pragma once

#include <rq/entity.hpp>

#include <llvm/ADT/DenseMapInfo.h>
#include <llvm/ADT/FoldingSet.h>

namespace rq {

struct Name final {
  using Self = rq::Name;

  // used for dense map operations. not for valid names.
  static constexpr rq::Keyword DENSE_MAP_KEY_EMPTY =
      rq::Keyword::INTEGER_LITERAL;
  static constexpr rq::Keyword DENSE_MAP_KEY_TOMBSTONE =
      rq::Keyword::FLOAT_LITERAL;

  llvm::StringRef _text{};
  rq::Keyword _keyword{rq::Keyword::NONE};

  explicit RQ_ALWAYS_INLINE Name() = default;
  explicit RQ_ALWAYS_INLINE Name(llvm::StringRef text) : _text(text) {}
  explicit RQ_ALWAYS_INLINE Name(rq::Keyword keyword) : _keyword(keyword) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_text.empty() && this->_keyword == rq::Keyword::NONE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getText() const {
    return this->_text;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getKeyword() const {
    return this->_keyword;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const rq::Name &rhs) const {
    return this->getKeyword() == rhs.getKeyword() &&
           this->getText() == rhs.getText();
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const rq::Name &rhs) const {
    return this->getKeyword() != rhs.getKeyword() &&
           this->getText() != rhs.getText();
  }

  inline void Profile(llvm::FoldingSetNodeID &inout_id) const {
    inout_id.AddString(this->getText());
    inout_id.AddInteger(rq::getUnderlyingValue(this->getKeyword()));
  }
};

} // namespace rq

namespace llvm {

template <> struct DenseMapInfo<rq::Name> {
  [[nodiscard]] static RQ_ALWAYS_INLINE rq::Name getEmptyKey() {
    return rq::Name(rq::Name::DENSE_MAP_KEY_EMPTY);
  }

  [[nodiscard]] static RQ_ALWAYS_INLINE rq::Name getTombstoneKey() {
    return rq::Name(rq::Name::DENSE_MAP_KEY_TOMBSTONE);
  }

  [[nodiscard]] static inline unsigned getHashValue(const rq::Name &name) {
    llvm::FoldingSetNodeID id;
    name.Profile(id);
    return id.ComputeHash();
  }

  [[nodiscard]] static RQ_ALWAYS_INLINE bool isEqual(const rq::Name &lhs,
                                                     const rq::Name &rhs) {
    return lhs == rhs;
  }
};

} // namespace llvm
