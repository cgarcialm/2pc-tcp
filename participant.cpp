#include "2PC_Participant.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "usage: participant serve_port accounts_filename log_filename" << endl;
        return EXIT_FAILURE;
    }
    int serve_port = atoi(argv[1]);
    if (serve_port < 1 || serve_port >= 1 << 16) {
        cerr << "invalid serve_port " << serve_port << endl;
        return EXIT_FAILURE;
    }
    Participant participant((u_short) serve_port, argv[3], argv[2]);
    ostringstream note;
    note << "Transaction service on port " << serve_port << " (Ctrl-C to stop)";
    participant.log(note.str());
    participant.serve();
    return EXIT_SUCCESS;
}
