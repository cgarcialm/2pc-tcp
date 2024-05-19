#include "TCPClient.h"
#include "Log.h"
#include "MessageTypes.h" 

using namespace std;

class Coordinator {
    public: 
        struct Transaction {
            int account;
            double amount;
        };

        explicit Coordinator(
            const std::string &logfile,
            const std::string &server_host_one, u_short listening_port_one,
            const std::string &server_host_two, u_short listening_port_two
            ) : client1(server_host_one, listening_port_one),
                client2(server_host_two, listening_port_two),
                state(INIT) {
                logFile = LOG_FILE_PATH + logfile;

                // Logging the connections
                string connectMsg = "Connected to " + server_host_one + ":" + to_string(listening_port_one);
                cout << connectMsg << endl;
                logToFile(connectMsg, logFile);

                connectMsg = "Connected to " + server_host_two + ":" + to_string(listening_port_two);
                cout << connectMsg << endl;
                logToFile(connectMsg, logFile);
            }

        void perform_transaction(const Transaction &request) {
            string msg = prepare_message(VOTEREQUEST, request);
            state = WAIT;

            while (state != DONE) {
                string rcvMsg1 = send_message(client1, msg);
                string rcvMsg2 = send_message(client2, msg);
                switch (state) {
                    case WAIT:
                        if (rcvMsg1 == message_type_to_string(VOTECOMMIT) 
                        && rcvMsg2 == message_type_to_string(VOTECOMMIT)) {
                            msg = message_type_to_string(GLOBALCOMMIT);
                            state = COMMIT;
                        } else {
                            msg = message_type_to_string(GLOBALABORT);
                            state = ABORT;
                        }
                        break;
                    case COMMIT:
                        if (rcvMsg1 == message_type_to_string(ACK) 
                        && rcvMsg2 == message_type_to_string(ACK)) {
                            state = DONE;
                        } else {
                            state = ABORT;
                        }
                        break;
                    case ABORT:
                        if (rcvMsg1 == message_type_to_string(ACK) 
                        && rcvMsg2 == message_type_to_string(ACK)) {
                            state = DONE;
                        }
                        break;
                    case INIT:
                    case DONE:
                        break;
                }
            }
        }

        string send_message(TCPClient &client, const string &msg) {
            client.send_request(msg);
            string sendMsg = "Sent to participant: '" + msg + "'";
            cout << sendMsg << endl;
            logToFile(sendMsg, logFile);

            string response = client.get_response();
            string receiveMsg = "Received from participant: '" + response + "'";
            cout << receiveMsg << endl;
            logToFile(receiveMsg, logFile);

            return response;
        }

    private:
        std::string logFile;
        const std::string LOG_FILE_PATH = "logs/";
        TCPClient client1;
        TCPClient client2;

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

