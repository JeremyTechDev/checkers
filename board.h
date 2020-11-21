#if !defined(BOARD)
#define BOARD

#include "structs.h"
#include "piece.h"
#include "helpers.h"
#include "move.h"

void printBoard(PieceList *, CoordList *);
void printSpot(PieceList *, int, int, int);

void printBoard(PieceList *pieceList, CoordList *coordsToHighlight)
{
    for (int x = 0; x < 8; x++)
    {
        yellow();
        printf("%d ", x + 1);
        reset();

        for (int y = 0; y < 8; y++)
        {
            if (coordsToHighlight)
                printSpot(pieceList, x, y, isCoordInList(coordsToHighlight, x, y));
            else
                printSpot(pieceList, x, y, 0);
        }
        printf("\n");
    }

    printColorText("   A   B   C   D   E   F   G   H\n", YELLOW);
}

/**
 * Prints a custom spot of the board
 */
void printSpot(PieceList *pieceList, int x, int y, int highlight)
{
    Piece *currentPiece = (Piece *)malloc(sizeof(Piece));

    if (highlight)
        orange();

    currentPiece = getPieceAtPosition(pieceList, x, y);
    if (currentPiece && currentPiece->isOnGame)
        printf("[%s] ", currentPiece->team == black ? "B" : "W");
    else
        printf("%s ", board[x][y]);

    reset();
}

#endif // BOARD