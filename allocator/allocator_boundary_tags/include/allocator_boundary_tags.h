#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>

class allocator_boundary_tags final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;


public:
    
    ~allocator_boundary_tags() override;
    
    allocator_boundary_tags(
        allocator_boundary_tags const &other);
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags const &other);
    
    allocator_boundary_tags(
        allocator_boundary_tags &&other) noexcept;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags &&other) noexcept;

public:
    
    explicit allocator_boundary_tags(
        size_t space_size,
        allocator *parent_allocator = nullptr,
        logger *logger = nullptr,
        allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:
    
    [[nodiscard]] void *allocate(
        size_t value_size,
        size_t values_count) override;
    
    void deallocate(
        void *at) override;

public:
    
    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;
private:

    block_pointer_t choosing_fit_mode(block_size_t block_size);
    block_pointer_t first_fit(block_size_t block_size);
    block_pointer_t best_fit(block_size_t block_size);
    block_pointer_t worst_fit(block_size_t block_size);
    inline block_pointer_t get_ptr_on_space_size() const;
    inline std::mutex &get_mutex() const;
    inline size_t get_global_metadata_size()const;
    inline size_t* get_space_size() const;
    inline size_t* get_allocator_avail_size() const;
    inline allocator_with_fit_mode::fit_mode* get_fit_mode() const;
    block_pointer_t insert_in_memory(block_size_t block_size, block_pointer_t at);
    inline block_pointer_t* get_ptr_next_block(block_pointer_t new_block) const;
    inline block_pointer_t* get_ptr_prev_block(block_pointer_t new_block) const;
    inline allocator_boundary_tags* get_block_allocator(block_pointer_t) const;
    inline size_t get_block_size(block_pointer_t block) const;
    inline size_t get_block_metadata_size() const;
    inline size_t get_block_metadata_size(block_pointer_t block)const;
    std::vector<allocator_test_utils::block_info> create_blocks_info()const;
    void state_blocks(const std::string name_of_function)const;


private:
    
    inline allocator *get_allocator() const override;

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:
    
    inline logger *get_logger() const override;

private:
    
    inline std::string get_typename() const noexcept override;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H