#include "TCPClient.h"
#include "Log.h"
using namespace std;


class Coordinator : public TCPClient {
public: 
    explicit Coordinator(
        const std::string &server_host, u_short listening_port, const std::string &logfile
        ) 
        : TCPClient (server_host, listening_port), logFile(logfile) {
            string connectMsg = "Connected to " + server_host + ":" + to_string(listening_port);
            cout << connectMsg << endl;
            logToFile(connectMsg, logFile);
        }

    void send_message(const string &request) {
        send_request(request);
        string sendMsg = "Sent to participant: '" + string(request) + "'";
        cout << sendMsg << endl;
        logToFile(sendMsg, logFile);

        string response = get_response();
        string receiveMsg = "Received from participant: '" + response + "'";
        cout << receiveMsg << endl;
        logToFile(receiveMsg, logFile);
    }

private:
    std::string logFile;
};

int main(int argc, char *argv[]) {
    const string logFile = "log.txt";

    if (argc < 4) {
        cerr << "usage: coordinator hostname port message" << endl;
        return EXIT_FAILURE;
    }
    int port = atoi(argv[2]);
    if (port < 1 || port >= 1 << 16) {
        cerr << "invalid port " << port << endl;
        return EXIT_FAILURE;
    }

    Coordinator coordinator(argv[1], (u_short) port, logFile);
    coordinator.send_message(argv[3]);
    
    return EXIT_SUCCESS;
}

