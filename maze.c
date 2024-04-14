/**
 * @file maze.c
 * @author (YOUR NAME)
 * @brief Code for the maze game for COMP1921 Assignment 2
 * NOTE - You can remove or edit this file however you like - this is just a provided skeleton code
 * which may be useful to anyone who did not complete assignment 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    char **map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail
 */
int create_maze(maze *this, int height, int width) // Lines 54 - 58 were adapted from an example provided on: https://www.studysmarter.co.uk/explanations/computer-science/computer-programming/2d-array-in-c/
{
    if (height > MAX_DIM || height < MIN_DIM || width > MAX_DIM || width < MIN_DIM) {
        return 1;
    }

    this->height = height;
    this->width = width;

    this->map = (char**)malloc(sizeof(char*) * height);

    for (int i = 0; i < height; ++i) {
        this->map[i] = (char*)malloc(sizeof(char) * width);
    }

    return 0;
}

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param this the pointer to the struct to free
 */
void free_maze(maze *this) // Lines 70 - 73 were adapted from an example provided on: https://www.studysmarter.co.uk/explanations/computer-science/computer-programming/2d-array-in-c/
{
    for (int i = 0; i < this->height; ++i) {
        free(this->map[i]);
    }
    free(this->map);
}

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)
 */
int get_width(FILE *file) // Lines 84 - 92 were adapted from an example provided on: https://www.quora.com/How-do-I-read-the-1st-line-of-a-file-in-C-using-File-Handling
{
    char c;
    int width = 0;

    fseek(file, 0, SEEK_SET);

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            break;
        }
        width++;
    }

    if (width > MAX_DIM || width < MIN_DIM) {
        return 0;
    }

    return width;
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE *file) // Line 119 was adapted from an example provided on: https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
{
    int buffer_size = 1024;
    char line[buffer_size];
    int height = 0;

    fseek(file, 0, SEEK_SET);

    while (fgets(line, buffer_size, file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        height++;
    }

    if (height > MAX_DIM || height < MIN_DIM) {
        return 0;
    }

    return height;
}

/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *this, FILE *file)
{
    int buffer_size = 1024;
    char line[buffer_size];
    int i, j = 0;
    int count_start = 0, count_end = 0;

    fseek(file, 0, SEEK_SET);

    while (fgets(line, buffer_size, file) != NULL) {
        line[strcspn(line, "\n")] = 0;

        if (j > this->height - 1) {
            return 1;
        }

        for (i = 0; i < strlen(line); i++) {
            char c = line[i];

            if (c != 'S' && c != 'E' && c != ' ' && c != '#' && c != '\n') {
                return 1;
            }

            this->map[j][i] = c;

            if (line[i] == 'S') {
                count_start++;
                this->start.x = i;
                this->start.y = j;
            }
            else if (line[i] == 'E') {
                count_end++;
                this->end.x = i;
                this->end.y = j;
            }
        }

        if (i != this->width) {
            return 1;
        }

        j++;
    }

    if (j != this->height) {
        return 1;
    }

    if (count_start != 1 || count_end != 1) {
        return 1;
    }

    return 0;
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
        // end each row with a newline.
        printf("\n");
    }
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
void move(maze *this, coord *player, char direction)
{
    int current_x = player->x;
    int current_y = player->y;
    
    switch(direction) {
        case 'W': 
        case 'w':
            current_y--;
            break;

        case 'A':
        case 'a':
            current_x--;
            break;

        case 'S':
        case 's':
            current_y++;
            break;

        case 'D':
        case 'd':
            current_x++;
            break;

        default:
            return;
    }
    
    if (current_x < 0 || current_x > this->width - 1|| current_y < 0 || current_y > this->height - 1) {
        printf("The player cannot move off the edge of the map, try again!\n");
        return;
    }

    if (this->map[current_y][current_x] == '#') {
        printf("The player cannot move throught walls, try again!\n");
        return;
    }

    player->x = current_x;
    player->y = current_y;
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *this, coord *player)
{
    if (player->x == this->end.x && player->y == this->end.y) {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // check args
    if (argc < 2) {
        return EXIT_ARG_ERROR;
    }

    // set up some useful variables (you can rename or remove these if you want)
    coord *player;
    maze *this_maze = malloc(sizeof(maze));
    FILE *file;

    // open and validate mazefile
    file = fopen(argv[1], "r");

    if (file == NULL) {
        return EXIT_FILE_ERROR;
    }

    // read in mazefile to struct
    if (create_maze(this_maze, 9, 15) != 0) {
        return EXIT_MAZE_ERROR;
    }

    if (read_maze(this_maze, file) != 0) {
        return EXIT_MAZE_ERROR;
    }

    // maze game loop

    // win

    // return, free, exit
    fclose(file);
    free_maze(this_maze);

    return EXIT_SUCCESS;
}