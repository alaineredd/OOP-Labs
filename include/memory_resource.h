#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <memory_resource>

namespace {
inline const size_t PSIZE = 4096;
}

class FixedBlockMemoryResource : public std::pmr::memory_resource {
private:
  char *_block = nullptr;
  size_t _block_size = 0;
  std::map<void *, size_t> _allocated;
  std::map<size_t, std::map<void *, bool>> _free_blocks;
  size_t current_allocated_size_ = 0;

  void *Find(size_t bytes);

  void AddBlock(void *ptr, size_t bytes);

protected:
  void *do_allocate(size_t bytes, size_t alignment) override;
  void do_deallocate(void *ptr, size_t bytes, size_t alignment) override;
  bool
  do_is_equal(const std::pmr::memory_resource &other) const noexcept override;

public:
  explicit FixedBlockMemoryResource(size_t pool_size = PSIZE);
  ~FixedBlockMemoryResource() override;

  FixedBlockMemoryResource(const FixedBlockMemoryResource &) = delete;
  FixedBlockMemoryResource &
  operator=(const FixedBlockMemoryResource &) = delete;
  FixedBlockMemoryResource(FixedBlockMemoryResource &&) = delete;
  FixedBlockMemoryResource &operator=(FixedBlockMemoryResource &&) = delete;
};