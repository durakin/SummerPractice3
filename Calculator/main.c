#define BUFFER_SIZE 200
#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No expression" );
        exit(EXIT_FAILURE);
    }
    if (argc > 2) {
        fprintf(stderr, "Too many arguments");
        exit(EXIT_SUCCESS);
    }
    char posfixbuff[BUFFER_SIZE];
    posfixbuff[0] = '\0';
    to_postfix_notation(argv[1], posfixbuff);
    fprintf(stdout,"%f\n", calculate_postfix_notation(posfixbuff));
    exit(EXIT_SUCCESS);
}
