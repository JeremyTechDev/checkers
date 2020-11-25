#if !defined(BOARD)
#define BOARD

#include "structs.h"
#include "piece.h"
#include "helpers.h"
#include "move.h"
#include "coord.h"

void printBoard(PieceList *, MoveList *);
void printSpot(PieceList *, int, int, int);
void setHighlightColor(int);

void printBoard(PieceList *pieceList, MoveList *coordsToHighlight)
{
    printColorText("Checkers Game!\n\n", YELLOW);
    for (int x = 0; x < 8; x++)
    {
        setColor(YELLOW);
        printf("%d ", x + 1);
        setColor(REGULAR);

        for (int y = 0; y < 8; y++)
        {
            if (coordsToHighlight)
            {
                Move *hasHighligh = isMoveInList(coordsToHighlight, x, y);
                if (hasHighligh)
                    printSpot(pieceList, x, y, hasHighligh->coord.colorCode);
                else
                    printSpot(pieceList, x, y, 0);
            }
            else
                printSpot(pieceList, x, y, 0);
        }
        printf("\n");
    }

    printColorText("   A   B   C   D   E   F   G   H\n\n", YELLOW);
}

/**
 * Prints a custom spot of the board
 */
void printSpot(PieceList *pieceList, int x, int y, int colorCode)
{
    Coord coord = {x, y, colorCode};
    Piece *currentPiece = (Piece *)malloc(sizeof(Piece));

    setHighlightColor(colorCode);

    currentPiece = getPieceAtPosition(pieceList, x, y);
    if (currentPiece && currentPiece->isOnGame != 0)
        printf("[%s] ", currentPiece->team == black ? "B" : "W");
    else
        colorCode ? printf(">%s ", convertCoordToText(coord)) : printf("%s ", board[x][y]);

    setColor(REGULAR);
}

void setHighlightColor(int colorCode)
{
    if (colorCode == 0)
        setColor(REGULAR);
    else if (colorCode == 1)
        setColor(YELLOW);
    else if (colorCode == 2)
        setColor(BLUE);
    else if (colorCode == 3)
        setColor(RED);
    else if (colorCode == 4)
        setColor(ORANGE);
}

#endif // BOARD