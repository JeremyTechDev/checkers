#if !defined(STRUCTS)
#define STRUCTS

typedef char Board[8][8][4];

const char *validLetters = "ABCDEFGH";

typedef enum Team
{
    black,
    white
} Team;

typedef struct Coord
{
    int x;
    int y;
    int colorCode;
} Coord;

typedef struct Piece
{
    int id;
    Coord coord;
    Team team;
    int isOnGame;
} Piece;

typedef struct PieceList
{
    Piece piece;
    struct PieceList *next;
    struct PieceList *prev;
} PieceList;

typedef struct CoordList
{
    Coord coord;
    struct CoordList *next;
    struct CoordList *prev;
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
