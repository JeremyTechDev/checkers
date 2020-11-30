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
    Team currentRoundTeam = black;
    PieceList *pieceList = initializePieces();

    while (1)
    {
        runRound(currentRoundTeam, pieceList);
        if (isGameOver(pieceList) != none)
        {
            printColorText("Game over!\n", RED);
            exit(0);
        }
        currentRoundTeam = currentRoundTeam == black ? white : black;
    }

    return 0;
}
