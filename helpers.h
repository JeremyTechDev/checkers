#if !defined(HELPERS)
#define HELPERS

#include "file.h"

const char REGULAR[15] = "[0m";       // code 0
const char GREEN[15] = "[1m\033[32m"; // code 1
const char BLUE[15] = "[1m\033[36m";  // code 2
const char RED[15] = "[1m\033[31m";   // code 3
const char ORANGE[15] = "[01;33m";    // code 4
const char PURPLE[15] = "[1;35m";     // code 5

Team isGameOver(PieceList *);
void clear(int);
void endGame(Team, Player, Player);
void printColorText(const char *, const char *);
void setColor(const char *);
void setHighlightColor(Color);

/**
 * Print a text with a given color
 * @param {const char *} text - text to print
 * @param {const char *} color - color of the text
 */
void printColorText(const char *text, const char *color)
{
    printf("\033%s%s\033%s", color, text, REGULAR);
}

/**
 * Sets the highlight color given a code
 * @param {Color} colorCode - code of the color
 */
void setHighlightColor(Color colorCode)
{
    if (colorCode == regular)
        setColor(REGULAR);
    else if (colorCode == green)
        setColor(GREEN);
    else if (colorCode == blue)
        setColor(BLUE);
    else if (colorCode == red)
        setColor(RED);
    else if (colorCode == orange)
        setColor(ORANGE);
    else if (colorCode == purple)
        setColor(PURPLE);
}

/**
 * Sets the terminal to print a given color
 * @param {const char *} color - color to set the terminal
 */
void setColor(const char *color)
{
    printf("\033%s", color);
}

/**
 * Clear the screen and waits for enter
 * @param {int} wait - whether to wait for enter or not
 */
void clear(int wait)
{
    if (wait)
    {
        printColorText("\nPress ENTER to continue\n", BLUE);
        fflush(stdin);
        getchar();
    }
    system("clear");
}

/**
 * Check whether the user want to give up or ask to draw
 * @param {Team} givingUp - the team that wants to end the game
 */
void endGame(Team givingUp, Player pl1, Player pl2)
{
    char giveUp, option;
    printColorText("Ask to draw (D) or give up (G)?\n", RED);

    while (1)
    {
        printColorText(">>> ", RED);
        scanf(" %c", &option);
        getchar();

        if (tolower(option) == 'd')
        {
            printColorText("Asking to draw! Insert Y if both players agree to draw, anything else to cancel: \n", RED);
            while (1)
            {
                printColorText(">>> ", RED);
                scanf(" %c", &giveUp);
                getchar();

                if (tolower(giveUp) == 'y')
                {
                    printf("Match end with a draw!\n");
                    savePlayerToFile(pl1, tie);
                    savePlayerToFile(pl2, tie);
                    exit(0);
                }
                break;
            }
            printColorText("Canceled! Continue playing...\nChoose the piece to move\n", GREEN);
            break;
        }
        if (tolower(option) == 'g')
        {
            printColorText("\nAre you sure you want to give up? [y/n]: \n", RED);
            while (1)
            {
                printColorText(">>> ", RED);
                scanf(" %c", &giveUp);
                getchar();

                if (tolower(giveUp) == 'y')
                {
                    printf(givingUp == black ? "Black gives up!\n" : "White gives up!\n");
                    printf(givingUp == black ? "White wins!\n" : "Black wins!\n");
                    savePlayerToFile(pl1.team == givingUp ? pl1 : pl2, lose);
                    savePlayerToFile(pl2.team == givingUp ? pl1 : pl2, win);
                    exit(0);
                }
                else if (tolower(giveUp) == 'n')
                    break;
            }
            printColorText("Canceled! Continue playing...\nChoose the piece/coord to move\n", GREEN);
            break;
        }
    }
}

/**
 * Returns the winner, if there is one
 * @param {PieceList *} pieceList - all pieces in game
 * @returns {Team} the team that won or none
 */
Team isGameOver(PieceList *pieceList)
{
    int whitePieces = 0, blackPieces = 0;
    while (pieceList)
    {
        if (pieceList->piece.team == white)
            whitePieces++;
        if (pieceList->piece.team == black)
            blackPieces++;
        pieceList = pieceList->next;
    }
    if (whitePieces != 0 && blackPieces != 0)
        return none;
    return whitePieces == 0 ? black : white;
}

#endif // HELPERS
