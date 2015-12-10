#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

/*
 * Linked list data structure definitions.  You can choose
 * to use these data structure and type definitions or modify
 * them to suit your taste.
 */
struct _wordnode {
    char word[256];
    struct _wordnode *next;
};
typedef struct _wordnode wordnode;

/*
 * Be happy you didn't have to write this function.
 * Prints a low-tech ASCII gallows.  Max number of misses
 * is 7 (i.e., if there are 7 misses, the player loses and
 * the poor sap gets hung).  
 */
void print_gallows(int num_missed) {
    // make sure that we got called in a reasonable way
    assert(num_missed >= 0 && num_missed <= 7);

    printf("\n\n       |||========|||\n");
    if (num_missed > 0) {
        printf ("       |||         |\n");
    } else {
        printf ("       |||          \n");
    }

    if (num_missed > 1) {
        printf ("       |||         O\n");
    } else {
        printf ("       |||          \n");
    }

    if (num_missed > 2) {
        if (num_missed > 4) {
            printf ("       |||        /|\\\n");
        } else if (num_missed > 3) {
            printf ("       |||        /| \n");
        } else {
            printf ("       |||        /  \n");
        }
    } else {
        printf ("       |||           \n");
    }

    if (num_missed > 5) {
        if (num_missed > 6) {
            printf ("       |||        / \\\n");
        } else {
            printf ("       |||        /  \n");
        }
    } else {
        printf ("       |||           \n");
    }

    printf ("       |||\n");
    printf ("       |||\n");
    printf ("     =================\n\n");
}

/*
 *Put here for debugging purposes. 
 */
void list_print(const wordnode *list) {
    int i = 0;
    printf("In list_print\n");
    while (list != NULL) {
        printf("List item %d: %s\n", i++, list->word);
        list = list->next;
    }
}

/*
 * Checks the validity of the letter that was input.  
 */
bool check_letter(char *ch) {
     if (strlen(ch) > 1) {
        printf("Only ONE letter! Don't get too excited...");
        return false;
     }
     else if (strlen(ch) < 1) {
        printf("You didn't type anything. I can't read minds! ");
        return false;
     }
     if (isalpha(ch[0]) == 0) {
        printf("How about a letter. Seriously. ");
        return false;
     }
     return true; 
}

/*
 *Handles the user input. Only returns when a valid guess has been entered. 
 */
char guess_letter(int num_guess) {
    printf("What is your guess? "); 
    char guess[32];
    fgets(guess, 32, stdin); 
    guess[strlen(guess)-1] = '\0';
    while (!check_letter(guess)) {
         printf("What is your guess? "); 
         fgets(guess, 32, stdin); 
         guess[strlen(guess)-1] = '\0';
    }
    return guess[0]; 
}

/*
 *Checks whether the character in each string is alphabetical or
 *not. 
 */
bool check_word(char *str) {
     bool is_valid = true;
     for (int i=0; i<strlen(str); i++) {
         if (!isalpha(str[i])) {
            is_valid = false;
            break;
         }
     }
     if (is_valid) {
        for (int j=0; j<strlen(str); j++) {
             str[j] = toupper(str[j]);
        }
     }
     return is_valid; 
}

/*
 *Checks to see whether if letter guessed is in the secret word.
 */
bool in_word(const char *word, char letter) {
     int i = 0;
     bool inword = false;
     while (i < strlen(word)) {
          if (word[i] == toupper(letter))  {
              inword = true;
              break; 
          }
          i++;
     } 
     return inword; 
}

/*
 *Initializes the correct array with underscores. 
 */
void initialize_correct(char correct[], const char *word) {
     int i = 0;
     while (i < (strlen(word) + 1)) {
           correct[i] = '_';
           i++; 
     }
}

/*
 *Initializes the guess array.
 */
void initialize_guesses(char letters_guessed[], int num) {
     int i = 0;
     while (i < num) { 
           letters_guessed[i] = "";
           i++;
     }
}

/*
 *Modifies the correct array in place whenever the user guesses a 
 *letter correctly. 
 */
void correct_letters(char correct[], const char *word, char letter) {
     int i = 0;
     while (i < strlen(word)) {
           if (word[i] == toupper(letter)) {
              correct[i] = word[i];
           }
           i++;
     }
}

/*
 * Compares whether the letters in the correct array match with  
 *letters in the secret word. 
 */
bool compare_words(char correct[], const char *word) {
     int i = 0;
     bool is_same = true;
     while (i < strlen(word)) {
           if (word[i] != correct[i]) {
               is_same = false;
               break;
           }
           i++;
     }
     return is_same; 
}


/*
 * Play one game of Hangperson.  The secret word is passed as a
 * parameter.  The function should return true if the player won,
 * and false otherwise.
 */

bool one_game(const char *word) {
     int max_guesses = 7; 
     int num_guesses = 0;
     //print the initial state of the gallows
     print_gallows(num_guesses); 
     char letters_guessed[27]; 
     char correct[strlen(word) + 1];
     correct[strlen(word)] = '\0'; 
     letters_guessed[26] = '\0';
     initialize_correct(correct, word); 
     initialize_guesses(letters_guessed, 27); 
     int num = 0;
     //print the initial state of the secret word
     for (int k=0; k<strlen(word); k++) {
         printf("%c ", correct[k]);
     }
     printf("\n"); 
     printf("Already guessed: (none)\n"); //beginning of the game
     while (num_guesses < max_guesses) {
           char guess = guess_letter(num_guesses);
           //if NOT in word
           if (!in_word(word, guess)) {
               printf("Bad guess. You stink. "); 
               letters_guessed[num] = toupper(guess);
               num_guesses++;
               print_gallows(num_guesses);
               if (num_guesses == max_guesses) {
                    printf("Sorry, the secret word was %s\n", word);
               }
           }
           //if in word
           else {
               printf("Good guess. ");  
               letters_guessed[num] = toupper(guess); 
               correct_letters(correct, word, guess); 
               if (compare_words(correct, word)) {
                  printf("Congratulations! You guessed the secret word, %s\n", word);
                  break;
               } 
               else {
                  print_gallows(num_guesses); 
               }
           }
           num++;
           //Prints the letters/dashes of the secret word
           for (int j=0; j<strlen(word); j++) {
               printf("%c ", correct[j]); 
           }
           printf("\n"); 
           //Prints the letters that have been guessed 
           printf("Already guessed: "); 
           for (int i=0; i<num; i++) {
                   printf("%c ", letters_guessed[i]); 
           }
           printf("\n");
     }
     return 0;
}

/* 
 * wordbank-related functions. please don't change the
 * function prototypes for these three functions.
 */
wordnode *load_words(const char *filename, int *num_words) {
   FILE *fp = fopen(filename, "r"); 
   wordnode *head = NULL;
   char str[256] = ""; 
   while (fgets(str, 256, fp) != NULL) { 
         char *new_word = strtok(str, " \t\n");
         if (check_word(new_word)) {
             wordnode *string = malloc(sizeof(wordnode));
             strcpy(string->word, new_word); 
             //basically, the append method from hw03
             if (head == NULL) {
                 string->next = NULL;
                 head = string;
             }
             else {
                 wordnode *current = head;
                 while (current->next != NULL) {
                       current = current->next;
                 }
                 string->next = NULL;
                 current->next = string;
             }
             (*num_words)++;
         }
   }
   fclose(fp);
   return head;     
}

void free_words(wordnode *words) {
    while (words != NULL) {
         wordnode *temp = words; 
         words = words->next;
	 free(temp);
    }
}

const char *choose_random_word(wordnode *wordlist, int *num_words){
    int random_num = rand()%(*num_words);
    int i = 0;
    wordnode *current = wordlist; 
    while (i < random_num) {
          current = current->next;
          i++;
    }
    const char *some_word = current->word;
    return some_word;
}


/*
 * The main function is where everything starts.  Choose a random
 * word and call one_game.  Start first on getting the word-loading
 * to work correctly, then handle the game playing capabilities.
 */
#ifndef AUTOTEST
int main() {
    srandom(time(NULL));
    int num_words = 0;
    wordnode *words = load_words("/usr/share/dict/words", &num_words); 
    //list_print(words); put here for debugging purposes 
    if (num_words == 0) {
        printf("Didn't load any words?!\n");
        return 0;
    }
    const char *word = choose_random_word(words, &num_words);
    one_game(word);
    free_words(words);
    return 0;
}
#endif
