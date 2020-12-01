#if !defined(MOVE)
#define MOVE

#include <string.h>
#include "structs.h"
#include "piece.h"
#include "coord.h"

void insertMove(MoveList **, Move, Piece *);
int isMovePossible(PieceList *, Coord);
MoveList *getPossibleMoves(PieceList *, Piece);
Move *getKillingMove(PieceList *, Piece, int, int);
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
    Coord posMove1, posMove2, posMove3, posMove4;
    Piece *pieceAtPosMove1 = NULL, *pieceAtPosMove2 = NULL, *pieceAtPosMove3 = NULL, *pieceAtPosMove4 = NULL;

    posMove1 = {piece.coord.x + (piece.team == black ? -1 : 1), piece.coord.y + 1, orange};
    posMove2 = {piece.coord.x + (piece.team == black ? -1 : 1), piece.coord.y - 1, orange};
    pieceAtPosMove1 = getPieceAtPosition(pieceList, posMove1.x, posMove1.y);
    pieceAtPosMove2 = getPieceAtPosition(pieceList, posMove2.x, posMove2.y);

    if (piece.isQueen)
    {
        posMove3 = {piece.coord.x + (piece.team == black ? 1 : -1), piece.coord.y + 1, orange};
        posMove4 = {piece.coord.x + (piece.team == black ? 1 : -1), piece.coord.y - 1, orange};
        pieceAtPosMove3 = getPieceAtPosition(pieceList, posMove3.x, posMove3.y);
        pieceAtPosMove4 = getPieceAtPosition(pieceList, posMove4.x, posMove4.y);
    }

    Coord posMoves[4] = {posMove1, posMove2, posMove3, posMove4};
    Piece *piecesAtMovingPos[4] = {pieceAtPosMove1, pieceAtPosMove2, pieceAtPosMove3, pieceAtPosMove4};

    for (int i = 0; i < 3; i++)
    {
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
    }

    // If no possible moves at this point, there are no killing moves
    if (!possibleMoves)
    {
        for (int i = 0; i < 3; i++)
        {
            Move move = {(*(posMoves + i)), NULL, -1};
            if (isMovePossible(pieceList, (*(posMoves + i))))
                insertMove(&possibleMoves, move, NULL);
        }
    }

    return possibleMoves;
}

Move *getKillingMove(PieceList *pieceList, Piece piece, int increment, int xIncrement)
{
    Move *move = (Move *)malloc(sizeof(Move));
    int x = piece.coord.x + xIncrement;
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
