
#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <C:\labs mai\C++\logger\logger\include\logger.h>
#include <map>
#include <unordered_map>
#include <set>
#include <fstream>
#include <cstring>
#include "client_logger_builder.h"

class client_logger final:
    public logger
{

private:
    std::map<std::string, std::pair<std::ostream*, std::set<logger::severity>>> _log;
    static std::unordered_map<std::string, std::pair<std::ostream*, size_t>> _count_loggers;
    std::string _log_format;

    void decrement_stream_users(std::string const &file_path) const noexcept;
 
public:

    friend class client_logger_builder;

    client_logger();

    client_logger(std::string const& format, std::map<std::string, std::set<logger::severity>> const& configs);

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    std::string parse_message(const std::string& message, logger::severity severity)const;

    std::string to_lower(const std::string& str)const;

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H