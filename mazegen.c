#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

typedef struct __Wall {
    int u;
    int v;
    int count;
} Wall;

Wall *create_wall(int height, int width);
void shuffle(Wall *wall, int n);
int find(int *parent, int i);
void uni(int *parent, int i, int j);
void kruskal(char **array, int *arr, int height, int width, int n);
void generate_maze(char **array, int *arr, int height, int width, int n);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s maze4.txt <width> <height>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen("maze4.txt", "w");

    if (file == NULL) {
        printf("Error: failed to open file\n");
        return 1;
    }

    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    if (height > MAX_DIM || height < MIN_DIM || width > MAX_DIM || width < MIN_DIM) {
        printf("Error: invalid dimensions\n");
        return 1;
    }

    int n = width * height;
    int set[n];

    char **array = (char**)malloc(sizeof(char*) * n);
    for(int i = 0; i < n; ++i) {
        array[i] = (char*)malloc(sizeof(char) * n);
    }

    kruskal(array, set, height, width, n);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%c", array[i][j]);
        }
        fprintf(file, "\n");
    }

    for (int i = 0; i < n; i++) {
        free(array[i]);
    }
    free(array);

    fclose(file);

    return 0;
}

Wall *create_wall(int height, int width) {
    int max = width * (height - 1) + height * (width - 1);
    Wall *wall = malloc(max * sizeof(Wall));
    int count = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j < width - 1) {
                wall[count].u = i;
                wall[count].v = j + 1;
                count++;
            }    
            if (i < height - 1) {
                wall[count].u = i + 1;
                wall[count].v = j;
                count++;
            }
        }
    }
    (*wall).count = count;

    return wall;
}

/**
* @brief arrange the n elements of array in random order
*
* @param wall
* @param n 
*/
void shuffle(void *wall, int n) {
    // This test is adapted from an example provided on: https://benpfaff.org/writings/clc/shuffle.html 
    srand(time(NULL));

    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        Wall t = wall[j];
        wall[j] = wall[i];
        wall[i] = t;
    }
}

/**
* @brief 
*
* @param parent
* @param i
*/
int find(int *parent, int i) {
    // This test is adapted from an example provided on: https://en.wikipedia.org/wiki/Disjoint-set_data_structure 
}

/**
* @brief 
*
* @param i
* @param j
*/
void uni(int *parent, int i, int j) {

}

/**
* @brief    
*
* @param this 
* @param arr
* @param n
*/
void kruskal(char **array, int *set, int height, int width, int n) {
    // create a list of all walls
    Wall *wall = create_wall(height, width);

    // create a set for each cell, each containing just that one cell
    for(int i = 0; i < n; i++) {
        set[i] = i;
    }

    for (int i = 0; i < (*wall).count; i++) {
        printf("%d %d\n", wall[i].u, wall[i].v);
    }
    // for each wall in a random order
    shuffle(wall, (*wall).count);

        // if the cells divided by this wall belong to distinct sets
            // remove the current wall
            // join the sets of the formally divided cells

    free(wall);
}

void generate_maze(char **array, int *set, int height, int width, int n) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            array[i][j] = '#';
        }
    }
}