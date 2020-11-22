#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "board.h"
#include "structs.h"
#include "piece.h"
#include "move.h"

void runRound(Team, PieceList *);
void printTurnInfo(Team, PieceList *, CoordList *);

int main(int argc, char const *argv[])
{
    PieceList *pieceList = initializePieces();

    while (1)
    {
        runRound(white, pieceList);
        runRound(black, pieceList);
    }

    return 0;
}

void runRound(Team team, PieceList *pieceList)
{
    printTurnInfo(team, pieceList, NULL);
    printColorText("Choose the piece you want to move: ", BLUE);

    while (1) // to get piece and moving spot
    {
        Coord pieceChoice = getCoordsFromUser();
        Piece *pieceToMove = getPieceAtPosition(pieceList, pieceChoice.x, pieceChoice.y);

        if (pieceToMove)
        {
            if (pieceToMove->team != team)
            {
                printColorText("That's not your piece, try another one: \a", RED);
                continue;
            }

            CoordList *possibleMoves = getPossibleMoves(pieceList, *pieceToMove);
            CoordList *toHighlight = possibleMoves;
            insertCoord(&toHighlight, pieceChoice); // insert the selected piece to highlight

            if (possibleMoves)
            {
                system("clear");
                printTurnInfo(team, pieceList, toHighlight);
                printColorText("These are your possible moves:\n", BLUE);
                printTextCoord(possibleMoves);
                printf(">>> ");

                while (1) // to move
                {
                    Coord moveChoice = getCoordsFromUser();
                    if (isCoordInList(possibleMoves, moveChoice.x, moveChoice.y))
                    {
                        Piece newPiece = {pieceToMove->id, moveChoice.x, moveChoice.y, team, 1};
                        modifyPiece(&pieceList, pieceToMove->id, newPiece);
                        break;
                    }
                    else
                        printColorText("That's not a possible move, try another one: \a", RED);
                }
                clear(0);
                break;
            }
            else
                printColorText("No possible moves for that piece, try another one: \a", RED);
        }
        else
            printColorText("No piece at that coord, try another one: \a", RED);
    }
}

void printTurnInfo(Team team, PieceList *pieceList, CoordList *toHighlight)
{
    printBoard(pieceList, toHighlight);
    printColorText(team == black ? "Playing: BLACKS\n" : "Playing: WHITES\n", YELLOW);
}