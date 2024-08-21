#include "../include/client_logger_builder.h"
#include "json.hpp"


client_logger_builder::client_logger_builder() : data("%t %d %s %m"){}

client_logger_builder::client_logger_builder(std::string _format) : data(_format) {}

logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    applications[stream_file_path].insert(severity);
    return this;
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    applications["console"].insert(severity);
    return this;
}

logger_builder *client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    json config_data = nullptr;
    try 
    {
        std::ifstream config_file(configuration_file_path);
        config_file >> config_data;
    } 
    catch(const std::exception& error) 
    {
        std::cerr << "Exception: " << error.what() << std::endl;
    }
    catch (json::parse_error& e) 
    {
        std::cerr << "Exception parse: " << e.what() << std::endl;
    }
    
    if (!config_data.empty())
    {
        if (config_data.find("loggers") != config_data.end()) {
            auto loggers = config_data["loggers"];
            try {
                for (const auto& logger : loggers) 
                {
                    auto severity_levels = logger["severity_levels"];
                    if (logger.contains("file_path")) 
                    {
                        for (const auto& level_str : severity_levels) 
                        {
                            add_file_stream(logger["file_path"], get_input_severity(level_str));
                        }
                    }
                    else
                    {
                        for (const auto& level_str : severity_levels) 
                        {
                            add_console_stream(get_input_severity(level_str));
                        }
                    }
                    
                }
            } 
            catch (json::exception& e) 
            {
                std::cerr << "Error accessing JSON data: " << e.what() << std::endl;
            }
        }
        else
        {
            std::cout << "Config file is no have configuration for logger"  << std::endl;
        }
    }
    else
    {
        std::cout << "Config file is empty"  << std::endl;
    }
    return this;

}

logger_builder *client_logger_builder::clear()
{
    applications.clear();
    data.clear();
}

logger* client_logger_builder::build() const 
{
    logger* temp = new client_logger(data, applications);
    return temp;
}

