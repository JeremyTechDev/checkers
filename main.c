#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "structs.h"
#include "piece.h"
#include "move.h"

int main(int argc, char const *argv[])
{
    printf("Checkers Game!\n\n");

    PieceList *pieceList = initializePieces();

    printBoard(pieceList);

    Piece *p = getPieceAtPosition(pieceList, 2, 1);

    MoveList move = getPossibleMoves(pieceList, *p);

    for (int i = 0; i < move.moveCount; i++)
    {
        printf(">%d %d\n", move.moves->x, move.moves->y);
        *move.moves = *(move.moves + 1);
    }

    return 0;
}
