#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "board.h"
#include "structs.h"
#include "piece.h"
#include "move.h"

int main(int argc, char const *argv[])
{
    printf("Checkers Game!\n\n");

    PieceList *pieceList = initializePieces();

    while (1)
    {
        printBoard(pieceList);

        printColorText("\nChoose the piece you want to move: ", BLUE);
        Move pieceChoice = getCordsFromUser();
        Piece *pieceToMove = getPieceAtPosition(pieceList, pieceChoice.x, pieceChoice.y);

        if (pieceToMove)
        {
            MoveList possibleMoves = getPossibleMoves(pieceList, *pieceToMove);

            if (possibleMoves.moveCount)
            {
                printColorText("\nThese are your posible moves:\n", BLUE);
                for (int i = 0; i < possibleMoves.moveCount; i++)
                {
                    printf(">%d %d\n", possibleMoves.moves->x, possibleMoves.moves->y);
                    *possibleMoves.moves = *(possibleMoves.moves + 1);
                }
            }
        }
        else
            printColorText("\nThere's no piece at that position, try another one\a\n\n", RED);
    }

    return 0;
}
