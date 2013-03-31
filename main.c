#include <stdio.h>
#include <stdlib.h>

struct coord_t {
    int x;
    int y;
};

struct coord_t inputSize(FILE *liveIn);
int **inputField(FILE *liveIn, struct coord_t size);
void generate(int **field, int genNum, struct coord_t size);
int check(int **field, struct coord_t size, int i, int j);

int main(int argc, char **argv)
{
    int **field, genNum = 1000;
    struct coord_t size;
    FILE *liveIn;
    liveIn = fopen("life.in", "r");
    size = inputSize(liveIn);
    field = inputField(liveIn, size);
    generate(field, genNum, size);
    return 0;
}

struct coord_t inputSize(FILE *liveIn)
{
    struct coord_t size;
    fscanf(liveIn, "%d", &size.x);
    fscanf(liveIn, "%d", &size.y);
    return size;
}

int **inputField(FILE *liveIn, struct coord_t size)
{
    int **field, i, j;
    field = (int **) calloc(size.x, sizeof(int *));
    for (i = 0; i < size.x; i++) {
        field[i] = (int *) calloc(size.y, sizeof(int));
    }
    for (i = 0; i < size.x; i++) {
        for (j = 0; j < size.y; j++) {
            if (!fscanf(liveIn, "%d", field[i] + j)) {
                printf("error at %d, %d\n", i, j);
                exit(1);
            }
        }
    }
    return field;
}

void generate(int **field, int genNum, struct coord_t size)
{
    int **nextField, i, j, gen;
    nextField = (int **) calloc(size.x, sizeof(int *));
    for (i = 0; i < size.x; i++) {
        nextField[i] = (int *) calloc(size.y, sizeof(int));
    }
    for (gen = 0; gen < genNum; gen++) {
        usleep(50000);
        system("clear");
        printf("=== %d ===\n", gen + 1);
        for (i = 0; i < size.x; i++) {
            for (j = 0; j < size.y; j++) {
                nextField[i][j] = check(field, size, i, j);
                if (nextField[i][j]) printf(" *");
                else printf("  ");
            }
            printf("|\n");
        }
        if (++gen > genNum) {
            break;
        }
        usleep(50000);
        system("clear");
        printf("=== %d ===\n", gen + 1);
        for (i = 0; i < size.x; i++) {
            for (j = 0; j < size.y; j++) {
                field[i][j] = check(nextField, size, i, j);
                if (field[i][j]) printf(" *");
                else printf("  ");
            }
            printf("|\n");
        }
    }
}

int check(int **field, struct coord_t size, int i, int j)
{
    int up, down, right, left, sum;
    (j == 0) ? (up = size.y - 1) : (up = j - 1);
    (i == 0) ? (left = size.x - 1) : (left = i - 1);
    (j == size.y - 1) ? (down = 0) : (down = j + 1); 
    (i == size.x - 1) ? (right = 0) : (right = i + 1);
    sum = field[i][up] + field[i][down] + field[left][j] + field[right][j] + field[left][up] + field[right][up] + field[left][down] + field[right][down];
    if (field[i][j]) {
        if (sum == 2 || sum == 3) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (sum == 3) {
            return 1;
        } else {
            return 0;
        }
    }
}