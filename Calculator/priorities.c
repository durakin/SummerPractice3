#include "priorities.h"
int get_priority(char operation)
{
    // Should I use something like dictionary map?
    switch (operation) {
        case '+':
        case '-': return 1;
        case '/':
        case '*': return 2;
        case '~': return 3;
        default:
            return -1;
    }
}
