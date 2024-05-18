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
            send_request(message_to_string(VOTEREQUEST));
            string sendMsg = "Sent to participant: '" + message_to_string(VOTEREQUEST) + "'";
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
        std::string message_to_string(messageType msg) {
            switch (msg) {
                case VOTEREQUEST: return "VOTEREQUEST";
                case VOTECOMMIT: return "VOTECOMMIT";
                case VOTEABORT: return "VOTEABORT";
                case GLOBALCOMMIT: return "GLOBALCOMMIT";
                case GLOBALABORT: return "GLOBALABORT";
                case ACK: return "ACK";
                default: return "UNKNOWN";
            }
        }

};
