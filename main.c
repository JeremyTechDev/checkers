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

Player getPlayer(Team);
void menu();
void startGame(Player, Player);

int main(int argc, char const *argv[])
{
    while (1)
        menu();
    return 0;
}

// Prints a menu for the user to pick their next action
void menu()
{
    Player whitePlayer, blackPlayer;
    printColorText("Welcome to Checkers!\n\n", BLUE);
    printColorText("Choose one option:\n", ORANGE);
    printColorText(" 1. New match\n", ORANGE);
    printColorText(" 2. See Players Stats\n", ORANGE);
    printColorText(" 3. See Matches Resume\n", ORANGE);
    printColorText(" 4. Exit\n\n", RED);
    printColorText(">>> ", BLUE);

    switch (getInt(1, 4))
    {
    case 1:
        clear(0);
        whitePlayer = getPlayer(white);
        blackPlayer = getPlayer(black);
        clear(0);
        startGame(whitePlayer, blackPlayer);
        clear(1);
        break;
    case 2:
        clear(0);
        printColorText("Here are the top players of checkers!\n\n", BLUE);
        readPlayersFile();
        clear(1);
        break;
    case 3:
        clear(0);
        printColorText("Here is a resume of the last games:\n\n", BLUE);
        readMatchStepsFile();
        clear(1);
        break;
    case 4:
        printColorText("\nThanks for playing checkers!\n", PURPLE);
        exit(0);
    default:
        printColorText("Invalid Input!\n", RED);
    }
}

/**
 * Gets a player's name
 * @param {Team} team - the team of the player
 * @returns {Player} the player
 */
Player getPlayer(Team team)
{
    Player player;
    char name[50];
    setColor(ORANGE);
    printf("Insert the name of the %s player: ", team == black ? "BLACK" : "WHITE");
    fgets(name, sizeof(name), stdin);
    setColor(REGULAR);

    for (int i = 0; name[i] != '\0'; i++) // remove the \n
        if (name[i] == '\n')
            name[i] = 0;

    strcpy(player.name, name);
    player.team = team;
    printf("\n");
    return player;
}

/**
 * Starts a new match
 * @param {Player} whitePlayer
 * @param {Player} blackPlayer
 */
void startGame(Player whitePlayer, Player blackPlayer)
{
    MatchState round;
    PieceList *pieceList = initializePieces();
    Player currPlayer = whitePlayer;
    Team winner;

    while (1)
    {
        round = runRound(currPlayer, currPlayer.team == white ? blackPlayer : whitePlayer, pieceList);
        winner = isGameOver(pieceList);
        if (round != inGame)
            break;
        if (winner != none)
        {
            printColorText("Game Over!\n", GREEN);
            printColorText(winner == black ? "Black wins!\n" : "White wins!\n", GREEN);
            savePlayerToFile(winner == black ? blackPlayer : whitePlayer, win);
            savePlayerToFile(winner == black ? whitePlayer : blackPlayer, lose);
            break;
        }
        clear(0);
        currPlayer = currPlayer.team == black ? whitePlayer : blackPlayer;
    }
}