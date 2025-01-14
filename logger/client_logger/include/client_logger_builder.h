#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H

#include <map>
#include <set>
#include <logger_builder.h>
#include "client_logger.h"
#include "json.hpp"


using json = nlohmann::json;

class client_logger_builder final:
    public logger_builder
{

private:
    std::string data;
    std::map<std::string, std::set<logger::severity>> applications;

public:

    client_logger_builder();

    client_logger_builder(
        std::string _format);
        
    ~client_logger_builder() noexcept override = default;

public:

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

    logger::severity get_input_severity(std::string severity_string)
    {
        return string_to_severity(severity_string);
    }

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H
