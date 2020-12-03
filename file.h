#if !defined(FILES)
#define FILES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
// #include "helpers.h"

// Function declarations
FILE *openFile(const char *, const char *);
void closeFile(FILE *);
void readPlayersFile();
void savePlayerToFile(Player, MatchState);
int countPlayersInFile();

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
    FILE *file = openFile(PLAYERS_RECORD_FILE, "a+b");
    PlayerMatch match = {player, matchState};
    fwrite(&match, sizeof(PlayerMatch), 1, file);
    closeFile(file);
}

/**
 * Counts the amount of players in the file
 * @returns {int} the amount of players
 */
int countPlayersInFile()
{
    FILE *file = openFile(PLAYERS_RECORD_FILE, "rb");
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
