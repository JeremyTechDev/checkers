#if !defined(ROUND)
#define ROUND

MoveList *getRoundMoves(PieceList *, Piece *, Team);
Piece getPieceToMove(PieceList *, Team);
void handleKill(PieceList **, int, Piece);
void handleMultipleKill(PieceList *, Piece);
void runRound(Team, PieceList *);

/**
 * Runs a full round for one team
 * @param {Team} team - team playing in the round
 * @param {PieceList *} pieceList - all pieces in the game
 */
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
                endGame(team);
        }

        Move *isMoveValid = isMoveInList(possibleMoves, (*moveChoice).x, (*moveChoice).y);
        if (isMoveValid)
        {
            Piece newPiece = {pieceToMove.id, {(*moveChoice).x, (*moveChoice).y}, team, isQueen(pieceToMove, (*moveChoice)), 1};
            modifyPiece(&pieceList, pieceToMove.id, newPiece);
            handleKill(&pieceList, isMoveValid->killedPieceId, newPiece);
            break;
        }
        else
            printColorText("That's not a possible move, try another one", RED);
    }
}

/**
 * Kills a piece
 * @param {PieceList **} pieceList - all pieces in game
 * @param {int} killedPieceId - the id of the piece to kill, -1 to kill no piece 
 * @param {Piece} killingPiece - the piece making the kill
 */
void handleKill(PieceList **pieceList, int killedPieceId, Piece killingPiece)
{
    if (killedPieceId != -1)
    {
        Piece killedPiece = {killedPieceId, {0, 0, regular}, none, 0, 0};
        modifyPiece(pieceList, killedPieceId, killedPiece);
        handleMultipleKill(*pieceList, killingPiece);
    }
}

/**
 * Check and runs if its possible to make a multiple kill movement
 * @param {PieceList *} pieceList - all pieces in game
 * @param {Piece} killingPiece - the piece making the kills
 */
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
                endGame(killingPiece.team);
        }

        Move *isMoveValid = isMoveInList(possibleMoves, (*moveChoice).x, (*moveChoice).y);
        if (isMoveValid)
        {
            killingPiece = {killingPiece.id, {(*moveChoice).x, (*moveChoice).y}, killingPiece.team, isQueen(killingPiece, (*moveChoice)), 1};
            modifyPiece(&pieceList, killingPiece.id, killingPiece); // move to killing coord

            Piece killedPiece = {isMoveValid->killedPieceId, {0, 0, regular}, none, 0, 0};
            modifyPiece(&pieceList, isMoveValid->killedPieceId, killedPiece); // remove killed from game

            hasMoreKillingMoves = pieceHasKillingMoves(pieceList, killingPiece); // re-check for more kills
        }
        else
            printColorText("That's not a possible move, try another one", RED);
    }
}

/**
 * Gets all the possible moves for a round
 * @param {PieceList *} pieceList - all pieces in game
 * @param {Piece *} pieceToMove - piece to move
 * @param {Team} team - team's round
 * @returns {MoveList *} all the possible moves or NULL
 */
MoveList *getRoundMoves(PieceList *pieceList, Piece *pieceToMove, Team team)
{
    int i = 0;
    MoveList *possibleMoves = checkIfKillingMoves(pieceList, team);
    if (possibleMoves)
        *pieceToMove = *possibleMoves->move.killingPiece;
    else
        while (!possibleMoves) // loop until a possible move is found
        {
            if (i++)
                printColorText("No possible moves for that piece, try another one: \a", RED);
            *pieceToMove = getPieceToMove(pieceList, team);
            possibleMoves = getPossibleMoves(pieceList, *pieceToMove);
        }

    MoveList *toHighlight = possibleMoves; // insert the selected piece to highlight
    insertMove(&toHighlight, {pieceToMove->coord, pieceToMove, -1}, NULL);

    system("clear");
    printRoundInfo(team, pieceList, toHighlight);
    printTextCoord(possibleMoves);
    return possibleMoves;
}

/**
 * Returns the piece the user wants to move
 * @param {PieceList **} pieceList - all pieces in game
 * @param {Team} team - team's round
 */
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
                endGame(team);
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

#endif // ROUND
