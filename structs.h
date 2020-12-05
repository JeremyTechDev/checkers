#if !defined(STRUCTS)
#define STRUCTS

typedef char Board[8][8][4];

const char *validLetters = "ABCDEFGH";
const char *PLAYERS_RECORD_FILE = "players.dat";
const char *MATCH_STEP_FILE = "matches.dat";

typedef enum Team
{
    black,
    white,
    none,
} Team;

typedef enum MatchState
{
    lose,
    win,
    tie
} MatchState;

typedef enum Color
{
    regular,
    green,
    blue,
    red,
    orange,
    purple,
} Color;

typedef struct Player
{
    char name[50];
    Team team;
} Player;

typedef struct PlayerMatch
{
    Player player;
    MatchState matchState;
} PlayerMatch;

typedef struct PlayerRecord
{
    Player player;
    int wins;
    int loses;
    int ties;
} PlayerRecord;

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
    int isQueen;
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
