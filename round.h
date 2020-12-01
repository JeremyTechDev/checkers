#if !defined(ROUND)
#define ROUND

#include "structs.h"

void runRound(Team, PieceList *);
void printRoundInfo(Team, PieceList *, MoveList *);
void handleKill(PieceList **, Move *, Piece);
void handleMultipleKill(PieceList *, Piece);
Piece getPieceToMove(PieceList *, Team);
MoveList *getRoundMoves(PieceList *, Piece *, Team);
int isNewQueen(Piece, Coord);
void giveUp(Team);
Team isGameOver(PieceList *);

void runRound(Team team, PieceList *pieceList)
{
    printRoundInfo(team, pieceList, NULL);
    printColorText("Choose the piece you want to move: ", BLUE);

    Piece pieceToMove;
    MoveList *possibleMoves = getRoundMoves(pieceList, &pieceToMove, team);

    while (1)
    {
        Coord *moveChoice = NULL;

        while (!moveChoice)
        {
            moveChoice = getCoordsFromUser();
            if (!moveChoice)
                giveUp(team);
        }

        Move *isMoveValid = isMoveInList(possibleMoves, (*moveChoice).x, (*moveChoice).y);
        if (isMoveValid)
        {
            int isQueen = isNewQueen(pieceToMove, (*moveChoice));
            Piece newPiece = {pieceToMove.id, {(*moveChoice).x, (*moveChoice).y}, team, isQueen, 1};
            modifyPiece(&pieceList, pieceToMove.id, newPiece);

            handleKill(&pieceList, isMoveValid, newPiece);
            break;
        }
        else
            printColorText("That's not a possible move, try another one", RED);
    }
}

void handleKill(PieceList **pieceList, Move *move, Piece killingPiece)
{
    if (move->killedPieceId != -1)
    {
        Piece killedPiece = {move->killedPieceId, {0, 0, regular}, none, 0, 0};
        modifyPiece(pieceList, move->killedPieceId, killedPiece);
        handleMultipleKill(*pieceList, killingPiece);
    }
}

void handleMultipleKill(PieceList *pieceList, Piece killingPiece)
{
    MoveList *hasMoreKillingMoves = pieceHasKillingMoves(pieceList, killingPiece);
    while (hasMoreKillingMoves)
    {
        MoveList *possibleMoves = getRoundMoves(pieceList, &killingPiece, killingPiece.team);
        Coord *moveChoice = NULL;

        while (!moveChoice)
        {
            moveChoice = getCoordsFromUser();
            if (!moveChoice)
                giveUp(killingPiece.team);
        }

        Move *isMoveValid = isMoveInList(possibleMoves, (*moveChoice).x, (*moveChoice).y);
        if (isMoveValid)
        {
            int isQueen = isNewQueen(killingPiece, (*moveChoice));
            killingPiece = {killingPiece.id, {(*moveChoice).x, (*moveChoice).y}, killingPiece.team, isQueen, 1};
            modifyPiece(&pieceList, killingPiece.id, killingPiece);

            Piece killedPiece = {isMoveValid->killedPieceId, {0, 0, regular}, none, 0, 0};
            modifyPiece(&pieceList, isMoveValid->killedPieceId, killedPiece);

            hasMoreKillingMoves = pieceHasKillingMoves(pieceList, killingPiece);
        }
        else
            printColorText("That's not a possible move, try another one", RED);
    }
}

MoveList *getRoundMoves(PieceList *pieceList, Piece *pieceToMove, Team team)
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
    printRoundInfo(team, pieceList, toHighlight);
    printTextCoord(possibleMoves);

    return possibleMoves;
}

Piece getPieceToMove(PieceList *pieceList, Team team)
{
    Coord *pieceChoice = NULL;
    Piece *pieceToMove = NULL;

    while (1)
    {
        while (!pieceChoice)
        {
            pieceChoice = getCoordsFromUser();
            if (!pieceChoice)
                giveUp(team);
        }

        pieceToMove = getPieceAtPosition(pieceList, (*pieceChoice).x, (*pieceChoice).y);

        if (pieceToMove)
        {
            if (pieceToMove->team != team)
                printColorText("That's not your piece, try another one: \a", RED);
            else
            {
                Piece pieceToReturn = {pieceToMove->id, pieceToMove->coord, pieceToMove->team, pieceToMove->isQueen, pieceToMove->isOnGame};
                pieceToReturn.coord.colorCode = orange; // set color code to orange to highlight in the board
                return pieceToReturn;
            }
        }
        else
            printColorText("No piece at that coord, try another one: \a", RED);
    }
}

/**
 * Returns the winner, if there is one
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

void giveUp(Team givingUp)
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

int isNewQueen(Piece p, Coord m)
{
    return p.isQueen || (p.team == black && m.x == 0) || (p.team == white && m.x == 7) ? 1 : 0;
}

void printRoundInfo(Team team, PieceList *pieceList, MoveList *toHighlight)
{
    printBoard(pieceList, toHighlight);
    printColorText(team == black ? "Playing: BLACKS\n" : "Playing: WHITES\n", GREEN);
    printColorText("Insert Q to give up or ask to draw\n", GREEN);
}

#endif // ROUND
