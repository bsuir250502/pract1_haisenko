#include <stdio.h>
#include <stdlib.h>

typedef struct coord_t {
    int x;
    int y;
} coord_t;

int fInputNumericalData(FILE *lifeInput);
int **fInputField(FILE *lifeInput, coord_t size);
void generate(int **field, coord_t size, int genNum);
int checkCell(int **field, coord_t size, int i, int j);
void fOutputField(FILE *lifeOutput, int **field, coord_t size);
void freeField(int **field, coord_t size);

int main(int argc, char **argv)
{
    int **field, genNum;
    coord_t size;
    FILE *lifeInput, *lifeOutput;

    lifeInput = fopen("life.in", "r");
    lifeOutput = fopen("life.out", "w");
    if (!lifeInput || !lifeOutput) {
        printf("There is no required files in this folder.\n");
        return 1;
    }
    size.x = fInputNumericalData(lifeInput);
    size.y = fInputNumericalData(lifeInput);
    genNum = fInputNumericalData(lifeInput);
    field = fInputField(lifeInput, size);
  
    generate(field, size, genNum);
    fOutputField(lifeOutput, field, size);
    freeField(field, size);
    fclose(lifeOutput);
    fclose(lifeInput);
    return 0;
}

int fInputNumericalData(FILE *lifeInput)
{
    int number;
    char wrongSymbol;
    if (!fscanf(lifeInput, "%d", &number)) {
        wrongSymbol = getc(lifeInput);
        printf("'%c' is not a number.\n", wrongSymbol);
        exit(1);
    }
    return number;
}

int **fInputField(FILE *lifeInput, coord_t size)
{
    int **field, i, j;
    char temp = 0;
    field = (int **) calloc(size.x, sizeof(int *));
    for (i = 0; i < size.x; i++) {
        field[i] = (int *) calloc(size.y, sizeof(int));
    }
    if (getc(lifeInput) != '\n') {
        printf("Fill the field according to the rules.\n");
        exit(1);
    }
    for (i = 0; i < size.x; i++) {
        for (j = 0, temp = 0; j <= size.y && temp != '\n'; j++) {
            temp = getc(lifeInput);
            if (j == size.y) {
                while (temp != '\n' && temp != EOF) {
                    temp = getc(lifeInput);
                }
            }
            switch (temp) {
            case '0':
                break;
            case '1':
                field[i][j] = 1;
                break;
            case '\n':
                break;
            case EOF:
                return field;
            default:
                printf("'%c' is wrong cell.\n", temp);
                exit(0);
            }
        }
    }
    return field;
}

void generate(int **field, coord_t size, int genNum)
{
    register int i, j;
    int currentGen, bufferField[size.x][size.y];
    for (currentGen = 0; currentGen < genNum; currentGen++) {
        for (i = 0; i < size.x; i++) {
            for (j = 0; j < size.y; j++) {
                bufferField[i][j] = checkCell(field, size, i, j);
            }
        }
        for (i = 0; i < size.x; i++) {
            for (j = 0; j < size.y; j++) {
                field[i][j] = bufferField[i][j];
            }
        }
    }
}

int checkCell(int **field, coord_t size, int i, int j)
{
    int up, down, right, left, sum;
    (j == 0) ? (up = size.y - 1) : (up = j - 1);
    (i == 0) ? (left = size.x - 1) : (left = i - 1);
    (j == size.y - 1) ? (down = 0) : (down = j + 1);
    (i == size.x - 1) ? (right = 0) : (right = i + 1);
    sum = field[i][up] + field[i][down] + field[left][j] + field[right][j]
        + field[left][up] + field[right][up] + field[left][down] + field[right][down];
    if (sum == 3) {
        return 1;
    } else if (field[i][j] && sum == 2) {
        return 1;
    }
    return 0;
}

void fOutputField(FILE *lifeOutput, int **field, coord_t size)
{
    int i, j;
    for (i = 0; i < size.x; i++) {
        for (j = 0; j < size.y; j++) {
            fprintf(lifeOutput, "%d", field[i][j]);
        }
        fprintf(lifeOutput, "\n"); 
    }
}

void freeField(int **field, coord_t size)
{
    int i;
    for (i = 0; i < size.x; i++) {
        free(field[i]);
    }
    free(field);
}
