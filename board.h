#if !defined(BOARD)
#define BOARD

#include "structs.h"
#include "piece.h"
#include "helpers.h"
#include "move.h"
#include "coord.h"

void printBoard(PieceList *, MoveList *);
void printSpot(PieceList *, Coord);
void printRoundInfo(Player, PieceList *, MoveList *);

/**
 * Prints a colored board with all the active pieces and the current state of the game
 * @param {PieceList*} pieceList - all pieces of the game
 * @param {MoveList*} coordsToHighlight - all coords to highlight with color
 */
void printBoard(PieceList *pieceList, MoveList *coordsToHighlight)
{
    printColorText("Checkers Game!\n\n", GREEN);
    for (int x = 0; x < 8; x++)
    {
        setColor(GREEN);
        printf("%d ", x + 1);
        setColor(REGULAR);

        for (int y = 0; y < 8; y++)
        {
            Move *hasHighlight = isMoveInList(coordsToHighlight, x, y);
            if (coordsToHighlight && hasHighlight)
                printSpot(pieceList, {x, y, hasHighlight->coord.colorCode});
            else
                printSpot(pieceList, {x, y, regular});
        }
        printf("\n");
    }
    printColorText("   A   B   C   D   E   F   G   H\n\n", GREEN);
}

/**
 * Prints a custom spot of the board
 * @param {PieceList*} pieceList - all pieces of the game
 * @param {int} x - x value of coord
 * @param {int} y - y value of coord
 */
void printSpot(PieceList *pieceList, Coord coord)
{
    Piece *currentPiece = getPieceAtPosition(pieceList, coord.x, coord.y);
    setHighlightColor(currentPiece && currentPiece->isQueen ? purple : coord.colorCode);
    if (currentPiece && currentPiece->isOnGame != 0)
        printf("[%s] ", currentPiece->team == black ? "B" : "W");
    else
        coord.colorCode ? printf(">%s ", convertCoordToText(coord)) : printf("%s ", board[coord.x][coord.y]);
    setHighlightColor(regular);
}

/**
 * Print round info
 * @param {Team} team - team's round
 * @param {PieceList *} pieceList - all pieces in game
 * @param {MoveList *} toHighlight - coords to highlight
 */
void printRoundInfo(Player player, PieceList *pieceList, MoveList *toHighlight)
{
    printBoard(pieceList, toHighlight);
    setColor(GREEN);
    printf(player.team == black ? "Playing: BLACKS - %s\n" : "Playing: WHITES - %s\n", player.name);
    printf("Insert Q to give up or ask to draw\n");
    setColor(REGULAR);
}
#endif // BOARD