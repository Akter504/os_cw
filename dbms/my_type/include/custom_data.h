#include <iostream>
#include <allocator.h>
#include <flyweight_pool.h>


using flyweight_tkey = std::shared_ptr<std::string>;

class tvalue
{

public:

    std::shared_ptr<flyweight_string> _fw_value;
    uint64_t _n_value;

public:

    tvalue();
    tvalue(std::string const &svalue,
        uint64_t nvalue);

};

class custom_data
{

protected:
    virtual ~custom_data() = default;

};

class custom_data_ram final:
    public custom_data
{

private:
    tvalue value;

public:
    custom_data_ram(tvalue const &value);
    custom_data_ram(tvalue &&value);


};

class custom_data_file final:
    public custom_data
{

private:
    long _file_pos;

public:
    custom_data_file(long file_pos = -1);

    void serialize(
		std::string const &path,
		std::string const &key,
		tvalue const &value,
		bool update_flag = false);
    
    tvalue deserialize(
		std::string const &path) const;


};