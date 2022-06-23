//
// Created by durakin on 23.06.22.
//

#ifndef CALCULATOR_CALC_H
#define CALCULATOR_CALC_H

//int calculate_expression(char* expression);
int to_postfix_notation(char* infix_notation, char* postfix_notation);

double calculate_postfix_notation(char* postfix_notation);

#endif //CALCULATOR_CALC_H
