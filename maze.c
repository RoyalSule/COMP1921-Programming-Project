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

// define for buffer size
#define BUFFER_SIZE 1024

typedef struct __Coord {
    int x;
    int y;
} Coord;

typedef struct __Maze {
    char **map;
    int height;
    int width;
    Coord start;
    Coord end;
} Maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail
 */
 int create_maze(Maze *this, int height, int width) { 
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
 void free_maze(Maze *this) { 
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
    int width = 0;
    char c;

    fseek(file, 0, SEEK_SET);

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            break;
        }
        width++;
    }

    if (width > MAX_DIM || width < MIN_DIM) {
        printf("ValueError: invalid width\n");
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
 int get_height(FILE *file) {
    int height = 0;
    char line[BUFFER_SIZE];

    fseek(file, 0, SEEK_SET);

    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        height++;
    }

    if (height > MAX_DIM || height < MIN_DIM) {
        printf("ValueError: invalid height\n");
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
 int read_maze(Maze *this, FILE *file) {
    int i = 0;
    char line[BUFFER_SIZE];
    int start_count = 0, end_count = 0;

    fseek(file, 0, SEEK_SET);

    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        int length = strlen(line);
        printf("%d\n", length);

        line[strcspn(line, "\n")] = 0;

        if (i >= this->height) {
            return 1;
        }

        for (int j = 0; j < length; j++) {
            char c = line[j];

            if (c != 'S' && c != 'E' && c != ' ' && c != '#' && c != '\n') {
                return 1;
            }

            this->map[i][j] = c;

            if (c == 'S') {
                start_count++;
                this->start.x = j;
                this->start.y = i;
            }
            else if (c == 'E') {
                end_count++;
                this->end.x = j;
                this->end.y = i;
            }
        }

        i++;
    }

    if (i != get_height(file)) {
        return 1;
    }

    if (start_count != 1 || start_count != 1) {
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
 void print_maze(Maze *this, Coord *player) {
    // make sure we have a leading newline..
    printf("\n");   
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i) {
                printf("X");
            }
            else {
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
 void move(Maze *this, Coord *player, char direction) {
    int current_x = player->x;
    int current_y = player->y;

    switch (direction) {
        case 'W': case 'w':
            current_y--;
            break;
        
        case 'A': case 'a':
            current_x--;
            break;

        case 'S': case 's':
            current_y++;
            break;

        case 'D': case 'd':
            current_x++;
            break;

        default:
            return;
    }

    if (current_x < 0 || current_x >= this->width || current_y < 0 || current_y >= this->height) {
        printf("The player cannot move off the edge of the map, try again!\n");
        return;
    }

    if (this->map[current_y][current_x] == '#') {
        printf("The player cannot move through walls, try again!\n");
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
 int has_won(Maze *this, Coord *player) {
    if (player->x == this->end.x && player->y == this->end.y) {
        return 0;
    }
    return 1;
 }

int main(int argc, char *argv[])
{
    // check args
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mazefile path>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }

    // open and validate mazefile
    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("File does not exist.\n");
        return EXIT_FILE_ERROR;
    }

    //
    Maze *this = malloc(sizeof(Maze));

    create_maze(this, 9, 15);
    // read in mazefile to struct
    read_maze(this, file);
    
    fclose(file);

    // maze game loop
    Coord *player;
    char input;

    player->x = this->start.x;
    player->y = this->start.y;

    while(has_won(this, player)) {
        scanf(" %c", &input);

        switch(input) {
            case 'W': case 'w':
            case 'A': case 'a':
            case 'S': case 's':
            case 'D': case 'd':
                move(this, player, input);
                break;

            case 'M': case 'm':
                print_maze(this, player);
                printf("\n");
                break;

            case 'Q': case 'q':
                return 1;

            default:
                printf("Invalid input, please try again.\n");
        }
    }

    // win
    printf("Congratulations, you have reached the end of the maze!\n");

    // return, free, exit
    free_maze(this);
    return EXIT_SUCCESS;
}