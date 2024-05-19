#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H

#include <string>

enum messageType {
    VOTEREQUEST,
    VOTECOMMIT,
    VOTEABORT,
    GLOBALCOMMIT,
    GLOBALABORT,
    ACK
};

// Helper function to convert enum to string
std::string message_type_to_string(messageType msg) {
    switch (msg) {
        case VOTEREQUEST: return "VOTE-REQUEST";
        case VOTECOMMIT: return "VOTE-COMMIT";
        case VOTEABORT: return "VOTE-ABORT";
        case GLOBALCOMMIT: return "GLOBAL-COMMIT";
        case GLOBALABORT: return "GLOBAL-ABORT";
        case ACK: return "ACK";
        default: return "UNKNOWN";
    }
}

#endif // MESSAGETYPES_H
