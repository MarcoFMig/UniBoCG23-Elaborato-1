#include "logger.hpp"

#include <ctime>
#include <string>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <filesystem>

Logger::Logger() {
  this->minimalInfoLevel = Level::info;
  if (!std::filesystem::exists(__LOGGER_DUMPPATH)) {
    if (!std::filesystem::create_directories(__LOGGER_DUMPPATH)) {
      printf("Logger could not create directories for logging");
    }
  }
}

Logger& Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::log(Logger::Level level, std::string message) {
  Logger::log(level, message.c_str());
}

void Logger::log(Level level, const char* message) {
  std::string formattedTime = std::string("[") + this->getFormattedTime() + std::string("]");
  this->buffer.push_back(formattedTime + std::string(" ") + message + std::string("\n"));
  if (this->buffer.size() > __LOGGER_MAXBUFSIZE) {
    this->dumpBuffer();
  }
  if (this->minimalInfoLevel > level) {
    return;
  }
  printf("%s [%s] %s\n", formattedTime.c_str(), this->levelAsString(level), message);
}

void Logger::setMinimalInfoLevel(Logger::Level level) {
  this->minimalInfoLevel = level;
}

std::string Logger::getFormattedTime() {
  return this->getFormattedTime(false);
}
std::string Logger::getFormattedTime(bool formatForFile) {
    auto now = std::time(nullptr);           // Acquisisce il tempo corrente come time_t
    auto localTime = std::localtime(&now);   // Converte time_t in tm come ora locale

    char buffer[100];
    if (formatForFile) {
      std::strftime(buffer, sizeof(buffer), "%d-%m-%Y", localTime); // Formatta l'ora
    } else {
      std::strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime); // Formatta l'ora
    }
    return std::string(buffer);
}

const char* Logger::levelAsString(Level level) {
  switch(level) {
    case (fine):
    return "FINE";
    case (info):
    return "INFO";
    case (warning):
    return "WARNING";
    case (error):
    return "ERROR";
    case (fatal):
    return "FATAL";
    default:
    return "WARNING";
  }
}

void Logger::dumpBuffer() {
  std::ofstream file(__LOGGER_DUMPPATH + this->getFormattedTime(true) + __LOGGER_FILENAME, std::ofstream::app);
  for (std::string log : this->buffer) {
    file.write(log.c_str(), log.size());
  }
  this->buffer.clear();
  file.close();
}

Logger::~Logger() {
  this->buffer.push_back(std::string("\\\\\\ PROGRAM - EXIT ///\n"));
  this->dumpBuffer();
}
