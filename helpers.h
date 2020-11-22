#if !defined(HELPERS)
#define HELPERS

const char ORANGE[] = "[01;33m";
const char RED[] = "[1m\033[31m";
const char YELLOW[] = "[1m\033[32m";
const char BLUE[] = "[1m\033[36m";
const char REGULAR[] = "[0m";

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
