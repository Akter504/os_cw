#ifndef OPERATING_SYSTEMS_COURSE_WORK_FLYWEIGHT_POOL_H
#define OPERATING_SYSTEMS_COURSE_WORK_FLYWEIGHT_POOL_H

#include <string>
#include <memory>
#include <map>
#include <unordered_map>

class flyweight_string
{

private:

    std::string _strdata;

public:

    flyweight_string(
        std::string const &str);

public:

    std::string get_str() const;


};

class flyweight_string_pool
{

private:

    std::unordered_map<std::string, std::shared_ptr<flyweight_string>> _flyweight_pool;

public:

    static flyweight_string_pool *get_instance();

public:

    std::shared_ptr<flyweight_string> make_flyweight(const std::string &str);

    void consolidate();

private:

    ~flyweight_string_pool();
    flyweight_string_pool();

private:

    flyweight_string_pool(const flyweight_string_pool&) = delete;
    flyweight_string_pool& operator=(const flyweight_string_pool&) = delete;

};

#endif //OPERATING_SYSTEMS_COURSE_WORK_FLYWEIGHT_POOL_H