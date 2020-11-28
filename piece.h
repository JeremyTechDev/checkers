#if !defined(PIECE)
#define PIECE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

void insertPiece(PieceList **, Piece);
PieceList *initializePieces();
void printPieces(PieceList *);
Piece *getPieceAtPosition(PieceList *, int, int);
void modifyPiece(PieceList **, int, Piece);

/**
 * Adds all the pieces to the Piece List
 */
PieceList *initializePieces()
{
    int id = 0;
    PieceList *pieceList = NULL;

    // white pieces
    for (int x = 0; x < 3; x++)
        for (int y = (x % 2 == 0 ? 1 : 0); y < 8; y += 2)
        {
            Piece p = {id, {x, y, regular}, white, 1};
            insertPiece(&pieceList, p);
            id++;
        }

    // black pieces
    for (int x = 5; x < 8; x++)
        for (int y = (x % 2 == 0 ? 1 : 0); y < 8; y += 2)
        {
            Piece p = {id, {x, y, regular}, black, 1};
            insertPiece(&pieceList, p);
            id++;
        }

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
    PieceList *node = *pieceList;
    PieceList *newNode = (PieceList *)malloc(sizeof(PieceList));

    while (node != NULL)
    {
        if (node->piece.id == id)
        {
            newNode->piece = piece;
            newNode->next = node->next;
            newNode->prev = node->prev;

            if ((*pieceList)->prev)
                (*pieceList)->prev->next = newNode;

            // return to first pos
            *pieceList = firstPos->next;

            free(node);
            return;
        }
        node = node->next;
        (*pieceList) = (*pieceList)->next;
    }

    // return to first pos
    *pieceList = firstPos;
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
            piece->isOnGame = 1;
            return piece;
        };
        node = node->next;
    }
    return NULL;
}

/**
 * Just for development
 * Prints all the pieces with their info
 */
void printPieces(PieceList *pieceList)
{
    PieceList *node = pieceList;
    while (node != NULL)
    {
        printf(">%d Team: %s\t X: %d - Y: %d\n", node->piece.id, node->piece.team == black ? "black" : "white", node->piece.coord.x, node->piece.coord.y);
        node = node->next;
    }
}

#endif // PIECE
