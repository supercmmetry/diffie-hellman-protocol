#ifndef DIFFIE_HELLMAN_TLS_CONTROL_H
#define DIFFIE_HELLMAN_TLS_CONTROL_H

enum ControlWord {
    EXCHANGE = 0x1,
    TEXT = 0x2,
    FILEDATA = 0x3
};

#endif
