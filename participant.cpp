#include "2PC_Participant.h"

using namespace std;

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
    string accFile = argv[3];
    Participant participant((u_short) port, logFile, accFile);
    participant.serve();
    return EXIT_SUCCESS;
}
