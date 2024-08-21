#include <not_implemented.h>
#include <mutex>
#include <sstream>
#include <iomanip>

#include "../include/allocator_boundary_tags.h"

allocator_boundary_tags::~allocator_boundary_tags()
{
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() called");
    
    logger* logger = get_logger();

    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);

    if (logger)
    {
        logger->trace(get_typename() + "::~allocator_boundary_tags() successfuly finished");
    }
    
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept
{
    trace_with_guard(get_typename() + "::(allocator_boundary_tags &&) called");

    other._trusted_memory = nullptr;

    trace_with_guard(get_typename() + "::(allocator_boundary_tags &&) successfuly finished");
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
    trace_with_guard(get_typename() + "::operator=(allocator_boundary_tags &&) called");

    if (this != &other)
    {
        get_mutex().~mutex();
        deallocate_with_guard(_trusted_memory);
        
        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }
    trace_with_guard(get_typename() + "::operator=(allocator_boundary_tags &&) successfuly finished");
    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{

    if (logger != nullptr)
    {
        logger->trace(get_typename() + "::allocator_boundary_tags(size_t, allocator *, logger *, fit_mode) called");
    }
    
    if (space_size < get_block_metadata_size())
    {
        if (logger != nullptr)
        {
            logger->error(get_typename() + "::allocator_boundary_tags(size_t, allocator *, logger *, fit_mode) : " 
                +"size of " + std::to_string(space_size) + "is unacceptable minimum size is" 
                + std::to_string(get_block_metadata_size()));
        }
        throw std::logic_error("cannot initialize allocator instance");
    }


    block_pointer_t* ptr_on_first_block;
    auto metadata_size = sizeof(class logger*)+ sizeof(allocator*) 
        + sizeof(size_t) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex) + sizeof(block_pointer_t*);
    try
    {
        _trusted_memory = parent_allocator == nullptr ? ::operator new(metadata_size+space_size) 
        : parent_allocator->allocate(metadata_size+space_size, 1);
        ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
        (*ptr_on_first_block) = nullptr;
        ptr_on_first_block++;
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::bad_alloc();
    }

    auto ptr_size = reinterpret_cast<size_t*>(ptr_on_first_block);
    (*ptr_size) = space_size;
    ptr_size++;

    auto free_size = reinterpret_cast<size_t*>(ptr_size);
    (*free_size) = space_size;
    free_size++;

    auto ptr_allocator = reinterpret_cast<allocator**>(free_size);
    (*ptr_allocator) = parent_allocator;
    ptr_allocator++;

    auto ptr_logger = reinterpret_cast<class logger**>(ptr_allocator);
    (*ptr_logger) = logger;
    ptr_logger++;

    auto ptr_mode = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr_logger);
    (*ptr_mode) = allocate_fit_mode;
    ptr_mode++;

    try
    {
        new(reinterpret_cast<std::mutex*>(ptr_mode)) std::mutex();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::bad_alloc();
    }

    trace_with_guard(get_typename() + "::allocator_boundary_tags(size_t, allocator *, logger *, fit_mode) successfuly finished");

}

inline size_t* allocator_boundary_tags::get_space_size() const
{
    block_pointer_t* ptr_on_first_block;
    ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    ptr_on_first_block++;

    auto ptr_size = reinterpret_cast<size_t*>(ptr_on_first_block);

    return ptr_size;
}

inline size_t* allocator_boundary_tags::get_allocator_avail_size() const
{
    block_pointer_t* ptr_on_first_block;
    ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    ptr_on_first_block++;

    auto ptr_size = reinterpret_cast<size_t*>(ptr_on_first_block);
    ptr_size++;

    auto free_size = reinterpret_cast<size_t*>(ptr_size);

    return free_size;
}

inline allocator::block_pointer_t allocator_boundary_tags::get_ptr_on_space_size() const
{
    auto ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    ptr_on_first_block++;

    auto ptr_size = reinterpret_cast<size_t*>(ptr_on_first_block);
    ptr_size++;

    auto free_size = reinterpret_cast<size_t*>(ptr_size);
    free_size++;

    auto ptr_allocator = reinterpret_cast<allocator**>(free_size);
    ptr_allocator++;

    auto ptr_logger = reinterpret_cast<logger**>(ptr_allocator);
    ptr_logger++;

    auto ptr_mode = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr_logger);
    ptr_mode++;

    auto ptr_mutex = reinterpret_cast<std::mutex*>(ptr_mode);
    ptr_mutex++;

    block_pointer_t at = reinterpret_cast<block_pointer_t>(ptr_mutex);

    return at;
}

inline std::mutex &allocator_boundary_tags::get_mutex() const
{
    auto ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    ptr_on_first_block++;

    auto ptr_size = reinterpret_cast<size_t*>(ptr_on_first_block);
    ptr_size++;

    auto free_size = reinterpret_cast<size_t*>(ptr_size);
    free_size++;

    auto ptr_allocator = reinterpret_cast<allocator**>(free_size);
    ptr_allocator++;

    auto ptr_logger = reinterpret_cast<logger**>(ptr_allocator);
    ptr_logger++;

    auto ptr_mode = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr_logger);
    ptr_mode++;

    auto ptr_mutex = reinterpret_cast<std::mutex*>(ptr_mode);


    return *ptr_mutex;
}

inline allocator_with_fit_mode::fit_mode* allocator_boundary_tags::get_fit_mode() const
{
    auto ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    ptr_on_first_block++;

    auto ptr_size = reinterpret_cast<size_t*>(ptr_on_first_block);
    ptr_size++;

    auto free_size = reinterpret_cast<size_t*>(ptr_size);
    free_size++;

    auto ptr_allocator = reinterpret_cast<allocator**>(free_size);
    ptr_allocator++;

    auto ptr_logger = reinterpret_cast<logger**>(ptr_allocator);
    ptr_logger++;

    auto ptr_mode = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr_logger);

    return ptr_mode;
}

inline allocator *allocator_boundary_tags::get_allocator() const
{
    auto ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    ptr_on_first_block++;

    auto ptr_size = reinterpret_cast<size_t*>(ptr_on_first_block);
    ptr_size++;

    auto free_size = reinterpret_cast<size_t*>(ptr_size);
    free_size++;

    auto ptr_allocator = reinterpret_cast<allocator**>(free_size);

    return *ptr_allocator;
}

inline size_t allocator_boundary_tags::get_block_size(block_pointer_t block) const
{
    auto ptr_on_next_block = reinterpret_cast<block_pointer_t*>(block);
    ptr_on_next_block--;

    auto ptr_on_prev_block = reinterpret_cast<block_pointer_t*>(ptr_on_next_block);
    ptr_on_prev_block--;

    auto ptr_on_size = reinterpret_cast<size_t*>(ptr_on_prev_block);
    ptr_on_size--;

    return (*ptr_on_size);
}

inline size_t allocator_boundary_tags::get_block_metadata_size() const
{
    return sizeof(allocator*) + 2*sizeof(block_pointer_t*) + sizeof(block_size_t);
}

inline size_t allocator_boundary_tags::get_block_metadata_size(block_pointer_t block) const
{
    auto ptr_on_next_block = reinterpret_cast<block_pointer_t*>(block);
    ptr_on_next_block--;

    auto ptr_on_prev_block = reinterpret_cast<block_pointer_t*>(ptr_on_next_block);
    ptr_on_prev_block--;

    auto ptr_on_size = reinterpret_cast<size_t*>(ptr_on_prev_block);
    ptr_on_size--;

    auto ptr_on_allocator = reinterpret_cast<allocator_boundary_tags**>(ptr_on_size);
    ptr_on_allocator--;

    return (reinterpret_cast<unsigned char*>(block) - reinterpret_cast<unsigned char*>(ptr_on_allocator));
}

inline allocator::block_pointer_t* allocator_boundary_tags::get_ptr_next_block(block_pointer_t block) const
{
    auto ptr_on_next_block = reinterpret_cast<block_pointer_t*>(block);
    ptr_on_next_block--;
    return ptr_on_next_block;
}

inline allocator::block_pointer_t* allocator_boundary_tags::get_ptr_prev_block(block_pointer_t block) const
{
    auto ptr_on_next_block = reinterpret_cast<block_pointer_t*>(block);
    ptr_on_next_block--;

    auto ptr_on_prev_block = reinterpret_cast<block_pointer_t*>(ptr_on_next_block);
    ptr_on_prev_block--;
    return ptr_on_prev_block;
}

allocator::block_pointer_t allocator_boundary_tags::insert_in_memory(block_size_t block_size, block_pointer_t at)
{
    auto ptr_on_allocator = reinterpret_cast<allocator_boundary_tags**>(at);
    (*ptr_on_allocator) = this;
    (ptr_on_allocator)++;

    auto ptr_on_size = reinterpret_cast<size_t*>(ptr_on_allocator);
    (*ptr_on_size) = block_size;
    ptr_on_size++;

    auto ptr_on_prev_block = reinterpret_cast<block_pointer_t*>(ptr_on_size);
    (*ptr_on_prev_block) = nullptr;
    ptr_on_prev_block++;

    auto ptr_on_next_block = reinterpret_cast<block_pointer_t*>(ptr_on_prev_block);
    (*ptr_on_next_block) = nullptr;
    ptr_on_next_block++;

    at = reinterpret_cast<block_pointer_t>(ptr_on_next_block);

    return at;
}

allocator::block_pointer_t allocator_boundary_tags::first_fit(block_size_t block_size)
{
    block_pointer_t* ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    size_t* space_size = get_space_size();
    auto avail_size = get_allocator_avail_size();
    if ((*ptr_on_first_block) == nullptr)
    {
        if ((*avail_size) >= block_size)
        {
            if ((*avail_size) - block_size < get_block_metadata_size())
            {
                block_size = (*avail_size);
                warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                    + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                    + "has been increased to: " + std::to_string((*avail_size)) + "bytes");
            }
            block_pointer_t ptr_on_space_size = get_ptr_on_space_size();
            auto temp = insert_in_memory(block_size, ptr_on_space_size);
            (*ptr_on_first_block) = temp;
            (*avail_size) -= block_size;
            return temp; 
        }
        else
        {
            error_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                + "ERROR: is not enough space for the block : " + "space_size = " + std::to_string(*space_size)
                + "block size = " + std::to_string(block_size));
            throw std::bad_alloc();
        }
    }
    else
    {
        auto current_block = (*ptr_on_first_block);
        block_size_t free_size = ((reinterpret_cast<unsigned char*>(current_block) - get_block_metadata_size(current_block))
        - (reinterpret_cast<unsigned char*>(_trusted_memory) + get_global_metadata_size()));
        if (free_size > 0)
        {
            if (free_size >= block_size)
            {
                if (free_size - block_size < get_block_metadata_size())
                {
                    warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                        + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                        + "has been increased to: " + std::to_string(free_size) + "bytes");
                    block_size = free_size;
                }
                auto new_block = insert_in_memory(block_size, 
                reinterpret_cast<block_pointer_t>((reinterpret_cast<unsigned char*>(_trusted_memory) + get_global_metadata_size())));
                auto next = get_ptr_next_block(new_block);
                (*next) = current_block;
                (*ptr_on_first_block) = new_block;
                auto prev = get_ptr_prev_block(current_block);
                (*prev) = new_block;

                return new_block;
            }
        }
        block_pointer_t temp_for_get = get_ptr_on_space_size();
        block_pointer_t global_right_bound = reinterpret_cast<block_pointer_t>((reinterpret_cast<unsigned char*>(temp_for_get) 
        + (*space_size)));

        size_t block_metadata_size = get_block_metadata_size();
        while((*avail_size) >= block_metadata_size)
        {
            block_pointer_t* next_block = get_ptr_next_block(current_block);
            auto prev_block = get_ptr_prev_block(current_block);
            size_t size_current_block = get_block_size(current_block);
            block_metadata_size = get_block_metadata_size(current_block);
            auto shift = reinterpret_cast<unsigned char*>(current_block);
            shift = shift - block_metadata_size + size_current_block;
            block_pointer_t* block_border = reinterpret_cast<block_pointer_t*>(shift);
            if ((*next_block) == nullptr)
            {
                ptrdiff_t remains = reinterpret_cast<unsigned char*>(global_right_bound) - shift;
                if ((remains >= block_size) && (global_right_bound != block_border))
                {
                    if (remains - block_size < block_metadata_size)
                    {
                        warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                            + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                            + "has been increased to: " + std::to_string(remains) + "bytes");
                        block_size = remains;
                    }
                    auto new_block = insert_in_memory(block_size, reinterpret_cast<block_pointer_t>(shift));
                    (*next_block) = new_block;

                    auto prev = get_ptr_prev_block(new_block);
                    (*prev) = current_block;
                    (*avail_size) -= block_size;
                    return new_block; // !!!!!!!!!!!!!!!!!!!

                }
                else
                {
                    error_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                        + "ERROR: there was no space for a block in the allocated memory area : ");
                    throw std::bad_alloc();
                }
            }
            else
            {
                auto right_border = reinterpret_cast<unsigned char*>(*(next_block)) - get_block_metadata_size(*next_block);
                auto left_border = reinterpret_cast<unsigned char*>(current_block) - block_metadata_size+ (size_current_block);
                if (left_border - right_border == 0)
                {
                    current_block = (*next_block); // !!!
                    next_block = get_ptr_next_block(current_block);
                }
                else
                {
                    if (right_border - left_border >= block_size)
                    {
                        if (right_border - left_border - block_size < block_metadata_size)
                        {
                            warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                                + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                                + "has been increased to: " + std::to_string(right_border - left_border) + "bytes");
                            block_size = right_border - left_border;
                        }
                        auto new_block = insert_in_memory(block_size, reinterpret_cast<block_pointer_t>(left_border));
                        auto new_next_block_ptr = get_ptr_prev_block(new_block);
                        auto new_prev_block_ptr = get_ptr_next_block(new_block); // get ptrs on next and prev blocks in new block.

                        (*new_next_block_ptr) = (*next_block); // change ptrs
                        (*new_prev_block_ptr) = current_block;

                        auto temp_ptr = get_ptr_prev_block(*next_block);
                        (*temp_ptr) = new_block; // ptr on prev in next block -> new_block;

                        (*next_block) = new_block; // (*next_block) is ptr in current_block; (*next_block) -> new_block.
                        (*avail_size) -= block_size;
                        return new_block; // !!!!!!!!!!!!!!!!!!!
                        
                    }
                    else
                    {
                        current_block = (*next_block);
                        next_block = get_ptr_next_block(current_block);
                    }
                }
            }
        }
        if ((*avail_size) < block_metadata_size)
        {
            error_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                + "ERROR: is not enough space for the block : " + "space_size = " + std::to_string(*space_size)
                + "block size = " + std::to_string(block_size));
            throw std::bad_alloc();
        }
    }
}

allocator::block_pointer_t allocator_boundary_tags::best_fit(block_size_t block_size)
{
    block_pointer_t* ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    size_t* space_size = get_space_size();
    auto avail_size = get_allocator_avail_size();
    if ((*ptr_on_first_block) == nullptr)
    {
        if ((*avail_size) >= block_size)
        {
            if ((*avail_size) - block_size < get_block_metadata_size())
            {
                block_size = (*avail_size);
                warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                    + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                    + "has been increased to: " + std::to_string((*avail_size)) + "bytes");
            }
            block_pointer_t ptr_on_space_size = get_ptr_on_space_size();
            auto temp = insert_in_memory(block_size, ptr_on_space_size);
            (*ptr_on_first_block) = temp;
            (*avail_size) -= block_size;
            return temp; 
        }
        else
        {
            error_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                + "ERROR: is not enough space for the block : " + "space_size = " + std::to_string(*space_size)
                + "block size = " + std::to_string(block_size));
            throw std::bad_alloc();
        }
    }
    else
    {
        auto current_block = (*ptr_on_first_block);
        block_pointer_t temp_for_get = get_ptr_on_space_size();
        block_pointer_t global_right_bound = reinterpret_cast<block_pointer_t>((reinterpret_cast<unsigned char*>(temp_for_get) + (*space_size)));
        block_pointer_t ptr = nullptr;
        block_pointer_t* best_space = &ptr;
        block_pointer_t prev_block_for_new;
        block_pointer_t next_block_for_new;
        size_t best_size = (*space_size);
        block_size_t free_size = ((reinterpret_cast<unsigned char*>(current_block) - get_block_metadata_size(current_block))
        - (reinterpret_cast<unsigned char*>(_trusted_memory) + get_global_metadata_size()));
        size_t block_metadata_size = get_block_metadata_size();
        size_t temp_size = block_size;
        if (free_size > 0 )
        {
            if (free_size < best_size && free_size >= block_size)
            {
                if (free_size - block_size < block_metadata_size)
                {
                    temp_size = free_size;
                }
                best_size = free_size;
                (*best_space) = reinterpret_cast<block_pointer_t>((reinterpret_cast<unsigned char*>(_trusted_memory) + get_global_metadata_size()));

                next_block_for_new = current_block; // change ptrs
                prev_block_for_new = nullptr;

            }
        }
        while((*avail_size) >= block_metadata_size)
        {
            block_pointer_t* next_block = get_ptr_next_block(current_block);
            auto prev_block = get_ptr_prev_block(current_block);
            size_t size_current_block = get_block_size(current_block);
            block_metadata_size = get_block_metadata_size(current_block);
            auto shift = reinterpret_cast<unsigned char*>(current_block) - block_metadata_size + size_current_block;
            block_pointer_t* block_border = reinterpret_cast<block_pointer_t*>(shift);
            if ((*next_block) == nullptr)
            {
                ptrdiff_t remains = reinterpret_cast<unsigned char*>(global_right_bound) - shift;
                if ((remains >= block_size) && (global_right_bound != block_border) && (remains < best_size))
                {
                    if (remains - block_size < block_metadata_size)
                    {
                        temp_size = remains;
                    }
                    if (temp_size != block_size)
                    {
                        warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                            + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                            + "has been increased to: " + std::to_string(temp_size) + "bytes");
                    }
                    auto new_block = insert_in_memory(temp_size, reinterpret_cast<block_pointer_t>(shift));
                    (*next_block) = new_block;

                    auto prev = get_ptr_prev_block(new_block);
                    (*prev) = current_block;
                    (*avail_size) -= block_size;
                    return new_block; // !!!!!!!!!!!!!!!!!!!

                }
                else
                {
                    if ((*best_space) != nullptr)
                    {
                        if (best_size - block_size < block_metadata_size)
                        {
                            warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                                + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                                + "has been increased to: " + std::to_string(best_size) + "bytes");
                            block_size = best_size;
                        }
                        auto new_block = insert_in_memory(block_size, *best_space);
                        auto next_in_new_block = get_ptr_next_block(new_block);
                        auto prev_in_new_block = get_ptr_prev_block(new_block);

                        (*next_in_new_block) = next_block_for_new;
                        (*prev_in_new_block) = prev_block_for_new;
                        
                        if (next_block_for_new != nullptr && prev_block_for_new == nullptr)
                        {
                            (*ptr_on_first_block) = new_block;
                            auto prev = get_ptr_prev_block(next_block_for_new);
                            (*prev) = new_block;

                            (*avail_size) -= best_size;
                            return new_block;
                        }
                        else if (next_block_for_new == nullptr && prev_block_for_new != nullptr)
                        {
                            auto next = get_ptr_next_block(prev_block_for_new);
                            (*next) = new_block;

                            (*avail_size) -= best_size;
                            return new_block;
                        }
                        auto temp_ptr_prev = get_ptr_next_block(prev_block_for_new);
                        (*temp_ptr_prev) = new_block;

                        auto temp_ptr_next = get_ptr_prev_block(next_block_for_new);
                        (*temp_ptr_next) = new_block;
                        (*avail_size) -= best_size;
                        return new_block; // !!!!!!!!!!!!!!!!!!!


                    }
                    else
                    {
                        error_with_guard(get_typename() + "::best_fit(block_size_t block_size)" 
                            + "ERROR: there was no space for a block in the allocated memory area : ");
                        throw std::bad_alloc();
                    }
                }
            }
            else
            {
                auto right_border = reinterpret_cast<unsigned char*>(*(next_block)) - get_block_metadata_size(*next_block);
                auto left_border = reinterpret_cast<unsigned char*>(current_block) - block_metadata_size + (size_current_block);
                if (left_border - right_border == 0)
                {
                    current_block = (*next_block); // !!!
                    next_block = get_ptr_next_block(current_block);
                }
                else
                {
                    if (right_border - left_border >= block_size)
                    {
                        if (right_border - left_border < best_size)
                        {
                            best_size = right_border - left_border;
                            (*best_space) = reinterpret_cast<block_pointer_t>(left_border);

                            next_block_for_new = (*next_block); // change ptrs
                            prev_block_for_new = current_block;

                        }
                        current_block = *next_block;
                        next_block = get_ptr_next_block(current_block);
                        
                    }
                    else
                    {
                        current_block = *next_block;
                        next_block = get_ptr_next_block(current_block);
                    }
                }
            }
        }
        if ((*avail_size) < block_metadata_size)
        {
             error_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                + "ERROR: is not enough space for the block : " + "space_size = " + std::to_string(*space_size)
                + "block size = " + std::to_string(block_size));
            throw std::bad_alloc();
        }
    }
}

allocator::block_pointer_t allocator_boundary_tags::worst_fit(block_size_t block_size)
{
    block_pointer_t* ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    size_t* space_size = get_space_size();
    auto avail_size = get_allocator_avail_size();
    if ((*ptr_on_first_block) == nullptr)
    {
        if ((*avail_size) >= block_size)
        {
            if ((*avail_size) - block_size < get_block_metadata_size())
            {
                warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                    + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                    + "has been increased to: " + std::to_string((*avail_size)) + "bytes");
                block_size = (*avail_size);
            }
            block_pointer_t ptr_on_space_size = get_ptr_on_space_size();
            auto temp = insert_in_memory(block_size, ptr_on_space_size);
            (*ptr_on_first_block) = temp;
            (*avail_size) -= block_size;
            return temp; 
        }
        else
        {
            error_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                + "ERROR: is not enough space for the block : " + "space_size = " + std::to_string(*space_size)
                + "block size = " + std::to_string(block_size));
            throw std::bad_alloc();
        }
    }
    else
    {
        auto current_block = (*ptr_on_first_block);
        block_pointer_t temp_for_get = get_ptr_on_space_size();
        block_pointer_t global_right_bound = reinterpret_cast<block_pointer_t>((reinterpret_cast<unsigned char*>(temp_for_get) + (*space_size)));
        block_pointer_t ptr = nullptr;
        block_pointer_t* worst_space = &ptr;
        block_pointer_t prev_block_for_new;
        block_pointer_t next_block_for_new;
        size_t worst_size = 1;
        block_size_t free_size = ((reinterpret_cast<unsigned char*>(current_block) - get_block_metadata_size(current_block))
        - (reinterpret_cast<unsigned char*>(_trusted_memory) + get_global_metadata_size()));
        size_t block_metadata_size = get_block_metadata_size(current_block);
        size_t temp_size = block_size;
        if (free_size > 0)
        {
            if (free_size > worst_size && free_size > block_size)
            {
                if (free_size - block_size < block_metadata_size)
                {
                    temp_size = free_size;
                }
                worst_size = free_size;
                (*worst_space) = reinterpret_cast<block_pointer_t>((reinterpret_cast<unsigned char*>(_trusted_memory) + get_global_metadata_size()));
                auto next = get_ptr_next_block(current_block);

                next_block_for_new = (*next); // change ptrs
                prev_block_for_new = current_block;

            }
        }
        while((*avail_size) >= block_metadata_size)
        {
            block_pointer_t* next_block = get_ptr_next_block(current_block);
            auto prev_block = get_ptr_prev_block(current_block);
            size_t size_current_block = get_block_size(current_block);
            block_metadata_size = get_block_metadata_size(current_block);
            auto shift = reinterpret_cast<unsigned char*>(current_block);
            shift = shift - block_metadata_size + size_current_block;
            block_pointer_t* block_border = reinterpret_cast<block_pointer_t*>(shift);
            if ((*next_block) == nullptr)
            {
                ptrdiff_t remains = reinterpret_cast<unsigned char*>(global_right_bound) - shift;
                if ((remains >= block_size) && (global_right_bound != block_border) && (remains > worst_size))
                {
                    if (remains - block_size < block_metadata_size)
                    {
                        temp_size = remains;
                    }
                    if (temp_size != block_size)
                    {
                        warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                            + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                            + "has been increased to: " + std::to_string(temp_size) + "bytes");
                    }
                    auto new_block = insert_in_memory(temp_size, reinterpret_cast<block_pointer_t>(shift));
                    (*next_block) = new_block;

                    auto prev = get_ptr_prev_block(new_block);
                    (*prev) = current_block;

                    return new_block; // !!!!!!!!!!!!!!!!!!!

                }
                else
                {
                    if ((*worst_space) != nullptr)
                    {
                        if (worst_size - block_size < block_metadata_size)
                        {
                            warning_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                                + "the requested memory for the block: " + std::to_string(block_size-get_block_metadata_size())
                                + "has been increased to: " + std::to_string(worst_size) + "bytes");
                            block_size = worst_size;
                        }
                        auto new_block = insert_in_memory(block_size, *worst_space);
                        auto next_in_new_block = get_ptr_next_block(new_block);
                        auto prev_in_new_block = get_ptr_prev_block(new_block);

                        (*next_in_new_block) = next_block_for_new;
                        (*prev_in_new_block) = prev_block_for_new;

                        if (next_block_for_new != nullptr && prev_block_for_new == nullptr)
                        {
                            (*ptr_on_first_block) = new_block;
                            auto prev = get_ptr_prev_block(next_block_for_new);
                            (*prev) = new_block;

                            return new_block;
                        }
                        else if (next_block_for_new == nullptr && prev_block_for_new != nullptr)
                        {
                            auto next = get_ptr_next_block(prev_block_for_new);
                            (*next) = new_block;

                            return new_block;
                        }
                        auto temp_ptr_prev = *get_ptr_next_block(prev_block_for_new);
                        temp_ptr_prev = new_block;

                        auto temp_ptr_next = *get_ptr_prev_block(next_block_for_new);
                        temp_ptr_next = new_block;
                        return new_block; // !!!!!!!!!!!!!!!!!!!


                    }
                    else
                    {
                        error_with_guard(get_typename() + "::worst_fit(block_size_t block_size)" 
                            + "ERROR: there was no space for a block in the allocated memory area : ");
                        throw std::bad_alloc();
                    }
                }
            }
            else
            {
                auto right_border = reinterpret_cast<unsigned char*>(*(next_block)) - get_block_metadata_size(*next_block);
                auto left_border = reinterpret_cast<unsigned char*>(current_block) - block_metadata_size+ (size_current_block);
                if (left_border - right_border == 0)
                {
                    current_block = (*next_block); // !!!
                    next_block = get_ptr_next_block(current_block);
                }
                else
                {

                    if (right_border - left_border >= block_size)
                    {
                        if (right_border - left_border > worst_size)
                        {
                            worst_size = right_border - left_border;
                            (*worst_space) = reinterpret_cast<block_pointer_t>(left_border);

                            next_block_for_new = (*next_block); // change ptrs
                            prev_block_for_new = current_block;

                        }
                        current_block = *next_block;
                        next_block = get_ptr_next_block(current_block);
                        
                    }
                    else
                    {
                        current_block = *next_block;
                        next_block = get_ptr_next_block(current_block);
                    }
                }
            }
        }
        if ((*avail_size) < block_metadata_size)
        {
             error_with_guard(get_typename() + "::first_fit(block_size_t block_size)" 
                + "ERROR: is not enough space for the block : " + "space_size = " + std::to_string(*space_size)
                + "block size = " + std::to_string(block_size));
            throw std::bad_alloc();
        }
    }
}

allocator::block_pointer_t allocator_boundary_tags::choosing_fit_mode(block_size_t block_size)
{
    block_pointer_t at;
    switch(*get_fit_mode())
    {
        case allocator_with_fit_mode::fit_mode::first_fit:
            at = first_fit(block_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_best_fit:
            at = best_fit(block_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            at = worst_fit(block_size);
            break;

    }

    return at;
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard(get_typename() + "::allocate(void *) called")
        ->debug_with_guard(get_typename() + "::allocate(void *) called");

    std::lock_guard<std::mutex> guard(get_mutex());
    block_size_t new_block_size = sizeof(allocator_boundary_tags*)+2*sizeof(block_pointer_t*)+sizeof(size_t)+value_size*values_count;

    block_pointer_t at = choosing_fit_mode(new_block_size);


    debug_with_guard(get_typename() + "::allocate(void *) allocated " + std::to_string(new_block_size - get_block_metadata_size())
            + "(+" + std::to_string(get_block_metadata_size()) + ") bytes");
    state_blocks(get_typename() + "::allocate(void *)");
    information_with_guard(get_typename() + "::allocate(void *) available size is " +
            std::to_string(*get_allocator_avail_size()) + " bytes")
        ->trace_with_guard(get_typename() + "::allocate(void *) successfuly finished")
        ->debug_with_guard(get_typename() + "::allocate(void *) successfuly finished");
    
    return at;
    
}

inline allocator_boundary_tags* allocator_boundary_tags::get_block_allocator(allocator::block_pointer_t block) const
{
    auto ptr_on_next_block = reinterpret_cast<block_pointer_t*>(block);
    ptr_on_next_block--;

    auto ptr_on_prev_block = reinterpret_cast<block_pointer_t*>(ptr_on_next_block);
    ptr_on_prev_block--;

    auto ptr_on_size = reinterpret_cast<size_t*>(ptr_on_prev_block);
    ptr_on_size--;

    auto ptr_on_allocator = reinterpret_cast<allocator_boundary_tags**>(ptr_on_size);
    (ptr_on_allocator--);

    return *ptr_on_allocator;
}


void allocator_boundary_tags::deallocate(
    void *at)
{
    std::lock_guard<std::mutex> guard(get_mutex());

    trace_with_guard(get_typename() + "::deallocate(void *) called")
        ->debug_with_guard(get_typename() + "::deallocate(void *) called");

    if (at == nullptr)
    {
        return;
    }
    
    unsigned char* mem_begin = reinterpret_cast<unsigned char*>(
            get_ptr_on_space_size());
    unsigned char* mem_end = reinterpret_cast<unsigned char*>(
            get_ptr_on_space_size()) + (*get_space_size());
    unsigned char* shift = reinterpret_cast<unsigned char*>(
            at);
    auto block_allocator = get_block_allocator(at);
    if (shift < mem_begin || shift >= mem_end || block_allocator != this)
    { 
        error_with_guard(get_typename() + "::deallocate(void *) tried to deallocate non-related memory");        
        throw std::logic_error("try of deallocation non-related memory");
    }

    auto next_block = get_ptr_next_block(at);
    auto prev_block = get_ptr_prev_block(at);
    auto block_size = get_block_size(at);
    auto ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    auto space_size = get_space_size();


    if ((*next_block) == nullptr && (*prev_block) == nullptr)
    {
        (*ptr_on_first_block) = nullptr;
    }
    else if ((*next_block) != nullptr && (*prev_block) == nullptr)
    {
        (*ptr_on_first_block) = (*next_block);
        (*next_block) = nullptr;
    }
    else if ((*next_block) == nullptr && (*prev_block) != nullptr)
    {
        auto temp_ptr = get_ptr_next_block(*prev_block);
        (*temp_ptr) = nullptr;
        (*prev_block) = nullptr;
    }
    else if ((*next_block) != nullptr && (*prev_block) != nullptr)
    {
        auto temp_ptr_next = get_ptr_next_block(*prev_block);
        (*temp_ptr_next) = (*next_block);

        auto temp_ptr_prev = get_ptr_prev_block(*next_block);
        (*temp_ptr_prev) = (*prev_block);

        (*next_block) = nullptr;
        (*prev_block) = nullptr;

    }
    auto avail_size = get_allocator_avail_size();
    (*avail_size) += block_size; 
    debug_with_guard(get_typename() + "::deallocate(void *) deallocated " + std::to_string(block_size - get_block_metadata_size())
            + "(+" + std::to_string(get_block_metadata_size()) + ") bytes");
    state_blocks(get_typename() + "::deallocate(void *)");
    information_with_guard(get_typename() + "::deallocate(void *) available size is " +
            std::to_string((*avail_size)) + " bytes")
        ->trace_with_guard(get_typename() + "::deallocate(void *) successfuly finished")
        ->debug_with_guard(get_typename() + "::deallocate(void *) successfuly finished");

}

inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    (*get_fit_mode()) = mode;
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    trace_with_guard(get_typename() + "::get_blocks_info() called");
    std::lock_guard<std::mutex> guard(get_mutex());

    auto block_array = create_blocks_info();

    trace_with_guard(get_typename() + "::get_blocks_info() successfuly finished");
    return block_array;
}

inline size_t allocator_boundary_tags::get_global_metadata_size()const
{
    auto ptr_on_space_size = reinterpret_cast<unsigned char*>( get_ptr_on_space_size() );
    auto ptr_on_meta = reinterpret_cast<unsigned char*>( _trusted_memory );

    return (ptr_on_space_size - ptr_on_meta);


}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::create_blocks_info()const
{
    trace_with_guard(get_typename() + "::create_blocks_info() called");

    std::vector<allocator_test_utils::block_info>blocks_info_array(0);
    auto ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);

    auto current_block = (*ptr_on_first_block);
    block_size_t unfilled_block_size;
    block_size_t global_metadata = get_global_metadata_size();
    block_size_t space_size = *get_space_size();
    unfilled_block_size = current_block == nullptr
    ? (reinterpret_cast<unsigned char*>(_trusted_memory) + space_size + global_metadata) 
    - (reinterpret_cast<unsigned char*>(_trusted_memory) + global_metadata)
    : ((reinterpret_cast<unsigned char*>(current_block) - get_block_metadata_size(current_block))
    - (reinterpret_cast<unsigned char*>(_trusted_memory) + get_global_metadata_size()));

    if (unfilled_block_size > 0)
    {
        blocks_info_array.push_back( {.block_size = unfilled_block_size, .is_block_occupied = false});
    }

    block_pointer_t next_block;
    block_size_t block_size;
    while (current_block != nullptr)
    {
        next_block = *get_ptr_next_block(current_block);

        block_size = get_block_size(current_block);
        blocks_info_array.push_back( {.block_size = block_size, .is_block_occupied = true});

        unsigned char* left_border = reinterpret_cast<unsigned char*>(current_block) - get_block_metadata_size(current_block) 
        + block_size;
        unsigned char* right_border = (next_block) == nullptr
        ? (reinterpret_cast<unsigned char*>(get_ptr_on_space_size()) + (*get_space_size()))
        : (reinterpret_cast<unsigned char*>(next_block) - get_block_metadata_size(next_block));
        
        unfilled_block_size = right_border - left_border;
        if (unfilled_block_size > 0)
        {
            blocks_info_array.push_back( {.block_size = unfilled_block_size, .is_block_occupied = false});
        }
        current_block = next_block;
    }

    trace_with_guard(get_typename() + "::create_blocks_info() successfuly finished");
    return blocks_info_array;
}

void allocator_boundary_tags::state_blocks(const std::string name_of_function)const
{
    std::ostringstream str_stream;
    auto block_array = create_blocks_info();

    for (auto elem : block_array)
    {
        if (elem.is_block_occupied)
        {
            str_stream << " occup " << elem.block_size - get_block_metadata_size() << " |";
        }
        else
        {
            str_stream << " avail " << elem.block_size << " |";
        }
    }

    debug_with_guard("std::vector<allocator_test_utils::block_info> allocator_boundary_tags::state_blocks(): blocks state: |" 
    + str_stream.str() + ".");

}

inline logger *allocator_boundary_tags::get_logger() const
{
    auto ptr_on_first_block = reinterpret_cast<block_pointer_t*>(_trusted_memory);
    ptr_on_first_block++;

    auto ptr_size = reinterpret_cast<size_t*>(ptr_on_first_block);
    ptr_size++;

    auto free_size = reinterpret_cast<size_t*>(ptr_size);
    free_size++;


    auto ptr_allocator = reinterpret_cast<allocator**>(free_size);
    ptr_allocator++;

    auto ptr_logger = reinterpret_cast<logger**>(ptr_allocator);

    return *ptr_logger;
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return "allocator_boundary_tags";
}