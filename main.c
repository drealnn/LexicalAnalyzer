#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

typedef struct {

    int class;
    char lexeme[11];

}token_t;

// constants indicating each token's type
typedef enum {
    nulsym= 1, identsym = 2, numbersym = 3, plussym = 4,
    minussym = 5, multsym = 6,  slashsym = 7, oddsym = 8,
    eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
    gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16,
    commasym = 17, semicolonsym = 18, periodsym = 19,
    becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23,
    thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
    varsym = 29, procsym = 30, writesym = 31, readsym = 32,
    elsesym = 33
    }tokenEnum;

token_t token;
vector codeAry; // Dynamic array structure for storing clean code input


void outputCleanTxt();          // creates cleaninput.txt and puts input into code array
void lexemeTable();             // creates lexeme table by analyzing code array character by character through deterministic finite automaton
int isLetter(char myChar);      // checks if character is [A-Ba-B]
int isDigit(char myChar);       // checks if character is [0-9]
int isSpecial(char myChar);     // checks for special characters (e.g. ' ', '\n', '\t' ...)

int main()
{
    vectorInit(&codeAry);
    outputCleanTxt();


    printf("%s\n", codeAry.data);

   // printf("%d\n", codeAry.size);
   // printf("%d\n", codeAry.capacity);




    vectorFree(&codeAry);

    return 0;
}
void lexemeTable()
{
    int state = 0;
    int i, last_index;

    token.class = -1;
    token.lexeme[0] = '\0';

    for (i = 0; i < codeAry.size; i++)
    {
        last_index = i+1;

        switch (state)
        {
            // state 0: check first character of token
            case(0):

                // check for rel-ops-------------------------------

                if (codeAry.data[i] == '=')
                {
                    token.class = eqlsym;
                    token.lexeme[strlen(token.lexeme)] = '=';

                }
                else if (codeAry.data[i] == '<')
                {
                    token.class = lessym;
                    token.lexeme[strlen(token.lexeme)] = '<';

                    // check next character for additional rel-ops
                    if (codeAry.data[last_index] == '=')
                    {
                        token.class = leqsym;
                        token.lexeme[strlen(token.lexeme)] = '=';
                    }
                    else  if (codeAry.data[last_index] == '>')
                    {
                        token.class = neqsym;
                        token.lexeme[strlen(token.lexeme)] = '>';
                    }

                }
                else if (codeAry.data[i] == '>')
                {
                    token.class = gtrsym;
                    token.lexeme[strlen(token.lexeme)] = '>';

                    // check next character for additional rel-ops
                    if (codeAry.data[last_index] == '=')
                    {
                        token.class = geqsym;
                        token.lexeme[strlen(token.lexeme)] = '=';
                    }
                }

                // Check for Special Symbols -----------------------

                else if (codeAry.data[i] == ';')
                {
                    token.class = semicolonsym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == ',')
                {
                    token.class = commasym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == '.')
                {
                    token.class = periodsym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == '(')
                {
                    token.class = lparentsym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == ')')
                {
                    token.class = rparentsym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == '+')
                {
                    token.class = plussym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == '-')
                {
                    token.class = minussym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == '*')
                {
                    token.class = multsym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == '/')
                {
                    token.class = slashsym;
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                }
                else if (codeAry.data[i] == ':')
                {
                    token.lexeme[strlen(token.lexeme)] = codeAry.data[i];
                    if (codeAry.data[last_index] == '=')
                    {
                        token.class = becomessym;
                        token.lexeme[strlen(token.lexeme)] = codeAry.data[last_index];
                    }
                }

                // Check first letter of Reserved Words--------------------------------------------


        }
    }

}

void outputCleanTxt()
{
    FILE* fptr = fopen("input.txt", "r");
    FILE* outPtr = fopen("cleaninput.txt", "w");

    char first, second;

    // error checking
    if (fptr == NULL || outPtr == NULL)
    {
        printf("Error reading input.txt or writing cleaninput.txt, shutting program down.\n");
        exit(1);
    }

    int commentFlag = 0;

    while (!feof(fptr)) // tests end of file
    {
        first = (char)fgetc(fptr);

        // if we encounter a forward slash, look ahead one character for comment indicator
        if (first == '/' && commentFlag == 0)
        {
            second = (char)fgetc(fptr);

            // if the second character is a star, we have a comment declaration.
            if (second == '*')
                commentFlag = 1;

            // else, append one or two characters to the char array
            else if (!feof(fptr))
            {
                vectorAppend(&codeAry, first);
                vectorAppend(&codeAry, second);
            }
            else
            {
                vectorAppend(&codeAry, first);
            }
        }

        else if (first == '*' && commentFlag == 1)
        {
            second = (char)fgetc(fptr);

            if (second == '/')
                commentFlag = 0;

        }

        else if (commentFlag == 0 && !feof(fptr))
        {
            vectorAppend(&codeAry, first);

        }

    }

    fprintf(outPtr, "%s", codeAry.data); // put our code data into the cleaninput text file

    fclose(fptr);
    fclose(outPtr);


}
