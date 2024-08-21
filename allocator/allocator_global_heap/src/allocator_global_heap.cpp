#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger): _logger(logger){}

allocator_global_heap::~allocator_global_heap()
{
    _logger = nullptr;
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept
{
    _logger = std::move(other._logger);
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    this->_logger->~logger();
    if (this != &other)
    {
        this->_logger = std::move(other._logger);
    }
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    logger_guardant::debug_with_guard("Try allocate memory\n");
    block_pointer_t block = nullptr;
    try
    {
        block = ::operator new(sizeof(allocator_global_heap*)+sizeof(size_t)+value_size*values_count);
    }
    catch(std::exception& e)
    {
        logger_guardant::error_with_guard(e.what());
        throw std::runtime_error("Failed to allocate memory");
    }
    auto **ptr_allocator = reinterpret_cast<allocator_global_heap**>(block);
    (*ptr_allocator) = this;
    (ptr_allocator++);

    auto* ptr_size = reinterpret_cast<size_t*>(ptr_allocator);
    (*ptr_size) = value_size;
    (ptr_size++);

    block = reinterpret_cast<void*>(ptr_size);
    logger_guardant::debug_with_guard("Success allocate memory\n");
    return block;
}

void allocator_global_heap::deallocate(
    void *at)
{
    logger_guardant::debug_with_guard("Try deallocate memory\n"); // byte array
    auto* ptr_s = reinterpret_cast<size_t*>(at);
    (ptr_s--);
    
    auto** ptr_a = reinterpret_cast<allocator_global_heap**>(ptr_s);
    (ptr_a--);

    if ((*ptr_a) == this)
    {
        ::operator delete(ptr_a);
        logger_guardant::debug_with_guard("Success deallocate memory\n");
    }
    else
    {
        logger_guardant::error_with_guard("In deallocate: This block was not allocated by this allocator\n");
    }
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    printf("Ya xz\n");
}