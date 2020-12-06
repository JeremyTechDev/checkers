#if !defined(FILES)
#define FILES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// Function declarations
FILE *openFile(const char *, const char *);
void closeFile(FILE *);
void readPlayersFile();
void readMatchStepsFile();
void savePlayerToFile(Player, MatchState);
void saveMatchStepToFile(Player, Coord *, Coord, int);
int countPlayersInFile();
int comparePlayersRecord(const void *, const void *);
char *formatCoord(Coord);

/*
 * Converts a [x,y] coord to @# text
 * @param {Coord} coord - the coord to convert to text
 * @returns {char *} the text value of coord
 */
char *formatCoord(Coord coord)
{
    char *result = (char *)malloc(sizeof(char) * 3);
    *(result) = (char)validLetters[coord.y];
    *(result + 1) = (char)((coord.x + 1) + '0');
    *(result + 2) = '\0';
    return result;
}

/**
 * Opens a file
 * @param {char *} fileName - the name and directory of the file to open
 * @param {char *} mode - the opening mode
 * @returns {FILE *} the opened file
 */
FILE *openFile(const char *fileName, const char *mode)
{
    FILE *file = fopen(fileName, mode);
    if (!file)
    {
        printf("\nAn error ocurred while opening the file\n");
        exit(-1);
    }
    return file;
}

/**
 * Closes a file
 * @param {FILE *} file - file to close
 */
void closeFile(FILE *file)
{
    if (fclose(file) != 0)
        printf("\nAn error ocurred while closing the file\n");
}

/**
 * Saves a player match to the players matches file
 * @param {Player} player - the player to save
 * @param {MatchState} matchState - whether the played lost, won or tied
 */
void savePlayerToFile(Player player, MatchState matchState)
{
    PlayerMatch match = {player, matchState};
    FILE *file = openFile(PLAYERS_RECORD_FILE, "a+b");
    fwrite(&match, sizeof(PlayerMatch), 1, file);
    closeFile(file);
}

/**
 * Saves a match step to the matches file
 * @param {Player} player - the player to save
 * @param {Cord} from - moved from coord
 * @param {Cord} to - moved to coord
 */
void saveMatchStepToFile(Player player, Coord *from, Coord to, int kill)
{
    char toSave[150] = {'\0'};
    strcat(toSave, player.name);
    strcat(toSave, " (");
    strcat(toSave, player.team == black ? "black" : "white");
    strcat(toSave, ") ");
    if (kill)
    {
        strcat(toSave, "killed at ");
        strcat(toSave, formatCoord(to));
    }
    else
    {
        strcat(toSave, "moved from ");
        strcat(toSave, formatCoord(*from));
        strcat(toSave, " to ");
        strcat(toSave, formatCoord(to));
    }
    strcat(toSave, " (");
    strcat(toSave, player.team == black ? "white" : "black");
    strcat(toSave, ") ");
    strcat(toSave, "\0");

    FILE *file = openFile(MATCH_STEP_FILE, "a+b");
    fwrite(&toSave, sizeof(char), 150, file);
    closeFile(file);
}

/**
 * Counts the amount of players in the file
 * @returns {int} the amount of players
 */
int countPlayersInFile()
{
    FILE *file = openFile(PLAYERS_RECORD_FILE, "a+");
    fseek(file, 0L, SEEK_END);
    int playersCount = ftell(file) / sizeof(PlayerMatch);
    closeFile(file);
    return playersCount;
}

/**
 * Counts the amount of match steps in the file
 * @returns {int} the amount of players
 */
int countMatchStepInFile()
{
    FILE *file = openFile(MATCH_STEP_FILE, "a+");
    fseek(file, 0L, SEEK_END);
    int matchSteps = ftell(file) / (sizeof(char) * 150);
    closeFile(file);
    return matchSteps;
}

/**
 * Compares to sort a PlayerRecords list by wins
 * @param {const void *} - the first value to compare 
 * @param {const void *} - the second value to compare 
 * @returns {int} - the result of the comparatation 
 */
int comparePlayersRecord(const void *valueA, const void *valueB)
{
    PlayerRecord *player1 = (PlayerRecord *)valueA;
    PlayerRecord *player2 = (PlayerRecord *)valueB;
    return player2->wins - player1->wins;
}

// Reads and prints all the players so far with their stats
void readPlayersFile()
{
    PlayerMatch tmp;
    PlayerRecord *playersRecords = (PlayerRecord *)malloc(sizeof(PlayerRecord));
    int playersCount = countPlayersInFile(), read = 0, saved, count = 0;
    FILE *file = openFile(PLAYERS_RECORD_FILE, "rb");

    if (playersCount == 0)
        printf("Sorry, there are no players records to show!");
    while (read < playersCount)
    {
        saved = 0;
        fread(&tmp, sizeof(PlayerMatch), 1, file);

        for (int i = 0; i < read; i++)
        {
            if ((playersRecords + i) && strcmp((playersRecords + i)->player.name, tmp.player.name) == 0)
            {
                saved = 1;
                (playersRecords + i)->player = tmp.player;
                if (tmp.matchState == win)
                    (playersRecords + i)->wins++;
                else if (tmp.matchState == lose)
                    (playersRecords + i)->loses++;
                else if (tmp.matchState == tie)
                    (playersRecords + i)->ties++;
                break;
            }
        }

        if (!saved)
        {
            playersRecords = (PlayerRecord *)realloc(playersRecords, sizeof(PlayerRecord));
            *(playersRecords + count) = (PlayerRecord){tmp.player, tmp.matchState == win ? 1 : 0, tmp.matchState == lose ? 1 : 0, tmp.matchState == tie ? 1 : 0};
            count++;
        }
        read++;
    }

    qsort(playersRecords, count, sizeof(PlayerRecord), comparePlayersRecord);
    for (int i = 0; i < count; i++)
        printf(">>> %d: %s (%s)\t\tWins: %d \tLoses: %d \tTies: %d\n", i + 1, (playersRecords + i)->player.name, (playersRecords + i)->player.team == black ? "Black" : "White", (playersRecords + i)->wins, (playersRecords + i)->loses, (playersRecords + i)->ties);
    closeFile(file);
}

// Reads and prints all the match steps in the file
void readMatchStepsFile()
{
    char tmp[150] = {'\0'};
    int matchStepsCount = countMatchStepInFile(), read = 0, count = 0;
    FILE *file = openFile(MATCH_STEP_FILE, "rb");

    if (matchStepsCount == 0)
        printf("Sorry, there are no matches to show!");
    while (read < matchStepsCount)
    {
        fread(&tmp, (sizeof(char) * 150), 1, file);
        fputs(tmp, stdout);
        printf("\n");
        read++;
    }
    closeFile(file);
}

#endif // FILES
