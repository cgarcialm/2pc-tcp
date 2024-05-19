#include "TCPServer.h"
#include "Log.h"
#include "MessageTypes.h"

#include <cstdlib>

using namespace std;

class Participant : public TCPServer {
    public:
        explicit Participant(u_short listening_port, const std::string &logfile) 
            : TCPServer(listening_port) {
                logFile = LOG_FILE_PATH + logfile;
            }

    protected:
        void start_client(const std::string &their_host, u_short their_port) override {
            string logMsg = "Accepted connection from " + their_host + ":" + to_string(their_port);
            cout << logMsg << endl;
            logToFile(logMsg, logFile);
        }

        bool process(const std::string &request) override {
            string logMsg = "Received request: '" + request + "'";
            cout << logMsg << endl;
            logToFile(logMsg, logFile);

            string response = message_type_to_string(VOTECOMMIT);
            respond(response);

            logMsg = "Sent reversed response: '" + response + "'";
            cout << logMsg << endl;
            logToFile(logMsg, logFile);

            logMsg = "Closing server connection...";
            cout << logMsg << endl;
            logToFile(logMsg, logFile);

            return false; // returning false is a signal to shut down server
        }

    private:
        std::string logFile;
        const std::string LOG_FILE_PATH = "logs/";
};