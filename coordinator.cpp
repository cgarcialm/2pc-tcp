#include "2PC_Coordinator.h"
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

    Coordinator coordinator(argv[1], (u_short) port, logFile);
    
    // Request transaction
    Coordinator::Transaction t {1000, 10.01};
    coordinator.send_message(t);
    
    return EXIT_SUCCESS;
}

