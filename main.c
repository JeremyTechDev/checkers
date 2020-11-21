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
        printBoard(pieceList, NULL);
        printColorText("\nChoose the piece you want to move: ", BLUE);
        Coord pieceChoice = getCoordsFromUser();
        Piece *pieceToMove = getPieceAtPosition(pieceList, pieceChoice.x, pieceChoice.y);

        if (pieceToMove)
        {
            system("clear");
            CoordList *possibleMoves = getPossibleMoves(pieceList, *pieceToMove);
            CoordList *toHighlight = possibleMoves;
            insertCoord(&toHighlight, pieceChoice);

            if (possibleMoves)
            {
                printBoard(pieceList, toHighlight);
                printColorText("\nThese are your posible moves:\n", BLUE);
                while (possibleMoves)
                {
                    printf(">%d %d\n", possibleMoves->coord.x, possibleMoves->coord.y);
                    possibleMoves = possibleMoves->next;
                }
            }
        }
        else
            printColorText("\nThere's no piece at that position, try another one\a\n\n", RED);

        fflush(stdin); // option ONE to clean stdin
        getchar();     // wait for ENTER
        system("clear");
    }

    return 0;
}
