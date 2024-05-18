#include "Log.h"

void logToFile(const std::string &message, const std::string &filename) {
    std::ofstream logFile;
    logFile.open(filename, std::ios_base::app); // Open log file in append mode
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    } else {
        std::cerr << "Unable to open log file: " << filename << std::endl;
    }
}