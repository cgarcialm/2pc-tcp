#include "TCPClient.h"
#include "Log.h"

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
            : TCPClient (server_host, listening_port), logFile(logfile) {
                string connectMsg = "Connected to " + server_host + ":" + to_string(listening_port);
                cout << connectMsg << endl;
                logToFile(connectMsg, logFile);
            }

        void send_message(const Transaction &request) {
            string msg = prepare_message(VOTEREQUEST, request);
            
            send_request(msg);
            string sendMsg = "Sent to participant: '" + msg + "'";
            cout << sendMsg << endl;
            logToFile(sendMsg, logFile);

            string response = get_response();
            string receiveMsg = "Received from participant: '" + response + "'";
            cout << receiveMsg << endl;
            logToFile(receiveMsg, logFile);
        }

    private:
        std::string logFile;

        enum messageType {
            VOTEREQUEST,
            VOTECOMMIT,
            VOTEABORT,
            GLOBALCOMMIT,
            GLOBALABORT,
            ACK
        };

        // Helper function to convert enum to string
        std::string message_type_to_string(messageType msg) {
            switch (msg) {
                case VOTEREQUEST: return "VOTE-REQUEST";
                case VOTECOMMIT: return "VOTE-COMMIT";
                case VOTEABORT: return "VOTE-ABORT";
                case GLOBALCOMMIT: return "GLOBAL-COMMIT";
                case GLOBALABORT: return "GLOBAL-ABORT";
                case ACK: return "ACK";
                default: return "UNKNOWN";
            }
        }

        string prepare_message(const messageType msgType, const Transaction &t) 
        {
            string msg = message_type_to_string(msgType) + ":";
            msg += to_string(t.account) + ":" + to_string(t.amount);

            return msg;
        }

};

