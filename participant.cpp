#include "2PC_Participant.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cerr << "usage: participant serve_port accounts_filename log_filename" << endl;
        return EXIT_FAILURE;
    }

    int serve_port = atoi(argv[1]);
    if (serve_port < 1 || serve_port >= 1 << 16) {
        cerr << "invalid serve_port " << serve_port << endl;
        return EXIT_FAILURE;
    }

    try {
        Participant participant((u_short) serve_port, argv[2], argv[3]);
        ostringstream note;
        note << "Transaction service on port " << serve_port << " (Ctrl-C to stop)";
        participant.log(note.str());
        participant.serve();
    } catch (const exception &e) {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
