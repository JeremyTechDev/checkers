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
Coord getKillingMove(PieceList *, Piece, int);
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

    Coord posMove1 = {piece.coord.x + (piece.team == black ? -1 : 1), piece.coord.y + 1, 4};
    Coord posMove2 = {piece.coord.x + (piece.team == black ? -1 : 1), piece.coord.y - 1, 4};

    Piece *pieceAtPosMove1 = getPieceAtPosition(pieceList, posMove1.x, posMove1.y);
    Piece *pieceAtPosMove2 = getPieceAtPosition(pieceList, posMove2.x, posMove2.y);

    /**
     * if there is a piece of the opposite team in the spot, check for killing move and append it to the list
     * otherwise, just add the move to the list
     */
    if (pieceAtPosMove1 && pieceAtPosMove1->team != piece.team)
        insertCoord(&possibleMoves, getKillingMove(pieceList, *pieceAtPosMove1, 1));
    else if (isMovePossible(pieceList, posMove1))
        insertCoord(&possibleMoves, posMove1);

    if (pieceAtPosMove2 && pieceAtPosMove2->team != piece.team)
        insertCoord(&possibleMoves, getKillingMove(pieceList, *pieceAtPosMove2, -1));
    else if (isMovePossible(pieceList, posMove2))
        insertCoord(&possibleMoves, posMove2);

    return possibleMoves;
}

Coord getKillingMove(PieceList *pieceList, Piece piece, int increment)
{
    Coord posKillingMove = {piece.coord.x + (piece.team == black ? 1 : -1), piece.coord.y + increment, 3};
    return posKillingMove;
}

#endif // MOVE
