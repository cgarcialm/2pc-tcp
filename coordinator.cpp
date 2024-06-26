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

    ostringstream oss;
    oss << fixed << setprecision(2); 
    oss << "Transaction: $" << atof(argv[1]) << endl;
    oss << "\tFrom: " << argv[4] << endl;
    oss << "\tTo: " << argv[7] << endl;
    cout << oss.str();

    string host1 = argv[2];
    int port1 = process_port(argv[3]);
    string host2 = argv[5];
    int port2 = process_port(argv[6]);

    try {
        Coordinator coordinator(logFile, 
            host1, (u_short) port1, 
            host2, (u_short) port2
        );
        coordinator.perform_transaction(argv[4], argv[7], atof(argv[1]));
    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
