#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    void __putc(char chr);

    char __getc();

    void console_handler();

    void printString(const char* s);

    void printInt(long n);

#ifdef __cplusplus
}
#endif