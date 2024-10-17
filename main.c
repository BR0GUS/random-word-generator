#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#define MAX_INPUT_LENGTH 10
#define MAX_ALLOWED_NUMBER 10

#define MAX_WORD_LENGTH 100
#define INITIAL_ARRAY_SIZE 10

unsigned long
get_user_number(void)
{
        char input[MAX_INPUT_LENGTH];
        unsigned int number;
        char *endptr;

        printf("How many words do you want? ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
                fprintf(stderr, "Error reading input.\n");
                return 1;
        }

        errno = 0;
        unsigned long temp = strtoul(input, &endptr, 10);

        if (endptr == input) {
                fprintf(stderr, "No digits were found.\n");
                return 1;
        }

        if (*endptr != '\n' && *endptr != '\0') {
                fprintf(stderr, "Extra characters after number.\n");
                return 1;
        }

        if (errno == ERANGE || temp > UINT_MAX) {
                fprintf(stderr, "Number out of range.\n");
                return 1;
        }

        number = (unsigned int)temp;

        if (number > MAX_ALLOWED_NUMBER) {
                fprintf(stderr, "Number must be between 0 and 10.\n");
        }

        return number;
}

char**
load_the_words(void)
{
        FILE *file;
        char word[MAX_WORD_LENGTH];
        char **words = NULL;
        int word_count = 0;
        int array_size = INITIAL_ARRAY_SIZE;

        file = fopen("words.txt", "r");
        if (file == NULL) {
                printf("Error opening file.\n");
                return NULL;
        }

        words = malloc(array_size * sizeof(char *));
        if (words == NULL) {
                printf("Memory allocation failed.\n");
                fclose(file);
                return NULL;
        }

        while (fgets(word, MAX_WORD_LENGTH, file) != NULL) {
                word[strcspn(word, "\n")] = 0;
                
                if (word_count >= array_size) {
                        array_size *= 2;
                        char **temp = realloc(words, array_size * sizeof(char *));
                        if (temp == NULL) {
                                printf("Memory reallocation failed.\n");
                                break;
                        }
                        words = temp;
                }

                words[word_count] = strdup(word);
                if (words[word_count] == NULL) {
                        printf("Memory allocation failed for word.\n");
                        break;
                }
                word_count++;
        }

        fclose(file);


        return words;
        free(words);
}

int
choose_random(char** words, unsigned long ammount)
{
        if (words == NULL) return 1;

        int size = 0;
        while (words[size] != NULL) {
                size++;
        }

        if (size == 0) return 1;

        static int seeded = 0;
        if (!seeded) {
                srand((unsigned int)time(NULL));
                seeded = 1;
        }

        for (int i = 0; i < ammount; i++) {
                int random_index = rand() % size;
                printf("%s\n", words[random_index]);
        }

        return 0;
}

int
main(void)
{

        unsigned long number_of_words = get_user_number();
        char** words = load_the_words();
        int r = choose_random(words, number_of_words);
        
        return 0;
}

