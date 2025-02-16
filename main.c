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


struct apostador {
    char nome[50];
    int idade;
    float saldo;
};

typedef struct apostador apostador;

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

void listar_jogos_disponiveis(jogo jogos[]) {
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

void cadastrarUsuario(apostador apostadores[]) {
    float valor_depositado = 0;
    int idade = 0;
    if (apostador_atual == 10) {
        printf("O numero maximo de apostadores foi atingido!\n");
        continua = 0;
    }
    printf("Bem-vindo ao cadastro do apostador,por favor siga as instrucoes abaixo:\n");

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
        continua = 0;
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

void jogarJogoDoBixo(apostador apostadores[], jogo jogos[]) {
    if (apostador_atual == 0 || apostadores[apostador_atual - 1].nome[0] == '\0') {
        printf("Nenhum apostador atual.\n");
        return;
    }
    printf("Bem vindo %s\n", apostadores[apostador_atual - 1].nome);
    Animal animais[NUM_ANIMAIS];

    inicializarAnimais(animais);

    srand(time(NULL));
    int numeroSorteado = rand() % NUM_ANIMAIS + 1;

    printf("Digite o numero do animal que voce quer apostar (1 a 25): ");
    int numeroApostado;
    scanf("%d", &numeroApostado);
    if (numeroApostado < 1 || numeroApostado > 25) {
        printf("Numero invalido. Tente novamente.\n");
        return;
    }
    printf("Digite quanto voce quer apostar: ");
    float valorApostado;
    scanf("%f", &valorApostado);
    if (valorApostado < jogos[0].valor_minimo_aposta) {
        printf("O valor minimo da aposta eh de %.2f\n", jogos[0].valor_minimo_aposta);
        continua = 0;
    }
    if (valorApostado > apostadores[apostador_atual - 1].saldo) {
        printf("Saldo insuficiente. Seu saldo eh de %.2f\n", apostadores[apostador_atual - 1].saldo);
        continua = 0;
    }
    float premio_acumulado = jogos[0].premio_total *= valorApostado;

    printf("Animal sorteado: %s (Numero %d)\n", animais[numeroSorteado - 1].nome, numeroSorteado);

    FILE *file = fopen("historico_aposta.txt", "a");
    if (numeroApostado == numeroSorteado) {
        printf("Parabens! Voce ganhou!\n");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo informacoes_apostadores.txt\n");
        } else {
            fprintf(file, "Apostador: %s\n", apostadores[apostador_atual - 1].nome);
            fprintf(file, "Jogo: %s\n", jogos[0].nome);
            fprintf(file, "Animal apostado: %s\n", animais[numeroApostado - 1].nome);
            fprintf(file, "Animal sorteado: %s\n", animais[numeroSorteado - 1].nome);
            fprintf(file, "Valor apostado: %.2f\n", valorApostado);
            fprintf(file, "Premio acumulado: %.2f\n", premio_acumulado);
            fprintf(file, "Saldo atual: %.2f\n", apostadores[apostador_atual - 1].saldo += premio_acumulado);
            fprintf(file, "\n");
            fclose(file);
        }
    } else {
        printf("Que pena! Voce perdeu. Tente novamente.\n");
        fprintf(file, "Apostador: %s\n", apostadores[apostador_atual - 1].nome);
        fprintf(file, "Jogo: %s\n", jogos[0].nome);
        fprintf(file, "Animal apostado: %s\n", animais[numeroApostado - 1].nome);
        fprintf(file, "Animal sorteado: %s\n", animais[numeroSorteado - 1].nome);
        fprintf(file, "Valor apostado: %.2f\n", valorApostado);
        fprintf(file, "Premio acumulado: %.2f\n", premio_acumulado);
        fprintf(file, "Saldo atual: %.2f\n", apostadores[apostador_atual - 1].saldo -= valorApostado);
        fprintf(file, "\n");
        fclose(file);
    }
    printf("Pressione Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
    getchar(); // Captura o Enter que ficou no buffer do scanf
}

void jogo_das_cores(apostador apostadores[], jogo jogos[]) {

    if (apostador_atual == 0 || apostadores[apostador_atual - 1].nome[0] == '\0') {
        printf("Nenhum apostador atual.\n");
        return;
    }
    printf("Bem vindo ao jogo das cores %s\n", apostadores[apostador_atual - 1].nome);
    printf("Voce tem apenas uma chance de acertar a cor unica que completara os 3 slotes!\n");

    const char *cores[] = {"vermelho", "azul", "verde"};
    printf("escolha umas das cores seguintes(Vermleho,verde,azul): \n");
    char cor_escolhida[10];
    scanf("%s", cor_escolhida);
    if (strcmp(cor_escolhida, cores[0]) == 0) {
        printf("Voce escolheu a cor vermelha\n");
    } else if (strcmp(cor_escolhida, cores[1]) == 0) {
        printf("Voce escolheu a cor azul\n");
    } else if (strcmp(cor_escolhida, cores[2]) == 0) {
        printf("Voce escolheu a cor verde\n");
    } else {
        printf("Cor invalida. Tente novamente.\n");
        return;
    }

    printf("Atencao se o resultado for 0 0 0 sera vermelho,1 1 1 sera azul e 2 2 2 sera verde\n");

    srand(time(NULL));
    int vet_verm[3] = {0, 0, 0};
    int vet_azul[3] = {1, 1, 1};
    int vet_verde[3] = {2, 2, 2};
    int roleta[3] = {rand() % 3, rand() % 3, rand() % 3};
    if (roleta[0] == 0 && roleta[1] == 0 && roleta[2] == 0 && strcmp(cor_escolhida, cores[0]) == 0) {
        printf("Voce ganhou\n");
    } else if (roleta[0] == 1 && roleta[1] == 1 && roleta[2] == 1 && strcmp(cor_escolhida, cores[1]) == 0) {
        printf("Voce ganhou\n");
    } else if (roleta[0] == 2 && roleta[1] == 2 && roleta[2] == 2 && strcmp(cor_escolhida, cores[2]) == 0) {
        printf("Voce ganhou\n");
    } else {
        printf("Voce perdeu\n");
        printf("A cor sorteada foi: \n");

        for (int i = 0; i < 3; i++) {
            printf("%d\n", roleta[i]);
        }
    }
    printf("Pressione Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
    getchar(); // Captura o Enter que ficou no buffer do scanf
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

void jogarAposta(apostador apostadores[], jogo jogos[]) {
    printf("Digite 1 para jogar no Jogo do Bixo\n");
    printf("Digite 2 para jogar na Roleta Acerta Cor\n");
    printf("Digite 3 para voltar ao menu\n");
    int opcao;
    scanf("%d", &opcao);
    switch (opcao) {
        case 1:
            jogarJogoDoBixo(apostadores, jogos);
            break;
        case 2:
            jogo_das_cores(apostadores, jogos);
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
    remove("historico_aposta.txt");
    remove("informacoes_apostadores.txt");
    apostador apostadores[10];
    jogo jogos[2];
    strcpy(jogos[0].nome, "Jogo do Bixo");
    jogos[0].valor_minimo_aposta = 100.0;
    jogos[0].premio_total = 10000.0;

    strcpy(jogos[1].nome, "Roleta Acerta Cor");
    jogos[1].valor_minimo_aposta = 300.0;
    jogos[1].premio_total = 30000.0;

    while (continua) {
        int opcao;
        printf("Menu:\n");
        printf("1. Listar os jogos disponiveis\n");
        printf("2. Cadastrar usuario\n");
        printf("3. Jogar aposta\n");
        printf("4. verificar saldo\n");
        printf("5. ver historico de apostas\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listar_jogos_disponiveis(jogos);
                break;
            case 2:
                cadastrarUsuario(apostadores);
                break;
            case 3:
                jogarAposta(apostadores, jogos);
                break;
            case 4:
                verificarSaldo();
                break;
            case 6:
                continua = 0;
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }
}