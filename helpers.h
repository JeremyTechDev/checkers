#if !defined(HELPERS)
#define HELPERS

const char REGULAR[15] = "[0m";       // code 0
const char GREEN[15] = "[1m\033[32m"; // code 1
const char BLUE[15] = "[1m\033[36m";  // code 2
const char RED[15] = "[1m\033[31m";   // code 3
const char ORANGE[15] = "[01;33m";    // code 4
const char PURPLE[15] = "[1;35m";     // code 5

void printColorText(const char *, const char *);
void setHighlightColor(Color);
void setColor(const char *);
void clear(int wait);

/**
 * Print a text with a given color
 * @param {const char *} text - text to print
 * @param {const char *} color - color of the text
 */
void printColorText(const char *text, const char *color)
{
    printf("\033%s%s\033%s", color, text, REGULAR);
}

/**
 * Sets the highlight color given a code
 * @param {Color} colorCode - code of the color
 */
void setHighlightColor(Color colorCode)
{
    if (colorCode == regular)
        setColor(REGULAR);
    else if (colorCode == green)
        setColor(GREEN);
    else if (colorCode == blue)
        setColor(BLUE);
    else if (colorCode == red)
        setColor(RED);
    else if (colorCode == orange)
        setColor(ORANGE);
    else if (colorCode == purple)
        setColor(PURPLE);
}

/**
 * Sets the terminal to print a given color
 * @param {const char *} color - color to set the terminal
 */
void setColor(const char *color)
{
    printf("\033%s", color);
}

/**
 * Clear the screen and waits for enter
 * @param {int} wait - whether to wait for enter or not
 */
void clear(int wait)
{
    if (wait)
    {
        printColorText("\nPress ENTER to continue\n", BLUE);
        fflush(stdin);
        getchar();
    }
    system("clear");
}
#endif // HELPERS
