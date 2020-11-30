#if !defined(STRUCTS)
#define STRUCTS

typedef char Board[8][8][4];

const char *validLetters = "ABCDEFGH";

typedef enum Team
{
    black,
    white,
    none,
} Team;

typedef enum Color
{
    regular,
    green,
    blue,
    red,
    orange,
} Color;

typedef struct Coord
{
    int x;
    int y;
    Color colorCode;
} Coord;

typedef struct Piece
{
    int id;
    Coord coord;
    Team team;
    int isOnGame;
} Piece;

typedef struct Move
{
    Coord coord;
    Piece *killingPiece;
    int killedPieceId; // -1 for no piece killed
} Move;

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

typedef struct MoveList
{
    Move move;
    struct MoveList *next;
    struct MoveList *prev;
} MoveList;

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
