#pragma once

#include <list>
#include <string>

#define __LOGGER_MAXBUFSIZE 5
#define __LOGGER_DUMPPATH "./logs/"
#define __LOGGER_FILENAME " log.txt"

class Logger {
  public:
    enum Level {
      fine = 0,
      info,
      warning,
      error,
      fatal
    };
  private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    ~Logger();

    Logger::Level minimalInfoLevel;
    std::list<std::string> buffer;

    std::string getFormattedTime();
    std::string getFormattedTime(bool formatForFile);
    const char* levelAsString(Logger::Level level);
    void dumpBuffer();
  public:
    static Logger& getInstance();
    void log(Logger::Level level, std::string message);
    void log(Logger::Level level, const char* message);
    void setMinimalInfoLevel(Logger::Level level);
};
