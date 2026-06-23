#include "../lib/console.h"

void idle (void* arg) {
    // printString ("idle\n");
    int x = 0;
    while (true) {
        x++;
        if (x % 10000000 == 0) {

            // __putc('a');
        }
        // __putc('\n');
    }
}