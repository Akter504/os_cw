#ifndef HANDLER_COMMAND_VALIDATION_H
#define HANDLER_COMMAND_VALIDATION_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>

class validation final
{

public:

    enum class commands
    {
        ADD,
        READ,
        READ_BETWEEN,
        UPDATE,
        DELETE,
        ADD_POOL,
        DELETE_POOL,
        ADD_SCHEME,
        DELETE_SCHEME,
        ADD_COLLECTION,
        DELETE_COLLECTION

    };

public:

    const std::vector<std::string> _commands =
    {
        "add", "read", "read_between", "update", "delete", 
        "add_pool", "delete_pool", "add_schema", "delete_schema", "add_collection", "delete_collection",
        "recovery_data"
    };

public:

    bool check_command (std::string const &new_command, commands &enum_command)
    {   
        int count = 0;
        for (auto command : _commands)
        {
            if (new_command.compare(command) == 0)
            {
                enum_command = static_cast<commands>(count);
                return true;
            }
            count++;
        }
        return false;
    }

    bool check_id(std::string const &str_id, size_t &id)
    {
        for (auto &ch : str_id)
        {
            if (!isdigit(ch))
            {
                return false;
            }
        }

        try
        {
            id = std::stoul(str_id);
        }
        catch (const std::invalid_argument& e) 
        {
            return false;
        } 
        catch (const std::out_of_range& e) 
        {
            return false;
        }
    }

    bool check_name (std::string const &str_name)
    {
        for (auto &ch : str_name)
        {
            if (!isalpha(ch))
            {
                return false;
            }
        }
        
        commands enum_command;
        if (check_command(str_name, enum_command))
        {
            return true;
        }
        return false;
    }
};

#endif // HANDLER_COMMAND_VALIDATION_H