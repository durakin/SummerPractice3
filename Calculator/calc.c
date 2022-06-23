//
// Created by durakin on 23.06.22.
//

#include "calc.h"
#include "string.h"
#include "ctype.h"
#include "priorities.h"

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
    char stackbuff[255];
    int stackpointer = -1;

    for (int i = 0; i < strlen(infix_notation); i++)
    {
        char current = infix_notation[i];
        if (isdigit(current))
        {
            i = last_digit_index(infix_notation, i, postfix_notation + strlen(postfix_notation));
            postfix_notation[strlen(postfix_notation)+1] = '\0';
            postfix_notation[strlen(postfix_notation)] = ' ';
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
                postfix_notation[(strlen(postfix_notation))+1] = '\0';
                postfix_notation[(strlen(postfix_notation))] = stackbuff[stackpointer];
                stackpointer--;
            }
            stackpointer--;
        }
        if(get_priority(current) != -1)
        {
            //TODO: unary minus
            while(stackpointer >= 0 && (get_priority(stackbuff[stackpointer]) >= get_priority(current)))
            {
                postfix_notation[strlen(postfix_notation)+1] = '\0';
                postfix_notation[strlen(postfix_notation)] = stackbuff[stackpointer];
                stackpointer--;
            }
            stackbuff[++stackpointer] = current;
        }
    }
    while (stackpointer!=-1)
    {
        postfix_notation[strlen(postfix_notation)+1] = '\0';
        postfix_notation[strlen(postfix_notation)] = stackbuff[stackpointer--];
    }
    return strlen(postfix_notation);
}
