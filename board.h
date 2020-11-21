#if !defined(BOARD)
#define BOARD

#include "structs.h"
#include "piece.h"
#include "helpers.h"

void printBoard(PieceList *);
void printSpot(PieceList *, int, int);

void printBoard(PieceList *pieceList)
{
    for (int x = 0; x < 8; x++)
    {
        yellow();
        printf("%d ", x + 1);
        reset();

        for (int y = 0; y < 8; y++)
            printSpot(pieceList, x, y);
        printf("\n");
    }

    printColorText("   A   B   C   D   E   F   G   H\n", YELLOW);
}

/**
 * Prints a custom spot of the board
 */
void printSpot(PieceList *pieceList, int x, int y)
{
    Piece *currentPiece = (Piece *)malloc(sizeof(Piece));

    currentPiece = getPieceAtPosition(pieceList, x, y);
    if (currentPiece && currentPiece->isOnGame)
        printf("[%s] ", currentPiece->team == black ? "B" : "W");
    else
        printf("%s ", board[x][y]);
}

#endif // BOARD