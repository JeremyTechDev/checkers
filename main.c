#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "board.h"
#include "structs.h"
#include "piece.h"
#include "move.h"
#include "round.h"
#include "file.h"

int main(int argc, char const *argv[])
{
    Player player1 = {"Jeremy", black}, player2 = {"Messi", white}, currPlayer = player1;
    PieceList *pieceList = initializePieces();
    Team winner;

    readPlayersFile();
    readMatchStepsFile();
    clear(1);

    while (1)
    {
        runRound(currPlayer, currPlayer.team == player1.team ? player2 : player1, pieceList);
        winner = isGameOver(pieceList);
        if (winner != none)
        {
            printColorText("Game Over!\n", GREEN);
            printColorText(winner == black ? "Black wins!\n" : "White wins!\n", GREEN);
            savePlayerToFile(winner == black ? player1 : player2, win);
            savePlayerToFile(winner == black ? player2 : player1, lose);
            exit(0);
        }
        clear(0);
        currPlayer = currPlayer.team == black ? player2 : player1;
    }

    return 0;
}
