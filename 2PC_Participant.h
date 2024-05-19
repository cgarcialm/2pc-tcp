#include "TCPServer.h"
#include "Log.h"
#include "MessageTypes.h"

#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

class Participant : public TCPServer {
    public:
        explicit Participant(
            u_short listening_port, 
            const std::string &logfile,
            const std::string &accfile) 
            : 
            TCPServer(listening_port), 
            state(INIT) {
                logFile = LOG_FILE_PATH + logfile;
                accFile = ACC_FILE_PATH + accfile;
                accounts = readAccounts(accFile);
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
                    bool approve = is_transaction_valid(tokens[1], stod(tokens[2])); 
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
        const std::string LOG_FILE_PATH = "logs/";
        std::string logFile;
        const std::string ACC_FILE_PATH = "accounts/";
        std::string accFile;
        unordered_map<string, double> accounts;

        unordered_map<string, double> readAccounts(const string &filename) {
            unordered_map<string, double> accounts;
            ifstream file(accFile);

            if (!file.is_open()) {
                cerr << "Unable to open file: " << filename << endl;
                return accounts;
            }

            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                double amount;
                string account;

                if (iss >> amount >> account) {
                    accounts[account] = amount;
                } else {
                    cerr << "Error processing line: " << line << endl;
                }
            }

            file.close();
            return accounts;
        }

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

        bool is_transaction_valid(string &acc, double amount) {
            return accounts[acc] > amount;
        }
};