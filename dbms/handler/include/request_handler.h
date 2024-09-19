#ifndef HANDLER_FILE_HANDLER_H
#define HANDLER_FILE_HANDLER_H

#include <commands_realization.h>
#include <chrono>
#include <ctime>


class handler
{

public:

    handler *_next_handler;

public:

    handler():
        _next_handler(nullptr)
    { };

    handler(handler *next):
        _next_handler(next)
    { };

public:

    virtual bool handle(std::string const &request, uint64_t current_time) = 0;

};

class command_request_handler final:
    public handler
{
    command *_current_command;

    public:

        command_request_handler():
            _current_command(nullptr)
        { };

        command_request_handler(command *new_command):
            _current_command(new_command)
        { };

    public:
        
        bool handle(std::string const &request, uint64_t current_time) override
        {
            if (current_time < _current_command->get_time_and_date())
            {
                if (!this->_next_handler) return false;
                return this->_next_handler->handle(request, current_time);
            }

            _current_command->execute(request);
            return true;
        }

};

class handler_chain final
{

private:

    handler *_first_handler;
    handler *_last_handler;

public:

    handler_chain():
        _first_handler(nullptr),
        _last_handler(nullptr)
    { };

public:

    ~handler_chain()
    {
        handler* temp = _first_handler;
        while (_first_handler != nullptr)
        {
            temp = _first_handler;
            _first_handler = _first_handler->_next_handler;
            delete temp;
        }
    }
    handler_chain(handler_chain const &other)
    {
        this->_first_handler = other._first_handler;
        this->_last_handler = other._last_handler;
    }
    handler_chain operator=(handler_chain const &other)
    {
        if (this->_first_handler != other._first_handler && this->_last_handler != other._last_handler)
        {
            this->~handler_chain();

            this->_first_handler = other._first_handler;
            this->_last_handler = other._last_handler;
        }
        return *this;
    }

public:

    bool handle(std::string const &request, uint64_t current_time)
    {
        handler *temp = _first_handler;
        bool check = 0;
        if (_first_handler == nullptr) return false;
        while (temp != nullptr)
        {
            check = temp->handle(request, current_time);
            temp = temp->_next_handler;
        }
        return true;
    }

    handler_chain &add_handler(command* command_to_handler)
    {
        if (_first_handler == nullptr)
        {
            _first_handler = new command_request_handler(command_to_handler);
            _last_handler = _first_handler;
        }
        else
        {
            _last_handler->_next_handler = new command_request_handler(command_to_handler);
            _last_handler = _last_handler->_next_handler;
        }
        return *this;
    }

    bool check_command(std::string const &request, uint64_t time, db *db_strg)
    {
        std::istringstream string_with_commands(request);
        std::string command;
        string_with_commands >> command;
        if (command == "add_pool")
        {
            add_pool *add_p = new add_pool(db_strg);
            if (!add_p->can_execute(request, time)) return false;
            this->add_handler(add_p);
            return true;
        }
        else if(command == "add_schema")
        {
            add_schema *add_s = new add_schema(db_strg);
            if (!add_s->can_execute(request, time)) return false;
            this->add_handler(add_s);
            return true;
        }
        else if(command == "add_collection")
        {
            add_collection *add_c = new add_collection(db_strg);
            if (!add_c->can_execute(request, time)) return false;
            this->add_handler(add_c);
            return true;
        }
        else if(command == "delete_pool")
        {
            dispose_pool *delete_p = new dispose_pool(db_strg);
            if (!delete_p->can_execute(request, time)) return false;
            this->add_handler(delete_p);
            return true;
        }
        else if(command == "delete_schema")
        {
            dispose_schema *delete_s = new dispose_schema(db_strg);
            if (!delete_s->can_execute(request, time)) return false;
            this->add_handler(delete_s);
            return true;
        }
        else if(command == "delete_collection")
        {
            dispose_collection *delete_c = new dispose_collection(db_strg);
            if (!delete_c->can_execute(request, time)) return false;
            this->add_handler(delete_c);
            return true;
        }
        else if(command == "add")
        {
            add_value *add_v = new add_value(db_strg, time);
            if (!add_v->can_execute(request, time)) return false;
            this->add_handler(add_v);
            return true;
        }
        else if(command == "read")
        {
            obtain_value *obtain_v = new obtain_value(db_strg);
            if (!obtain_v->can_execute(request, time)) return false;
            this->add_handler(obtain_v);
            return true;
        }
        else if(command == "read_between")
        {
            obtain_between_value *obtain_b_v = new obtain_between_value(db_strg);
            if (!obtain_b_v->can_execute(request, time)) return false;
            this->add_handler(obtain_b_v);
            return true;
        }
        else if(command == "update")
        {
            update_value *update_v = new update_value(db_strg, time);
            if (!update_v->can_execute(request, time)) return false;
            this->add_handler(update_v);
            return true;
        }
        else if(command == "delete")
        {
            dispose_value *delete_v = new dispose_value(db_strg, time);
            if (!delete_v->can_execute(request, time)) return false;
            this->add_handler(delete_v);
            return true;
        }
        else if(command == "recovery_data")
        {
            if (db_strg->get_instance()->get_mode() == db::mode::file_system)
            {
                throw std::logic_error("Data recovery is only available for the mode in memory cache.");
            }
            recovery_data *rec_data = new recovery_data(db_strg);
            if (!rec_data->can_execute(request, time)) return false;
            if (this->_first_handler == nullptr)
            {
                this->add_handler(rec_data);
            }
            else
            {
                throw std::logic_error("Recovery the system state is possible only at startup.");
            }
            return true;
        }
        return false;
    }

    bool process_chain(std::string const &request, std::string const &cmd, db *db_strg)
    {
        // read pool schema collection id time
        command *read_v = nullptr;
        if (cmd == "read")
        {
            try
            {
                read_v = new obtain_value(db_strg);
            }
            catch(const std::bad_alloc& e)
            {
                throw e;
            }
        }
        else
        {
            try
            {
                read_v = new obtain_between_value(db_strg);
            }
            catch(const std::bad_alloc& e)
            {
                throw e;
            }
        }
        if (!read_v->can_execute(request, 0)) return false;
        
        if (this->handle(request, read_v->get_time_and_date()))
        {
            read_v->execute(request);
            return true;
        }
        return false;
            
    }
};

#endif // HANDLER_FILE_HANDLER_H