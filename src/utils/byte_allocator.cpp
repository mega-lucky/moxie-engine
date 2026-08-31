#include "./byte_allocator.h"

byte_allocator::byte_allocator(std::size_t align) noexcept 
    : alignment(align) {}

byte_allocator::byte_allocator(const byte_allocator& other) noexcept 
    : alignment(other.alignment) {}

[[nodiscard]] std::byte* byte_allocator::allocate(std::size_t n) {
    if (n == 0) return nullptr;
    
    void* ptr = ::operator new(n, std::align_val_t(alignment));
    return static_cast<std::byte*>(ptr);
}

void byte_allocator::deallocate(std::byte* p, std::size_t n) noexcept {
    (void)n;
    if (p == nullptr) return;
    ::operator delete(p, std::align_val_t(alignment));
}

bool byte_allocator::operator==(const byte_allocator& other) const noexcept {
    return alignment == other.alignment;
}
bool byte_allocator::operator!=(const byte_allocator& other) const noexcept {
    return alignment != other.alignment;
}