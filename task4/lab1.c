#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("%s", "Specify only the file name, the name of the input and output files as arguments");
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if (in == NULL){
        printf("%s", "The file cannot be opened");
        fclose(in);
        return 1;
    }
    int n;
    fscanf(in, "%i", &n);
    float *solutions = malloc(n * sizeof(float));
    float *matrix = malloc((n * (n + 1)) * sizeof(float));
    if ((matrix == NULL) || (solutions == NULL)) {
        printf("%s", "memory allocation error");
        fclose(in);
        free(matrix);
        free(solutions);
        return 2;
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n + 1; j++){
            fscanf(in, "%g", &matrix[i * (n + 1) + j]);
        }
    }
    fclose(in);
    FILE *out = fopen(argv[2], "w");
    int ind; float c;
    for (int i = 0; i < n - 1; i++){
        ind = -1;
        if (matrix[i * (n + 1) + i] == 0){
            ind = 0;
            for (int k = i + 1; k < n; k++){
                if ((matrix[k * (n + 1) + i] != 0) && (matrix[i * (n + 1) + i] == 0)){
                    for (int y = 0; y < n + 1; y++) {
                        c = matrix[k * (n + 1) + y];
                        matrix[k * (n + 1) + y] = matrix[i * (n + 1) + y];
                        matrix[i * (n + 1) + y] = c;
                    }
                    break;
                    ind = 1;
                }
            }
        }
        if (ind == 0) {
            fprintf(out, "%s", "no solution or many solution");
            free(matrix);
            free(solutions);
            fclose(out);
            return 0;
        }
        if (matrix[i * (n + 1) + i] != 0) {
            for (int j = i + 1; j < n; j++){
                float x = (-1 * matrix[j * (n + 1) + i]) / matrix[i * (n + 1) + i];
                for (int k = i; k < n + 1; k++){
                    matrix[j * (n + 1) + k] += matrix[i * (n + 1) + k] * x;
                }
            }
        }
    }
    int res = 0;
    for (int i = n - 1; i >= 0; i--) {
        ind = 0;
        for (int j = 0; j < n + 1; j++) {
            if ((j != n) && (matrix[i * (n + 1) + j] != 0)) {
                ind = -1;
            } else if ((ind != -1) && (j == n) && (matrix[i * (n + 1) + j] != 0)) {
                fprintf(out, "%s", "no solution"); 
                free(matrix);
                free(solutions);
                fclose(out);
                return 0;
            } else if ((ind != -1) && (j == n) && (matrix[i * (n + 1) + j] == 0)) {
                res = 1;
            }
        }
    }
    if (res == 1) {
        fprintf(out, "%s", "many solutions"); 
        free(matrix);
        free(solutions);
        fclose(out);
        return 0;
    } else {
        for (int i = n - 1; i >= 0; i--) {
            for (int j = n - 1; j > i; j--) {
                matrix[i * (n + 1) + n] -= matrix[i * (n + 1) + j];
            }
            solutions[i] = matrix[i * (n + 1) + n] / matrix[i * (n + 1) + i];
            for (int j = i; j >= 0; j--) {
                matrix[j * (n + 1) + i] *= solutions[i];
            }
        }
        for (int i = 0; i < n; i++) {
            fprintf(out, "%g\n", solutions[i]); 
        }
        free(matrix);
        free(solutions);
        fclose(out);
        return 0;
    }
}
