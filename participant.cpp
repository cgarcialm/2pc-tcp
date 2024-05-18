#include "TCPServer.h"
#include "Log.h"
#include <iostream>
#include <cstdlib>

using namespace std;

class Participant : public TCPServer {
public:
    explicit Participant(u_short listening_port, const std::string &logfile) 
        : TCPServer(listening_port), logFile(logfile) {}

protected:
    void start_client(const std::string &their_host, u_short their_port) override {
        string logMsg = "Accepted connection from " + their_host + ":" + to_string(their_port);
        cout << logMsg << endl;
        logToFile(logMsg, logFile);
    }

    bool process(const std::string &request) override {
        string logMsg = "Received request: '" + request + "'";
        cout << logMsg << endl;
        logToFile(logMsg, logFile);

        string response(request);
        for (u_long i = 0, j = response.length() - 1; i <= j; i++, j--)
            swap(response[i], response[j]);
        respond(response);

        logMsg = "Sent reversed response: '" + response + "'";
        cout << logMsg << endl;
        logToFile(logMsg, logFile);

        logMsg = "Closing server connection...";
        cout << logMsg << endl;
        logToFile(logMsg, logFile);

        return false; // returning false is a signal to shut down server
    }

private:
    std::string logFile;
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "usage: participant port logfile" << endl;
        return EXIT_FAILURE;
    }
    int port = atoi(argv[1]);
    if (port < 1 || port >= 1 << 16) {
        cerr << "invalid port " << port << endl;
        return EXIT_FAILURE;
    }
    string logFile = argv[2];
    Participant server((u_short) port, logFile);
    server.serve();
    return EXIT_SUCCESS;
}
