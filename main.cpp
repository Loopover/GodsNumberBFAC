#include <iostream>

#define N 2
#define M 2

int** getInitBoard()
{
    int* values = static_cast<int *>(calloc(M * N, sizeof(int)));
    int** rows = static_cast<int **>(malloc(M * sizeof(int*)));
    for (int i=0; i<M; ++i) {
        rows[i] = values + i*N;
    }
    for (int i=0; i<M; i++) {
        for(int j=0; j<N; j++) {
            rows[i][j] = j + i*N + 1;
        }
    }
    return rows;
}
long hashBoard(int** board) {
    long boardHash = 0;
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            long result = 1;
            for (int k = 1; k <= (i*N+j); k++) {
                result *= N*M;
            }
            boardHash += board[i][j]*result;
        }
    }
    return boardHash;
}

int** unHashBoard(long boardHash) {
    int* values = static_cast<int *>(calloc(M * N, sizeof(int)));
    int** rows = static_cast<int **>(malloc(M * sizeof(int*)));
    for (int i=0; i<M; ++i) {
        rows[i] = values + i*N;
    }
    for (int i=0; i<M; i++) {
        for(int j=0; j<N; j++) {
            rows[i][j] = 0;
        }
    }
    return rows;
}

int main() {

    int** boardTest = getInitBoard();

    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            printf("%d ", boardTest[i][j]);
        }
        printf("\n");
    }

    printf("%li\n", hashBoard(boardTest));

    int** boardTest2;

    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            printf("%d ", boardTest2[i][j]);
        }
        printf("\n");
    }

    return 0;
}