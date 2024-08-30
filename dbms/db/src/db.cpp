#include <db.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>

#include <allocator_global_heap.h>
#include <allocator_buddies_system.h>
#include <allocator_sorted_list.h>
#include <allocator_red_black_tree.h>
#include <allocator_boundary_tags.h>

#pragma region collection implementation

db::collection::collection(
    b_tree_variants tree_variant,
    allocator_variant allocator_variant,
    allocator_with_fit_mode::fit_mode mode,
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
            _data = new b_tree<flyweight_tkey, custom_data *>(t_for_b_trees, tkey_comparer());
            break;
    }

    try
    {
        switch(allocator_variant)
        {
            case allocator_variant::boundary_tags:
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

db::collection::collection(db::collection const &other)
{
    copy_from(other);
}

db::collection::collection(db::collection &&other) noexcept
{
    
    move_from(std::move(other));
}

db::collection &db::collection::operator=(db::collection const &other)
{
    if (this != &other)
    {
        clear();

        copy_from(other);
    }
    
    return *this;
}

db::collection &db::collection::operator=(db::collection &&other) noexcept
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

void db::collection::insert(
    tkey const &key,
    tvalue const &value,
    std::string const &path)
{

    collect_garbage(path);

    flyweight_tkey fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
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
        throw db::insertion_of_existent_struct_attempt_exception();
        // TODO
    }
    
    ++_records_cnt;
    

}

void db::collection::insert(
    tkey const &key,
    tvalue &&value,
    std::string const &path)
{

    collect_garbage(path);

    flyweight_tkey fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
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

    flyweight_tkey fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
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
        _data->update(fw_key, data);
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
    tvalue &&value,
    std::string const &path)
{
    collect_garbage(path);

    flyweight_tkey fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
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

tvalue db::collection::dispose(
    tkey const &key,
    std::string const &path)
{
    flyweight_tkey fw_key = flyweight_string_pool::get_instance()->make_flyweight(key);
    custom_data* data = nullptr;
    tvalue value;

    try
    {
        data = _data->dispose(fw_key);
    }
    catch (search_tree<tkey, custom_data *>::disposal_of_nonexistent_key_attempt_exception)
    {
        throw db::disposal_of_nonexistent_key_attempt_exception();
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
        data = _data->obtain(fw_key);
    }
    catch(search_tree<tkey, custom_data *>::obtaining_of_nonexistent_key_attempt_exception)
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
    result.reserve(data.size());

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
                result.emplace_back(kvp.key.get()->get_str(), dynamic_cast<custom_data_file*>(kvp.value)->deserialize(path));
            }
            catch (std::ios::failure const &)
            {
                throw std::ios::failure("Failed to read data");
            }

        }
        else
        {
            result.emplace_back(kvp.key.get()->get_str(), dynamic_cast<custom_data_ram*>(kvp.value)->value);
        }
    
    return result;

}

void db::collection::collect_garbage(std::string const &path)
{ }

void db::collection::consolidate(std::string const &path)
{ }


/*std::pair<tkey, tvalue> db::obtain_min(
		std::string const &path)
{ }

std::pair<tkey, tvalue> db::obtain_max(
		std::string const &path)
{ }

std::pair<tkey, tvalue> db::obtain_next(
		std::string const &path,
        tkey const &key)
{ }*/

void db::collection::move_from(db::collection &&other)
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
            _data = new b_tree<flyweight_tkey, custom_data*>(
                std::move(*dynamic_cast<b_tree<flyweight_tkey, custom_data*> *>(other._data)));
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

void db::collection::copy_from(db::collection const &other)
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
            _data = new b_tree<flyweight_tkey, custom_data*>(
                *dynamic_cast<b_tree<flyweight_tkey, custom_data*> *>(other._data));
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
    size_t t_for_b_trees)
{
    _tree_variant = tree_variant;
    switch(tree_variant)
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
                _collections = new b_tree<flyweight_tkey, std::shared_ptr<collection>>(t_for_b_trees, tkey_comparer());
            }
            catch(std::bad_alloc const &)
            {
                throw;
            }
            break;
    }
        
}

db::schema::schema(db::schema const &other)
{
    copy_from(other);
}

db::schema::schema(db::schema &&other) noexcept
{ 
    move_from(std::move(other));
}

db::schema &db::schema::operator=(db::schema const &other)
{
    if (this != &other)
    {
        clear();

        copy_from(other);
    }
    return *this;
}

db::schema &db::schema::operator=(db::schema &&other) noexcept
{
    if (this != &other)
    {
        clear();

        move_from(std::move(other));
    }
    return *this;
}

db::schema::~schema()
{
    clear();
}


void db::schema::consolidate(
    std::string const &path)
{
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
        auto iter_start = dynamic_cast<b_tree<std::string, schema> *>(_collections)->begin_infix();
        auto iter_end = dynamic_cast<b_tree<std::string, schema> *>(_collections)->end_infix();
        
        for (; iter_start != iter_end; ++iter_start)
        {
            std::get<3>(*iter_start).consolidate(extra_utility::make_path({path, std::get<2>(*iter_start)}));
        }
    }
}

void db::schema::add(
    std::string const &collection_name,
    b_tree_variants tree_variant,
    allocator_variant allocator_variant,
    allocator_with_fit_mode::fit_mode fit_mode,
    size_t t_for_b_trees)
{
    _tree_variant = tree_variant;
    try
    {
        _collections->insert(flyweight_string_pool::get_instance()->make_flyweight(collection_name), std::make_shared<collection>(collection(tree_variant, allocator_variant, fit_mode, t_for_b_trees)));
    }
    catch (search_tree<flyweight_tkey, std::shared_ptr<collection>>::insertion_of_existent_key_attempt_exception_exception const &)
    {
        throw db::insertion_of_existent_struct_attempt_exception();
        // TODO LOG;
    }
}

db::collection db::schema::dispose(
    std::string const &collection_name)
{
    std::shared_ptr<collection> value = nullptr;
    try
    {
        value = _collections->dispose(flyweight_string_pool::get_instance()->make_flyweight(collection_name));
    }
    catch(search_tree<flyweight_tkey, std::shared_ptr<collection>>::disposal_of_nonexistent_key_attempt_exception const &)
    {
        throw db::disposal_of_nonexistent_key_attempt_exception();
    }
    return *(value.get());
    
}
		
db::collection &db::schema::obtain(
    std::string const &collection_name)
{
    try
    {
        return *((_collections->obtain(flyweight_string_pool::get_instance()->make_flyweight(collection_name))).get());
    }
    catch (search_tree<std::string, collection>::obtaining_of_nonexistent_key_attempt_exception const &)
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

void db::schema::copy_from(db::schema const &other)
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
            _collections = new b_tree<flyweight_tkey, std::shared_ptr<collection>>(
                *dynamic_cast<b_tree<flyweight_tkey, std::shared_ptr<collection>> *>(other._collections));
        }
        catch (std::bad_alloc const &)
        {
            // TODO LOG
            throw;
        }
        break;
    }
}

void db::schema::move_from(db::schema &&other)
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
            _collections = new b_tree<flyweight_tkey, std::shared_ptr<collection>>(
                std::move(*dynamic_cast<b_tree<flyweight_tkey, std::shared_ptr<collection>> *>(other._collections)));
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
    b_tree_variants tree_variant,
    size_t t_for_b_trees)
{
    _tree_variant = tree_variant;
    switch(tree_variant)
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
                _schemas = new b_tree<flyweight_tkey, std::shared_ptr<schema>>(t_for_b_trees, tkey_comparer());
            }
            catch(std::bad_alloc const &)
            {
                throw;
            }
            break;
    }
        
}

db::pool::pool(db::pool const &other)
{
    copy_from(other);
}

db::pool::pool(db::pool &&other) noexcept
{
    move_from(std::move(other));
}

db::pool &db::pool::operator=(db::pool const &other)
{
    if (this != &other)
    {
        clear();

        copy_from(other);
    }
    return *this;
}

db::pool &db::pool::operator=(db::pool &&other) noexcept
{
    if (this != &other)
    {
        clear();

        move_from(std::move(other));
    }
    return *this;
}

db::pool::~pool()
{
    clear();
}

void db::pool::consolidate(
    std::string const &path)
{
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
        auto iter = dynamic_cast<b_tree<std::string, schema> *>(_schemas)->begin_infix();
        auto iter_end = dynamic_cast<b_tree<std::string, schema> *>(_schemas)->end_infix();
        
        for (; iter != iter_end; ++iter)
        {
            std::get<3>(*iter).consolidate(extra_utility::make_path({path, std::get<2>(*iter)}));
        }
    }
}

void db::pool::add(
    std::string const &schema_name,
    b_tree_variants tree_variant,
    size_t t_for_b_trees)
{
    try
    {
        _schemas->insert(flyweight_string_pool::get_instance()->make_flyweight(schema_name), std::make_shared<schema>(schema(tree_variant, t_for_b_trees)));
    }
    catch (search_tree<flyweight_tkey, std::shared_ptr<schema>>::insertion_of_existent_key_attempt_exception_exception const &)
    {
        throw db::insertion_of_existent_struct_attempt_exception();
        // TODO LOG;
    }
    
}

db::schema db::pool::dispose(
    std::string const &schema_name)
{
    std::shared_ptr<schema> value = nullptr;
    try
    {
       value = _schemas->dispose(flyweight_string_pool::get_instance()->make_flyweight(schema_name));
    }
    catch (search_tree<flyweight_tkey, std::shared_ptr<schema>>::disposal_of_nonexistent_key_attempt_exception const &)
    {
        throw db::disposal_of_nonexistent_key_attempt_exception();
        // TODO LOG;
    }
    return *(value.get());
}

db::schema &db::pool::obtain(
    std::string const &schema_name)
{
    try
    {
       return *((_schemas->obtain(flyweight_string_pool::get_instance()->make_flyweight(schema_name))).get());
    }
    catch (search_tree<flyweight_tkey, std::shared_ptr<schema>>::obtaining_of_nonexistent_key_attempt_exception const &)
    {
        throw db::obtaining_of_nonexistent_key_attempt_exception();
        // TODO LOG;
    }
}

void db::pool::clear()
{
    delete _schemas;
    _schemas = nullptr;
}

void db::pool::copy_from(db::pool const &other)
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
            _schemas = new b_tree<flyweight_tkey, std::shared_ptr<schema>>(
                *dynamic_cast<b_tree<flyweight_tkey, std::shared_ptr<schema>> *>(other._schemas));
        }
        catch (std::bad_alloc const &)
        {
            // TODO LOG
            throw;
        }
        break;
    }
}

void db::pool::move_from(db::pool &&other)
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
            _schemas = new b_tree<flyweight_tkey, std::shared_ptr<schema>>(
                std::move(*dynamic_cast<b_tree<flyweight_tkey, std::shared_ptr<schema>> *>(other._schemas)));
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

db *db::get_instance()
{
    static auto *instance = new db();
    return instance;
}

db *db::setup(
    size_t id,
    db::mode mode)
{ 
    throw_if_initialized_at_setup()
        .throw_if_invalid_setup(id,mode);

    if (_mode == mode::file_system) 
    {
        if (!std::filesystem::exists("pools")) 
        {
            std::filesystem::create_directory("pools");
            std::filesystem::permissions("pools", std::filesystem::perms::all);
        }
    }
    return this;
}

db *db::add_pool(
		std::string const &pool_name,
		b_tree_variants tree_variant,
		size_t t_for_b_trees)
{
    std::string path = extra_utility::make_path({"pools", pool_name});
    
    throw_if_unitialized()
        .throw_if_invalid_file_name(pool_name)
        .add(pool_name, tree_variant, t_for_b_trees);
    
    if (get_instance()->_mode == mode::file_system)
    {	
        std::string cfg_path = extra_utility::make_path({path, "cfg"});
        if (!std::filesystem::exists(path) || !std::filesystem::exists(cfg_path))
        {
            std::ofstream stream(cfg_path);
            try
            {
                if (!stream) 
                {
                    throw std::ios::failure("Failed to open the configuration file");
                }

                stream << static_cast<int>(tree_variant) << std::endl;
                stream << t_for_b_trees << std::endl;
                stream.flush();

                if (stream.fail()) 
                {
                    throw std::ios::failure("Failed to write the configuration file");
                }
            }
            catch (std::ios::failure const &) 
            {
                std::filesystem::remove(cfg_path);
                std::filesystem::remove(path);
                dispose(path);
                
                throw db::insertion_of_struct_failure();
            }
        }
    }
    return this;
}
	
db *db::dispose_pool(
    std::string const &pool_name)
{
    std::string path = extra_utility::make_path({"pools", pool_name});
    
    throw_if_unitialized()
        .throw_if_invalid_file_name(pool_name)
        .dispose(pool_name);
    
    std::filesystem::remove_all(path);
    return this;
}

db *db::add_schema(
    std::string const &pool_name,
    std::string const &schema_name,
    b_tree_variants tree_variant,
    size_t t_for_b_trees)
{
    std::string pool_path = extra_utility::make_path({"pools", pool_name});
    std::string schema_path = extra_utility::make_path({"pools", pool_name, schema_name});

    pool &pool = throw_if_unitialized()
        .throw_if_invalid_path(pool_path)
        .throw_if_invalid_file_name(schema_name)
        .obtain(pool_name);
    pool.add(schema_name, tree_variant, t_for_b_trees);

    if (get_instance()->_mode == mode::file_system)
    {	
        std::string cfg_path = extra_utility::make_path({schema_path, "cfg"});
        if (!std::filesystem::exists(schema_path) || !std::filesystem::exists(cfg_path))
        {
            std::ofstream stream(cfg_path);
            try
            {
                if (!stream) 
                {
                    throw std::ios::failure("Failed to open the configuration file");
                }

                stream << static_cast<int>(tree_variant) << std::endl;
                stream << t_for_b_trees << std::endl;
                stream.flush();

                if (stream.fail()) 
                {
                    throw std::ios::failure("Failed to write the configuration file");
                }
            }
            catch (std::ios::failure const &) 
            {
                std::filesystem::remove(cfg_path);
                std::filesystem::remove(schema_path);
                dispose(schema_path);
                
                throw db::insertion_of_struct_failure();
            }
        }
    }
    return this;
}
	
db *db::dispose_schema(
    std::string const &pool_name,
    std::string const &schema_name)
{
    throw_if_unitialized()
        .throw_if_invalid_file_name(pool_name)
        .throw_if_invalid_file_name(schema_name)
        .obtain(pool_name)
        .dispose(schema_name);
    
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name});
    std::filesystem::remove_all(path);
    
    return this;
}

db *db::add_collection(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    db::b_tree_variants tree_variant,
    db::allocator_variant allocator_variant,
    allocator_with_fit_mode::fit_mode fit_mode,
    size_t t_for_b_trees)
{
    std::string schema_path = extra_utility::make_path({"pools", pool_name, schema_name});
    std::string collection_path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name});

    schema &schema = throw_if_unitialized()
        .throw_if_invalid_path(schema_path)
        .throw_if_invalid_file_name(collection_name)
        .obtain(pool_name)
        .obtain(schema_name);
    schema.add(collection_name, tree_variant, allocator_variant, fit_mode, t_for_b_trees);
    // TODO ID FILE?

    if (get_instance()->_mode == mode::file_system)
    {
        std::string cfg_path = extra_utility::make_path({collection_path, "cfg"});
        if (!std::filesystem::exists(cfg_path) || !std::filesystem::exists(collection_path))
        {
            std::ofstream stream(cfg_path);
            try
            {
                if (!stream) 
                {
                    throw std::ios::failure("Failed to open the configuration file");
                }

                stream << static_cast<int>(tree_variant) << std::endl;
                stream << t_for_b_trees << std::endl;
                stream.flush();

                if (stream.fail()) 
                {
                    throw std::ios::failure("Failed to write the configuration file");
                }
            }
            catch (std::ios::failure const &) 
            {
                std::filesystem::remove(cfg_path);
                std::filesystem::remove(collection_path);
                dispose(collection_path);
                
                throw db::insertion_of_struct_failure();
            }
        }
    }

    return this;
}
	
db *db::dispose_collection(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name)
{
    throw_if_unitialized()
        .throw_if_invalid_file_name(pool_name)
        .throw_if_invalid_file_name(schema_name)
        .throw_if_invalid_file_name(collection_name)
        .obtain(pool_name)
        .obtain(schema_name)
        .dispose(collection_name);

    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name});
    std::filesystem::remove_all(path);

    return this;
}

db *db::add(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    tkey const &key,
    tvalue const &value)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .insert(key,value,path);
    
    return this;
}

db *db::add(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    tkey const &key,
    tvalue &&value)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .insert(key, std::move(value), path);
    
    return this;
}

db *db::update(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    tkey const &key,
    tvalue const &value)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .update(key, value, path);
    
    return this;
}

db *db::update(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    tkey const &key,
    tvalue &&value)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .update(key, std::move(value), path);
    
    return this;
}

tvalue db::dispose(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    tkey const &key)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    return throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .dispose(key, path);
    
}

tvalue db::obtain(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    tkey const &key)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    return throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .obtain(key, path);
}

std::vector<std::pair<tkey, tvalue>> db::obtain_between(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});

    return throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive, path);
}

std::pair<tkey, tvalue> db::obtain_max(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    return throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .obtain_max(path);
}

std::pair<tkey, tvalue> db::obtain_min(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    return throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .obtain_min(path);
}

std::pair<tkey, tvalue> db::obtain_next(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name,
    tkey const &key)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    return throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .obtain_next(path, key);
}

db *db::consolidate()
{
    auto iter_start = _pools.cbegin_infix();
    auto iter_end = _pools.cend_infix();
    for (; iter_start != iter_end; ++iter_start)
    {
        std::get<3>(*iter_start).consolidate(extra_utility::make_path({"pools", std::get<2>(*iter_start)}));
    }
    
    return this;
}

size_t db::get_collection_records_cnt(
    std::string const &pool_name,
    std::string const &schema_name,
    std::string const &collection_name)
{
    std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
    
    return throw_if_initialized_at_setup()
        .throw_if_invalid_path(path)
        .obtain(pool_name)
        .obtain(schema_name)
        .obtain(collection_name)
        .get_records_cnt();
}



/*db *db::load_db(std::string const &path)
{
    // TODO SERVER
}

db* db::clear()
{
    if (get_instance()->_mode == mode::in_memory_cache)
    {
        return this;
    }

    if (access("pools", F_OK) == -1)
    {
        return this;
    }
}

void db::collect_garbage(std::string const &path)
{ }*/

    db &db::throw_if_initialized_at_setup()
    {
        if (_mode == mode::uninitialized)
        {
            return *this;
        }

        throw db::setup_failure("attempt to change previously set up mode");
    }

	db &db::throw_if_unitialized()
    {
        if (_mode != mode::uninitialized)
    {
        return *this;
    }
    
        throw db::logic_failure("attempt to perform an operation while mode not initialized");
    }

	db &db::throw_if_invalid_setup(size_t id, db::mode mode)
    {
        if (mode != mode::uninitialized && id > 0)
        {
            return *this;
        }

        throw db::setup_failure("invalid setup data");
    }

	db &db::throw_if_invalid_path(std::string const &path)
    {
        if (path.empty()) 
        {
            throw std::invalid_argument("Path is empty.");
        }

        if (_mode == mode::file_system)
        {
            if (access(path.c_str(), F_OK) == -1)
            {
                throw db::invalid_path();
            }
        }
        return *this;
    }

	db &db::throw_if_invalid_file_name(std::string const &file_name)
    {
        if (_mode == mode::file_system)
        {
            bool space = false;
            for (char ch : file_name)
            {
                if (!isalnum(ch) && ch != '_' && ch != '-' && ch != ' ')
                {
                    throw db::incorrect_file_name();
                }
                space = ch == ' ';
            }
            if (space)
            {
                throw db::incorrect_file_name();
            }
        }

        return *this;
    }

