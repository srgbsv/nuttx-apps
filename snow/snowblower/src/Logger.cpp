#include "Logger.h"
#include <syslog.h>

Logger::Logger(/* args */)
{
}

Logger::~Logger()
{
    if (is_init() && _fd) {
        std::fclose(_fd);
    }
}

Logger::init (const char* log_path) {
    if ((c != NULL) && (c[0] == '\0')) {
        syslog(LOG_ERR, "Empty filename");
    }
    _log_file = std::string(log_path);
    _fd = std::fopen(log_path, "w+");
    if (!_fd)
    {
        std::perror("File opening failed");
    }
    _is_init = true;
}

Logger::log (LogType log_type, const char* msg) {
    if (is_init()) {
        char * logstring = char[300];
        strftime(logstring, 100, "%Y-%m-%d %H:%M:%S", timeinfo);

        sprintf 
        std::fwrite(logstring, sizeof(char), strlen(msg), _fd);
    }
}