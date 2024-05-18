#include "TCPClient.h"
#include "Log.h"
using namespace std;


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
    TCPClient client(argv[1], (u_short) port);

    string connectMsg = "Connected to " + string(argv[1]) + ":" + to_string(port);
    cout << connectMsg << endl;
    logToFile(connectMsg, logFile);

    client.send_request(argv[3]);
    string sendMsg = "Sent to participant: '" + string(argv[3]) + "'";
    cout << sendMsg << endl;
    logToFile(sendMsg, logFile);

    string response = client.get_response();
    string receiveMsg = "Received from participant: '" + response + "'";
    cout << receiveMsg << endl;
    logToFile(receiveMsg, logFile);

    return EXIT_SUCCESS;
}

