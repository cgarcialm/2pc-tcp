#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Log {
    private:
        string logFile;

    public:
        Log(const string &logfile) : logFile(logfile) {}

        void logToFile(const string &message, const string &filename) {
            ofstream logFile;
            logFile.open(filename, ios_base::app); // Open log file in append mode
            if (logFile.is_open()) {
                logFile << message << endl;
                logFile.close();
            } else {
                cerr << "Unable to open log file: " << filename << endl;
            }
        }

        void log(const string &msg) {
            cout << msg << endl;
            logToFile(msg, logFile);
        }
};
#endif // LOG_H
