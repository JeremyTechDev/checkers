#if !defined(MOVE)
#define MOVE

#include <string.h>
#include "structs.h"
#include "piece.h"

CoordList getPossibleMoves(PieceList *, Piece);
int isMovePossible(PieceList *, Coord);
Coord getCordsFromUser();

/**
 * Gets a valid [x,y] cord from the user 
 */
Coord getCordsFromUser()
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

    Coord coord = {xCord, yCord};
    return coord;
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
CoordList getPossibleMoves(PieceList *pieceList, Piece piece)
{
    CoordList possibleMoves;
    possibleMoves.moveCount = 0;
    possibleMoves.moves = (Coord *)malloc(sizeof(Coord));

    Coord posMove1 = {piece.x + (piece.team == black ? -1 : 1), piece.y + 1};
    Coord posMove2 = {piece.x + (piece.team == black ? -1 : 1), piece.y - 1};

    if (isMovePossible(pieceList, posMove1))
    {
        possibleMoves.moves = (Coord *)realloc(possibleMoves.moves, sizeof(Coord) * (possibleMoves.moveCount + 1));
        *(possibleMoves.moves + possibleMoves.moveCount) = posMove1;
        possibleMoves.moveCount++;
    }

    if (isMovePossible(pieceList, posMove2))
    {
        possibleMoves.moves = (Coord *)realloc(possibleMoves.moves, sizeof(Coord) * (possibleMoves.moveCount + 1));
        *(possibleMoves.moves + possibleMoves.moveCount) = posMove2;
        possibleMoves.moveCount++;
    }

    return possibleMoves;
}

#endif // MOVE
