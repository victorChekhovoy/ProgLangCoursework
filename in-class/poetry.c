#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
int main() {
    // Prepare determiners
    char *determiners[3];  // Hmm, which type is declared here?
    determiners[0] = (char *) (malloc (5 * sizeof(char)));
    strcpy(determiners[0], "a");
    determiners[1] = (char *) (malloc (5 * sizeof(char)));
    strcpy(determiners[1], "the");
    determiners[2] = (char *) (malloc (5 * sizeof(char)));
    strcpy(determiners[2], "that");
    //strcpy(determiners[1], "the");
    //cpy(determiners[2], "most");
    // Prepare adjectives
    char *adjectives[3];
    adjectives[0] = (char *) (malloc (5 * sizeof(char)));
    strcpy(adjectives[0], "funky");
    adjectives[1] = (char *) (malloc (5 * sizeof(char)));
    strcpy(adjectives[1], "devilish");
    adjectives[2] = (char *) (malloc (5 * sizeof(char)));
    strcpy(adjectives[2], "hubristic");
    // Prepare nouns
    char *nouns[3];
    nouns[0] = (char *) (malloc (5 * sizeof(char)));
    strcpy(nouns[0], "monkey");
    nouns[1] = (char *) (malloc (5 * sizeof(char)));
    strcpy(nouns[1], "balliwick");
    nouns[2] = (char *) (malloc (5 * sizeof(char)));
    strcpy(nouns[2], "fez");

    // Prepare verbs
    char *verbs[3];
    verbs[0] = (char *) (malloc (5 * sizeof(char)));
    strcpy(verbs[0], "mystifies");
    verbs[1] = (char *) (malloc (5 * sizeof(char)));
    strcpy(verbs[1], "garbles");
    verbs[2] = (char *) (malloc (5 * sizeof(char)));
    strcpy(verbs[2], "cruises");

    // Intialize random number generator (RNG)
    int i;
    time_t t;
    srand((unsigned) time(&t));
    char *line;
    // Generate text!
    for (i = 0 ; i < 10000 ; i++) {
        line = (char *) malloc(12 * 6 * sizeof(char));
        //det adj noun verb adj noun
        // TODO prepare variable called "line" (type: string)
        // Gradually build the line by making random choices about
        // components. We'll use strcat to do this; look this up
        // now to see how it works! The "determiners[rand() % 3]"
        // part of this line uses the (RNG) to select one of the
        // three determiners that you prepared above.
        strcat(line, determiners[rand() % 3]);
        strcat(line, " ");
        strcat(line, adjectives[rand() % 3]);
        strcat(line, " ");
        strcat(line, nouns[rand() % 3]);
        strcat(line, " ");
        strcat(line, verbs[rand() % 3]);
        strcat(line, " ");
        strcat(line, adjectives[rand() % 3]);
        strcat(line, " ");
        strcat(line, nouns[rand() % 3]);
        // Print next line, along with a line number (may help debugging)
        printf("%i\t%s\n", i, line);
        // "Flush" the print buffer to force the line to print immediately
        fflush(stdout);
        free(line);
        // Sleep for one second before printing the next line
        sleep(1);
    }

}
