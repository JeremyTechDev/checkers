#if !defined(MOVE)
#define MOVE

#include "structs.h"
#include "piece.h"

MoveList getPossibleMoves(PieceList *, Piece);
int isMovePossible(PieceList *, Move);

/**
 * Returns whether a move is possible or not
 */
int isMovePossible(PieceList *pieceList, Move move)
{
    if (move.x > 7 || move.x < 0)
        return 0;

    if (move.y > 7 || move.x < 0)
        return 0;

    Piece *isTaken = getPieceAtPosition(pieceList, move.x, move.y);
    if (isTaken)
        return 0;

    return 1;
}

/**
 * Returns all the posible moves for a piece
 */
MoveList getPossibleMoves(PieceList *pieceList, Piece piece)
{
    MoveList possibleMoves;
    possibleMoves.moveCount = 0;
    possibleMoves.moves = (Move *)malloc(sizeof(Move));

    Move posMove1 = {piece.x + (piece.team == black ? -1 : 1), piece.y + 1};
    Move posMove2 = {piece.x + (piece.team == black ? -1 : 1), piece.y - 1};

    if (isMovePossible(pieceList, posMove1))
    {
        possibleMoves.moves = (Move *)realloc(possibleMoves.moves, sizeof(Move) * (possibleMoves.moveCount + 1));
        *(possibleMoves.moves + possibleMoves.moveCount) = posMove1;
        possibleMoves.moveCount++;
    }

    if (isMovePossible(pieceList, posMove2))
    {
        possibleMoves.moves = (Move *)realloc(possibleMoves.moves, sizeof(Move) * (possibleMoves.moveCount + 1));
        *(possibleMoves.moves + possibleMoves.moveCount) = posMove2;
        possibleMoves.moveCount++;
    }

    return possibleMoves;
}

#endif // MOVE
