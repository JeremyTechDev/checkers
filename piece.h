#if !defined(PIECE)
#define PIECE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

void insertPiece(PieceList **, Piece);
PieceList *initializePieces();
Piece *getPieceAtPosition(PieceList *, int, int);
void modifyPiece(PieceList **, int, Piece);

/**
 * Adds all the pieces to the Piece List
 */
PieceList *initializePieces()
{
    int id = 0;
    PieceList *pieceList = NULL;

    Piece p1 = {1, {4, 3, regular}, black, 0, 1};
    Piece p2 = {2, {3, 2, regular}, white, 0, 1};
    Piece p3 = {3, {1, 2, regular}, white, 0, 1};
    Piece p4 = {4, {1, 4, regular}, white, 0, 1};
    Piece p5 = {5, {6, 1, regular}, white, 0, 1};

    insertPiece(&pieceList, p1);
    insertPiece(&pieceList, p2);
    insertPiece(&pieceList, p3);
    insertPiece(&pieceList, p4);
    insertPiece(&pieceList, p5);

    // white pieces
    // for (int x = 0; x < 3; x++)
    //     for (int y = (x % 2 == 0 ? 1 : 0); y < 8; y += 2)
    //     {
    //         Piece p = {id, {x, y, regular}, white, 0, 1};
    //         insertPiece(&pieceList, p);
    //         id++;
    //     }

    // // black pieces
    // for (int x = 5; x < 8; x++)
    //     for (int y = (x % 2 == 0 ? 1 : 0); y < 8; y += 2)
    //     {
    //         Piece p = {id, {x, y, regular}, black, 0, 1};
    //         insertPiece(&pieceList, p);
    //         id++;
    //     }

    return pieceList;
}

/**
 * Insert a piece into the piece list
 */
void insertPiece(PieceList **pieceList, Piece piece)
{
    PieceList *node = (PieceList *)malloc(sizeof(PieceList));

    if (node == NULL)
    {
        printf("No space for the new piece\n");
        exit(-1);
    }

    node->piece = piece;
    node->next = NULL;
    node->prev = NULL;

    // insert in the first position
    if (*pieceList == NULL)
    {
        *pieceList = node;
        return;
    }

    // inserting at first position (push)
    node->next = *pieceList;
    (*pieceList)->prev = node;
    *pieceList = node;
}

/**
 * Finds by id a piece in the piece list and changes it
 */
void modifyPiece(PieceList **pieceList, int id, Piece piece)
{
    PieceList *firstPos = *pieceList;

    while (*pieceList)
    {
        if ((*pieceList)->piece.id == id)
        {
            (*pieceList)->piece = piece;
            break;
        }
        (*pieceList) = (*pieceList)->next;
    }
    *pieceList = firstPos; // return to first pos
}

/**
 * If there is a piece in the position [x, y], it returns it, NULL otherwise
 */
Piece *getPieceAtPosition(PieceList *pieceList, int x, int y)
{
    PieceList *node = pieceList;

    while (node != NULL)
    {
        if (node->piece.coord.x == x && node->piece.coord.y == y && node->piece.isOnGame == 1)
        {
            Piece *piece = (Piece *)malloc(sizeof(Piece));
            piece->id = node->piece.id;
            piece->coord.x = node->piece.coord.x;
            piece->coord.y = node->piece.coord.y;
            piece->team = node->piece.team;
            piece->isQueen = node->piece.isQueen;
            piece->isOnGame = 1;
            return piece;
        };
        node = node->next;
    }
    return NULL;
}

#endif // PIECE
