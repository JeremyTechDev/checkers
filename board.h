#if !defined(BOARD)
#define BOARD

#include "structs.h"
#include "piece.h"

void printBoard(PieceList *);

void printBoard(PieceList *pieceList)
{
    Piece *currentPiece = (Piece *)malloc(sizeof(Piece));

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            currentPiece = getPieceAtPosition(pieceList, x, y);
            if (currentPiece && currentPiece->isOnGame)
                printf("[%s] ", currentPiece->team == black ? "B" : "W");
            else
                printf("%s ", board[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

#endif // BOARD
