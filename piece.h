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
void kill(PieceList **, Piece);

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
            Piece p = {id, x, y, white, 1};
            insertPiece(&pieceList, p);
            id++;
        }

    // black pieces
    for (int x = 5; x < 8; x++)
        for (int y = (x % 2 == 0 ? 1 : 0); y < 8; y += 2)
        {
            Piece p = {id, x, y, black, 1};
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
    PieceList *node = *pieceList;
    PieceList *newNode = (PieceList *)malloc(sizeof(PieceList));

    while (node != NULL)
    {
        if (node->piece.id == id)
        {
            newNode->piece = piece;
            newNode->next = node->next;
            newNode->prev = node->prev;

            (*pieceList)->prev->next = newNode;

            free(node);
            return;
        }
        node = node->next;
        (*pieceList) = (*pieceList)->next;
    }
}

/**
 * If there is a piece in the position [x, y], it returns it, NULL otherwise
 */
Piece *getPieceAtPosition(PieceList *pieceList, int x, int y)
{
    PieceList *node = pieceList;

    while (node != NULL)
    {
        if (node->piece.x == x && node->piece.y == y && node->piece.isOnGame)
        {
            Piece *piece = (Piece *)malloc(sizeof(Piece));
            piece->id = node->piece.id;
            piece->x = node->piece.x;
            piece->y = node->piece.y;
            piece->team = node->piece.team;
            piece->isOnGame = 1;
            return piece;
        };
        node = node->next;
    }
    return NULL;
}

/**
 * Kills a piece if it is possible
 */
void kill(PieceList **pieceList, Piece pieceToKill)
{
    Piece newPiece = {pieceToKill.id, pieceToKill.x, pieceToKill.y, pieceToKill.team, 0};
    modifyPiece(pieceList, pieceToKill.id, newPiece);
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
        printf(">%d Team: %s\t X: %d - Y: %d\n", node->piece.id, node->piece.team == black ? "black" : "white", node->piece.x, node->piece.y);
        node = node->next;
    }
}

#endif // PIECE
