/**
 * This is for parser example and not to be compiled
 */

// Another comment style
#include "test.h"

int main()
{
    float x = 5.0;
    float y = .5;
    float z = 5.7589f;
    int xI = 5;
    int yI = 5.0;
    char *str = "String 1\n";
    char *str = "String 1\
    ";
    char cr = '\n';
    str[0] = 'c';
    printf("Statement to print"); // Comment to be ignored
    bool y = false;               /*
                  Another ingnored awkward comment*/
    return 0;
}