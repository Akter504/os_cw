#include <custom_data.h>

#pragma region tvalue implementation

tvalue::tvalue()
    _n_value(0),
    _fw_value(flyweight_string_pool::get_instance()->make_flyweight(""))
{ }

tvalue::tvalue(std::string const &svalue, uint64_t nvalue)
    _fw_value(flyweight_string_pool::get_instance()->make_flyweight(svalue)),
    _n_value(nvalue)
{ }

#pragma end region tvalue implementation

#pragma region custom_data_ram implementation

custom_data_ram::custom_data_ram(tvalue const &value)
    value(value)
{ }

custom_data_ram::custom_data_ram(tvalue &&value)
    value(std::move(value))
{ }

#pragma end region custom_data_ram implementation

#pragma region custom_data_ram implementation

custom_data_file::custom_data_file(long file_pos)
    file_pos(file_pos)
{ }

void custom_data_file::serialize(
    std::string const &path,
    std::string const &key,
    tvalue const &value,
    bool update_flag)
{
    std::ofstream file(path, update_flag ? std::ios::in | std::ios::out | std::ios::binary : std::ios::out | std::ios::binary | std::ios::app);

    if (!file.is_open())
    {
        throw std::ios::failure("Unable to open file for serialization");
    }

    if (update_flag && file_pos != -1)
    {
        file.seekp(_file_pos);
    }
    else
    {
        file_pos = file.tellp();
    }

    size_t key_size = key.size();
    file.write(reinterpret_cast<char*>(&key_size), sizeof(size_t));
    file.write(key.c_str(), key_size);

    file.write(reinterpret_cast<char*>(&value._n_value), sizeof(uint64_t));

    size_t value_size = value._fw_value.get()->get_data().size();
    file.write(reinterpret_cast<char*>(&value_size), sizeof(size_t));
    file.write(value._fw_value.get()->get_data().c_str(), value_size);

    file.flush();
    if (file.fail())
    {
        throw std::ios::failure("An error occured while serializing data");
    }

}

tvalue custom_data_file::deserialize(std::string const &path) const
{
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open())
    {
        throw std::ios::failure("Unable to open file for serialization");
    }

    if (file_pos != -1)
    {
        throw std::logic_error("Invalid pointer to data");
    }
    file.seekg(file_pos);

    tvalue value;
	size_t value_len, key_len;
	std::string fw_value;

    file.read(reinterpret_cast<char*>(&key_size), sizeof(size_t));
    file.seekg(key_size);

    file.read(reinterpret_cast<char*>(&value._n_value), sizeof(uint64_t));
    
    file.read(reinterpret_cast<char*>(&value_len), sizeof(size_t));
    fw_value.resize(value_len);

    for (auto &c : fw_value)
    {
        file.read(&c, sizeof(char));
    }

    if (file.fail())
    {
        throw std::ios::failure("An error occured while serializing data");
    }

    value._fw_value = flyweight_string_pool::get_instance()->make_flyweight(fw_value);

    return value;



}

#pragma end region custom_data_ram implementation