#pragma once

#include <rq/utility.hpp>

#include <cstdint>
#include <limits>
#include <utility>

namespace rq {

template<typename T>
concept HasRelease = requires(T &t) { t.release(); };

template <typename DataParam> struct Gendex final {
  using Data = DataParam;
  using Self = rq::Gendex<Data>;

  std::uint32_t _index{0};
  std::uint32_t _generation{0};

  Gendex() = default;
  Gendex(std::uint32_t index, std::uint32_t generation)
      : _index(index), _generation(generation) {}
  Gendex(const Self &) = default;
  Gendex(Self &&) = default;
  ~Gendex() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint32_t getIndex() const {
    return this->_index;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint32_t getGeneration() const {
    return this->_generation;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_index == rhs._index && this->_generation == rhs._generation;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_index != rhs._index && this->_generation != rhs._generation;
  }
};

template <typename DataParam> struct Slot final {
  using Data = DataParam;
  using Gendex = rq::Gendex<Data>;
  using Self = rq::Slot<Data>;

  std::uint32_t _generation;
  Data _data;

  template <typename... ArgNParam>
  Slot(std::uint32_t generation, ArgNParam &&...arg_n)
      : _generation(generation), _data(std::forward(arg_n)...) {}
  Slot(const Self&) = default;
  Slot(Self&&) = default;
  ~Slot();
  Self& operator=(const Self&) = default;
  Self& operator=(Self&&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint32_t getGeneration() const {
    return this->_generation;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Data& getData() {
    return this->_data;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Data& getData() const {
    return this->_data;
  }
  void clear() requires rq::HasRelease<Data> {
    this->_generation = 0;
    this->_data.release();
  }
  void clear() {
    this->_generation = 0;
  }
  template<typename... ArgNParam>
  void reset(std::uint32_t generation, ArgNParam... arg_n) {
    this->_generation = generation;
    this->_data = Data(std::forward(arg_n)...);
  }
};

static constexpr std::uint32_t MAX_GENERATION =
    std::numeric_limits<std::uint32_t>::max();

template <typename DataParam> struct GenerationalArena final {
  using Data = DataParam;
  using Gendex = rq::Gendex<Data>;
  using Slot = rq::Slot<Data>;
  using Self = rq::GenerationalArena<Data>;

  std::uint32_t _generation{0};
  std::vector<Slot> _slots{};
  std::vector<std::size_t> _recycle_bin{};

  GenerationalArena() = default;
  GenerationalArena(const Self&) = default;
  GenerationalArena(Self&&) = default;
  ~GenerationalArena() = default;
  Self& operator=(const Self&) = default;
  Self& operator=(Self&&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint32_t getGeneration() const {
    return this->_generation;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const std::vector<Slot>& getSlots() const {
    return this->_slots;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const std::vector<std::size_t> getRecycleBin() const {
    return this->_recycle_bin;
  }
  template<typename... ArgNParam>
  [[nodiscard]] inline Gendex fillSlot(ArgNParam&&... arg_n) {
    const std::uint32_t generation = ++this->_generation;
    if (this->_recycle_bin.empty()) {
        const std::uint32_t index = this->_slots.size();
        std::ignore = this->_slots.emplace_back(generation, std::forward(arg_n)...);
        return Gendex(generation, index);
    }
    const std::uint32_t index = this->_recycle_bin.back();
    this->_recycle_bin.pop_back();
    Slot& slot = this->_slots[index];
    slot.reset(generation, std::forward(arg_n)...);
    return Gendex(generation, index);
  }
  [[nodiscard]] inline Data* tryGetData(Gendex gendex) {
    if (this->_slots.size() >= gendex.getIndex()) {
        return nullptr;
    }
    Slot& slot = this->_slots[gendex.getIndex()];
    if (slot.getGeneration() != gendex.getGeneration()) {
        return nullptr;
    }
    return &slot.getData();
  } 
  [[nodiscard]] inline const Data* tryGetData(Gendex gendex) const {
    if (this->_slots.size() >= gendex.getIndex()) {
        return nullptr;
    }
    const Slot& slot = this->_slots[gendex.getIndex()];
    if (slot.getGeneration() != gendex.getGeneration()) {
        return nullptr;
    }
    return &slot.getData();
  } 
  void recycleSlot(Gendex gendex) {
    RQ_ASSERT(this->_slots.size() < gendex.getIndex(), "index out of range");
    Slot& slot = this->_slots[gendex.getIndex()];
    if (slot.getGendex() == gendex.getGendex()) {
        slot.clear();
        this->_recycle_bin.push_back(gendex.getIndex());
    }
  }
};

} // namespace rq