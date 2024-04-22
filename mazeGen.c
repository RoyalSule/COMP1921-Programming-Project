#include <studio.h>
#include <stdlib.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5


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
    // This test is adapted from an example provided on: https://www.studysmarter.co.uk/explanations/computer-science/computer-programming/2d-array-in-c/
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
    // This test is adapted from an example provided on: https://www.studysmarter.co.uk/explanations/computer-science/computer-programming/2d-array-in-c/
    for (int i = 0; i < this->height; ++i) {
        free(this->map[i]);
    }
    free(this->map);
}

/**
* @brief    
*
* @param this 
*/
void kruskal(Maze *this) {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {

        }
    }
}

void make_set() {

}

void find() {

}

void uni() {

}

void generate_maze() {

}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s maze4.txt <width> <height>", argv[0]);
        return 1;
    }

    FILE *file = fopen("maze4.txt", "w");

    if (file == NULL) {
        printf("Error: failed to open file\n");
        return 1;
    }

    Maze *this = malloc(sizeof(Maze));

    if (this == NULL) {
        fprintf(stderr, "Error: memory allocation failed");
        return 1;
    }

    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    if (create_maze(this, height, width) != 0) {
        printf("Error: failed to create maze\n");
        fclose(file);
        free_maze(this);
        return 1;
    }

    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            fprintf(file, "%c", this->map[i][j]);
        }
        fprinf(file, "\n");
    }

    free_maze(this);
    fclose(file);

    return 0;
}
