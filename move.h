#if !defined(MOVE)
#define MOVE

#include "structs.h"
#include "piece.h"
#include "coord.h"

int isMovePossible(PieceList *, Coord);
Move *getKillingMove(PieceList *, Piece, int, int);
Move *isMoveInList(MoveList *, int, int);
MoveList *checkIfKillingMoves(PieceList *, Team);
MoveList *getPossibleMoves(PieceList *, Piece);
MoveList *pieceHasKillingMoves(PieceList *, Piece);
void insertMove(MoveList **, Move, Piece *);

/**
 * Inserts a move into the move list
 * @param {MoveList **} - all the moves
 * @param {Move} move - the move to add
 * @param {Piece *} killingPiece - the piece thats killing or null
 */
void insertMove(MoveList **moveList, Move move, Piece *killingPiece)
{
    MoveList *node = (MoveList *)malloc(sizeof(MoveList));
    if (node == NULL)
    {
        printf("No space for the new move...\n");
        exit(-1);
    }

    node->move = move;
    node->move.killingPiece = killingPiece;
    node->next = NULL;
    node->prev = NULL;

    if (*moveList == NULL) // insert in the first position
    {
        *moveList = node;
        return;
    }

    node->next = *moveList; // inserting at first position (push)
    (*moveList)->prev = node;
    *moveList = node;
}

/**
 * Returns all the posible moves for a piece
 * @param {PieceList *} pieceList - all the pieces
 * @param {Piece} piece - the piece to looks moves from
 * @returns {MoveList *} - the list of possible moves
 */
MoveList *getPossibleMoves(PieceList *pieceList, Piece piece)
{
    MoveList *possibleMoves = NULL;
    Coord posMove3, posMove4;
    Piece *pieceAtPosMove1 = NULL, *pieceAtPosMove2 = NULL, *pieceAtPosMove3 = NULL, *pieceAtPosMove4 = NULL;

    Coord posMove1 = {piece.coord.x + (piece.team == black ? -1 : 1), piece.coord.y + 1, orange};
    Coord posMove2 = {piece.coord.x + (piece.team == black ? -1 : 1), piece.coord.y - 1, orange};
    pieceAtPosMove1 = getPieceAtPosition(pieceList, posMove1.x, posMove1.y);
    pieceAtPosMove2 = getPieceAtPosition(pieceList, posMove2.x, posMove2.y);

    if (piece.isQueen)
    {
        posMove3 = (Coord){piece.coord.x + (piece.team == black ? 1 : -1), piece.coord.y + 1, orange};
        posMove4 = (Coord){piece.coord.x + (piece.team == black ? 1 : -1), piece.coord.y - 1, orange};
        pieceAtPosMove3 = getPieceAtPosition(pieceList, posMove3.x, posMove3.y);
        pieceAtPosMove4 = getPieceAtPosition(pieceList, posMove4.x, posMove4.y);
    }

    Coord posMoves[4] = {posMove1, posMove2, posMove3, posMove4};
    Piece *piecesAtMovingPos[4] = {pieceAtPosMove1, pieceAtPosMove2, pieceAtPosMove3, pieceAtPosMove4};

    for (int i = 0; i < 3; i++)
        if ((*(piecesAtMovingPos + i)) && (*(piecesAtMovingPos + i))->team != piece.team)
        {
            int increment = i % 2 == 0 ? 1 : -1; // switch between 1 and -1
            int xIncrement = i >= 2              // if the moves belong to queen, increment in the opposite way
                                 ? ((*(piecesAtMovingPos + i))->team == black ? -1 : 1)
                                 : ((*(piecesAtMovingPos + i))->team == black ? 1 : -1);
            Move *killingMove = getKillingMove(pieceList, *(*(piecesAtMovingPos + i)), increment, xIncrement);
            if (killingMove)
                insertMove(&possibleMoves, *killingMove, &piece);
        }

    if (!possibleMoves) // If no possible moves at this point, there are no killing moves
        for (int i = 0; i < 3; i++)
        {
            Move move = {(*(posMoves + i)), NULL, -1};
            if (isMovePossible(pieceList, (*(posMoves + i))))
                insertMove(&possibleMoves, move, NULL);
        }
    return possibleMoves;
}

/**
 * Returns the killing move for a piece
 * @param {PieceList *} pieceList - all the pieces
 * @param {Piece} piece - the piece to looks moves from
 * @param {int} increment - increment in y coord
 * @param {int} xIncrement - increment in x coord
 * @returns {Move *} the killing move or null
 */
Move *getKillingMove(PieceList *pieceList, Piece piece, int increment, int xIncrement)
{
    Move *move = (Move *)malloc(sizeof(Move));
    int x = piece.coord.x + xIncrement;
    int y = piece.coord.y + increment;

    if (!getPieceAtPosition(pieceList, x, y))
    {
        move->coord = (Coord){x, y, red};
        move->killedPieceId = piece.id;
        move->killingPiece = NULL;
        if (isMovePossible(pieceList, move->coord))
            return move;
    }
    return NULL;
}

/**
 * Returns a piece to move in case there is a killing move for a team
 * @param {PieceList *} pieceList - all the pieces
 * @param {Team} team - round's team
 * @returns {MoveList *} - the list of possible killing moves
 */
MoveList *checkIfKillingMoves(PieceList *pieceList, Team team)
{
    PieceList *node = pieceList;
    while (node)
    {
        if (node->piece.team == team)
        {
            MoveList *possibleMovesForPiece = getPossibleMoves(pieceList, node->piece);
            if (possibleMovesForPiece)
            {
                MoveList *moveNode = possibleMovesForPiece;
                while (moveNode)
                {
                    if (moveNode->move.killedPieceId != -1)
                    {
                        moveNode->move.killingPiece->coord.colorCode = orange;
                        return moveNode;
                    }
                    moveNode = moveNode->next;
                }
            }
        }
        node = node->next;
    }
    return NULL;
}

/**
 * Returns if a piece has killing moves or not
 * @param {PieceList *} pieceList - all the pieces
 * @param {Piece} piece - the piece to looks moves from
 * @returns {MoveList *} - the list of possible killing moves
 */
MoveList *pieceHasKillingMoves(PieceList *pieceList, Piece piece)
{
    MoveList *possibleMoves = getPossibleMoves(pieceList, piece), *result = NULL;
    while (possibleMoves)
    {
        if (possibleMoves->move.killedPieceId != -1)
            insertMove(&result, possibleMoves->move, &piece);
        possibleMoves = possibleMoves->next;
    }
    return result;
}

/**
 * Returns whether a move is in the given move list
 * @param {MoveList *} moveList - the list of moves
 * @param {int} x - the x coord
 * @param {int} y - the y coord
 * @returns {Move *} the move or null
 */
Move *isMoveInList(MoveList *moveList, int x, int y)
{
    while (moveList)
    {
        if (moveList->move.coord.x == x && moveList->move.coord.y == y)
        {
            Move *move = (Move *)malloc(sizeof(Move));
            move->killingPiece = moveList->move.killingPiece;
            move->coord = moveList->move.coord;
            move->killedPieceId = moveList->move.killedPieceId;
            return move;
        }
        moveList = moveList->next;
    }
    return NULL;
}

/**
 * Returns whether a move is possible or not
 * @param {PieceList *} pieceList - all the pieces
 * @param {Move} move - the move to validate
 * @returns {int} whether a move is possible or not
 */
int isMovePossible(PieceList *pieceList, Coord move)
{
    if (move.x > 7 || move.x < 0)
        return 0;
    if (move.y > 7 || move.y < 0)
        return 0;
    Piece *isTaken = getPieceAtPosition(pieceList, move.x, move.y);
    if (isTaken)
        return 0;
    return 1;
}

#endif // MOVE
