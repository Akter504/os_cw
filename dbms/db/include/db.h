#ifndef OPERATING_SYSTEMS_COURSE_WORK_DB
#define OPERATING_SYSTEMS_COURSE_WORK_DB

#include <allocator.h>
#include <allocator_with_fit_mode.h>
#include <logger.h>
#include <b_tree.h>
#include <custom_data.h>
#include <search_tree.h>
#include <iostream>
#include <memory>
#include <extra_utility.h>
#include <unistd.h>

class db final
{

	class setup_failure final:
		public std::logic_error
	{
			
	public:
		
		setup_failure(
			std::string const &error_msg);
		
	};

	class load_failure final:
		public std::logic_error
	{
			
	public:
		
		load_failure(
			std::string const &error_msg);
		
	};
	
	class invalid_path final:
		public std::logic_error
	{
			
	public:
		
		invalid_path();
		
	};

	class incorrect_file_name final:
		public std::logic_error
	{
			
	public:
		
		incorrect_file_name();
		
	};

	class insertion_of_struct_failure final:
		public std::logic_error
	{
		
	public:
	
        insertion_of_struct_failure();
	
	};

	class obtaining_of_nonexistent_key_attempt_exception final:
		public std::logic_error
	{
			
	public:
		
		obtaining_of_nonexistent_key_attempt_exception();
		
	};

	class insertion_of_existent_key_attempt_exception final:
		public std::logic_error
	{
			
	public:
		
		insertion_of_existent_key_attempt_exception();
		
	};

	class insertion_of_existent_struct_attempt_exception final:
		public std::logic_error
	{
			
	public:
		
		insertion_of_existent_struct_attempt_exception();
		
	};


	class updating_of_nonexistent_key_attempt_exception final:
		public std::logic_error
	{
			
	public:
		
		updating_of_nonexistent_key_attempt_exception();
		
	};

	class disposal_of_nonexistent_key_attempt_exception final:
		public std::logic_error
	{
			
	public:
		
		disposal_of_nonexistent_key_attempt_exception();
		
	};

	class disposal_of_nonexistent_struct_attempt_exception final:
		public std::logic_error
	{
			
	public:
		
		disposal_of_nonexistent_struct_attempt_exception();
		
	};

public:

	enum class mode
	{
		uninitialized,
		in_memory_cache,
		file_system
	};

    enum class b_tree_variants
    {
        b,
		b_plus,
		b_star,
		b_star_plus
    };

    enum class allocator_variant
    {
        global_heap,
		sorted_list,
		buddy_system,
		boundary_tags,
		red_black_tree
    };

public:
        
    class collection final:
        protected allocator_guardant
    {

    private:

        search_tree<flyweight_tkey, custom_data*> *_data;
        b_tree_variants _tree_variant;
    	std::shared_ptr<allocator> _allocator;
		allocator_variant _allocator_variant;
		allocator_with_fit_mode::fit_mode _fit_mode;
        size_t _records_cnt = 0;
        size_t _disposed_cnt = 0;

    public:

        collection(collection const &other);
        collection(collection &&el) noexcept;
        collection &operator=(collection const &other);
        collection &operator=(collection &&el)noexcept;
        ~collection();
    
    public:

        explicit collection(
            b_tree_variants tree_variant,
            allocator_variant allocator_variant,
            allocator_with_fit_mode::fit_mode mode,
            size_t t_for_b_trees = 8);

    public:

        tvalue obtain(
            tkey const &key, 
            std::string const &path);

        std::vector<std::pair<tkey,tvalue>> obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool const &lower_bound_inclusive,
            bool const &upper_bound_inclusive,   
            std::string const &path);
		
		std::pair<flyweight_tkey, tvalue> obtain_min(
			std::string const &path);
		
		std::pair<flyweight_tkey, tvalue> obtain_max(
			std::string const &path);
		
		std::pair<flyweight_tkey, tvalue> obtain_next(
			std::string const &path,
			tkey const &key);
        
        void insert(
            tkey const &key,
            tvalue const &value,
            std::string const &path);

        void insert(
            tkey const &key,
            tvalue &&value,
            std::string const &path);
        
        void update(
            tkey const &key,
            tvalue const &value,
            std::string const &path);
        
        void update(
            tkey const &key,
            tvalue &&value,
            std::string const &path);
        
        tvalue dispose(
            tkey const &key,
            std::string const &path);
		
		size_t get_records_cnt();


	public:

		void consolidate(
			std::string const &path);

		void load(
			tkey const &key,
			tvalue &&value,
			std::string const &path,
			long file_pos);

    private:
	
		void collect_garbage(
			std::string const &path);

		[[nodiscard]] inline allocator *get_allocator() const final;

    private:

		void clear();
        void move_from(collection &&other);
        void copy_from(collection const &other);

    };

    class schema final
    {
        private:

            search_tree<flyweight_tkey, std::shared_ptr<collection>> *_collections;
            b_tree_variants _tree_variant;
        
        public:
	
		~schema();
		
		schema(
			schema const &other);
		
		schema &operator=(
			schema const &other);
		
		schema(
			schema &&other) noexcept;
		
		schema &operator=(
			schema &&other) noexcept;

        public:
	
		void add(
			std::string const &collection_name,
			b_tree_variants tree_variant,
			allocator_variant allocator_variant,
			allocator_with_fit_mode::fit_mode fit_mode,
			size_t t_for_b_trees = 8);
		
		collection dispose(
			std::string const &collection_name);
		
		collection &obtain(
			std::string const &collection_name);
	
    public:

        explicit schema(
            b_tree_variants tree_variant,
			size_t t_for_b_trees = 8);

	public:
	
		void consolidate(
			std::string const &path);
	
	private:
	
		void clear();
        void move_from(schema &&other);
        void copy_from(schema const &other);

    };

    class pool final
    {
        private:

            search_tree<flyweight_tkey, std::shared_ptr<schema>> *_schemas;
            b_tree_variants _tree_variant;

        public:
	
            ~pool();
            
            pool(
                pool const &other);
            
            pool &operator=(
                pool const &other);
            
            pool(
                pool &&other) noexcept;
            
            pool &operator=(
                pool &&other) noexcept;
        
        public:

        explicit pool(
            b_tree_variants tree_variant,
			size_t t_for_b_trees = 8);

        public:
	
		void add(
			std::string const &schema_name,
			b_tree_variants tree_variant,
			size_t t_for_b_trees = 8);
		
		schema dispose(
			std::string const &schema_name);
		
		schema &obtain(
			std::string const &schema_name);
	
	public:
	
		void consolidate(
			std::string const &path);
	
	private:
	
		void clear();
        void move_from(pool &&other);
        void copy_from(pool const &other);

    };

private:

	size_t _id;
	mode _mode = mode::uninitialized;
	b_tree<flyweight_tkey, std::shared_ptr<pool>> _pools;

public:

	static db *get_instance();

public:

	db(
		db const &) = delete;
	
	db(
		db &&) = delete;

public:

	size_t get_id();
	mode get_mode();

public:

	void free_pools();

	db *setup(
		size_t id,
		mode mode);
	
	db *load_db(
		std::string const &path);
		
	db *clear();

	db *add_pool(
		std::string const &pool_name,
		b_tree_variants tree_variant,
		size_t t_for_b_trees = 8);
	
	db *dispose_pool(
		std::string const &pool_name);
	
	db *add_schema(
		std::string const &pool_name,
		std::string const &schema_name,
		b_tree_variants tree_variant,
		size_t t_for_b_trees = 8);
	
	db *dispose_schema(
		std::string const &pool_name,
		std::string const &schema_name);
	
	db *add_collection(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		b_tree_variants tree_variant,
		allocator_variant allocator_variant,
		allocator_with_fit_mode::fit_mode fit_mode,
		size_t t_for_b_trees = 8);
	
	db *dispose_collection(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name);
	
	db *add(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key,
		tvalue const &value);
	
	db *add(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key,
		tvalue &&value);
	
	db *update(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key,
		tvalue &&value);
	
	db *update(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key,
		tvalue const &value);
	
	tvalue dispose(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key);
	
	tvalue obtain(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key);
	
	std::vector<std::pair<tkey, tvalue>> obtain_between(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive);
	
	std::pair<flyweight_tkey, tvalue> obtain_min(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name);
		
	std::pair<flyweight_tkey, tvalue> obtain_max(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name);
		
	std::pair<flyweight_tkey, tvalue> obtain_next(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key);
	
	db *consolidate();

	size_t get_collection_records_cnt(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name);

private:

	db();

private:

	void add(
		std::string const &pool_name,
		b_tree_variants tree_variant,
		size_t t_for_b_trees = 8);
	
	db::pool dispose(
		std::string const &pool_name);
	
	db::pool &obtain(
		std::string const &pool_name);
	
	void load_collection(
		std::string const &prefix,
		std::string const &ppool_name,
		std::string const &pschema_name,
		std::string const &pcollection_name);

private:
	
	db &throw_if_initialized_at_setup();
	db &throw_if_unitialized();
	db &throw_if_invalid_setup(size_t id, db::mode mode);
	db &throw_if_invalid_path(std::string const &path);
	db &throw_if_invalid_file_name(std::string const &file_name);
    
};

#endif //OPERATING_SYSTEMS_COURSE_WORK_DB