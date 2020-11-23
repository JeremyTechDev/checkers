#if !defined(HELPERS)
#define HELPERS

const char REGULAR[15] = "[0m";        // code 0
const char YELLOW[15] = "[1m\033[32m"; // code 1
const char BLUE[15] = "[1m\033[36m";   // code 2
const char RED[15] = "[1m\033[31m";    // code 3
const char ORANGE[15] = "[01;33m";     // code 4

void printColorText(const char *text, const char *color)
{
    printf("\033%s", color);
    printf("%s", text);
    printf("\033%s", REGULAR);
}

void setColor(const char *color)
{
    printf("\033%s", color);
}

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
