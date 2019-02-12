#include <iostream>

#define N 3
#define M 3

inline uint_fast64_t factorial(int n) {
    uint_fast64_t c, fact = 1;
    for (c = 1; c <= n; c++) {
        fact = fact * c;
    }
    return fact;
}

void getInitBoard(uint_fast8_t board[N][M]) {
    for (uint_fast8_t i=0; i<M; i++) {
        for(uint_fast8_t j=0; j<N; j++) {
            board[i][j] = static_cast<uint_fast8_t>(j + i * N + 1);
        }
    }
}
uint_fast64_t hashBoard(uint_fast8_t board[N][M]) {
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
void unHashBoard(uint_fast64_t boardHash, uint_fast8_t board[N][M]) {
    uint_fast64_t currentHash = boardHash;
    for (uint_fast8_t i=0; i<M; i++) {
        for(uint_fast8_t j=0; j<N; j++) {
            auto current = static_cast<uint_fast8_t>(currentHash % (M * N));
            if (current) {
                board[i][j] = current;
                currentHash /= N*M;
            } else {
                board[i][j] = N * M;
                currentHash /= N*M;
                currentHash--;
            }
        }
    }
}

int main() {
    clock_t start = clock();

#if ((N*M)%2)
    uint_fast64_t numBoards = factorial(N * M);
#else
    uint_fast64_t numBoards = factorial(N * M)/2;
#endif

    auto * boards = static_cast<uint_fast64_t *>(malloc(numBoards * sizeof(uint_fast64_t)));
    uint_fast64_t lastIndex = 1;
    auto * currentBoards = static_cast<uint_fast64_t *>(malloc(1 * sizeof(uint_fast64_t)));
    uint_fast64_t numberCurrentBoards = 1;

    uint_fast8_t initBoard[N][M] = {{0}};
    getInitBoard(initBoard);
    boards[0] = hashBoard(initBoard);
    currentBoards[0] = boards[0];

    uint_fast8_t iteration = 0;
    bool found = true;

    uint_fast8_t board[N][M] = {{0}};
    uint_fast8_t tmpBoard[N][M] = {{0}};

    while (found) {
        iteration++;
        found = false;
        auto * nextBoards = static_cast<uint_fast64_t *>(malloc(numberCurrentBoards*(N*2+M*2) * sizeof(uint_fast64_t)));
        uint_fast64_t nextBoardsIndex = 0;
        for (uint_fast64_t i = 0; i<numberCurrentBoards; i++) {
            unHashBoard(currentBoards[i], board);

            //up
            for (uint_fast8_t j = 0; j<N; j++) {
                memcpy(tmpBoard, board, sizeof(tmpBoard));
                for (uint_fast8_t k = 1; k<M; k++) {
                    tmpBoard[j][k] = board[j][k-1];
                }
                tmpBoard[j][0] = board[j][M-1];

                bool inBoards = false;
                uint_fast64_t tmpBoardHash = hashBoard(tmpBoard);
                for (uint_fast64_t k = 0; k<lastIndex; k++) {
                    if (boards[k]==tmpBoardHash) {
                        inBoards = true;
                    }
                }
                if (!inBoards) {
                    found = true;
                    boards[lastIndex++] = tmpBoardHash;
                    nextBoards[nextBoardsIndex++] = tmpBoardHash;
                }
            }
            //down
            for (uint_fast8_t j = 0; j<N; j++) {
                memcpy(tmpBoard, board, sizeof(tmpBoard));
                for (uint_fast8_t k = 0; k<M-1; k++) {
                    tmpBoard[j][k] = board[j][k+1];
                }
                tmpBoard[j][M-1] = board[j][0];

                bool inBoards = false;
                uint_fast64_t tmpBoardHash = hashBoard(tmpBoard);
                for (uint_fast64_t k = 0; k<lastIndex; k++) {
                    if (boards[k]==tmpBoardHash) {
                        inBoards = true;
                    }
                }
                if (!inBoards) {
                    found = true;
                    boards[lastIndex++] = tmpBoardHash;
                    nextBoards[nextBoardsIndex++] = tmpBoardHash;
                }
            }
            //left
            for (uint_fast8_t j = 0; j<M; j++) {
                memcpy(tmpBoard, board, sizeof(tmpBoard));
                for (uint_fast8_t k = 0; k<N-1; k++) {
                    tmpBoard[k][j] = board[k+1][j];
                }
                tmpBoard[N-1][j] = board[0][j];

                bool inBoards = false;
                uint_fast64_t tmpBoardHash = hashBoard(tmpBoard);
                for (uint_fast64_t k = 0; k<lastIndex; k++) {
                    if (boards[k]==tmpBoardHash) {
                        inBoards = true;
                    }
                }
                if (!inBoards) {
                    found = true;
                    boards[lastIndex++] = tmpBoardHash;
                    nextBoards[nextBoardsIndex++] = tmpBoardHash;
                }
            }
            //right
            for (uint_fast8_t j = 0; j<M; j++) {
                memcpy(tmpBoard, board, sizeof(tmpBoard));
                for (uint_fast8_t k = 1; k<N; k++) {
                    tmpBoard[k][j] = board[k-1][j];
                }
                tmpBoard[0][j] = board[N-1][j];

                bool inBoards = false;
                uint_fast64_t tmpBoardHash = hashBoard(tmpBoard);
                for (uint_fast64_t k = 0; k<lastIndex; k++) {
                    if (boards[k]==tmpBoardHash) {
                        inBoards = true;
                    }
                }
                if (!inBoards) {
                    found = true;
                    boards[lastIndex++] = tmpBoardHash;
                    nextBoards[nextBoardsIndex++] = tmpBoardHash;
                }
            }
        }
        free(currentBoards);
        currentBoards = nextBoards;
        numberCurrentBoards = nextBoardsIndex;
        for (uint_fast64_t i = 0; i<numberCurrentBoards; i++) {
            printf("(");
            unHashBoard(currentBoards[i], board);
            for (auto &k : board) {
                printf("(");
                for (uint_fast8_t j : k) {
                    printf("%d, ", j);
                }
                printf(")");
            }
            printf(")\n");
        }
        printf("Iteration: %i\n\n",iteration);
    }

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
    return 0;
}