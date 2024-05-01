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

// Defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// Defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

// Define for buffer size
#define BUFFER_SIZE 1024

typedef struct __Coord {
    int x, y;
} Coord;

typedef struct __Maze {
    char **map;
    int height, width;
    Coord start, exit;
} Maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this Pointer to the maze to be initialised
 * @param height Height to allocate
 * @param width Width to allocate
 * @return int 0 on success, 1 on fail
 */
int create_maze(Maze *this, int height, int width) { 
    // This test is adapted from an example provided on: https://www.studysmarter.co.uk/explanations/computer-science/computer-programming/2d-array-in-c/
    if (this == NULL) {
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
 * @param this Pointer to the struct to free
 */
void free_maze(Maze *this) { 
    // This test is adapted from an example provided on: https://www.studysmarter.co.uk/explanations/computer-science/computer-programming/2d-array-in-c/
    for (int i = 0; i < this->height; ++i) {
        free(this->map[i]);
    }
    free(this->map);
}

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file File pointer to check
 * @return int 0 for error, or a valid width (5-100)
 */
int get_width(FILE *file) {
     // This test is adapted from an example provided on: https://www.quora.com/How-do-I-read-the-1st-line-of-a-file-in-C-using-File-Handling
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
        return 0;
    }

    return width;
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file File pointer to check
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
        return 0;
    }

    return height;
}

/**
 * @brief Read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(Maze *this, FILE *file) {
    int i = 0;
    char line[BUFFER_SIZE];
    int start_count = 0, exit_count = 0;
    
    fseek(file, 0, SEEK_SET);

    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        int length = 0;
        line[strcspn(line, "\n")] = 0;

        for (int j = 0; line[j] != '\0'; j++) {
            char c = line[j];

            if (i >= this->height) {
                return 1;
            }

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
                exit_count++;
                this->exit.x = j;
                this->exit.y = i;
            }

            length++;
        }

        if (length != this->width) {
            return 1;
        }

        i++;
    }

    if (i != this->height) {
        return 1;
    }

    if (start_count != 1 || exit_count != 1) {
        return 1;
    }
    
    return 0;
}

/**
* @brief Prints the maze out - code provided to ensure correct formatting
*
* @param this Pointer to the maze to print
* @param player Player's current position
*/
void print_maze(Maze *this, Coord *player) {
    printf("\n");   
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (player->x == j && player->y == i) {
                printf("X");
            }
            else {
                printf("%c", this->map[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player Player's current position
 * @param direction Direction to move
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
        printf("Can't move off the edge of the map!\n");
        return;
    }

    if (this->map[current_y][current_x] == '#') {
        printf("Can't move through walls!\n");
        return;
    } 

    player->x = current_x;
    player->y = current_y;
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this Current maze
 * @param player Player's position
 * @return int 0 for false, 1 for true
 */
int has_won(Maze *this, Coord *player) {
    return (player->x == this->exit.x && player->y == this->exit.y);
}

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mazefile path>\n", argv[0]);
        return EXIT_ARG_ERROR;
    }

    // Open and validate mazefile
    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("FileError: file does not exist\n");
        return EXIT_FILE_ERROR;
    }

    // Validate height and width of the mazefile
    int width = get_width(file);

    if (width == 0) {
        printf("MazeError: invalid width\n");
        return EXIT_MAZE_ERROR;
    }

    int height = get_height(file);

    if (height == 0) {
        printf("MazeError: invalid height\n");
        return EXIT_MAZE_ERROR;
    }

    // Read in mazefile to struct
    Maze *this = malloc(sizeof(Maze));

    if (create_maze(this, height, width) != 0) {
        printf("MazeError: memory allocation failed\n");
        return EXIT_MAZE_ERROR;
    }

    if (read_maze(this, file) != 0) {
        printf("MazeError: faile to read file\n");
        return EXIT_FILE_ERROR;
    }
    
    fclose(file);

    // Maze game loop
    Coord *player = malloc(sizeof(Coord));

    if (player == NULL) {
        printf("MazeError: memory allocation failed\n");
        return EXIT_MAZE_ERROR;
    }

    player->x = this->start.x;
    player->y = this->start.y;

    char input;
    printf("Enter WASD to move through the maze, M to view an image of the map.\n");

    while (!has_won(this, player)) {
        scanf(" %c", &input);
        move(this, player, input);

        if (input == 'm' || input == 'M') {
            print_maze(this, player);
        }
    }

    printf("Congratulations, you have reached the end of the maze!\n");

    // Free maze and exit
    free_maze(this);
    return EXIT_SUCCESS;
}
