#if !defined(MOVE)
#define MOVE

#include <string.h>
#include "structs.h"
#include "piece.h"
#include "coord.h"

void insertMove(MoveList **, Move, Piece *);
int isMovePossible(PieceList *, Coord);
MoveList *getPossibleMoves(PieceList *, Piece);
Move *getKillingMove(PieceList *, Piece, int);
MoveList *checkIfKillingMoves(PieceList *, Team);
MoveList *pieceHasKillingMoves(PieceList *, Piece);
Move *isMoveInList(CoordList *, int, int);

/**
 * Inserts a move into the move list
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
    {
        Move *killingMove = getKillingMove(pieceList, *pieceAtPosMove1, 1);
        if (killingMove)
            insertMove(&possibleMoves, *killingMove, &piece);
    }

    if (pieceAtPosMove2 && pieceAtPosMove2->team != piece.team)
    {
        Move *killingMove = getKillingMove(pieceList, *pieceAtPosMove2, -1);
        if (killingMove)
            insertMove(&possibleMoves, *killingMove, &piece);
    }

    // If no possible moves at this point, there are no killing moves
    if (!possibleMoves)
    {
        Move move1 = {posMove1, NULL, -1};
        Move move2 = {posMove2, NULL, -1};
        if (isMovePossible(pieceList, posMove1))
            insertMove(&possibleMoves, move1, NULL);
        if (isMovePossible(pieceList, posMove2))
            insertMove(&possibleMoves, move2, NULL);
    }

    return possibleMoves;
}

Move *getKillingMove(PieceList *pieceList, Piece piece, int increment)
{
    Move *move = (Move *)malloc(sizeof(Move));
    int x = piece.coord.x + (piece.team == black ? 1 : -1);
    int y = piece.coord.y + increment;

    if (!getPieceAtPosition(pieceList, x, y))
    {
        move->coord = {x, y, red};
        move->killedPieceId = piece.id;
        move->killingPiece = NULL;
        if (isMovePossible(pieceList, move->coord))
            return move;
        return NULL;
    }

    return NULL;
}

/**
 * Returns a piece to move in case there is a killing move for a team
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
 */
Move *isMoveInList(MoveList *moveList, int x, int y)
{
    while (moveList)
    {
        if (moveList->move.coord.x == x && moveList->move.coord.y == y)
        {
            Move *move = (Move *)malloc(sizeof(Move));
            move->killingPiece = moveList->move.killingPiece;
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
