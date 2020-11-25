#if !defined(MOVE)
#define MOVE

#include <string.h>
#include "structs.h"
#include "piece.h"
#include "coord.h"

void insertMove(MoveList **, Move);
int isMovePossible(PieceList *, Coord);
MoveList *getPossibleMoves(PieceList *, Piece);
Move getKillingMove(PieceList *, Piece, int);
Move *isMoveInList(CoordList *, int, int);

/**
 * Inserts a move into the move list
 */
void insertMove(MoveList **moveList, Move move)
{
    MoveList *node = (MoveList *)malloc(sizeof(MoveList));

    if (node == NULL)
    {
        printf("No space for the new move...\n");
        exit(-1);
    }

    node->move = move;
    node->next = NULL;
    node->prev = NULL;

    // insert in the first position
    if (*moveList == NULL)
    {
        *moveList = node;
        return;
    }

    // inserting at first position (push)
    node->next = *moveList;
    (*moveList)->prev = node;
    *moveList = node;
}

/**
 * Returns whether a move is possible or not
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

/**
 * Returns all the posible moves for a piece
 */
MoveList *getPossibleMoves(PieceList *pieceList, Piece piece)
{
    MoveList *possibleMoves = NULL;

    Coord posMove1 = {piece.coord.x + (piece.team == black ? -1 : 1), piece.coord.y + 1, orange};
    Coord posMove2 = {piece.coord.x + (piece.team == black ? -1 : 1), piece.coord.y - 1, orange};

    Piece *pieceAtPosMove1 = getPieceAtPosition(pieceList, posMove1.x, posMove1.y);
    Piece *pieceAtPosMove2 = getPieceAtPosition(pieceList, posMove2.x, posMove2.y);

    /**
     * if there is a piece of the opposite team in the spot, check for killing move and append it to the list
     * otherwise, just add the move to the list
     */
    if (pieceAtPosMove1 && pieceAtPosMove1->team != piece.team)
        insertMove(&possibleMoves, getKillingMove(pieceList, *pieceAtPosMove1, 1));

    if (pieceAtPosMove2 && pieceAtPosMove2->team != piece.team)
        insertMove(&possibleMoves, getKillingMove(pieceList, *pieceAtPosMove2, -1));

    // If no possible moves at this point, there are no killing moves
    if (!possibleMoves)
    {
        Move move1 = {posMove1, -1};
        Move move2 = {posMove2, -1};
        if (isMovePossible(pieceList, posMove1))
            insertMove(&possibleMoves, move1);
        if (isMovePossible(pieceList, posMove2))
            insertMove(&possibleMoves, move2);
    }

    return possibleMoves;
}

Move getKillingMove(PieceList *pieceList, Piece piece, int increment)
{
    Move posKillingMove = {{piece.coord.x + (piece.team == black ? 1 : -1), piece.coord.y + increment, red}, piece.id};
    return posKillingMove;
}

/**
 * Returns whether a move is in the given move list
 */
Move *isMoveInList(MoveList *moveList, int x, int y)
{
    while (moveList)
    {
        if (moveList->move.coord.x == x && moveList->move.coord.y == y)
        {
            Move *move = (Move *)malloc(sizeof(Move));
            move->coord.x = moveList->move.coord.x;
            move->coord.y = moveList->move.coord.y;
            move->coord.colorCode = moveList->move.coord.colorCode;
            move->killedPieceId = moveList->move.killedPieceId;
            return move;
        }
        moveList = moveList->next;
    }
    return NULL;
}

#endif // MOVE
