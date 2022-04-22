void printOutput (int** matrix, int R, int C) 
{
    printf("[");
    for (int row =0; row < R; row++){
        printf("[");
        for (int col =0; col < C; col++){
            if (col == (C-1)) {
                printf("%d", matrix[row][col]);
            } else {
                printf("%d,", matrix[row][col]);
            }
        }
        if (row == (R-1)) {
            printf("]");    
        } else {
            printf("],");    
        }
        
    }
    printf("]");
}

void solve (int** matrix, int R, int C) {

    int RowMarker[R];
    int ColMarker[C];
    
    for (int row = 0; row < R; row++) {
        RowMarker[row] = 1;
    }
    
    for (int col = 0; col < C; col++) {
        ColMarker[col] = 1;
    }
    
    for (int row = 0; row < R; row++) {
        for (int col = 0; col < C; col++) {
            if (matrix[row][col] == 0) {
                RowMarker[row] = 0;
                ColMarker[col] = 0;
            }
        }
    }

    for (int row = 0; row < R; row++) {
        for (int col = 0; col < C; col++) {
            if ((RowMarker[row] == 0) || (ColMarker[col] == 0)) {
                matrix[row][col] = 0;               
            }
        }
    }
}

void setZeroes(int** matrix, int matrixSize, int* matrixColSize){

    solve(matrix, matrixSize, *matrixColSize);
    printOutput(matrix, matrixSize, *matrixColSize);
}
