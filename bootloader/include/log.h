#ifndef LOG_H_
#define LOG_H_

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_PANIC,
};

void clear_debug_buffer();
void log(enum LogLevel, char*);
void logf(enum LogLevel, char* fmt, ...);

#endif // LOG_H_
