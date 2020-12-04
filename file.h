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
void savePlayerToFile(Player, MatchState);
void saveMatchStepToFile(Player, Coord, Coord);
int countPlayersInFile();
char *formatCoord(Coord);

/**
 * Converts a [x,y] coord to @# text
 * @param {Coord} coord - the coord to convert to text
 * @returns {char *} the text value of coord
 */
char *formatCoord(Coord coord)
{
    char *result = (char *)malloc(sizeof(char) * 3);
    *(result) = (char)validLetters[coord.y];
    *(result + 1) = (char)((coord.x + 1) + '0');
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

void saveMatchStepToFile(Player player, Coord from, Coord to)
{
    char toSave[150];
    strcat(toSave, player.name);
    strcat(toSave, " moved from ");
    strcat(toSave, formatCoord(from));
    strcat(toSave, " to ");
    strcat(toSave, formatCoord(to));

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

// Reads and prints all the players so far with their stats
void readPlayersFile()
{
    PlayerMatch tmp;
    PlayerRecord *playersRecords = (PlayerRecord *)malloc(0);
    int playersCount = countPlayersInFile(), read = 0, saved, count = 0;
    FILE *file = openFile(PLAYERS_RECORD_FILE, "rb");

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
            *(playersRecords + count) = {tmp.player, tmp.matchState == win ? 1 : 0, tmp.matchState == lose ? 1 : 0, tmp.matchState == tie ? 1 : 0};
            count++;
        }
        read++;
    }
    closeFile(file);

    for (int i = 0; i < count; i++)
        printf("Name: %s - Team: %s - Wins: %d - Loses: %d - Ties: %d \n", (playersRecords + i)->player.name, (playersRecords + i)->player.team == black ? "Black" : "white", (playersRecords + i)->wins, (playersRecords + i)->loses, (playersRecords + i)->ties);
}

#endif // FILES
