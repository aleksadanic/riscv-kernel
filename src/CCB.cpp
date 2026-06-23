#include "../h/CCB.hpp"
#include "../h/SCB.hpp"
#include "../h/TCB.hpp"
#include "../lib/hw.h"

#include "../lib/console.h"

char CCB::getc () {
    if (inputData->wait () < 0) {
        return -1;
    }
    return inputBuffer.get ();
}

void CCB::putc (char c) {
    outputSpace->wait ();
    outputBuffer.put (c);
    outputData->signal ();
}

void CCB::handleInterrupt () {
    while (*((volatile char*) CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) {
        char c = *((volatile char*) CONSOLE_RX_DATA);
        if (!inputBuffer.put (c)) {
            inputData->signal();
        }
    }
}

void CCB::printer (void*) {
    while (true) {
        outputData->wait ();
        char c = outputBuffer.get ();
        outputSpace->signal ();
        while (!(*((volatile char*) CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT)) {
            TCB::dispatch ();
        }
        *((volatile char*) CONSOLE_TX_DATA) = c;
    }
}

void CCB::init () {
    SCB::open (&outputData, 0);
    SCB::open (&outputSpace, Buffer::N);
    SCB::open (&inputData, 0);
}

SCB* CCB::outputData = 0;
SCB* CCB::outputSpace = 0;
SCB* CCB::inputData = 0;

Buffer CCB::inputBuffer;
Buffer CCB::outputBuffer;