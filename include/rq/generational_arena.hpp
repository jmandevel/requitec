#pragma once

#include <rq/utility.hpp>

#include <cstdint>
#include <limits>
#include <utility>
#include <vector>
#include <memory>
#include <cstddef>

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

  std::uint32_t _generation{0};
  Data _data{};

  Slot() = default;
  template <typename... ArgNParam>
  Slot(std::uint32_t generation, ArgNParam &&...arg_n)
      : _generation(generation), _data(std::forward<ArgNParam>(arg_n)...) {}
  Slot(const Self &) = default;
  Slot(Self &&) = default;
  ~Slot() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
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

template <typename DataParam, std::size_t SLAB_SIZE_PARAM = 4096>
struct GenerationalArena final {
  using Data = DataParam;
  static constexpr std::size_t SLAB_SIZE = SLAB_SIZE_PARAM;
  using Gendex = rq::Gendex<Data>;
  using Slot = rq::Slot<Data>;
  using Self = rq::GenerationalArena<Data, SLAB_SIZE>;

  static_assert(SLAB_SIZE > 0, "SLAB_SIZE must be positive");
  static_assert(SLAB_SIZE >= sizeof(Slot), "SLAB_SIZE must fit at least one slot");

  static constexpr std::size_t BYTES_PER_SLAB = SLAB_SIZE;
  static constexpr std::size_t SLOTS_PER_SLAB = BYTES_PER_SLAB / sizeof(Slot);

  static constexpr std::size_t MAX_INDEX = std::numeric_limits<std::uint32_t>::max();

  std::uint32_t _generation{0};
  std::vector<std::unique_ptr<Slot[]>> _slabs{};
  std::size_t _next_index{0};
  std::vector<std::size_t> _recycle_bin{};

  GenerationalArena() = default;
  GenerationalArena(const Self &) = default;
  GenerationalArena(Self &&) = default;
  ~GenerationalArena() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint32_t getGeneration() const {
    return this->_generation;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getAllocatedCount() const {
    return this->_next_index;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getSlabCount() const {
    return this->_slabs.size();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const std::vector<std::size_t> getRecycleBin()
      const {
    return this->_recycle_bin;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Slot &slotAt(std::size_t index) {
    const std::size_t slab = index / SLOTS_PER_SLAB;
    const std::size_t off = index % SLOTS_PER_SLAB;
    return this->_slabs[slab][off];
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Slot &slotAt(std::size_t index) const {
    const std::size_t slab = index / SLOTS_PER_SLAB;
    const std::size_t off = index % SLOTS_PER_SLAB;
    return this->_slabs[slab][off];
  }
  [[nodiscard]] inline std::size_t ensureSlabForIndex(std::size_t index) {
    const std::size_t slab = index / SLOTS_PER_SLAB;
    if (slab >= this->_slabs.size()) {
      while (this->_slabs.size() <= slab) {
        this->_slabs.push_back(std::unique_ptr<Slot[]>(new Slot[SLOTS_PER_SLAB]));
      }
    }
    return slab;
  }
  template <typename... ArgNParam>
  [[nodiscard]] inline Gendex fillSlot(ArgNParam &&...arg_n) {
    RQ_ASSERT(this->_next_index != MAX_INDEX, "too many slots");
    const std::uint32_t generation = ++this->_generation;
    std::size_t index;
    if (this->_recycle_bin.empty()) {
      index = this->_next_index++;
      ensureSlabForIndex(index);
      Slot &slot = slotAt(index);
      slot.reset(generation, std::forward<ArgNParam>(arg_n)...);
      return Gendex(static_cast<std::uint32_t>(index), generation);
    }
    index = this->_recycle_bin.back();
    this->_recycle_bin.pop_back();
    Slot &slot = slotAt(index);
    slot.reset(generation, std::forward<ArgNParam>(arg_n)...);
    return Gendex(static_cast<std::uint32_t>(index), generation);
  }
  [[nodiscard]] inline Data *tryGetData(Gendex gendex) {
    const std::size_t index = static_cast<std::size_t>(gendex.getIndex());
    if (index >= this->_next_index) {
      return nullptr;
    }
    const Slot &slot = slotAt(index);
    if (slot.getGeneration() != gendex.getGeneration()) {
      return nullptr;
    }
    return const_cast<Data *>(&slot.getData());
  }
  [[nodiscard]] inline const Data *tryGetData(Gendex gendex) const {
    const std::size_t index = static_cast<std::size_t>(gendex.getIndex());
    if (index >= this->_next_index) {
      return nullptr;
    }
    const Slot &slot = slotAt(index);
    if (slot.getGeneration() != gendex.getGeneration()) {
      return nullptr;
    }
    return &slot.getData();
  }
  void recycleSlot(Gendex gendex) {
    const std::size_t index = static_cast<std::size_t>(gendex.getIndex());
    RQ_ASSERT(index < this->_next_index, "index out of range");
    Slot &slot = slotAt(index);
    if (slot.getGeneration() == gendex.getGeneration()) {
      slot.clear();
      this->_recycle_bin.push_back(index);
    }
  }
};

} // namespace rq