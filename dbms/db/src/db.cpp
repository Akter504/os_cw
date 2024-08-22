#include <db.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>

#pragma region collection implementation

db::collection::collection(
    b_tree_variants const &tree_variant,
    allocators_variant const &allocator_variant,
    allocator_with_fit_mode::fit_mode const &mode,
    size_t t_for_b_trees):
        _tree_variant(tree_variant),
        _allocator_variant(allocator_variant),
        _fit_mode(mode),
        _records_cnt(0),
        _disposed_cnt(0)
{
    switch (tree_variant)
    {
        case b_tree_variants::b:
            //break;
        case b_tree_variants::b_plus:
            //break;
        case b_tree_variants::b_star:
            //break;
        case b_tree_variants::b_star_plus:
            //break;
        default:
            _data = new b_tree<flyweight_tkey, tdata *>(t_for_b_trees, tkey_comparer());
            break;
    }

    try
    {
        switch(allocator_variant)
        {
            case allocator_variant::allocator_boundary_tags:
                _allocator = std::make_shared<allocator_boundary_tags>(1 << 22, nullptr, nullptr, _fit_mode);
                break;
            case allocator_variant::buddy_system:
                _allocator = std::make_shared<allocator_buddies_system>(22, nullptr, nullptr, _fit_mode);
                break;
            case allocator_variant::global_heap:
                _allocator = std::make_shared<allocator_global_heap>();
                break;
            case allocator_variant::red_black_tree:
                _allocator = std::make_shared<allocator_red_black_tree>(1 << 22, nullptr, nullptr, _fit_mode);
                break;
            case allocator_variant::sorted_list:
                _allocator = std::make_shared<allocator_sorted_list>(1 << 22, nullptr, nullptr, _fit_mode);
                break;

        }
    }
    catch (std::bad_alloc const &)
    {
        delete _data;
        throw;
    }
}

db::collection::collection(collection const &other) noexcept
{
    copy_from(other);
}

db::collection::collection(collection &&other)
{
    
    move_from(std::move(other));
}

db::collection::collection::operator=(collection const &other) noexcept
{
    if (this != &other)
    {
        clear();

        copy_from(other);
    }
    
    return *this;
}

db::collection::collection::operator=(collection &&other)
{
    if (this != &other)
    {
        clear();

        move_from(std::move(other));
    }
    
    return *this;
}

db::collection::~collection()
{
    clear();
}

void db::insert(
    tkey const &key,
    tvalue const &value,
    std::string const &path)
{

    collect_garbage(path);

    std::shared_ptr<std::string> fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
    custom_data* data = nullptr;

    try
    {
        if (get_instance()->_mode == mode::file_system)
        {
            data = reinterpret_cast<custom_data_file *>(allocate_with_guard(sizeof(custom_data_file), 1));
            allocator::construct(reinterpret_cast<custom_data_file *>(data));
        }
        else
        {
            data = reinterpret_cast<custom_data_ram *>(allocate_with_guard(sizeof(custom_data_ram), 1));
            allocator::construct(reinterpret_cast<custom_data_ram *>(data), value);
        }
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(data);
        throw;
        // TODO
    }

    try
    {
        if (get_instance()->_mode == mode::file_system)
        {
            reinterpret_cast<custom_data_file *>(data)->serialize(path, key, value);
        }
        
        _data->insert(fw_key, data);
    }
    catch (std::ios::failure const &)
    {
        allocator::destruct(data);
        deallocate_with_guard(data);
        throw std::ios::failure("Failed to write data");
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(data);
        throw;
    }
    catch (search_tree<tkey, custom_data *>::insertion_of_existent_key_attempt_exception_exception const &)
    {
        allocator::destruct(data);
        deallocate_with_guard(data);
        throw db::insertion_of_existent_key_attempt_exception();
        // TODO
    }
    
    ++_records_cnt;
    

}

void db::insert(
    tkey const &key,
    tvalue const &&value,
    std::string const &path)
{

    collect_garbage(path);

    std::shared_ptr<std::string> fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
    custom_data* data = nullptr;

    try
    {
        if (get_instance()->_mode == mode::file_system)
        {
            data = reinterpret_cast<custom_data_file *>(allocate_with_guard(sizeof(custom_data_file), 1));
            allocator::construct(reinterpret_cast<custom_data_file *>(data));
        }
        else
        {
            data = reinterpret_cast<custom_data_ram *>(allocate_with_guard(sizeof(custom_data_ram), 1));
            allocator::construct(reinterpret_cast<custom_data_ram *>(data), value);
        }
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(data);
        throw;
        // TODO
    }

    try
    {
        if (get_instance()->_mode == mode::file_system)
        {
            reinterpret_cast<custom_data_file *>(data)->serialize(path, key, value);
        }
        
        _data->insert(fw_key, data);
    }
    catch (std::ios::failure const &)
    {
        allocator::destruct(data);
        deallocate_with_guard(data);
        throw std::ios::failure("Failed to write data");
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(data);
        throw;
    }
    catch (search_tree<tkey, custom_data *>::insertion_of_existent_key_attempt_exception_exception const &)
    {
        allocator::destruct(data);
        deallocate_with_guard(data);
        throw db::insertion_of_existent_key_attempt_exception();
        // TODO
    }
    
    ++_records_cnt;
    

}

void db::collection::update(
    tkey const &key,
    tvalue const &value,
    std::string const &path)
{
    collect_garbage(path);

    std::shared_ptr<std::string> fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
    custom_data* data = nullptr;

    try
    {
        if (get_instance()->_mode == mode::file_system)
        {
            data = reinterpret_cast<custom_data_file *>(allocate_with_guard(sizeof(custom_data_file), 1));
            allocator::construct(reinterpret_cast<custom_data_file *>(data));
        }
        else
        {
            data = reinterpret_cast<custom_data_ram *>(allocate_with_guard(sizeof(custom_data_ram), 1));
            allocator::construct(reinterpret_cast<custom_data_ram *>(data), std::move(value));
        }
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(data);
        throw;
        // TODO
    }

    try
    {
        if (get_instance()->_mode == mode::file_system)
        {
            reinterpret_cast<custom_data_file *>(data)->serialize(path, key, value);
        }

        _data->update(fw_key,data);
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(data);
        throw;
    }
    catch (std::ios::failure const &)
    {
        allocator::destruct(data);
        deallocate_with_guard(data);
        throw std::ios::failure("Failed to write data");
    }
    catch (search_tree<tkey, custom_data *>::updating_of_nonexistent_key_attempt_exception const &)
    {
        allocator::destruct(data);
        deallocate_with_guard(data);
        throw db::updating_of_nonexistent_key_attempt_exception();
        // TODO
    }
    


}

void db::collection::update(
    tkey const &key,
    tvalue const &&value,
    std::string const &path)
{
    collect_garbage(path);

    std::shared_ptr<std::string> fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
    custom_data* data = nullptr;

    try
    {
        if (get_instance()->_mode == mode::file_system)
        {
            data = reinterpret_cast<custom_data_file *>(allocate_with_guard(sizeof(custom_data_file), 1));
            allocator::construct(reinterpret_cast<custom_data_file *>(data));
        }
        else
        {
            data = reinterpret_cast<custom_data_ram *>(allocate_with_guard(sizeof(custom_data_ram), 1));
            allocator::construct(reinterpret_cast<custom_data_ram *>(data), std::move(value));
        }
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(data);
        throw;
        // TODO
    }

    try
    {
        if (get_instance()->_mode == mode::file_system)
        {
            reinterpret_cast<custom_data_file *>(data)->serialize(path, key, value);
        }

        _data->update(fw_key,data);
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(data);
        throw;
    }
    catch (std::ios::failure const &)
    {
        allocator::destruct(data);
        deallocate_with_guard(data);
        throw std::ios::failure("Failed to write data");
    }
    catch (search_tree<tkey, custom_data *>::updating_of_nonexistent_key_attempt_exception const &)
    {
        allocator::destruct(data);
        deallocate_with_guard(data);
        throw db::updating_of_nonexistent_key_attempt_exception();
        // TODO
    }
    


}

tvalue db::colletion::dispose(
    tkey const &key,
    std::string const path)
{
    std::shared_ptr<std::string> fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
    custom_data* data = nullptr;
    tvalue value;

    try
    {
        data = _data->dispose(fw_key);
    }
    catch (search_tree<tkey, custom_data *>::disposal_of_nonexistent_key_attempt_exception)
    {
        throw db_storage::disposal_of_nonexistent_key_attempt_exception();
        // TODO
    }

    if (get_instance()->_mode == mode::file_system)
    {
        try
        {
            value = dynamic_cast<custom_data_file*>(data)->deserialize(path);
        }
        catch (std::ios::failure const &)
        {
            allocator::destruct(data);
            deallocate_with_guard(data);
            throw std::ios::failure("Failed to parse disposed data");
        }
        
    }
    else
    {
        value = dynamic_cast<custom_data_ram*>(data)->value;
    }  
    
    

    allocator::destruct(data);
    deallocate_with_guard(data);

    _records_cnt--;
    _disposed_cnt++;
    return value;
}

tvalue db::collection::obtain(
    tkey const& key,
    std::string const &path)
{
    collect_garbage(path);

    flyweight_tkey fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
    custom_data* data = nullptr;

    try
    {
        data = _data.obtain(fw_key);
    }
    catch(search_tree<tkey, tdata *>::obtaining_of_nonexistent_key_attempt_exception)
    {
        throw db::obtaining_of_nonexistent_key_attempt_exception();
    }

    if (get_instance()->_mode == mode::file_system)
    {
        try
        {
            return dynamic_cast<custom_data_file *>(data)->deserialize(path);
        }
        catch (std::ios::failure const &)
        {
            throw std::ios::failure("Failed to read data");
        }
    }
    else
    {
        return dynamic_cast<custom_data_ram *>(data)->value;
    }
    
}

std::vector<std::pair<tkey,tvalue>> db::collection::obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool const &lower_bound_inclusive,
            bool const &upper_bound_inclusive,   
            std::string const &path)
{
    collect_garbage(path);

    flyweight_tkey fw_lower_bound = flyweight_string_pool::get_instance()->make_flyweight(lower_bound);
    flyweight_tkey fw_upper_bound = flyweight_string_pool::get_instance()->make_flyweight(upper_bound);
    std::vector<typename associative_container<flyweight_tkey, custom_data *>::key_value_pair> data;
    std::vector<std::pair<tkey, tvalue>> result;
    value_vec.reserve(data_vec.size());

    try
    {
        data = _data->obtain_between(fw_lower_bound, fw_upper_bound, lower_bound_inclusive,upper_bound_inclusive);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    for (auto kvp : data)
        if (get_instance()->_mode == mode::file_system)
        {
            try
            {
                result.emplace_back(kvp.key->get_str(), dynamic_cast<custom_data_file*>(data)->deserialize(path));
            }
            catch (std::ios::failure const &)
            {
                throw std::ios::failure("Failed to read data");
            }

        }
        else
        {
            result.emplace_back(kvp.key->get_str(), dynamic_cast<custom_data_ram*>(data));
        }
    
    return result;

}

std::pair<tkey, tvalue> db::obtain_min(
		std::string const &path)
{ }

std::pair<tkey, tvalue> db::obtain_max(
		std::string const &path)
{ }

std::pair<tkey, tvalue> db::obtain_next(
		std::string const &path,
        tkey const &key)
{ }

void db::collection::move_from(collection &&other)
{
    _tree_variant = other._tree_variant;
    switch (_tree_variant)
    {
    case b_tree_variants::b:
        //break;
    case b_tree_variants::b_plus:
        //break;
    case b_tree_variants::b_star:
        //break;
    case b_tree_variants::b_star_plus:
        //break;
    default:
        try
        {
            _data = new b_tree<std::shared_ptr<std::string>, custom_data*>(
                std::move(*dynamic_cast<b_tree<std::shared_ptr<std::string>, custom_data*> *>(other._data)));
        }
        catch (std::bad_alloc const &)
        {
            // TODO LOG
            throw;
        }
        break;
    }
    
    _fit_mode = other._fit_mode;
    _allocator = std::move(other._allocator);
}

void db::collection::copy_from(collection const &other)
{
    _tree_variant = other._tree_variant;
    switch (_tree_variant)
    {
    case b_tree_variants::b:
        //break;
    case b_tree_variants::b_plus:
        //break;
    case b_tree_variants::b_star:
        //break;
    case b_tree_variants::b_star_plus:
        //break;
    default:
        try
        {
            _data = new b_tree<std::shared_ptr<std::string>, custom_data*>(
                *dynamic_cast<b_tree<std::shared_ptr<std::string>, custom_data*> *>(other._data));
        }
        catch (std::bad_alloc const &)
        {
            // TODO LOG
            throw;
        }
        break;
    }
    
    _fit_mode = other._fit_mode;
    _allocator = other._allocator;

    // TODO other class el
}

void db::collection::clear()
{
    delete _data;
    _data = nullptr;
    _allocator = nullptr;

}

#pragma endregion collection implementation

#pragma region schema implementation
// TODO EXCEPTION FOR ADD, DISPOSE, OBTAIN;
db::schema::schema(
    b_tree_variants tree_variant,
    size_t t_for_b_trees):
    _tree_variant(tree_variant)
{
    switch(tree_variant):
        case b_tree_variantss::b:
            //break;
        case b_tree_variants::b_plus:
        //break;
        case b_tree_variants::b_star:
            //break;
        case b_tree_variants::b_star_plus:
            //break;
        default:
            try
            {
                _collections = new b_tree<std::shared_ptr<std::string>, std::shared_ptr<collection>>(t_for_b_trees);
            }
            catch(std::bad_alloc const &)
            {
                throw;
            }
            break;
        
}

db::schema::schema(schema const &other) noexcept
{
    copy_from(other);
}

db::schema::schema(schema &&other)
{
    move_from(std::move(other));
}

db::schema::schema::operator=(schema const &other) noexcept
{
    if (this != &other)
    {
        clear(*this);

        copy_from(other);
    }
    return *this;
}

db::schema::schema::operator=(schema &&other)
{
    if (this != &other)
    {
        clear(*this);

        move_from(std::move(other));
    }
    return *this;
}

db::schema::~schema()
{
    clear();
}

void db::schema::add(
    std::string const &collection_name,
    b_tree_variants tree_variant,
    allocator_variant allocator_variant,
    allocator_with_fit_mode::fit_mode fit_mode,
    size_t t_for_b_trees):
        _tree_variant(tree_variant)
{
    try
    {
        _collections->insert(collection_name, collection(tree_variant, allocator_variant, fit_mode, t_for_b_trees));
    }
    catch (search_tree<std::shared_ptr<std::string>, std::shared_ptr<collection>>::insertion_of_existent_key_attempt_exception_exception const &)
    {
        throw db::insertion_of_existent_struct_attempt_exception();
        // TODO LOG;
    }
}

collection db::schema::dispose(
    std::string const &collection_name)
{
    collection value;
    try
    {
        value = _collections->dispose(collection_name);
    }
    catch(search_tree<std::shared_ptr<std::string>, std::shared_ptr<collection>>::disposal_of_nonexistent_key_attempt_exception() const &)
    {
        throw db::disposal_of_nonexistent_key_attempt_exception();
    }
    return value;
    
}
		
collection db::schema::&obtain(
    std::string const &collection_name)
{
    try
    {
        return _collections->obtain(collection_name);
    }
    catch (std::shared_ptr<std::string>, std::shared_ptr<collection>>::obtaining_of_nonexistent_key_attempt_exception const &)
    {
        throw db::obtaining_of_nonexistent_key_attempt_exception();
        // TODO LOG;
    }
}

void db::schema::clear()
{
    delete _collections;
    _collections = nullptr;
}

void db::schema::copy_from(schema const &other)
{
    _tree_variant = other._tree_variant;
    switch (_tree_variant)
    {
    case b_tree_variants::b:
        //break;
    case b_tree_variants::b_plus:
        //break;
    case b_tree_variants::b_star:
        //break;
    case b_tree_variants::b_star_plus:
        //break;
    default:
        try
        {
            _collections = new b_tree<std::shared_ptr<std::string>, std::shared_ptr<collection>>(
                *dynamic_cast<b_tree<std::shared_ptr<std::string>, std::shared_ptr<collection>> *>(other._collections));
        }
        catch (std::bad_alloc const &)
        {
            // TODO LOG
            throw;
        }
        break;
    }
}

void db::schema::move_from(schema const &other)
{
    _tree_variant = other._tree_variant;
    switch (_tree_variant)
    {
    case b_tree_variants::b:
        //break;
    case b_tree_variants::b_plus:
        //break;
    case b_tree_variants::b_star:
        //break;
    case b_tree_variants::b_star_plus:
        //break;
    default:
        try
        {
            _collections = new b_tree<std::shared_ptr<std::string>, std::shared_ptr<collection>>(
                std::move(*dynamic_cast<b_tree<std::shared_ptr<std::string>, std::shared_ptr<collection>> *>(other._collections)));
        }
        catch (std::bad_alloc const &)
        {
            // TODO LOG
            throw;
        }
        break;
    }
}

#pragma endregion schema implementation

#pragma region pool implementation
// TODO EXCEPTION FOR ADD, DISPOSE, OBTAIN;
db::pool::pool(
    b_tree_variants const &tree_variant,
    size_t t_for_b_trees):
    _tree_variant(tree_variant)
{
    switch(tree_variant):
        case b_tree_variantss::b:
            //break;
        case b_tree_variants::b_plus:
        //break;
        case b_tree_variants::b_star:
            //break;
        case b_tree_variants::b_star_plus:
            //break;
        default:
            try
            {
                _schemas = new b_tree<std::shared_ptr<std::string>, std::shared_ptr<schema>>(t_for_b_trees);
            }
            catch(std::bad_alloc const &)
            {
                throw;
            }
            break;
        
}

db::pool::pool(pool const &other) noexcept
{
    copy_from(other);
}

db::pool::pool(pool &&other)
{
    move_from(std::move(other));
}

db::pool::pool::operator=(pool const &other) noexcept
{
    if (this != &other)
    {
        clear(*this);

        copy_from(other);
    }
    return *this;
}

db::pool::pool::operator=(pool &&other)
{
    if (this != &other)
    {
        clear(*this);

        move_from(std::move(other));
    }
    return *this;
}

db::pool::~pool()
{
    clear();
}

void db::pool::add(
    std::string const &schema_name,
    b_tree_variants tree_variant,
    size_t t_for_b_trees)
{
    try
    {
        _schemas->insert(schema_name, schema(tree_variant, t_for_b_trees));
    }
    catch (search_tree<std::shared_ptr<std::string>, std::shared_ptr<schema>>::insertion_of_existent_key_attempt_exception_exception const &)
    {
        throw db::insertion_of_existent_struct_attempt_exception();
        // TODO LOG;
    }
    
}

schema db::pool::dispose(
    std::string const &schema_name)
{
    schema value;
    try
    {
       value = _schemas->dispose(schema_name);
    }
    catch (search_tree<std::shared_ptr<std::string>, std::shared_ptr<schema>>::disposal_of_nonexistent_key_attempt_exception() const &)
    {
        throw db::disposal_of_nonexistent_key_attempt_exception();
        // TODO LOG;
    }
    return value;
}

schema db::pool::&obtain(
    std::string const &schema_name)
{
    try
    {
       return _schemas->obtain(schema_name);
    }
    catch (search_tree<std::shared_ptr<std::string>, std::shared_ptr<schema>>::obtaining_of_nonexistent_key_attempt_exception const &)
    {
        throw db::obtaining_of_nonexistent_key_attempt_exception();
        // TODO LOG;
    }
    return value;
}

void db::pool::clear()
{
    delete _schemas;
    _schemas = nullptr;
}

void db::pool::copy_from(pool const &other)
{
    _tree_variant = other._tree_variant;
    switch (_tree_variant)
    {
    case b_tree_variants::b:
        //break;
    case b_tree_variants::b_plus:
        //break;
    case b_tree_variants::b_star:
        //break;
    case b_tree_variants::b_star_plus:
        //break;
    default:
        try
        {
            _schemas = new b_tree<std::shared_ptr<std::string>, std::shared_ptr<schema>>(
                *dynamic_cast<b_tree<std::shared_ptr<std::string>, std::shared_ptr<schema>> *>(other._schemas));
        }
        catch (std::bad_alloc const &)
        {
            // TODO LOG
            throw;
        }
        break;
    }
}

void db::schema::move_from(schema const &other)
{
    _tree_variant = other._tree_variant;
    switch (_tree_variant)
    {
    case b_tree_variants::b:
        //break;
    case b_tree_variants::b_plus:
        //break;
    case b_tree_variants::b_star:
        //break;
    case b_tree_variants::b_star_plus:
        //break;
    default:
        try
        {
            _schemas = new b_tree<std::shared_ptr<std::string>, schema>(
                std::move(*dynamic_cast<std::shared_ptr<std::string>, std::shared_ptr<schema>> *>(other._schemas)));
        }
        catch (std::bad_alloc const &)
        {
            // TODO LOG
            throw;
        }
        break;
    }
}

#pragma region db implementation

static db *db::get_instance()
{
    static auto *instance = new db_storage();
    return instance;
}
