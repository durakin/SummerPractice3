#define BUFFER_SIZE 200
#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#include "string.h"
#include "ctype.h"
#include "priorities.h"

void string_push(char* string, char char_to_push)
{
    string[strlen(string)+1] = '\0';
    string[strlen(string)] = char_to_push;
}

int last_digit_index(char* string, int start, char* digitbuffer)
{
    int position = start;

    while (isdigit(string[position]) && position < strlen(string))
    {
        digitbuffer[position-start] = string[position];
        position++;
    }
    digitbuffer[position-start] = '\0';
    return --position;
}

int to_postfix_notation(char* infix_notation, char* postfix_notation)
{
    char stackbuff[BUFFER_SIZE];
    int stackpointer = -1;

    for (int i = 0; i < strlen(infix_notation); i++)
    {
        char current = infix_notation[i];
        if (isdigit(current))
        {
            i = last_digit_index(infix_notation, i, postfix_notation + strlen(postfix_notation));
            string_push(postfix_notation, ' ');
        }
        if (current == '(')
        {
            stackpointer++;
            stackbuff[stackpointer] = '(';
        }
        if (current == ')')
        {
            while (stackpointer >= 0 && stackbuff[stackpointer] != '(')
            {
                string_push(postfix_notation, stackbuff[stackpointer]);
                stackpointer--;
            }
            stackpointer--;
        }
        if(get_priority(current) != -1)
        {
            //TODO: unary minus
            while(stackpointer >= 0 && (get_priority(stackbuff[stackpointer]) >= get_priority(current)))
            {
                string_push(postfix_notation, stackbuff[stackpointer]);
                stackpointer--;
            }
            stackbuff[++stackpointer] = current;
        }
    }
    while (stackpointer!=-1)
    {
        string_push(postfix_notation, stackbuff[stackpointer--]);
    }
    return (int) strlen(postfix_notation);
}

double execute_operator(char operator, double first, double second)
{
    switch (operator) {
        case '+': return first+second;
        case '-': return first-second;
        case '*': return first*second;
        case '/': return first/second;
        default: {
            fprintf(stderr, "Operator not found: %c", operator);
            return 0;
        }
    }
}

double calculate_postfix_notation(char* postfix_notation)
{
    double stackbuff[BUFFER_SIZE];
    int stackpointer = -1;
    int counter = 0;
    for (int i = 0; i < strlen(postfix_notation); i++)
    {
        char current = postfix_notation[i];
        if(isdigit(current))
        {
            char number[BUFFER_SIZE];
            i = last_digit_index(postfix_notation, i, number);
            stackbuff[++stackpointer] = strtod(number, NULL);
        }
        if(get_priority(current) != -1)
        {
            counter++;
            // TODO: Unary minus
            double second = stackbuff[stackpointer--];
            double first = stackbuff[stackpointer--];
            stackbuff[++stackpointer] = execute_operator(current, first,second);
        }
    }
    return stackbuff[stackpointer];
}
