#if !defined(COORDS)
#define COORDS
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "helpers.h"

void insertCoord(CoordList **, Coord);
int isCoordInList(CoordList, int, int);

/**
 * Gets a valid [x,y] cord from the user 
 */
Coord getCoordsFromUser()
{
    int xCord, yCord;
    char input[10];

    while (1)
    {
        fgets(input, sizeof(input), stdin);

        char letter = toupper(input[0]);
        int number = input[1] - '0';

        xCord = number - 1;
        yCord = (int)(strchr(validLetters, letter) - validLetters);

        // validate move
        if ((xCord >= 0 && xCord <= 7) && (yCord >= 0 && yCord <= 7))
            break;
        printColorText("\nInvalid coord, try another one: ", RED);
    }

    Coord coord = {xCord, yCord};
    return coord;
}

/**
 * Inserts a coord into the coords list
 */
void insertCoord(CoordList **coordsList, Coord coord)
{
    CoordList *node = (CoordList *)malloc(sizeof(CoordList));

    if (node == NULL)
    {
        printf("No space for the new coord\n");
        exit(-1);
    }

    node->coord = coord;
    node->next = NULL;
    node->prev = NULL;

    // insert in the first position
    if (*coordsList == NULL)
    {
        *coordsList = node;
        return;
    }

    // inserting at first position (push)
    node->next = *coordsList;
    (*coordsList)->prev = node;
    *coordsList = node;
}

/**
 * Returns whether a coord is in the given coord list
 */
int isCoordInList(CoordList *coordList, int x, int y)
{
    while (coordList)
    {
        if (coordList->coord.x == x && coordList->coord.y == y)
            return 1;
        coordList = coordList->next;
    }

    return 0;
}

#endif // COORDS
