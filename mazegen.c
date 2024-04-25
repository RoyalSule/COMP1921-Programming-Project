#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// This test is adapted from an example provide on: https://stackoverflow.com/questions/3437404/min-and-max-in-c
#define min(x, y) ((x) < (y) ? (x) : (y))

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
* @brief 
*
* @param height
* @param width
* @param wall_count
* 
* @return 
*/
Wall *create_wall(int height, int width, int *wall_count) {
    int max = height * (width - 1) + width * (height - 1);

    Wall *wall = malloc(sizeof(Wall) * max);
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
    *wall_count = count;

    return wall;
}

/**
* @brief 
*
* @param wall
* @param n
* 
* @return 
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
* @brief 
*
* @param set
* @param x
* 
* @return 
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
* @brief 
*
* @param set
* @param x
* @param y
* 
* @return 
*/
void uni(DisjointSet *set, int x, int y) {
    // This test is adapted from an example provided on: https://en.wikipedia.org/wiki/Disjoint-set_data_structure 
    int root_x = find(set, x);
    int root_y = find(set, y);

    set[root_y].parent = root_x;
}

/**
* @brief 
*
* @param arr
* @param height
* @param width
* 
* @return 
*/
void kruskal(char **arr, int height, int width) {
    // This test is adapted from an example provided on: https://en.wikipedia.org/wiki/Maze_generation_algorithm
    // Create a list of all the walls
    int wall_count;
    Wall *wall = create_wall(height, width, &wall_count);

    // Create a set for each wall, each containing just that one cell
    int n = height * width;
    DisjointSet *set = malloc(sizeof(DisjointSet) * n);

    for (int i = 0; i < n; i++) {
        set[i].parent = i;
    }

    // For each wall in some random order
    shuffle(wall, wall_count);

    for(int i = 0; i < wall_count; i++) {
        int u = wall[i].u.i * width + wall[i].u.j;
        int v = wall[i].v.i * width + wall[i].v.j;

        // If the cells divided by this wall belong to distinct sets
        if (find(set, u) != find(set, v)) {
            // Remove the current wall
            if (wall[i].u.i == wall[i].v.i) {
                arr[wall[i].u.i][min(wall[i].u.j, wall[i].v.j) + 1] = ' ';
            }
            else {
                arr[min(wall[i].u.i, wall[i].v.i) + 1][wall[i].u.j] = ' ';
            }

            // Join the sets of the formally divided cells
            uni(set, u, v);
        }
    }

    free(wall);
    free(set);
}

/**
* @brief 
*
* @param arr
* @param height
* @param width
* 
* @return 
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
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <mazefile path> <width> <height>\n", argv[0]);
        return 1;
    }  

    // Lines 134 - 138 were adapted from an example provided on: https://stackoverflow.com/questions/9840629/create-a-file-if-one-doesnt-exist-c
    FILE *file = fopen(argv[1], "w");

    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    if (height > MAX_DIM || height < MIN_DIM || width > MAX_DIM || width < MIN_DIM) {
        printf("Error: invalid dimensions\n");
        return 1;
    }

    char **arr = (char**)malloc(sizeof(char*) * height);
    for(int i = 0; i < height; ++i) {
        arr[i] = (char*)malloc(sizeof(char) * width);
    }

    generate_maze(arr, height, width);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%c", arr[i][j]);
        }
        fprintf(file, "\n");
    }

    for (int i = 0; i < height; i++) {
        free(arr[i]);
    }
    free(arr);

    fclose(file);

    return 0;
}