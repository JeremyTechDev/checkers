#if !defined(ROUND)
#define ROUND

#include "file.h"

MoveList *getRoundMoves(PieceList *, Piece *, Player, Player);
Piece getPieceToMove(PieceList *, Team, Player, Player);
MatchState handleKill(PieceList **, int, Piece, Player, Player);
MatchState runRound(Player, Player, PieceList *);

/**
 * Runs a full round for one team
 * @param {Team} team - team playing in the round
 * @param {PieceList *} pieceList - all pieces in the game
 * @returns {MatchState} the current state of the round (win, lose, tie or in game)
 */
MatchState runRound(Player player, Player opponent, PieceList *pieceList)
{
    printRoundInfo(player, pieceList, NULL);
    printColorText("Choose the piece you want to move: ", BLUE);
    Piece pieceToMove;
    MoveList *possibleMoves = getRoundMoves(pieceList, &pieceToMove, player, opponent);

    while (1)
    {
        Coord *moveChoice = NULL;
        while (!moveChoice)
        {
            moveChoice = getCoordsFromUser();
            if (!moveChoice)
            {
                MatchState endTheGame = endGame(player.team, player, opponent);
                if (endTheGame != inGame)
                    return endTheGame;
            }
        }

        Move *isMoveValid = isMoveInList(possibleMoves, (*moveChoice).x, (*moveChoice).y);
        if (isMoveValid)
        {
            Piece newPiece = {pieceToMove.id, {(*moveChoice).x, (*moveChoice).y}, player.team, isQueen(pieceToMove, (*moveChoice)), 1};
            modifyPiece(&pieceList, pieceToMove.id, newPiece);
            saveMatchStepToFile(player, &(pieceToMove).coord, isMoveValid->coord, 0);
            handleKill(&pieceList, isMoveValid->killedPieceId, newPiece, player, opponent);
            break;
        }
        else
            printColorText("That's not a possible move, try another one", RED);
    }
    return inGame;
}

/**
 * Kills a piece and handles if more kills can be done
 * @param {PieceList **} pieceList - all pieces in game
 * @param {int} killedPieceId - the id of the piece to kill, -1 to kill no piece 
 * @param {Piece} killingPiece - the piece making the kill
 * @param {Player} player - the player of the round
 * @returns {MatchState} the current state of the round (win, lose, tie or in game)
 */
MatchState handleKill(PieceList **pieceList, int killedPieceId, Piece killingPiece, Player player, Player opponent)
{
    if (killedPieceId != -1)
    {
        Piece killedPiece = {killedPieceId, {0, 0, regular}, none, 0, 0};
        modifyPiece(pieceList, killedPieceId, killedPiece);
        saveMatchStepToFile(player, NULL, killingPiece.coord, 1);

        // handle multiple kills
        MoveList *hasMoreKillingMoves = pieceHasKillingMoves(*pieceList, killingPiece);
        while (hasMoreKillingMoves)
        {
            MoveList *possibleMoves = getRoundMoves(*pieceList, &killingPiece, player, opponent);
            Coord *moveChoice = NULL;

            while (!moveChoice)
            {
                moveChoice = getCoordsFromUser();
                if (!moveChoice)
                {
                    MatchState endTheGame = endGame(player.team, player, opponent);
                    if (endTheGame != inGame)
                        return endTheGame;
                }
            }

            Move *isMoveValid = isMoveInList(possibleMoves, (*moveChoice).x, (*moveChoice).y);
            if (isMoveValid)
            {
                killingPiece = (Piece){killingPiece.id, (Coord){(*moveChoice).x, (*moveChoice).y, regular}, killingPiece.team, isQueen(killingPiece, (*moveChoice)), 1};
                modifyPiece(pieceList, killingPiece.id, killingPiece); // move to killing coord

                Piece killedPiece = {isMoveValid->killedPieceId, {0, 0, regular}, none, 0, 0};
                modifyPiece(pieceList, isMoveValid->killedPieceId, killedPiece); // remove killed from game
                saveMatchStepToFile(player, NULL, isMoveValid->coord, 1);

                hasMoreKillingMoves = pieceHasKillingMoves(*pieceList, killingPiece); // re-check for more kills
            }
            else
                printColorText("That's not a possible move, try another one", RED);
        }
    }
    return inGame;
}

/**
 * Gets all the possible moves for a round
 * @param {PieceList *} pieceList - all pieces in game
 * @param {Piece *} pieceToMove - piece to move
 * @param {Team} team - team's round
 * @returns {MoveList *} all the possible moves or NULL
 */
MoveList *getRoundMoves(PieceList *pieceList, Piece *pieceToMove, Player player, Player opponent)
{
    int i = 0;
    MoveList *possibleMoves = checkIfKillingMoves(pieceList, player.team);
    if (possibleMoves)
        *pieceToMove = *possibleMoves->move.killingPiece;
    else
        while (!possibleMoves) // loop until a possible move is found
        {
            if (i++)
                printColorText("No possible moves for that piece, try another one: \a", RED);
            *pieceToMove = getPieceToMove(pieceList, player.team, player, opponent);
            possibleMoves = getPossibleMoves(pieceList, *pieceToMove);
        }

    MoveList *toHighlight = possibleMoves; // insert the selected piece to highlight
    insertMove(&toHighlight, (Move){pieceToMove->coord, pieceToMove, -1}, NULL);

    system("clear");
    printRoundInfo(player, pieceList, toHighlight);
    printTextCoord(possibleMoves);
    return possibleMoves;
}

/**
 * Returns the piece the user wants to move
 * @param {PieceList **} pieceList - all pieces in game
 * @param {Team} team - team's round
 * @returns {Piece} the piece to move
 */
Piece getPieceToMove(PieceList *pieceList, Team team, Player player, Player opponent)
{
    Coord *pieceChoice = NULL;
    Piece *pieceToMove = NULL;

    while (1)
    {
        while (!pieceChoice)
            pieceChoice = getCoordsFromUser();
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
        {
            printColorText("No piece at that coord, try another one: \a", RED);
            pieceChoice = NULL;
        }
    }
}

#endif // ROUND
