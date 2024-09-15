#ifndef HANDLER_COMMAND_REALIZATION_H
#define HANDLER_COMMAND_REALIZATION_H

#include <file_validation.h>
#include <db.h>
#include <filesystem>
#include <allocator_with_fit_mode.h>
#include <sstream>

class command
{

protected:

    db *db_strg = nullptr;
    uint64_t _end_time;

public:

    command(db *db, uint64_t const &time = 0):
        db_strg(db->get_instance()),
        _end_time(time)
    { };

public:

    virtual void execute(std::string const &str) = 0;
    virtual bool can_execute(std::string const &str, uint64_t const &time) = 0;
    virtual ~command() {};

public:

    [[nodiscard]] std::int64_t get_time_and_date() const
    {
        return _end_time;
    }

};

class add_pool final:
    public command
{
    std::string _pool_name;
    db::b_tree_variants _tree_variant;
    size_t _t_for_b_trees = 8;

public:

    add_pool(db* db):
        command(db)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        int tree_variant, t_for_b_trees;
        if (command == "add_pool")
        {
            string_with_commands >> _pool_name;
            if (string_with_commands.fail())
            {
                return false;
            }
            string_with_commands >> tree_variant;
            /*if (string_with_commands.fail())
            {
                return false;
            }*/
            string_with_commands >> t_for_b_trees;
            /*if (string_with_commands.fail())
            {
                return false;
            }*/
            _tree_variant = static_cast<db::b_tree_variants>(tree_variant);
            _t_for_b_trees = static_cast<size_t>(t_for_b_trees);
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->add_pool(_pool_name, _tree_variant, _t_for_b_trees); 
    }   
}; 

class dispose_pool final:
    public command
{
    std::string _pool_name;

public:

    dispose_pool(db* db):
        command(db)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "delete_pool")
        {
            string_with_commands >> _pool_name;
            if (string_with_commands.fail())
            {
                return false;
            }
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->dispose_pool(_pool_name); 
    }   
}; 

class add_schema final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;
    db::b_tree_variants _tree_variant;
    size_t _t_for_b_trees = 8;

public:

    add_schema(db* db):
        command(db)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        int tree_variant, t_for_b_trees;
        if (command == "add_schema")
        {
            string_with_commands >> _pool_name >> _schema_name;
            string_with_commands >> tree_variant;
            if (string_with_commands.fail())
            {
                return false;
            }
            string_with_commands >> t_for_b_trees;
            if (string_with_commands.fail())
            {
                return false;
            }
            _tree_variant = static_cast<db::b_tree_variants>(tree_variant);
            _t_for_b_trees = static_cast<size_t>(t_for_b_trees);
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->add_schema(_pool_name, _schema_name, _tree_variant, _t_for_b_trees); 
    }   
}; 

class dispose_schema final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;

public:

    dispose_schema(db* db):
        command(db)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "delete_schema")
        {
            string_with_commands >> _pool_name >> _schema_name;
            if (string_with_commands.fail())
            {
                return false;
            }
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->dispose_schema(_pool_name, _schema_name); 
    }   
};

class add_collection final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;
    std::string _collection_name;
    db::b_tree_variants _tree_variant;
    db::allocator_variant _allocator_variant;
    allocator_with_fit_mode::fit_mode _fit_mode;
    size_t _t_for_b_trees = 8;

public:

    add_collection(db* db):
        command(db)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        int tree_variant, t_for_b_trees, allocator_variant, fit_mode;
        if (command == "add_collection")
        {
            string_with_commands >> _pool_name >> _schema_name >> _collection_name;
            string_with_commands >> tree_variant;
            if (string_with_commands.fail())
            {
                return false;
            }
            string_with_commands >> allocator_variant;
            if (string_with_commands.fail())
            {
                return false;
            }
            string_with_commands >> fit_mode;
            if (string_with_commands.fail())
            {
                return false;
            }
            string_with_commands >> t_for_b_trees;
            if (string_with_commands.fail())
            {
                return false;
            }
            _tree_variant = static_cast<db::b_tree_variants>(tree_variant);
            _allocator_variant = static_cast<db::allocator_variant>(allocator_variant);
            _fit_mode = static_cast<allocator_with_fit_mode::fit_mode>(fit_mode);
            _t_for_b_trees = static_cast<size_t>(t_for_b_trees);
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->add_collection(_pool_name, _schema_name, _collection_name, _tree_variant, _allocator_variant, _fit_mode, _t_for_b_trees); 
    }   
}; 

class dispose_collection final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;
    std::string _collection_name;

public:

    dispose_collection(db* db):
        command(db)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "delete_collection")
        {
            string_with_commands >> _pool_name >> _schema_name >> _collection_name;
            if (string_with_commands.fail())
            {
                return false;
            }
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->dispose_collection(_pool_name, _schema_name, _collection_name); 
    }   
};

class add_value final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;
    std::string _collection_name;
    tkey _key;
    tvalue _value;

public:

    add_value(db* db, uint64_t const &time):
        command(db, time)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        if (time != 0 && this->get_time_and_date() > time) return false;
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "add")
        {
            uint64_t value_n;
            std::string value_s;
            string_with_commands >> _pool_name >> _schema_name >> _collection_name  >> _key >> value_n >> value_s;
            if (string_with_commands.fail())
            {
                return false;
            }
            _value = tvalue(value_s, value_n);
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->add(_pool_name, _schema_name, _collection_name, _key, _value); 
    }   
}; 

class dispose_value final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;
    std::string _collection_name;
    tkey _key;

public:

    dispose_value(db* db, uint64_t const &time):
        command(db, time)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        if (time != 0 && this->get_time_and_date() > time) return false;
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "delete")
        {
            string_with_commands >> _pool_name >> _schema_name >> _collection_name >> _key;
            if (string_with_commands.fail())
            {
                return false;
            }
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->dispose(_pool_name, _schema_name, _collection_name, _key); 
    }   
};

class obtain_value final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;
    std::string _collection_name;
    tkey _key;

public:

    obtain_value(db* db, uint64_t const &time):
        command(db, time)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        if (time != 0 && this->get_time_and_date() > time) return false;
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "read")
        {
            string_with_commands >> _pool_name >> _schema_name >> _collection_name >> _key;
            if (string_with_commands.fail())
            {
                return false;
            }
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        tvalue temp = db_strg->get_instance()->obtain(_pool_name, _schema_name, _collection_name, _key); 
        std::cout << std::endl <<  "VALUE_N: " << temp._n_value << " " << "VALUE_S: " << temp._fw_value->get_str() << std::endl;
    }   
};

class update_value final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;
    std::string _collection_name;
    tkey _key;
    tvalue _value;

public:

    update_value(db* db, uint64_t const &time):
        command(db, time)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        if (time != 0 && this->get_time_and_date() > time) return false;
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "update")
        {
            uint64_t value_n;
            std::string value_s;
            string_with_commands >> _pool_name >> _schema_name >> _collection_name  >> _key >> value_n >> value_s;
            if (string_with_commands.fail())
            {
                return false;
            }
            _value = tvalue(value_s, value_n);
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->update(_pool_name, _schema_name, _collection_name, _key, _value); 
    }   
}; 

class obtain_between_value final:
    public command
{
    std::string _pool_name;
    std::string _schema_name;
    std::string _collection_name;
    tkey _key_min, _key_max;
    bool _lower_bound_inclusive, _upper_bound_inclusive;

public:

    obtain_between_value(db* db, uint64_t const &time):
        command(db, time)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        if (time != 0 && this->get_time_and_date() > time) return false;
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "read_between")
        {
            string_with_commands >> _pool_name >> _schema_name >> _collection_name >> _key_min >> _key_max;
            if (string_with_commands.fail())
            {
                return false;
            }
            string_with_commands >> _lower_bound_inclusive >> _upper_bound_inclusive;
            if (string_with_commands.fail())
            {
                return false;
            }
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        std::vector<std::pair<tkey,tvalue>> temp_vector;
        temp_vector = db_strg->get_instance()->obtain_between(
            _pool_name, 
            _schema_name, 
            _collection_name, 
            _key_min, 
            _key_max, 
            _lower_bound_inclusive, 
            _upper_bound_inclusive); 

        for (auto kvp : temp_vector)
        {
            std::cout << std::endl <<  "VALUE_N: " << kvp.second._n_value << " " << "VALUE_S: " << kvp.second._fw_value->get_str() << std::endl;
        }
    }   
};

class recovery_data final:
    public command
{
    std::string _path;

public:

    recovery_data(db* db):
        command(db)
    { };

public:

    bool can_execute(std::string const& request, uint64_t const &time) override
    {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "recovery_data")
        {
            std::string path = extra_utility::make_path({"pools"});
            if (!std::filesystem::exists(path) || !(db_strg->get_instance()->get_mode() == db::mode::in_memory_cache))
            {
                return false;
            }
            _path = path;
            return true;
        }
        return false;
    }

    void execute(std::string const& request) override
    {
        db_strg->get_instance()->load_db(_path);
    }   
};


#endif // HANDLER_COMMAND_REALIZATION_H
