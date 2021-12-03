#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int user_input, number_of_tries = 1;
    unsigned int seed;
    printf("Enter seed: ");
    scanf("%u", &seed);
    printf("Enter your guess: ");
    scanf("%i", &user_input);

    srandom(seed);
    int hidden_number = random()%100 + 1;
    while (user_input != hidden_number){
      if (user_input > hidden_number){
        printf("Too high!\n");
      }
      if (user_input < hidden_number){
        printf("Too low!\n");
      }
      number_of_tries++;
      printf("Enter another guess: ");
      scanf("%i", &user_input);
    }
    printf("Total guesses = %i", number_of_tries);
    return 0;
  }
