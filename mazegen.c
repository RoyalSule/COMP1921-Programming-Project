#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

typedef struct __Cell {
    int i, j;
} Cell;

typedef struct __Wall {
    Cell u, v;
} Wall;

typedef struct _DisjointSet {
    int parent;
} DisjointSet;

/**
* @brief Creates the maximum number of walls possible in an array presenting the maze
*
* @param height Height of the maze
* @param width Width of the maze
* @param total Total number of walls 
* 
* @return Wall struct
*/
Wall *create_wall(int height, int width, int *total) {
    int n = height * (width - 1) + width * (height - 1);

    Wall *wall = malloc(sizeof(Wall) * n);
    int count = 0;

    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j < width - 1) { 
                Cell u = {i, j};
                Cell v = {i, j + 1};
                wall[count] = (Wall){u, v};
                count++;
            }
            if (i < height - 1) {
                Cell u = {i, j};
                Cell v = {i + 1, j};
                wall[count] = (Wall){u, v};
                count++;
            }
        }
    }
    *total = count;

    return wall;
}

/**
* @brief Shuffles the elements of the Wall struct
*
* @param wall Wall struct to be used
* @param n Number of elements in wall
*/
void shuffle(Wall *wall, int n) {
    // This test is adapated from an example provided on: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle 
    srand(time(NULL));

    for (int i = n - 1; i > 0; i--) {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        Wall t = wall[j];
        wall[j] = wall[i];
        wall[i] = t;
    }
}

/**
* @brief Follows the chain of parent pointers from a specified query node x until it reaches a root element
*
* @param set DisjointSet strcut to be used
* @param x Given element
* 
* @return Root element it reaches
*/
int find(DisjointSet *set, int x) {
    // This test is adapted from an example provided on: https://en.wikipedia.org/wiki/Disjoint-set_data_structure 
    if (set[x].parent != x) {
        set[x].parent = find(set, set[x].parent);
        return set[x].parent;
    }
    else {
        return x;
    }
}

/**
* @brief Replaces the set containing x and the set containing y with their union
*
* @param set DisjointSet struct to be used
* @param x Given element
* @param y Given element
*/
void uni(DisjointSet *set, int x, int y) {
    // This test is adapted from an example provided on: https://en.wikipedia.org/wiki/Disjoint-set_data_structure 
    int root_x = find(set, x);
    int root_y = find(set, y);

    set[root_y].parent = root_x;
}

/**
* @brief Generate a maze using Kruskal's algorithm - produces regular patterns which are fairly easy to solve
*
* @param arr Array reprenseting the maze
* @param height Height of the maze
* @param width Width of the maze
*/
void kruskal(char **arr, int height, int width) {
    // This test is adapted from an example provided on: https://en.wikipedia.org/wiki/Maze_generation_algorithm
    // Create a list of all the walls
    int total;
    Wall *wall = create_wall(height, width, &total);

    // Create a set for each wall, each containing just that one cell
    DisjointSet *set = malloc(sizeof(DisjointSet) * total);

    for (int i = 0; i < total; i++) {
        set[i].parent = i;
    }

    // For each wall in some random order
    shuffle(wall, total);

    for(int i = 0; i < total; i++) {
        int u = wall[i].u.i * width + wall[i].u.j; 
        int v = wall[i].v.i * width + wall[i].v.j; 

        // If the cells divided by this wall belong to distinct sets
        if (find(set, u) != find(set, v)) {
            // Remove the current wall
            if (wall[i].u.i == wall[i].v.i) {
                arr[wall[i].u.i][wall[i].u.j + 1] = ' '; 
            }
            else {
                arr[wall[i].u.i + 1][wall[i].u.j] = ' '; 
            }

            // Join the sets of the formally divided cells
            uni(set, u, v);
        }
    }

    free(wall);
    free(set);
}

/**
* @brief Initialise an array and call kruskal to generate a maze - place the start and exit point in the maze
*
* @param arr Array representing the maze
* @param height Height of the maze
* @param width Width of the maze
*/
void generate_maze(char **arr, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            arr[i][j] = '#';
        }
    }

    kruskal(arr, height, width);

    arr[0][0] = 'S';
    arr[height - 1][width - 1] = 'E';
}

int main(int argc, char *argv[]) {
    // Check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <mazefile path> <width> <height>\n", argv[0]);
        return 1;
    }  

    // Open mazefile
    FILE *file = fopen(argv[1], "w");

    // Validate height and width of the mazefile
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    if (height > MAX_DIM || height < MIN_DIM || width > MAX_DIM || width < MIN_DIM) {
        printf("Error: invalid dimensions\n");
        return 1;
    }

    // Initialise array 
    char **arr = (char**)malloc(sizeof(char*) * height);
    for(int i = 0; i < height; ++i) {
        arr[i] = (char*)malloc(sizeof(char) * width);
    }

    // Generate maze
    generate_maze(arr, height, width);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%c", arr[i][j]);
        }
        fprintf(file, "\n");
    }

    // Free array, close file and exit
    for (int i = 0; i < height; i++) {
        free(arr[i]);
    }
    free(arr);

    fclose(file);

    return 0;
}