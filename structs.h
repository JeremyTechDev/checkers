#if !defined(STRUCTS)
#define STRUCTS

typedef char Board[8][8][4];

const char *validLetters = "ABCDEFGH";

typedef enum Team
{
    black,
    white
} Team;

typedef struct Piece
{
    int id;
    int x;
    int y;
    Team team;
    int isOnGame;
} Piece;

typedef struct PieceList
{
    Piece piece;
    struct PieceList *next;
    struct PieceList *prev;
} PieceList;

typedef struct Coord
{
    int x;
    int y;
} Coord;

typedef struct CoordList
{
    Coord *moves;
    int moveCount;
} CoordList;

Board board = {
    {"( )", "[ ]", "( )", "[ ]", "( )", "[ ]", "( )", "[ ]"},
    {"[ ]", "( )", "[ ]", "( )", "[ ]", "( )", "[ ]", "( )"},
    {"( )", "[ ]", "( )", "[ ]", "( )", "[ ]", "( )", "[ ]"},
    {"[ ]", "( )", "[ ]", "( )", "[ ]", "( )", "[ ]", "( )"},
    {"( )", "[ ]", "( )", "[ ]", "( )", "[ ]", "( )", "[ ]"},
    {"[ ]", "( )", "[ ]", "( )", "[ ]", "( )", "[ ]", "( )"},
    {"( )", "[ ]", "( )", "[ ]", "( )", "[ ]", "( )", "[ ]"},
    {"[ ]", "( )", "[ ]", "( )", "[ ]", "( )", "[ ]", "( )"},
};

#endif // STRUCTS
