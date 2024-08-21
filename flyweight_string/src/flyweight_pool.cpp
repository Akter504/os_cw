#include <flyweight_pool.h>

#pragma region flyweight_string implementation

flyweight_string::flyweight_string(
    std::string const &str):
        _strdata(str)
{ }

std::string flyweight_string::get_str() const
{
    return _strdata;
}

#pragma endregion flyweight_string implementation

#pragma region flyweight_pool implementation

std::shared_ptr<flyweight_string> flyweight_pool::make_flyweight(std::string const &str)
{
    auto it = _flyweight_pool.find(str);
    if (it != _flyweight_pool.end())
    {
        return it->second;
    }

    std::shared_ptr<flyweight_string> flyweight_str = std::make_shared<flyweight_string>(str);
    _flyweight_pool[str] = flyweight_str;
    return flyweight_str;
}

void flyweight_pool::consolidate()
{
    for (auto iter = _flyweight_pool.begin(); iter != _flyweight_pool.end())
    {
        if (iter.use_count() == 1)
        {
           iter =  _flyweight_pool.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

static flyweight_string_pool *flyweight_pool::get_instance()
{
    static auto *flyweight_pool = new flyweight_string_pool();
    return flyweight_pool; 
}

flyweight_string_pool::flyweight_string_pool() { }

/*flyweight_string_pool::~flyweight_string_pool() 
{ 
    _flyweight_pool.clear();
}*/