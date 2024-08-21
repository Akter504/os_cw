#include <iostream>
#include <fstream>
#include <json.hpp>
#include "../include/client_logger.h" 
#include "../include/client_logger_builder.h"
//#include <C:\labs mai\C++\logger\logger\include\logger.h>
//#include <C:\labs mai\C++\logger\logger\include\logger_builder.h>



using json = nlohmann::json;

int main() {
    client_logger_builder* builder_logger = new client_logger_builder();
    logger* new_logger = nullptr;
    logger *logger_instance = builder_logger
        ->add_file_stream("test.txt", logger::severity::debug)
        ->build();
    delete builder_logger;
    logger_instance->log("1", logger::severity::debug);
    //builder_logger
    //->transform_with_configuration("C:\\labs mai\\mp_os\\logger\\client_logger\\tests\\config.json", "loggers");

    /*try
    {
        new_logger = builder_logger->build();
        new_logger->log("I be afraid", logger::severity(1));
        new_logger->log("Who cared?", logger::severity(2));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }*/
    return 0;
}