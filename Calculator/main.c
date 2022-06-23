#include <stdio.h>
#include "calc.h"

int main() {
    char booba[256];
    booba[0] = '\0';

    char hooba[256] = "0-3+4*5-1*3+1/2";

    printf("%d\n", to_postfix_notation(hooba, booba));
    printf("%s\n", booba);
    printf("%f\n", calculate_postfix_notation(booba));
    return 0;
}
