#if !defined(MOVE)
#define MOVE

#include <string.h>
#include "structs.h"
#include "piece.h"
#include "coord.h"

CoordList *getPossibleMoves(PieceList *, Piece);
int isMovePossible(PieceList *, Coord);
Coord getCoordsFromUser();
int isCoordInList(CoordList, int, int);

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
CoordList *getPossibleMoves(PieceList *pieceList, Piece piece)
{
    CoordList *possibleMoves = NULL;

    Coord posMove1 = {piece.x + (piece.team == black ? -1 : 1), piece.y + 1};
    Coord posMove2 = {piece.x + (piece.team == black ? -1 : 1), piece.y - 1};

    if (isMovePossible(pieceList, posMove1))
        insertCoord(&possibleMoves, posMove1);

    if (isMovePossible(pieceList, posMove2))
        insertCoord(&possibleMoves, posMove2);

    return possibleMoves;
}

#endif // MOVE
