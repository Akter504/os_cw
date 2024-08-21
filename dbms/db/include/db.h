#include <allocator.h>
#include <logger.h>
#include <b_tree.h>
#include <custom_data.h>
#include <search_tree.h>
#include <iostream>
#include <memory>

class db final
{

	class obtaining_of_nonexistent_key_attempt_exception final:
		public std::logic_error
	{
			
	public:
		
		obtaining_of_nonexistent_key_attempt_exception();
		
	};

	class insertion_of_existent_key_attempt_exception_exception final:
		public std::logic_error
	{
			
	public:
		
		insertion_of_existent_key_attempt_exception_exception();
		
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

    enum class allocators_variant
    {
        global_heap,
		sorted_list,
		buddy_system,
		boundary_tags,
		red_black_tree
    };

protected:
        
    class collection final:
        protected allocator_guardant
    {

    private:

        search_tree<std::shared_ptr<std::string>, custom_data*> _data;
        b_tree_variants _tree_variant;
    	std::shared_ptr<allocator> _allocator;
		allocator_variant _allocator_variant;
		allocator_with_fit_mode::fit_mode _fit_mode;
        size_t _records_cnt;
        size_t _disposed_cnt;

    public:

        collection(collection const &other);
        collection(collection &&el) noexcept;
        collection &operator=(collection const &other);
        collection &operator=(collection &&el)noexcept;
        ~collection();
    
    public:

        explicit collection(
            b_tree_variants const &tree_variant,
            allocators_variant const &allocator_variant,
            allocator_with_fit_mode::fit_mode const &mode,
            size_t t_for_b_trees = 8);

    public:

        tvalue obtain(
            tkey cosnt &key, 
            std::string const &path);

        std::vector<std::pair<tkey,tvalue>> obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool const &lower_bound_inclusive,
            bool const &upper_bound_inclusive,   
            std::string const &path);
        
        void insert(
            tkey const &key,
            tvalue const &value,
            std::string const &path);

        void insert(
            tkey const &key,
            tvalue const &&value,
            std::string const &path);
        
        void update(
            tkey const &key,
            tvalue const &value,
            std::string const &path);
        
        void update(
            tkey const &key,
            tvalue const &&value,
            std::string const &path);
        
        tvalue dispose(
            tkey const &key,
            std::string const &path);

    private:
	
		[[nodiscard]] inline allocator *get_allocator() const final;

    private:
	
		void clear();
        void move_from(collection &&other);
        void copy_from(collection const &other);

    };

    class schema final
    {
        private:

            search_tree<std::shared_ptr<std::string>, collection> *_collections;
            search_tree_variant _tree_variant;
        
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
			search_tree_variant tree_variant,
			allocator_variant allocator_variant,
			allocator_with_fit_mode::fit_mode fit_mode,
			size_t t_for_b_trees = 8);
		
		void dispose(
			std::string const &collection_name);
		
		collection &obtain(
			std::string const &collection_name);
	
    public:

        explicit schema(
            b_tree_variants const &tree_variant);

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

            search_tree<std::shared_ptr<std::string>, schema> *_schemas;
            search_tree_variant _tree_variant;

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
            b_tree_variants const &tree_variant);

        public:
	
		void add(
			std::string const &schema_name,
			search_tree_variant tree_variant,
			size_t t_for_b_trees = 8);
		
		void dispose(
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
	mode _mode;
	b_tree<std::shared_ptr<std::string>, pool> _pools;

public:

	static db_storage *get_instance();

public:

	db_storage(
		db_storage const &) = delete;
	
	db_storage(
		db_storage &&) = delete;

public:

	size_t get_id();

public:

	db_storage *setup(
		size_t id,
		mode mode);
	
	db_storage *load_db(
		std::string path);
		
	db_storage *clear();

	db_storage *add_pool(
		std::string const &pool_name,
		search_tree_variant tree_variant,
		size_t t_for_b_trees = 8);
	
	db_storage *dispose_pool(
		std::string const &pool_name);
	
	db_storage *add_schema(
		std::string const &pool_name,
		std::string const &schema_name,
		search_tree_variant tree_variant,
		size_t t_for_b_trees = 8);
	
	db_storage *dispose_schema(
		std::string const &pool_name,
		std::string const &schema_name);
	
	db_storage *add_collection(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		search_tree_variant tree_variant,
		allocator_variant allocator_variant,
		allocator_with_fit_mode::fit_mode fit_mode,
		size_t t_for_b_trees = 8);
	
	db_storage *dispose_collection(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name);
	
	db_storage *add(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key,
		tvalue const &value);
	
	db_storage *add(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key,
		tvalue &&value);
	
	db_storage *update(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key,
		tvalue &&value);
	
	db_storage *update(
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
	
	std::pair<tkey, tvalue> obtain_min(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name);
		
	std::pair<tkey, tvalue> obtain_max(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name);
		
	std::pair<tkey, tvalue> obtain_next(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name,
		tkey const &key);
	
	db_storage *consolidate();

	size_t get_collection_records_cnt(
		std::string const &pool_name,
		std::string const &schema_name,
		std::string const &collection_name);

private:

	db_storage();

private:

	void collect_garbage(
		std::string const &path);

private:

	void add(
		std::string const &pool_name,
		search_tree_variant tree_variant,
		size_t t_for_b_trees = 8);
	
	void dispose(
		std::string const &pool_name);
	
	pool &obtain(
		std::string const &pool_name);
	
	void load_collection(
		std::string prefix,
		std::string pool_name,
		std::string schema_name,
		std::string collection_name);

    
};