#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "board.h"
#include "structs.h"
#include "piece.h"
#include "move.h"
#include "round.h"

int main(int argc, char const *argv[])
{
    Team currentRoundTeam = black, winner;
    PieceList *pieceList = initializePieces();

    while (1)
    {
        runRound(currentRoundTeam, pieceList);
        winner = isGameOver(pieceList);
        if (winner != none)
        {
            printf("Game over!\n%s", winner == black ? "Black wins!\n" : "White wins!\n");
            exit(0);
        }
        clear(0);
        currentRoundTeam = currentRoundTeam == black ? white : black;
    }

    return 0;
}
