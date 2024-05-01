#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_SIZE 20

struct boardInfo {
    bool isHit;
    unsigned int boatType;
};

int assembleBoard(); // ask the player the size of the board
void initBoard(struct boardInfo (*board)[MAX_SIZE], unsigned int size); // initializes the board
void placeBoatPlayer(struct boardInfo board[][MAX_SIZE], unsigned int size); // Ask the player where he will put the boards
void placeBoatNPC(struct boardInfo board[][MAX_SIZE], unsigned int size); // NPC puts boards
void playGame(unsigned int size); // Starts the game
void showRules(); // Show the rules
void showBoard(struct boardInfo board[][MAX_SIZE], unsigned int size); // Show the board

int main(void) {
    int choice;

    do {
        printf("\n----------- Menu -----------\n");
        printf("1. Jogar\n");
        printf("2. Ver regras\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int boardSize = assembleBoard();
                playGame(boardSize);
                break;
            }
            case 2:
                showRules();
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Por favor, escolha novamente.\n");
        }
    } while (choice != 3);

    return 0;
}

void showRules() {
    printf("\n1) Objetivo do jogo: O objetivo do jogo é destruir todos os navios do oponente antes que ele destrua todos os seus navios. \n");
    printf("2) Preparação do jogo: Cada jogador recebe um tabuleiro, normalmente de tamanho 10x10, onde eles posicionam seus navios.");
    printf("Os navios não podem se sobrepor nem ficar fora dos limites do tabuleiro. Os jogadores também precisam decidir quem começará primeiro.\n");
    printf("3) Tipos de navios: Os jogadores têm vários tipos de navios para posicionar, cada um com um tamanho diferente. Os tamanhos padrão dos navios podem variar, mas geralmente incluem: \n");
    printf(" - Porta Aviões (5 casas) \n");
    printf(" - Encouraçado (4 casas) \n");
    printf(" - Contratorpedeiro (3 casas) \n");
    printf(" - Submarino (2 casas) \n");
    printf("4) Jogabilidade: Os jogadores alternam suas jogadas. Cada jogador escolhe uma coordenada no tabuleiro do oponente para atacar.");
    printf("O jogador atacante diz as coordenadas, e o oponente informa se o ataque atingiu um navio ou não. Se um navio for atingido, o atacante marca a coordenada como atingida em seu próprio tabuleiro.\n");
    printf("5) Resultados de Ataque: Se não acertar nada marcará como água e perderá a vez, caso acerte um navio ele será marcado como acertado e você poderá disparar mais vezes até errar \n");
    printf("6) Vitória: O jogo continua até que todos os navios de um jogador sejam afundados. O jogador que afundar todos os navios do oponente primeiro vence o jogo.\n");
}

int assembleBoard() {
    int boardSize;

    while (true) {
        printf("Deseja jogar o tabuleiro em qual tamanho? \n 1) 10x10 \n 2) 15x15 \n 3) 20x20 \n");
        scanf("%d", &boardSize);
        switch (boardSize) {
            case 1:
                return 10;
            case 2:
                return 15;
            case 3:
                return 20;
            default:
                printf("\n Escolha um tamanho válido\n");
        }
    }
}

void initBoard(struct boardInfo (*board)[MAX_SIZE], unsigned int size) {
    for (unsigned int c = 0; c < size; c++) {
        for (unsigned int j = 0; j < size; j++) {
            board[c][j].isHit = false;
            board[c][j].boatType = 0;
        }
    }
}

void placeBoatPlayer(struct boardInfo board[][MAX_SIZE], unsigned int size) {
    unsigned int row, col, direcao = 0;
    unsigned int num_boats = 4;
    int ship_sizes[4] = {2, 3, 4, 5};

    printf("\n---- Começo do posicionamento de seus barcos ----");
    printf("\nDigite a linha, coluna e direção(0 para horizontal 1 para vertical) de seu barco \n");

    for (int c = 0; c < num_boats; c++) {
        unsigned int ship_size = ship_sizes[c];
        printf("\nBarco %d que ocupa %d de espaço\n", c + 1, c + 2);
        scanf("%d %d %d", &row, &col, &direcao);

        // Validate coordinates and ship placement
        if (row < 0 || row >= size || col < 0 || col >= size || direcao > 1) {
            printf("\nCoordenadas inválidas! Tente novamente.\n");
            c--; // Decrement counter to allow player to retry
            continue;
        }

        if (!direcao && col + ship_size > size) {
            printf("\nO barco não cabe na posição horizontal! Tente novamente.\n");
            c--;
            continue;
        }

        if (direcao && row + ship_size > size) {
            printf("\nO barco não cabe na posição vertical! Tente novamente.\n");
            c--;
            continue;
        }

        // Place boat cells on the board
        for (unsigned int j = 0; j < ship_size; j++) {
            if (direcao) {
                board[row + j][col].boatType = ship_size;
            } else {
                board[row][col + j].boatType = ship_size;
            }
        }
    }
}

void placeBoatNPC(struct boardInfo board[][MAX_SIZE], unsigned int size) {
    unsigned int num_boats = 4;
    int ship_sizes[4] = {2, 3, 4, 5};

    for (unsigned int i = 0; i < num_boats; i++) {
        unsigned int ship_size = ship_sizes[i];
        unsigned int row, col, direcao;

        row = rand() % size;
        col = rand() % size;
        direcao = rand() % 2;

        // Adjust coordinates if necessary to ensure boat fits within bounds
        if (direcao) {
            if (col + ship_size > size) {
                col = size - ship_size;
            }
        } else {
            if (row + ship_size > size) {
                row = size - ship_size;
            }
        }

        // Place boat cells on the board
        for (unsigned int j = 0; j < ship_size; j++) {
            if (direcao) {
                board[row][col + j].boatType = ship_size;
            } else {
                board[row + j][col].boatType = ship_size;
            }
        }
    }
}

void playGame(unsigned int size) {
    struct boardInfo board[MAX_SIZE][MAX_SIZE];
    struct boardInfo boardNPC[MAX_SIZE][MAX_SIZE];

    bool playerShipHit[MAX_SIZE][MAX_SIZE] = {false}; // Initialize to false
    bool npcShipHit[MAX_SIZE][MAX_SIZE] = {false}; // Initialize to false

    int playerLife = 4; // HP PLAYER
    int npcLife = 4; // HP NPC

    int npcShipCount[6] = {0}; // NPC BOATS
    int playerShipCount[6] = {0}; // PLAYER BOATS

    initBoard(board, size);
    initBoard(boardNPC, size);

    placeBoatPlayer(board, size);
    placeBoatNPC(boardNPC, size);

    int ship_sizes[4] = {2, 3, 4, 5};
    unsigned int row, col;
    printf("\n\n ----Iniciando a partida---- \n ");

    while (playerLife > 0 && npcLife > 0) {
        printf("Aonde deseja realizar o seu disparo? (LxC) \n");
        scanf("%d %d", &row, &col);
        printf("\n");

        if (row < 0 || row >= size || col < 0 || col >= size) {
            printf("\nCoordenadas inválidas! Tente novamente.\n");
            continue;
        }

        if (boardNPC[row][col].boatType != 0 && !boardNPC[row][col].isHit) {
            printf("Você atingiu um navio do inimigo!\n");

            npcShipHit[row][col] = true; // MARKS THE BOARD

            // Check if ship is completely destroyed
            unsigned int ship_type = boardNPC[row][col].boatType;
            unsigned int ship_size = ship_sizes[ship_type - 2];
            bool destroyed = true;
            for (unsigned int i = 0; i < ship_size; i++) {
                if (row + i < size && !npcShipHit[row + i][col] && col + i < size && !npcShipHit[row][col + i]) {
                    destroyed = false;
                    break;
                }
            }
            if (destroyed) {
                printf("Você destruiu um navio do inimigo!\n");
                npcLife--;
                npcShipCount[boardNPC[row][col].boatType]--; 
            }
        } else {
            printf("Você não acertou nenhum navio do inimigo!\n");
        }

        printf("Vez do computador \n");

        row = rand() % size;
        col = rand() % size;

        printf("Disparos na posição %d x %d \n", row, col);

        if (board[row][col].boatType != 0 && !board[row][col].isHit) {
            printf("O computador acertou um navio seu \n");

            playerShipHit[row][col] = true;
        }

        printf("\n----- Seu tabuleiro -----\n");
        showBoard(board, size);
        printf("\n----- Tabuleiro do oponente -----\n");
        showBoard(boardNPC, size);
    }

    printf("Fim do jogo!\n");
    if (playerLife > npcLife) {
        printf("Você venceu ficando com %d pontos de vida \n", playerLife);
    } else {
        printf("Você perdeu, o computador ficou com %d pontos de vida \n", npcLife);
    }
}

void showBoard(struct boardInfo board[][MAX_SIZE], unsigned int size) {
    printf("  ");
    for (unsigned int j = 0; j < size; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (unsigned int i = 0; i < size; i++) {
        printf("%d ", i);
        for (unsigned int j = 0; j < size; j++) {
            if (board[i][j].isHit) {
                if (board[i][j].boatType == 0) {
                    printf("X "); // Mark hits with "X"
                } else {
                    printf("H "); // Mark ship hits with "H"
                }
            } else {
                if (board[i][j].boatType != 0) {
                    printf("O "); // Mark boats with "O"
                } else {
                    printf("- "); // Mark misses with "-"
                }
            }
        }
        printf("\n");
    }
}
