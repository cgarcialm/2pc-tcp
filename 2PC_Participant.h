#include "TCPServer.h"
#include "Log.h"
#include "MessageTypes.h"

#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>

using namespace std;

class Participant : public TCPServer {
public:
    explicit Participant(
        u_short listening_port,
        const std::string &accfile, 
        const std::string &logfile) 
        : 
        TCPServer(listening_port), 
        partLogger(LOG_FILE_PATH + logfile),
        state(INIT) {
            accFile = ACC_FILE_PATH + accfile;
            accounts = readAccounts(accFile); 
        }

    void log(const string &msg) {
        partLogger.log(msg);
    }

protected:
    void start_client(const std::string &their_host, u_short their_port) override {
        string logMsg = "Accepting coordinator connection. State: " + state_to_string(state);
        log(logMsg);
    }

    bool process(const std::string &request) override {
        string logMsg;
        if (request.empty()) {
            return false;
        }

        vector<string> tokens = parse_request(request);
        string command = tokens[0];

        string response;
        switch (state) {
            case INIT:
                if (command == message_type_to_string(VOTEREQUEST)) {
                    bool approve = is_transaction_valid(tokens[1], stod(tokens[2])); 
                    if (approve) {
                        response = message_type_to_string(VOTECOMMIT);
                        state = READY;
                        logMsg = "Holding: " + tokens[2] + " from account " + tokens[1];
                        log(logMsg);
                    } else {
                        response = message_type_to_string(VOTEABORT);
                        state = ABORT;
                    }
                    logMsg = "Got " + command + ", replying " + response + ". State: " + state_to_string(state);
                    log(logMsg);
                }
                break;
            case READY:
                if (command == message_type_to_string(GLOBALABORT)) {
                    response = message_type_to_string(ACK);
                    state = ABORT;
                } else if (command == message_type_to_string(GLOBALCOMMIT)) {
                    response = message_type_to_string(ACK);
                    state = COMMIT;
                }
                logMsg = "Got " + command + ", replying " + response + ". State: " + state_to_string(state);
                log(logMsg);
                break;
            case COMMIT:
                response = message_type_to_string(ACK);
                state = DONE;

                logMsg = "Committing transaction.";
                log(logMsg);
                break;
            case ABORT:
                response = message_type_to_string(ACK);
                state = DONE;
                break;
            case DONE:
                return false; // Close the connection
        }
        
        respond(response);

        if (state == DONE) {
            return false; // Close the connection
        }

        return true; // Keep the connection open
    }

private:
    const std::string LOG_FILE_PATH = "logs/";
    std::string logFile;
    const std::string ACC_FILE_PATH = "accounts/";
    std::string accFile;
    Log partLogger;
    unordered_map<string, double> accounts;

    unordered_map<string, double> readAccounts(const string &filename) {
        unordered_map<string, double> accounts;
        ifstream file(filename);

        if (!file.is_open()) {
            throw runtime_error("Unable to open account file: " + filename);
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            double amount;
            string account;

            if (iss >> amount >> account) {
                accounts[account] = amount;
            } else {
                throw runtime_error("Error processing line: " + line);
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

    std::string state_to_string(State state) {
        switch (state) {
            case INIT: return "INIT";
            case READY: return "READY";
            case COMMIT: return "COMMIT";
            case ABORT: return "ABORT";
            case DONE: return "DONE";
            default: return "UNKNOWN";
        }
    }

    vector<string> parse_request(const string &request) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(request);
        while (getline(tokenStream, token, ':')) {
            tokens.push_back(token);
        }
        return tokens;
    }

    bool is_transaction_valid(const string &acc, double amount) {
        auto it = accounts.find(acc);
        if (it != accounts.end()) {
            return it->second > amount;
        } else {
            return false; // Account not found
        }
    }
};
