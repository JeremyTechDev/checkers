#if !defined(BOARD)
#define BOARD

#include "structs.h"
#include "piece.h"
#include "helpers.h"
#include "move.h"
#include "coord.h"

void printBoard(PieceList *, MoveList *);
void printSpot(PieceList *, int, int, Color);
void setHighlightColor(Color);

void printBoard(PieceList *pieceList, MoveList *coordsToHighlight)
{
    printColorText("Checkers Game!\n\n", GREEN);
    for (int x = 0; x < 8; x++)
    {
        setColor(GREEN);
        printf("%d ", x + 1);
        setColor(REGULAR);

        for (int y = 0; y < 8; y++)
        {
            if (coordsToHighlight)
            {
                Move *hasHighlight = isMoveInList(coordsToHighlight, x, y);
                if (hasHighlight)
                    printSpot(pieceList, x, y, hasHighlight->coord.colorCode);
                else
                    printSpot(pieceList, x, y, regular);
            }
            else
                printSpot(pieceList, x, y, regular);
        }
        printf("\n");
    }

    printColorText("   A   B   C   D   E   F   G   H\n\n", GREEN);
}

/**
 * Prints a custom spot of the board
 */
void printSpot(PieceList *pieceList, int x, int y, Color colorCode)
{
    Coord coord = {x, y, colorCode};
    Piece *currentPiece = (Piece *)malloc(sizeof(Piece));

    setHighlightColor(colorCode);

    currentPiece = getPieceAtPosition(pieceList, x, y);
    if (currentPiece && currentPiece->isOnGame != 0)
    {
        if (currentPiece->isQueen)
            setColor(PURPLE);
        printf("[%s] ", currentPiece->team == black ? "B" : "W");
    }
    else
        colorCode ? printf(">%s ", convertCoordToText(coord)) : printf("%s ", board[x][y]);

    setColor(REGULAR);
}

void setHighlightColor(Color colorCode)
{
    if (colorCode == regular)
        setColor(REGULAR);
    else if (colorCode == green)
        setColor(GREEN);
    else if (colorCode == blue)
        setColor(BLUE);
    else if (colorCode == red)
        setColor(RED);
    else if (colorCode == orange)
        setColor(ORANGE);
    else if (colorCode == purple)
        setColor(PURPLE);
}

#endif // BOARD