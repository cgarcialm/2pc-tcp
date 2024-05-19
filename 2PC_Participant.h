#include "TCPServer.h"
#include "Log.h"
#include "MessageTypes.h"

#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Participant : public TCPServer {
    public:
        explicit Participant(u_short listening_port, const std::string &logfile) 
            : TCPServer(listening_port), state(INIT) {
                logFile = LOG_FILE_PATH + logfile;
            }

    protected:
        void start_client(const std::string &their_host, u_short their_port) override {
            string logMsg = "Accepted connection from " + their_host + ":" + to_string(their_port);
            cout << logMsg << endl;
            logToFile(logMsg, logFile);
        }

bool process(const std::string &request) override {
        if (request.empty()) {
            return false;
        }

        string logMsg = "Received request: '" + request + "'";
        cout << logMsg << endl;
        logToFile(logMsg, logFile);

        vector<string> tokens = parse_request(request);
        string command = tokens[0];
        logMsg = "Command: '" + command + "'";
        cout << logMsg << endl;

        string response;
        switch (state) {
            case INIT:
                if (command == "VOTE-REQUEST") {
                    // TODO: Placeholder condition for approval
                    bool approve = true; 
                    if (approve) {
                        response = message_type_to_string(VOTECOMMIT);
                        state = READY;
                    } else {
                        response = message_type_to_string(VOTEABORT);
                        state = ABORT;
                    }
                }
                break;
            case READY:
                if (command == "GLOBAL-ABORT") {
                    response = message_type_to_string(ACK);
                    state = ABORT;
                } else if (command == "GLOBAL-COMMIT") {
                    response = message_type_to_string(ACK);
                    state = COMMIT;
                }
                break;
            case COMMIT:
                response = message_type_to_string(ACK);
                state = DONE;
                break;
            case ABORT:
                response = message_type_to_string(ACK);
                state = DONE;
                break;
            case DONE:
                return false; // Close the connection
        }

        respond(response);

        logMsg = "Sent response: '" + response + "'";
        cout << logMsg << endl;
        logToFile(logMsg, logFile);

        if (state == DONE) {
            logMsg = "Closing server connection...";
            cout << logMsg << endl;
            logToFile(logMsg, logFile);
            return false; // Close the connection
        }

        return true; // Keep the connection open
    }

    private:
        std::string logFile;
        const std::string LOG_FILE_PATH = "logs/";

    enum State {
        INIT,
        READY,
        COMMIT,
        ABORT,
        DONE
    };

    State state;

    vector<string> parse_request(const string &request) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(request);
        while (getline(tokenStream, token, ':')) {
            tokens.push_back(token);
        }
        return tokens;
    }
};