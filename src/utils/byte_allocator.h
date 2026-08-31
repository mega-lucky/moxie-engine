#ifndef BYTE_ALLOCATOR
#define BYTE_ALLOCATOR

#include <cstddef>

class byte_allocator {
public:
    using value_type = std::byte;
    std::size_t alignment;
    explicit byte_allocator(std::size_t align = alignof(std::byte)) noexcept;
    byte_allocator(const byte_allocator& other) noexcept;
    [[nodiscard]] std::byte* allocate(std::size_t n);
    void deallocate(std::byte* p, std::size_t n) noexcept;
    bool operator==(const byte_allocator& other) const noexcept;
    bool operator!=(const byte_allocator& other) const noexcept;

    template <typename U>
    struct rebind {
        using other = byte_allocator;
    };
};

#endif