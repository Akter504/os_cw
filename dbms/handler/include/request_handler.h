#ifndef HANDLER_FILE_HANDLER_H
#define HANDLER_FILE_HANDLER_H

#include <commands_realization.h>


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
            if (current_time < _current_command->get_time_and_date()) return false;

            if (!_current_command->can_execute(request, current_time))
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
        if (_first_handler == nullptr) return false;
        return _first_handler->handle(request, current_time);
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
};

#endif // HANDLER_FILE_HANDLER_H