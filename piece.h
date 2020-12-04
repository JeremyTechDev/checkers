#if !defined(PIECE)
#define PIECE

#include "helpers.h"
#include "structs.h"
#include "coord.h"

int isQueen(Piece, Coord);
Piece *getPieceAtPosition(PieceList *, int, int);
PieceList *initializePieces();
void insertPiece(PieceList **, Piece);
void modifyPiece(PieceList **, int, Piece);

/**
 * Adds all the pieces to the Piece List
 * @returns {PieceList *} - all the pieces of the game
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
    // insertPiece(&pieceList, p5);

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
 * @param {PieceList **} - all the pieces of the game
 * @param {Piece} piece - the piece to add
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

    if (*pieceList == NULL) // insert in the first position

    {
        *pieceList = node;
        return;
    }

    node->next = *pieceList; // inserting at first position (push)
    (*pieceList)->prev = node;
    *pieceList = node;
}

/**
 * Finds by id a piece in the piece list and changes it
 * @param {PieceList **} - all the pieces of the game
 * @param {int} id - the id of the piece to change
 * @param {Piece} piece - the new piece
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
 * @param {PieceList *} - all the pieces of the game
 * @param {int} x - x coord
 * @param {int} y - y coord
 * @returns {Piece *} the piece at pos [x,y] or null
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
            piece->coord = node->piece.coord;
            piece->team = node->piece.team;
            piece->isQueen = node->piece.isQueen;
            piece->isOnGame = 1;
            return piece;
        };
        node = node->next;
    }
    return NULL;
}

/**
 * Checks if a piece is a queen
 * @param {Piece} p - the piece to check if queen
 * @param {Coord} c - the new coord of the piece
 * @returns {int} whether the piece is queen or not
 */
int isQueen(Piece p, Coord c)
{
    return p.isQueen || (p.team == black && c.x == 0) || (p.team == white && c.x == 7) ? 1 : 0;
}

#endif // PIECE
