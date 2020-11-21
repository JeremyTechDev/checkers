#if !defined(HELPERS)
#define HELPERS

const char YELLOW[] = "[01;33m";
const char REGULAR[] = "[0m";

void printColorText(const char *text, const char *color)
{
    printf("\033%s", color);
    printf("%s", text);
    printf("\033%s", REGULAR);
}

void yellow()
{
    printf("\033%s", YELLOW);
}

void reset()
{
    printf("\033%s", REGULAR);
}
#endif // HELPERS
