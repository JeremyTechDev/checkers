#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "structs.h"
#include "piece.h"

int main(int argc, char const *argv[])
{
    printf("Checkers Game!\n\n");

    PieceList *pieceList = initializePieces();
    printPieces(pieceList);

    return 0;
}
