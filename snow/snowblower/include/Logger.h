#ifndef SNOWBLOWER_LOGGER_H
#define SNOWBLOWER_LOGGER_H 

#include <stdio.h>
#include <string>

enum LogType {
    Critical = 1,
    Error    = 2,
    Warn     = 3,
    Info     = 4,
    Debug    = 5
} 

class Logger 
{
private:
    int _log_type = 3;
    std::string _log_file_name;
    std::FILE* _log_file;
    bool _is_init = false;

   [[nodiscard]] bool is_init() { return _is_init; }

public:
    Logger(/* args */);
    ~Logger();
    init(const char* log_path);
    log(LogType log_type, const char* msg);
};

#endif //SNOWBLOWER_LOGGER_H