#pragma once

#include <rq/utility.hpp>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <llvm/Support/AlignOf.h>
#include <llvm/Support/Casting.h>

namespace rq {

using Generation = std::uint64_t;

template <typename> struct Gendex;

template <typename DataParam> struct Slot final {
  using Data = DataParam;
  using Gendex = rq::Gendex<Data>;
  using Self = rq::Slot<Data>;

  rq::Generation _generation{0};
  llvm::AlignedCharArrayUnion<Self *, Data> _data_or_next_ptr{};

  Slot() = default;
  template <typename... ArgNParam>
  inline Slot(rq::Generation generation, ArgNParam &&...arg_n)
      : _generation(generation) {
    std::construct_at(this->getDataPtr(), std::forward<ArgNParam>(arg_n)...);
  }
  Slot(const Self &) = delete;
  Slot(Self &&) = delete;
  inline ~Slot() {
    if (this->_generation != 0) {
      std::destroy_at(this->getDataPtr());
    }
  }
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE Data *getDataPtr() {
    return std::launder(reinterpret_cast<Data *>(&this->_data_or_next_ptr));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Data *getDataPtr() const {
    return std::launder(
        reinterpret_cast<const Data *>(&this->_data_or_next_ptr));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() {
    return this->_generation == 0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Generation getGeneration() const {
    return this->_generation;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Data &getData() {
    RQ_ASSERT(this->_generation != 0, "no data");
    return *this->getDataPtr();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Data &getData() const {
    RQ_ASSERT(this->_generation != 0, "no data");
    return *this->getDataPtr();
  }
  inline void clear() {
    if (this->_generation != 0) {
      this->_generation = 0;
      std::destroy_at(this->getDataPtr());
    }
  }
  template <typename... ArgNParam>
  inline void reset(std::uint32_t generation, ArgNParam... arg_n) {
    if (this->_generation != 0) {
      std::destroy_at(this->getDataPtr());
    }
    this->_generation = generation;
    std::construct_at(this->getDataPtr(), std::forward<ArgNParam>(arg_n)...);
  }
};

template <typename DataParam> struct Gendex final {
  using Data = DataParam;
  using Slot = rq::Slot<Data>;
  using Self = rq::Gendex<Data>;

  Slot *_slot_ptr{nullptr};
  rq::Generation _generation{0};

  Gendex() = default;
  Gendex(Slot &slot, rq::Generation generation)
      : _slot_ptr(&slot), _generation(generation) {}
  Gendex(const Self &) = default;
  Gendex(Self &&) = default;
  ~Gendex() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Generation getGeneration() const {
    return this->_generation;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasData() const {
    if (this->_generation == 0) {
      return false;
    }
    const Slot &slot = rq::dereferencePtr(this->_slot_ptr);
    if (slot.getGeneration() != this->getGeneration()) {
      return false;
    }
    return true;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Data &getData() {
    RQ_ASSERT(this->getHasData(), "no data");
    Slot &slot = rq::dereferencePtr(this->_slot_ptr);
    return slot.getData();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Data &getData() const {
    RQ_ASSERT(this->getHasData(), "no data");
    const Slot &slot = rq::dereferencePtr(this->_slot_ptr);
    return slot.getData();
  }
  RQ_ALWAYS_INLINE void clear() {
    this->_slot_ptr = nullptr;
    this->_generation = 0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_slot_ptr == rhs._slot_ptr &&
           this->_generation == rhs._generation;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_slot_ptr != rhs._slot_ptr ||
           this->_generation != rhs._generation;
  }
};

static constexpr rq::Generation MAX_GENERATION =
    std::numeric_limits<rq::Generation>::max();

template <typename DataParam, std::size_t SLAB_SIZE_PARAM = 4096>
struct GenerationalArena final {
  using Data = DataParam;
  static constexpr std::size_t SLAB_SIZE = SLAB_SIZE_PARAM;
  using Gendex = rq::Gendex<Data>;
  using Slot = rq::Slot<Data>;
  using Self = rq::GenerationalArena<Data, SLAB_SIZE>;

  static_assert(SLAB_SIZE > 0, "SLAB_SIZE must be positive");
  static_assert(SLAB_SIZE >= sizeof(Slot),
                "SLAB_SIZE must fit at least one slot");

  static constexpr std::size_t BYTES_PER_SLAB = SLAB_SIZE;
  static constexpr std::size_t SLOTS_PER_SLAB = BYTES_PER_SLAB / sizeof(Slot);

  rq::Generation _generation{0};
  std::vector<std::unique_ptr<Slot[]>> _slabs{};
  std::size_t _next_index{0};
  Slot *_next_free_ptr{nullptr};

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
        this->_slabs.push_back(
            std::unique_ptr<Slot[]>(new Slot[SLOTS_PER_SLAB]));
      }
    }
    return slab;
  }
  [[nodiscard]] inline bool getOwnsSlot(const Slot &slot) const {
    for (std::size_t i = 0; i < this->_slabs.size(); ++i) {
      const Slot *base = this->_slabs[i].get();
      const Slot *end = base + SLOTS_PER_SLAB;
      if (&slot >= base && &slot < end) {
        return true;
      }
    }
    return false;
  }
  template <typename... ArgNParam>
  [[nodiscard]] inline Gendex fillSlot(ArgNParam &&...arg_n) {
    const rq::Generation generation = ++this->_generation;
    if (this->_next_free_ptr == nullptr) {
      const std::size_t index = this->_next_index++;
      std::ignore = this->ensureSlabForIndex(index);
      Slot &slot = this->slotAt(index);
      slot.reset(generation, std::forward<ArgNParam>(arg_n)...);
      return Gendex(slot, generation);
    }
    Slot *slot_ptr = this->_next_free_ptr;
    Slot &slot = rq::dereferencePtr(slot_ptr);
    this->_next_free_ptr = *slot.getDataPtr();
    slot.reset(generation, std::forward<ArgNParam>(arg_n)...);
    return Gendex(slot, generation);
  }
  void releaseSlot(Gendex &gendex) {
    if (!gendex.getHasData()) {
      return;
    }
    Slot &slot = gendex.getSlot();
    RQ_ASSERT(this->getOwnsSlot(slot), "does not own slot");
    if (gendex.getGeneration() != slot.getGeneration()) {
      return;
    }
    slot.clear();
    std::construct_at(slot.getDataPtr(), this->_next_free_ptr);
    this->_next_free_ptr = &slot;
    gendex.clear();
  }
};

} // namespace rq