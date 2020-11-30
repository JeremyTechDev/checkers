#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "board.h"
#include "structs.h"
#include "piece.h"
#include "move.h"

void runRound(Team, PieceList *);
void printTurnInfo(Team, PieceList *, MoveList *);
void handleMultipleKill(PieceList *, Piece);
void handleKill(PieceList **, Move *, Piece);
Piece getPieceToMove(PieceList *, Team);
MoveList *getTurnMoves(PieceList *, Piece *, Team);

int main(int argc, char const *argv[])
{
    PieceList *pieceList = initializePieces();

    while (1)
    {
        runRound(black, pieceList);
        runRound(white, pieceList);
    }

    return 0;
}

void runRound(Team team, PieceList *pieceList)
{
    printTurnInfo(team, pieceList, NULL);
    printColorText("Choose the piece you want to move: ", BLUE);

    Piece pieceToMove;
    MoveList *possibleMoves = getTurnMoves(pieceList, &pieceToMove, team);

    while (1)
    {
        Coord moveChoice = getCoordsFromUser();
        Move *isMoveValid = isMoveInList(possibleMoves, moveChoice.x, moveChoice.y);
        if (isMoveValid)
        {
            Piece newPiece = {pieceToMove.id, {moveChoice.x, moveChoice.y}, team, 1};
            modifyPiece(&pieceList, pieceToMove.id, newPiece);

            handleKill(&pieceList, isMoveValid, newPiece);
            break;
        }
        else
            printColorText("That's not a possible move, try another one", RED);
    }
    clear(0);
}

void handleKill(PieceList **pieceList, Move *move, Piece killingPiece)
{
    if (move->killedPieceId != -1)
    {
        Piece killedPiece = {move->killedPieceId, {0, 0, regular}, white, 0};
        modifyPiece(pieceList, move->killedPieceId, killedPiece);
        handleMultipleKill(*pieceList, killingPiece);
    }
}

void handleMultipleKill(PieceList *pieceList, Piece killingPiece)
{
    MoveList *hasMoreKillingMoves = pieceHasKillingMoves(pieceList, killingPiece);
    while (hasMoreKillingMoves)
    {
        MoveList *possibleMoves = getTurnMoves(pieceList, &killingPiece, killingPiece.team);
        Coord moveChoice = getCoordsFromUser();
        Move *isMoveValid = isMoveInList(possibleMoves, moveChoice.x, moveChoice.y);
        if (isMoveValid)
        {
            killingPiece = {killingPiece.id, {moveChoice.x, moveChoice.y}, killingPiece.team, 1};
            modifyPiece(&pieceList, killingPiece.id, killingPiece);

            Piece killedPiece = {isMoveValid->killedPieceId, {0, 0, regular}, white, 0};
            modifyPiece(&pieceList, isMoveValid->killedPieceId, killedPiece);

            hasMoreKillingMoves = pieceHasKillingMoves(pieceList, killingPiece);
        }
        else
            printColorText("That's not a possible move, try another one", RED);
    }
}

MoveList *getTurnMoves(PieceList *pieceList, Piece *pieceToMove, Team team)
{
    int i = 0;
    MoveList *possibleMoves = checkIfKillingMoves(pieceList, team);
    if (possibleMoves)
        *pieceToMove = *possibleMoves->move.killingPiece;
    else
    {
        while (!possibleMoves) // loop until a possible move is found
        {
            if (i++)
                printColorText("No possible moves for that piece, try another one: \a", RED);
            *pieceToMove = getPieceToMove(pieceList, team);
            possibleMoves = getPossibleMoves(pieceList, *pieceToMove);
        }
    }
    MoveList *toHighlight = possibleMoves; // insert the selected piece to highlight
    insertMove(&toHighlight, {pieceToMove->coord, pieceToMove, -1}, NULL);

    system("clear");
    printTurnInfo(team, pieceList, toHighlight);
    printColorText("These are your possible moves:\n", BLUE);
    printTextCoord(possibleMoves);
    printf(">>> ");

    return possibleMoves;
}

Piece getPieceToMove(PieceList *pieceList, Team team)
{
    Coord pieceChoice;
    Piece *pieceToMove;

    while (1)
    {
        pieceChoice = getCoordsFromUser();
        pieceToMove = getPieceAtPosition(pieceList, pieceChoice.x, pieceChoice.y);

        if (pieceToMove)
        {
            if (pieceToMove->team != team)
                printColorText("That's not your piece, try another one: \a", RED);
            else
            {
                Piece pieceToReturn = {pieceToMove->id, pieceToMove->coord, pieceToMove->team, pieceToMove->isOnGame};
                pieceToReturn.coord.colorCode = orange; // set color code to orange to highlight in the board
                return pieceToReturn;
            }
        }
        else
            printColorText("No piece at that coord, try another one: \a", RED);
    }
}

void printTurnInfo(Team team, PieceList *pieceList, MoveList *toHighlight)
{
    printBoard(pieceList, toHighlight);
    printColorText(team == black ? "Playing: BLACKS\n" : "Playing: WHITES\n", GREEN);
}