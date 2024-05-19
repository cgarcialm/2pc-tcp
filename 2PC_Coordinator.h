#include "TCPClient.h"
#include "Log.h"
#include "MessageTypes.h" 

using namespace std;

class Coordinator : public TCPClient {
    public: 
        struct Transaction {
            int account;
            double amount;
        };

        explicit Coordinator(
            const std::string &server_host, u_short listening_port, const std::string &logfile
            ) 
            : TCPClient (server_host, listening_port), state(INIT) {
                string connectMsg = "Connected to " + server_host + ":" + to_string(listening_port);
                cout << connectMsg << endl;
                logFile = LOG_FILE_PATH + logfile;
            }

        void perform_transaction(const Transaction &request) {
            string msg = prepare_message(VOTEREQUEST, request);
            state = WAIT;

            while (state != DONE) {
                string rcvMsg = send_message(msg);
                switch (state) {
                    case WAIT:
                        if (rcvMsg == message_type_to_string(VOTECOMMIT)) {
                            msg = message_type_to_string(GLOBALCOMMIT);
                            state = COMMIT;
                        } else if (rcvMsg == message_type_to_string(VOTEABORT)) {
                            msg = message_type_to_string(GLOBALABORT);
                            state = ABORT;
                        }
                        break;
                    case COMMIT:
                        if (rcvMsg == message_type_to_string(ACK)) {
                            state = DONE;
                        }
                        break;
                    case ABORT:
                        if (rcvMsg == message_type_to_string(ACK)) {
                            state = DONE;
                        }
                        break;
                    case INIT:
                    case DONE:
                        break;
                }
            }
        }

        string send_message(const string &msg) {
            send_request(msg);
            string sendMsg = "Sent to participant: '" + msg + "'";
            cout << sendMsg << endl;
            logToFile(sendMsg, logFile);

            string response = get_response();
            string receiveMsg = "Received from participant: '" + response + "'";
            cout << receiveMsg << endl;
            logToFile(receiveMsg, logFile);

            return response;
        }

    private:
        std::string logFile;
        const std::string LOG_FILE_PATH = "logs/";

        string prepare_message(const messageType msgType, const Transaction &t) 
        {
            string msg = message_type_to_string(msgType) + ":";
            msg += to_string(t.account) + ":" + to_string(t.amount);

            return msg;
        }

        enum State {
            INIT,
            WAIT,
            COMMIT,
            ABORT,
            DONE
        };

        State state;

};

