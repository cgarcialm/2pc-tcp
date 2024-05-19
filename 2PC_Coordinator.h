#include "TCPClient.h"
#include "Log.h"
#include "MessageTypes.h" 

#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

class Coordinator {
    public: 
        struct Transaction {
            string accFrom;
            string accTo;
            double amount;

            string to_string() const {
                ostringstream oss;
                oss << fixed << setprecision(2); 
                oss << "Transaction: $" << amount << endl;
                oss << "\tFrom: " << accFrom << endl;
                oss << "\tTo: " << accTo << endl;
                return oss.str();
            }
        };

        explicit Coordinator(
            const string &logfile,
            const string &server_host_one, u_short listening_port_one,
            const string &server_host_two, u_short listening_port_two
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
            string msg1 = prepare_message(VOTEREQUEST, request.accFrom, request.amount);
            string msg2 = prepare_message(VOTEREQUEST, request.accTo, -request.amount);
            state = WAIT;

            while (state != DONE) {
                string rcvMsg1 = send_message(client1, msg1);
                string rcvMsg2 = send_message(client2, msg2);
                switch (state) {
                    case WAIT:
                        if (rcvMsg1 == message_type_to_string(VOTECOMMIT) 
                        && rcvMsg2 == message_type_to_string(VOTECOMMIT)) {
                            msg1 = message_type_to_string(GLOBALCOMMIT);
                            msg2 = message_type_to_string(GLOBALCOMMIT);
                            state = COMMIT;
                        } else {
                            msg1 = message_type_to_string(GLOBALABORT);
                            msg2 = message_type_to_string(GLOBALABORT);
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
        string logFile;
        const string LOG_FILE_PATH = "logs/";
        TCPClient client1;
        TCPClient client2;

        string prepare_message(const messageType msgType, const string &acc, const double &amount) 
        {
            string msg = message_type_to_string(msgType) + ":";
            msg += acc + ":" + to_string(amount);

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

