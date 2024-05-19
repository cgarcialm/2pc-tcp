#include "2PC_Coordinator.h"
using namespace std;

int process_port(char *argv) {
    // Process args
    int port = atoi(argv);
    if (port < 1 || port >= 1 << 16) {
        cerr << "invalid port " << port << endl;
        exit(EXIT_FAILURE);
    }
    return port;
}

int main(int argc, char *argv[]) {
    const string logFile = "log.txt";

    if (argc < 8) {
        cerr << "usage: coordinator amount hostname1 port1 acc1 hostname2 port2 acc2" << endl;
        return EXIT_FAILURE;
    }

    Coordinator::Transaction t {argv[4], argv[7], atof(argv[1])};

    string host1 = argv[2];
    int port1 = process_port(argv[3]);
    string host2 = argv[5];
    int port2 = process_port(argv[6]);

    Coordinator coordinator(logFile, 
        host1, (u_short) port1, 
        host2, (u_short) port2
    );
    coordinator.log(t.to_string());
    coordinator.perform_transaction(t);
    
    return EXIT_SUCCESS;
}
