#if !defined(COORDS)
#define COORDS

Coord *getCoordsFromUser();
void insertCoord(CoordList **, Coord);
char *convertCoordToText(Coord);
void printTextCoord(MoveList *);
int isMovePossible(PieceList *, Coord);

/**
 * Gets a valid [x,y] cord from the user or NULL if the user wants to give up
 * @returns {Coord *} NULL or valid coord from the user
 */
Coord *getCoordsFromUser()
{
    int xCord, yCord;
    char input[10];

    while (1)
    {
        fflush(stdin);
        fgets(input, sizeof(input), stdin);

        char letter = toupper(*input);
        int number = *(input + 1) - '0';

        if (letter == 'Q') // player gives up
            return NULL;

        xCord = number - 1;
        yCord = (int)(strchr(validLetters, letter) - validLetters);

        // validate move
        if ((xCord >= 0 && xCord <= 7) && (yCord >= 0 && yCord <= 7))
            break;
        printColorText("Invalid coord, try another one: ", RED);
    }

    Coord *coord = (Coord *)malloc(sizeof(Coord));
    coord->x = xCord;
    coord->y = yCord;
    coord->colorCode = orange;
    return coord;
}

/**
 * Inserts a coord into the coords list
 * @param {CoordList **} coordsList - double linked list with all the coords so far
 * @param {Coord} coord - the coord to insert
 */
void insertCoord(CoordList **coordsList, Coord coord)
{
    CoordList *node = (CoordList *)malloc(sizeof(CoordList));

    if (!node)
    {
        printColorText("No space for the new coord...\n", RED);
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
 * Converts a [x,y] coord to @# text
 * @param {Coord} coord - the coord to convert to text
 * @returns {char *} the text value of coord
 */
char *convertCoordToText(Coord coord)
{
    char *result = (char *)malloc(sizeof(char) * 3);
    *(result) = (char)validLetters[coord.y];
    *(result + 1) = (char)((coord.x + 1) + '0');
    return result;
}

/**
 * Prints a list of text coords
 * @param {MoveList *} moveList - all the moves to print
 */
void printTextCoord(MoveList *moveList)
{
    setColor(BLUE);
    printf("These are your possible moves:\n");
    while (moveList)
    {
        printf("> %s\n", convertCoordToText(moveList->move.coord));
        moveList = moveList->next;
    }
    printf(">>> ");
    setColor(REGULAR);
}

#endif // COORDS
