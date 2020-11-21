#if !defined(MOVE)
#define MOVE

#include <string.h>
#include "structs.h"
#include "piece.h"

MoveList getPossibleMoves(PieceList *, Piece);
int isMovePossible(PieceList *, Move);
Move getCordsFromUser();

/**
 * Gets a valid [x,y] cord from the user 
 */
Move getCordsFromUser()
{
    int xCord, yCord;
    char input[10];

    while (1)
    {
        fgets(input, sizeof(input), stdin);

        char letter = toupper(input[0]);
        int number = input[1] - '0';

        xCord = number - 1;
        yCord = (int)(strchr(validLetters, letter) - validLetters);

        // validate move
        if ((xCord >= 0 && xCord <= 7) && (yCord >= 0 && yCord <= 7))
            break;
    }

    Move move = {xCord, yCord};
    return move;
}

/**
 * Returns whether a move is possible or not
 */
int isMovePossible(PieceList *pieceList, Move move)
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
