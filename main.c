#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_ANIMAIS 25
static int continua = 1;
static int apostador_atual = 0;

struct Jogo {
    char nome[50];
    float valor_minimo_aposta;
    float premio_total;
};

typedef struct Jogo jogo;

typedef struct {
    char nome[20];
    int numero;
} Animal;

typedef struct jogo_loteria jogo_loteria;

struct jogo_roleta_acerta_cor {
    jogo jogo;
    char cor[50];
};

typedef struct jogo_roleta_acerta_cor jogo_roleta_acerta_cor;

struct Aposta {
    jogo jogo;
    float valor_aposta;
};

typedef struct Aposta aposta;

struct apostador {
    char nome[50];
    int idade;
    float saldo;
};

typedef struct apostador apostador;

struct historico_aposta {
    aposta aposta;
    apostador apostador;
};

typedef struct historico_aposta historico_aposta;

void inicializarAnimais(Animal animais[]) {
    const char *nomes[] = {
        "Avestruz", "Aguia", "Burro", "Borboleta", "Cachorro",
        "Cabra", "Carneiro", "Camelo", "Cobra", "Coelho",
        "Cavalo", "Elefante", "Galo", "Gato", "Jacare",
        "Leao", "Macaco", "Porco", "Pavao", "Peru",
        "Touro", "Tigre", "Urso", "Veado", "Vaca"
    };

    for (int i = 0; i < NUM_ANIMAIS; i++) {
        strcpy(animais[i].nome, nomes[i]);
        animais[i].numero = i + 1;
    }
}

void jogarJogoDoBixo() {
    system("cls");
    apostador apostadores[10];
    if (apostadores[apostador_atual - 1].nome[0] == '\0') {
        printf("Nenhum apostador atual.\n");
        return;
    }
    Animal animais[NUM_ANIMAIS];
    jogo jogos[2];
    strcpy(jogos[0].nome, "Jogo do Bixo");
    jogos[0].valor_minimo_aposta = 100.0;
    jogos[0].premio_total = 10000.0;
    inicializarAnimais(animais);

    char nome_apostador_atual[50];
    strcpy(nome_apostador_atual, apostadores[apostador_atual].nome);

    srand(time(NULL));
    int numeroSorteado = rand() % NUM_ANIMAIS + 1;

    printf("Digite o numero do animal que voce quer apostar (1 a 25): ");
    int numeroApostado;
    scanf("%d", &numeroApostado);
    printf("Digite quanto voce quer apostar: ");
    float valorApostado;
    scanf("%f", &valorApostado);
    if (valorApostado < jogos[0].valor_minimo_aposta) {
        printf("O valor minimo da aposta eh de %.2f\n", jogos[0].valor_minimo_aposta);
        return;
    }
    int premio_acumulado = jogos[0].premio_total *= valorApostado;

    printf("Animal sorteado: %s (Numero %d)\n", animais[numeroSorteado - 1].nome, numeroSorteado);

    if (numeroApostado == numeroSorteado) {
        printf("Parabens! Voce ganhou!\n");
        apostadores[apostador_atual].saldo += premio_acumulado;
        FILE *file = fopen("historico_aposta.txt", "a");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo informacoes_apostadores.txt\n");
        } else {
            fprintf(file, "Apostador: %s\n", nome_apostador_atual);
            fprintf(file, "Jogo: %s\n", jogos[0].nome);
            fprintf(file, "Animal apostado: %s\n", animais[numeroApostado - 1].nome);
            fprintf(file, "Animal sorteado: %s\n", animais[numeroSorteado - 1].nome);
            fprintf(file, "Valor apostado: %.2f\n", valorApostado);
            fprintf(file, "Premio acumulado: %.2f\n", premio_acumulado);
            fprintf(file, "Saldo atual: %.2f\n", apostadores[apostador_atual].saldo);
            fprintf(file, "\n");
            fclose(file);
            printf("Informações salvas com sucesso no arquivo historico_aposta.txt\n");
        }
    } else {
        printf("Que pena! Voce perdeu. Tente novamente.\n");
    }
}


void listar_jogos_disponiveis() {
    system("cls");
    jogo jogos[2];
    strcpy(jogos[0].nome, "Jogo do Bixo");
    jogos[0].valor_minimo_aposta = 100.0;
    jogos[0].premio_total = 10000.0;

    strcpy(jogos[1].nome, "Roleta Acerta Cor");
    jogos[1].valor_minimo_aposta = 300.0;
    jogos[1].premio_total = 30000.0;

    for (int i = 0; i < 2; i++) {
        printf("Jogo: %s\n", jogos[i].nome);
        printf("Valor minimo da aposta: %.2f\n", jogos[i].valor_minimo_aposta);
        printf("Premio total: %.2f\n", jogos[i].premio_total);
        printf("\n");
    }
    printf("Pressione Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
    getchar(); // Captura o Enter que ficou no buffer do scanf
}

void cadastrarUsuario() {
    system("cls");
    apostador apostadores[10];
    float valor_depositado = 0;
    int idade = 0;
    if (apostador_atual == 10) {
        printf("O numero maximo de apostadores foi atingido!\n");
        continua = 0;
    }
    printf("Bem-vindo ao cadastro do apostador,por favor siga s instrucoes abaixo:\n");

    printf("digite somente o seu primeiro nome:\n");
    scanf("%s", apostadores[apostador_atual].nome);

    printf("Digite a sua idade: \n");
    scanf("%d", &idade);
    if (idade > 18) {
        apostadores[apostador_atual].idade = idade;
    } else {
        printf("O sistema detectou que voce eh de menor,portanto proibimos seu cadastro!\n");
        continua = 0;
    }

    printf("Digite quanto voce quer depositar,Lembrando que o deposito minimo eh 50 reais:\n");
    scanf("%f", &valor_depositado);
    if (valor_depositado < 50.0) {
        printf("O deposito minimo eh de 50 reais,por favor deposite um valor valido:\n");
        scanf("%f", &valor_depositado);
        apostadores[apostador_atual].saldo = valor_depositado;
    } else {
        apostadores[apostador_atual].saldo = valor_depositado;
    }
    FILE *file = fopen("informacoes_apostadores.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo informacoes_apostadores.txt\n");
    } else {
        fprintf(file, "Nome: %s\n", apostadores[apostador_atual].nome);
        fprintf(file, "Idade: %d\n", apostadores[apostador_atual].idade);
        fprintf(file, "Saldo: %.2f\n", apostadores[apostador_atual].saldo);
        fprintf(file, "\n");
        fclose(file);
        printf("Informações salvas com sucesso no arquivo informacoes_apostadores.txt\n");
    }
    apostador_atual++;
    getchar();
    getchar();
}

void verificarSaldo() {
    char nome[50];
    char nomes[50];
    float saldo;
    int found = 0;
    FILE *file = fopen("informacoes_apostadores.txt", "r");
    printf("Digite o nome do apostador para verificar o saldo: ");
    scanf("%s", nome);
    if (file == NULL) {
        printf("Arquivo ainda nao foi criado\n");
    } else {
        while (fscanf(file, "Nome: %s\n", nomes) != EOF) {
            fscanf(file, "Idade: %*d\n");
            fscanf(file, "Saldo: %f\n", &saldo);
            if (strcmp(nomes, nome) == 0) {
                printf("Saldo do apostador %s: %.2f\n", nome, saldo);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Apostador nao encontrado\n");
        }
        fclose(file);
    }
    printf("Pressione Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
    getchar(); // Captura o Enter que ficou no buffer do scanf
}

void jogoDoBixo() {
}

void jogarAposta() {
    printf("Digite 1 para jogar no Jogo do Bixo\n");
    printf("Digite 2 para jogar na Loteria\n");
    printf("Digite 3 para jogar na Roleta Acerta Cor\n");
    printf("Digite 4 para voltar ao menu\n");
    int opcao;
    scanf("%d", &opcao);
    switch (opcao) {
        case 1:
            jogarJogoDoBixo();
            break;
        case 2:
            // Função para jogar na Loteria
            break;
        case 3:

            break;
        case 4:
            continua = 0;
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
    }
}

int main(void) {
    while (continua) {
        int opcao;
        printf("Menu:\n");
        printf("1. Listar os jogos disponiveis\n");
        printf("2. Cadastrar usuario\n");
        printf("3. Jogar aposta\n");
        printf("4. verificar saldo\n");
        printf("5. ver historico de apostas\n");

        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listar_jogos_disponiveis();
                break;
            case 2:
                cadastrarUsuario();
                break;
            case 3:
                jogarAposta();
                break;
            case 4:
                verificarSaldo();
                break;
            case 5:
                continua = 0;
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }
}
