#ifndef HANDLER_INFORMATION_FROM_FILE_H
#define HANDLER_INFORMATION_FROM_FILE_H

#include <request_handler.h>
#include <fstream>

class parser final
{   

private:

    handler_chain _chain;
    db *db_strg;

public:

    explicit parser(
        size_t id,
        uint64_t time_for_delete = 0,
        uint64_t time_for_insert = 0,
        uint64_t time_for_obtain = 0,
        uint64_t time_for_obtain_between = 0,
        uint64_t time_for_update = 0,
        db::mode mode = db::mode::in_memory_cache
    )
    {
        db_strg->get_instance()->setup(id,mode);
    }

public:

    void parsing_file(std::string const &file_name)
    {
        std::ifstream file(file_name);
        std::string str_command;

        if (!file.is_open())
        {
            throw std::logic_error("Wrong input file\n");
        }

        while (std::getline(file, str_command))
        {
            if (str_command.empty()) continue;
            if (str_command[str_command.length() - 1] == '\n') str_command.erase(str_command.length() - 1);

            if (str_command == "exit")
            {
                std::cout << "End of program" << std::endl;
                db_strg->get_instance()->clear();
                break;
            }

            std::istringstream part(str_command);
            std::string word;
            validation get_array;
            part >> word;
            bool is_command = 0;
            for (auto elem : get_array._commands)
            {
                if (word == elem)
                {
                    is_command = 1;
                }
            }
            
            std::int64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();  
            std::cout << "Check time: " << current_time << std::endl; 

            if (word != "read" && word != "read_between")
            {
                _chain.check_command(str_command, current_time, db_strg->get_instance());
            }
            else
            {
                db_strg->get_instance()->free_pools();
                _chain.process_chain(str_command, word, db_strg->get_instance());
            }

         }
    }

    void parsing_dialog()
    {
        std::cout << "Information for reference" << std::endl
        << "b tree variants for command:" << std::endl
        << "b" << std::endl << "b_plus" << std::endl << "b_star" << std::endl << "b_star_plus" << std::endl
        << "allocator variants: " << std::endl
        << "global_heap" << std::endl << "sorted_list" << std::endl << "buddy_system" << std::endl << "boundary_tags" << std::endl << "red_black_tree" << std::endl
        << "fit mode variants: " << std::endl
        << "first_fit" << std::endl << "best_fit" << std::endl << "worst_fit" << std::endl << std::endl;

        std::cout << "Commands:" << std::endl;

        std::cout 
        << "add_pool " << "<pool_name> " << "<b tree variant> " << "<t for b tree> " << std::endl
        << "add_schema " << "<pool_name> " << "<schema_name> " << "<b tree variant> " << "<t for b tree> " << std::endl
        << "add_collection " << "<pool_name> " << "<schema_name> " << "<collection_name> " << "<b tree variant> "  << "<allocator variant> " << "<fit mode for allocator> " << "<t for b tree> " << std::endl
        << "delete_pool " << "<pool_name>" << std::endl
        << "delete_schema " << "<pool_name> " << "<schema_name> " << std::endl
        << "delete_collection " << "<pool_name> " << "<schema_name> " << "<collection_name> " << std::endl
        << "add " << "<pool_name> " << "<schema_name> " << "<collection_name> " << "<id> " << "<numeric value> " << "string value " << std::endl
        << "delete " << "<pool_name> " << "<schema_name> " << "<collection_name> " << "<id> "  << std::endl
        << "read " << "<pool_name> " << "<schema_name> " << "<collection_name> " << "<id> " << "<time(HH::MM::SS)> " << "<date(YY::MM::DD)> " << std::endl
        << "read_between " << "<pool_name> " << "<schema_name> " << "<collection_name> " << "<id_min> " << "<id_max> " << "<lower_boundary_inclusive " << "<upper_boundary_inclusive " << "<time(HH::MM::SS)> " << "<date(YY::MM::DD)> " << std::endl
        << "update " << "<pool_name> " << "<schema_name> " << "<collection_name> " << "<id> " << std::endl
        << "exit" << std::endl
        << "insert_files " << "<file_name_1> " << "<file_name_2> " << "... " << "<file_name>" << std::endl
        << "recovery_data " << "(ONLY IF THE DATA WAS STORED IN MEMORY)" << std::endl << std::endl;

        std::string str_command;
        std::cout << "Input command: " << std::endl;
        while (true)
        {
            std::cout << "> ";
            std::getline(std::cin, str_command);
            if (str_command.empty()) continue;
            if (str_command[str_command.length() - 1] == '\n') str_command.erase(str_command.length() - 1);

            if (str_command == "exit")
            {
                std::cout << "End of program" << std::endl;
                db_strg->get_instance()->clear();
                break;
            }

            std::istringstream part(str_command);
            std::string word;
            validation get_array;
            part >> word;
            bool is_command = 0;
            for (auto elem : get_array._commands)
            {
                if (word == elem)
                {
                    is_command = 1;
                }
            }
            if (!is_command && (word != "insert_files"))
            {
                std::cout << "Unknown command " << std::endl;
                continue;
            }
            
            if (word == "insert_files")
            {
                std::vector<std::string> files;
                while (part >> word) 
                {
                    files.push_back(word);
                }
                if (files.empty())
                {
                    std::cout << "You didn't specify any files" << std::endl;
                    continue;
                }
                else
                {
                    for (auto elem : files)
                    {
                        parsing_file(elem);
                    }
                }
                continue;
            }
            std::int64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();  
            std::cout << "Check time: " << current_time << std::endl; 

            if (word != "read" && word != "read_between")
            {
                _chain.check_command(str_command, current_time, db_strg->get_instance());
            }
            else
            {
                db_strg->get_instance()->free_pools();
                _chain.process_chain(str_command, word, db_strg->get_instance());
            }
        }

    }

};

#endif // HANDLER_INFORMATION_FROM_FILE_H