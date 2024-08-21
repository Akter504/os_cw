#include "../include/client_logger.h"

std::unordered_map<std::string, std::pair<std::ostream*, size_t>> client_logger::_count_loggers;

std::string client_logger::to_lower(const std::string& str)const{
    std::string result;
    result.reserve(str.length());

    for (char c : str) {
        result += std::tolower(c);
    }

    return result;
}

client_logger::client_logger(std::string const& format, std::map<std::string, std::set<logger::severity>> const& configs)
{
    _log_format = format;
    for (auto& el : configs)
    {
        const auto& find_elem = client_logger::_count_loggers.find(el.first);
        if (find_elem == client_logger::_count_loggers.end())
        {
            const std::string name_of_file = el.first;
                std::ostream* new_stream = nullptr;
                if (to_lower(name_of_file) == "console")
                {
                    new_stream = &std::cout;
                }
                else
                {
                    try
                    {
                        new_stream = (new std::ofstream(el.first));
                    }
                    catch(const std::exception& error)
                    {
                        delete new_stream;
                        std::cerr << error.what() << std::endl;
                        continue;
                    }
                }
                client_logger::_count_loggers.insert(std::make_pair(el.first, std::make_pair(new_stream,1)));
                _log[el.first] = std::make_pair(new_stream, el.second);
        }
        else
        {
            find_elem->second.second++;
            _log[el.first] = std::make_pair(find_elem->second.first, el.second);
        }
    }

}

client_logger::client_logger(
    client_logger const &other)
{
    _log = other._log;
    _log_format = other._log_format;
    for (auto elem : _log)
    {
        _count_loggers[elem.first].second++;
    }
}

client_logger &client_logger::operator=(
    client_logger const &other)
{
        if (this != &other) 
        {
            for (auto elem : _log)
            {
                decrement_stream_users(elem.first);
            }
            _log = other._log;
            _log_format = other._log_format;
            for (auto elem : _log)
            {
                _count_loggers[elem.first].second++;
            }
        }
        return *this;
}

client_logger::client_logger(client_logger&& other) noexcept
        : _log(std::move(other._log)),
          _log_format(std::move(other._log_format)) 
            { 

            }

client_logger &client_logger::operator=(
    client_logger &&other) noexcept
{
    if (this != &other) 
    {
        for (auto elem : _log)
        {
            decrement_stream_users(elem.first);
        }
        _log = (std::move(other._log));
        _log_format = (std::move(other._log_format));
    }
    return *this;

}

client_logger::~client_logger() noexcept
{
    for (auto elem : _log)
    {
        decrement_stream_users(elem.first);
    }

    _log.clear();
    _count_loggers.clear();
}

std::string client_logger::parse_message(const std::string& message, logger::severity severity)
const{
    std::string string_severity = severity_to_string(severity);
    std::string format = _log_format;
    std::string current_time_loc = current_time_to_string();
    std::string current_data_loc = current_date_to_string();

    int flag_error = 0;
    size_t count_char = 0;
    size_t length = _log_format.length();
    int i = 0;
    std::string result_string = "";
    while (i <= (length))
    {
        if (_log_format[i] == '%' && 
        (_log_format[i+1] == 'd' 
        ||_log_format[i+1] == 't' 
        || _log_format[i+1] == 's' 
        || _log_format[i+1] == 'm'))
        {
            switch (_log_format[i+1])
            {
            case 'd':
                result_string += current_data_loc;
                break;
            case 't':
                result_string += current_time_loc;
                break;
            case 's':
                result_string += string_severity;
                break;
            case 'm':
                result_string += message;
                break;
            default:
                break;
            }
            result_string += " ";
            i +=3;
        }
    }
    result_string += "\n";
    return result_string;

    


}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    for (const auto& pair: _log)
    {
        const auto& set_in_pair = pair.second.second;
        if (set_in_pair.find(severity) != set_in_pair.end())
        {
            std::string output_text = parse_message(text, severity);
            if (pair.second.first != nullptr) 
            {
                (*pair.second.first) << output_text << std::endl;
            }
        }
    }
}

void client_logger::decrement_stream_users(std::string const &file_path) const noexcept
{
    if (file_path.size() == 0)
    {
        return;
    }
    
    auto iter = _count_loggers.find(file_path);
    
    auto &stream = iter->second.first;
    auto &counter = iter->second.second;
    
    counter--;
    
    if (counter == 0)
    {
        stream->flush();
        if (file_path != "console")
        {
            delete stream;
        }
        _count_loggers.erase(iter);
    }
}