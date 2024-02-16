#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "core.h"

// Clear the standard input buffer
void clearInputBuffer(void)
{
    // Discard all remaining char's from the standard input buffer:
    while (getchar() != '\n')
    {
        ; // do nothing!
    }
}

// Wait for user to input the "enter" key to continue
void suspend(void)
{
    printf("<ENTER> to continue...");
    clearInputBuffer();
    putchar('\n');
}

int inputInt(void)
{
    int tInt = 0, i = 0;
    char tChar = 0;

    i = 1;
    while (i)
    {
        scanf("%d%c", &tInt, &tChar);
        if (tChar != '\n')
        {
            printf("Error! Input a whole number: ");
            clearInputBuffer();
        }
        else
        {
            i = 0;
        }
    }

    return tInt;
}

int inputIntPositive(void)
{
    int tInt = 0, i = 0;

    i = 1;
    while (i)
    {
        tInt = inputInt();
        if (tInt < 0 || tInt == 0)
        {
            printf("ERROR! Value must be > 0: ");
            clearInputBuffer();
        }
        else
        {
            i = 0;
        }
    }

    return tInt;
}

int inputIntRange(lBound, uBound)
{
    int tInt = 0, i = 0;
    char tChar = 0;

    i = 1;
    while (i)
    {
        tInt = 0, tChar = 0;
        scanf("%d%c", &tInt, &tChar);

        if (tChar != '\n')
        {
            printf("Error! Input a whole number: ");
            clearInputBuffer();
        }
        else  if (tInt < lBound || tInt > uBound)
        {
            printf("ERROR! Value must be between %d and %d inclusive: ", lBound, uBound);
        }
        else
        {
            i = 0;
        }
    }

    return tInt;
}

char inputCharOption(const char charString[])
{
    int n = 0, max = 2, i = 0;
    char tChar;
    char check;

    i = 1;
    while (i)
    {

        scanf("%c%c", &tChar, &check);


        for (n = 0; n < max; n++)
        {
            if (tChar == charString[n] && check == '\n')
            {
                i = 0;
            }
        }

        if (i == 1)
        {
            printf("ERROR: Character must be one of [%s]: ", charString);
            clearInputBuffer();
        }
    }
    return tChar;
}

void inputCString(char* string, int min, int max)
{
    int i = 1, len = 0, checker = 0;
    char tChar[40] = { '\0' };

    while (i)
    {
        scanf("%40[^\n]", tChar);
        clearInputBuffer();

        len = strlen(tChar);

        if (tChar[0] >= 48 && tChar[0] <= 57)
        {
            checker = 1;
        }

        if (min == max && len > max)
        {
            if (checker)
            {
                printf("Invalid 10-digit number! Number: ");
            }
            else
            {
                printf("ERROR: String length must be more than %d chars: ", min);
            }
        }
        else if (len < min)
        {
            if (checker)
            {
                printf("Invalid 10-digit number! Number: ");
            }
            else
            {
                printf("ERROR: String length must be between %d and %d: ", min, max);
            }
        }
        else if (len > max)
        {

            if (checker)
            {
                printf("Invalid 10-digit number! Number: ");
            }
            else
            {
                printf("ERROR: String length must be no more than %d chars: ", max);
            }
        }
        else
        {
            i = 0;
        }
    }
    strcpy(string, tChar);
}

void displayFormattedPhone(const char* pNumber)
{
    int pLength = 10, n, i;

    i = 1, n = 0;
    while (i)
    {
        if (pNumber == NULL)
        {
            i = 0;
        }
        else
        {
            if (pNumber[n] != '\0' && pNumber[n] >= 48 && pNumber[n] <= 57)
            {
                n++;
            }
            else
            {
                i = 0;
            }
        }
    }

    if (n != pLength)
    {
        printf("(___)___-____");
    }
    else
    {
        printf("(");
        for (n = 0; n < 10; n++)
        {
            printf("%c", pNumber[n]);

            if (n == 2)
            {
                printf(")");
            }
            else if (n == 5)
            {
                printf("-");
            }
        }
    }
}

