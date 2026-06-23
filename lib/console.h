#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    void __putc(char chr);

    char __getc();

    void console_handler();

    void PrintString(const char* s);

    void PrintInt(long n);

#ifdef __cplusplus
}
#endif