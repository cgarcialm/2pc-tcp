#include "2PC_Coordinator.h"
using namespace std;

int process_port(char *argv) {
    // Process args
    int port = atoi(argv);
    if (port < 1 || port >= 1 << 16) {
        cerr << "invalid port " << port << endl;
        return EXIT_FAILURE;
    }
    return port;
}

int main(int argc, char *argv[]) {
    const string logFile = "log.txt";
    Coordinator::Transaction t {1000, 10.01};

    if (argc < 4) {
        cerr << "usage: coordinator hostname1 port1 hostname2 port2" << endl;
        return EXIT_FAILURE;
    }

    int portOne = process_port(argv[2]);
    int portTwo = process_port(argv[4]);

    Coordinator coordinator(logFile, argv[1], (u_short) portOne, argv[1], (u_short) portTwo);
    coordinator.perform_transaction(t);
    
    return EXIT_SUCCESS;
}

