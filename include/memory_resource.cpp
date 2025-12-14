#include <stdexcept>

#include "memory_resource.h"

void *FixedBlockMemoryResource::Find(size_t bytes) {
  auto it = _free_blocks.lower_bound(bytes);
  if (it != _free_blocks.end()) {
    auto &free_map = it->second;
    auto free_it = free_map.begin();
    if (free_it != free_map.end() && !free_it->second) {
      void *ptr = free_it->first;
      free_it->second = true;
      if (free_map.size() == 1) {
        _free_blocks.erase(it);
      } else {
        free_map.erase(free_it);
      }
      return ptr;
    }
  }
  return nullptr;
}

void FixedBlockMemoryResource::AddBlock(void *ptr, size_t bytes) {
  if (bytes == 0)
    return;
  auto &free_map = _free_blocks[bytes];
  free_map[ptr] = false;
}

void *FixedBlockMemoryResource::do_allocate(size_t bytes, size_t alignment) {
  if (bytes == 0) {
    return nullptr;
  }

  size_t aligned_bytes = (bytes + alignment - 1) / alignment * alignment;

  void *ptr = Find(aligned_bytes);
  if (ptr) {
    _allocated[ptr] = aligned_bytes;
    return ptr;
  }

  size_t required_space = current_allocated_size_ + aligned_bytes;
  if (required_space > _block_size) {
    throw std::runtime_error("out of memory");
  }

  void *new_ptr = _block + current_allocated_size_;
  current_allocated_size_ += aligned_bytes;

  _allocated[new_ptr] = aligned_bytes;
  return new_ptr;
}

void FixedBlockMemoryResource::do_deallocate(void *ptr, size_t bytes,
                                             size_t alignment) {
  if (ptr == nullptr || bytes == 0) {
    return;
  }

  if (ptr < _block || ptr >= _block + _block_size) {
    return;
  }

  auto it = _allocated.find(ptr);
  if (it != _allocated.end() && it->second == bytes) {
    _allocated.erase(it);
    AddBlock(ptr, bytes);
  }
}

bool FixedBlockMemoryResource::do_is_equal(
    const std::pmr::memory_resource &other) const noexcept {
  return this == &other;
}

FixedBlockMemoryResource::FixedBlockMemoryResource(size_t pool_size)
    : _block_size(pool_size) {
  _block = static_cast<char *>(std::malloc(_block_size));
  if (!_block) {
    throw std::runtime_error("alloc fail\n");
  }
}

FixedBlockMemoryResource::~FixedBlockMemoryResource() {
  std::free(_block);
  _block = nullptr;
}