#include <iostream>

#define N 2
#define M 2

uint_fast8_t** getInitBoard() {
    auto * values = static_cast<uint_fast8_t *>(calloc(M * N, sizeof(uint_fast8_t)));
    auto ** rows = static_cast<uint_fast8_t **>(malloc(M * sizeof(uint_fast8_t*)));
    for (uint_fast8_t i=0; i<M; ++i) {
        rows[i] = values + i*N;
    }
    for (uint_fast8_t i=0; i<M; i++) {
        for(uint_fast8_t j=0; j<N; j++) {
            rows[i][j] = static_cast<uint_fast8_t>(j + i * N + 1);
        }
    }
    return rows;
}
uint_fast64_t hashBoard(uint_fast8_t** board) {
    uint_fast64_t boardHash = 0;
    for(uint_fast8_t i = 0; i < M; i++) {
        for(uint_fast8_t j = 0; j < N; j++) {
            uint_fast64_t result = 1;
            for (uint_fast8_t k = 1; k <= (i*N+j); k++) {
                result *= N*M;
            }
            boardHash += board[i][j]*result;
        }
    }
    return boardHash;
}
uint_fast8_t** unHashBoard(uint_fast64_t boardHash) {
    uint_fast64_t currentHash = boardHash;
    auto * values = static_cast<uint_fast8_t *>(calloc(M * N, sizeof(uint_fast8_t)));
    auto ** rows = static_cast<uint_fast8_t **>(malloc(M * sizeof(uint_fast8_t*)));
    for (uint_fast8_t i=0; i<M; ++i) {
        rows[i] = values + i*N;
    }
    for (uint_fast8_t i=0; i<M; i++) {
        for(uint_fast8_t j=0; j<N; j++) {
            auto current = static_cast<uint_fast8_t>(currentHash % (M * N));
            if (current) {
                rows[i][j] = current;
            } else {
                rows[i][j] = N*M;
            }
            currentHash /= N*M;
        }
    }
    return rows;
}
/*inline uint_fast64_t factorial(int n) {
    uint_fast64_t c, fact = 1;
    uin
}*/

int main() {
    uint_fast8_t** boardTest = getInitBoard();
    for(uint_fast8_t i = 0; i < M; i++) {
        for(uint_fast8_t j = 0; j < N; j++) {
            printf("%d ", boardTest[i][j]);
        }
        printf("\n");
    }
    printf("%llu\n", hashBoard(boardTest));
    uint_fast8_t** boardTest2 = unHashBoard(hashBoard(boardTest));
    for(uint_fast8_t i = 0; i < M; i++) {
        for(uint_fast8_t j = 0; j < N; j++) {
            printf("%d ", boardTest2[i][j]);
        }
        printf("\n");
    }

    uint_fast64_t numBoards = 0;

    auto * boards = static_cast<uint_fast64_t *>(malloc(M*N * sizeof(uint_fast64_t)));

    return 0;
}