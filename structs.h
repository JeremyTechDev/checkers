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

typedef struct Move
{
    int x;
    int y;
} Move;

typedef struct MoveList
{
    Move *moves;
    int moveCount;
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
