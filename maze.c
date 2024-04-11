/**
 * @file maze.c
 * @author Royal Sule
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
int create_maze(maze *this, int height, int width) // This test is adapted from an example provided on: https://www.studysmarter.co.uk/explanations/computer-science/computer-programming/2d-array-in-c/
{
    if (height > MAX_DIM || height < MIN_DIM || width > MAX_DIM || width < MIN_DIM) {
        printf("Error: invalid dimensions\n");
        return 1;
    }

    height = this->height;
    width = this->width;

    this->map = (char**)malloc(sizeof(char) * height);

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
void free_maze(maze *this) // This test is adapted from an example provided on: https://www.studysmarter.co.uk/explanations/computer-science/computer-programming/2d-array-in-c/
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
int get_width(FILE *file) {
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
        printf("Error: invalid width\n");
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
int get_height(FILE *file)
{
    char c;
    int height = 0;

    fseek(file, 0, SEEK_SET);

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            height++;
        }
    }

    if (height > MAX_DIM || height < MIN_DIM) {
        printf("Error: invalid width\n");
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
    char c;
    int i, j = 0;

    fseek(file, 0, SEEK_SET);

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            i = 0;
            j++;
            continue;
        }

        if (i < this->width) {
            this->map[i][j] = c;

            if (c == 'S') {
                this->start.x = i;
                this->start.y = j;
            }
            else if (c == 'E') {
                this->end.x = i;
                this->end.y = j;
            }
        }
        else {
            printf("Error: invalid width\n");
            return 1;
        }

        i++;
    }

    if (j > this->height) {
        printf("Error: invalid height\n");
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
        printf("The player cannot move off the edge of the map\n");
        return;
    }

    if (this->map[player->x][player->y] = '#') {
        printf("The player cannot move throught walls\n");
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
        printf("Error: invalid argument\n");
        return EXIT_ARG_ERROR; 
    }

    // set up some useful variables (you can rename or remove these if you want)
    coord *player;
    maze *this_maze = malloc(sizeof(maze));
    FILE *f;

    // open and validate mazefile
    f = fopen(argv[1], "r");

    if (f = NULL) {
        printf("Error: invalid file\n");
        return EXIT_FILE_ERROR;
    }

    // read in mazefile to struct
    read_maze(this_maze, f);

    // maze game loop
    char input;

    while (has_won(this_maze, player)) {
        printf("Enter command (W/A/S/D) to move through the maze, M to view an image of the map.\n");
        scanf("%c", &input);

        switch(input) {
            case 'W': case 'w':
            case 'A': case 'a':
            case 'S': case 's':
            case 'D': case 'd':
                move(this_maze, player, input);
                break;
            
            case 'M': case 'm':
                print_maze(this_maze, player);
                break;

            default:
                printf("Error: invalid input");
        }
    }

    // win
    printf("Congratulations, you have exiteed the maze!\n");

    // return, free, exit
    fclose(f);
    free_maze(this_maze);

    return 0;
}