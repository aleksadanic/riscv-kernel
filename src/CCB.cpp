#include "../h/CCB.hpp"
#include "../h/SCB.hpp"
#include "../h/TCB.hpp"
#include "../lib/hw.h"

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
    while (*((char*) CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) {
        char c = *((char*) CONSOLE_RX_DATA);
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
        while (!(*((char*) CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT)) {
            TCB::dispatch ();
        }
        *((char*) CONSOLE_TX_DATA) = c;
    }
}

bool CCB::outputBufferEmpty () {
    return outputBuffer.empty ();
}

void CCB::init () {
    SCB::open (&outputData, 0);
    SCB::open (&outputSpace, BoundedBuffer::N);
    SCB::open (&inputData, 0);
}

SCB* CCB::outputData = 0;
SCB* CCB::outputSpace = 0;
SCB* CCB::inputData = 0;

BoundedBuffer CCB::inputBuffer;
BoundedBuffer CCB::outputBuffer;

int BoundedBuffer::put (char c) {
    if (size == N) {
        return -1;
    }
    buffer[tail] = c;
    tail = (tail + 1) % N;
    size++;
    return 0;
}

char BoundedBuffer::get () {
    char c = buffer[head];
    head = (head + 1) % N;
    size--;
    return c;
}

bool BoundedBuffer::empty () {
    return size == 0;
}