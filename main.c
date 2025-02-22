#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_ANIMAIS 25              //total de animais no jogo
static int continua = 1;            //se o menu continua sendo exibido
static int apostador_atual = 0;     //registrar o apostador atual

//struct jogo
struct Jogo {
    char nome[50];
    float valor_minimo_aposta;
    float premio_total;
};

typedef struct Jogo jogo;

//struct animal
typedef struct {
    char nome[20];
    int numero;
} Animal;

//struct apostador
struct apostador {
    char nome[50];
    int idade;
    float saldo;
};

typedef struct apostador apostador;


void listar_jogos_disponiveis(jogo jogos[]);
void cadastrarUsuario(apostador apostadores[]);
void jogarAposta(apostador apostadores[], jogo jogos[]);
void inicializarAnimais(Animal animais[]);
void jogarJogoDoBixo(apostador apostadores[], jogo jogos[]);
void jogo_das_cores(apostador apostadores[], jogo jogos[]);
void verificarSaldo();
void historicoApostas();
void adicionarSaldo(apostador apostadores[]);


int main(void) {
    //apaga arquivos antigos
    remove("historico_aposta.txt");
    remove("informacoes_apostadores.txt");
    apostador apostadores[10];
    jogo jogos[2];
    //inicializar jogo do bixo
    strcpy(jogos[0].nome, "Jogo do Bixo");
    jogos[0].valor_minimo_aposta = 100.0;
    jogos[0].premio_total = 10000.0;
    //inicializar roleta acerta cor
    strcpy(jogos[1].nome, "Roleta Acerta Cor");
    jogos[1].valor_minimo_aposta = 300.0;
    jogos[1].premio_total = 30000.0;

    //menu com as opções
    while (continua) {
        int opcao;
        printf("Menu:\n");
        printf("1. Listar os jogos disponiveis\n");
        printf("2. Cadastrar usuario\n");
        printf("3. Jogar aposta\n");
        printf("4. Verificar saldo\n");
        printf("5. Ver historico de apostas\n");
        printf("6. Adicionar saldo\n");
        printf("7. Sair\n");
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
            case 5:
                historicoApostas();
                break;
            case 6:
                adicionarSaldo(apostadores);
                break;
            case 7:
                continua = 0;
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }
}

//inicializar array com os 25 animais e seus números correspondentes
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

//listar jogos disponíveis e suas informações até o usuário pressionar enter
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

//cadatrar apostador
void cadastrarUsuario(apostador apostadores[]) {
    float valor_depositado = 0;
    int idade = 0;
    if (apostador_atual == 10) {    //no máximo 10 apostadores
        printf("O numero maximo de apostadores foi atingido!\n");
        continua = 0;
    }
    printf("Bem-vindo ao cadastro do apostador, por favor siga as instrucoes abaixo:\n");

    printf("Digite somente o seu primeiro nome:\n");    //informar nome
    scanf("%s", apostadores[apostador_atual].nome);

    printf("Digite a sua idade: \n");       //informar idade, só pode acima de 18
    scanf("%d", &idade);
    if (idade >= 18) {
        apostadores[apostador_atual].idade = idade;
    } else {
        printf("O sistema detectou que voce eh de menor, portanto proibimos seu cadastro!\n");
        continua = 0;
    }

    printf("Digite quanto voce quer depositar (deposito minimo = 50 reais):\n");    //informar valor a ser depositado, minimo de 50
    scanf("%f", &valor_depositado);
    if (valor_depositado < 50.0) {
        printf("O deposito minimo eh de 50 reais, por favor deposite um valor valido:\n");
        continua = 0;
    } else {
        apostadores[apostador_atual].saldo = valor_depositado;
    }
    FILE *file = fopen("informacoes_apostadores.txt", "a"); //adicionar informações ao arquivo com info dos apostadores
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
    printf("Pressione Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
    getchar(); // Captura o Enter que ficou no buffer do scanf
}

//falta atualizar o saldo do apostador quando terminar o jogo

//jogar jogo do bixo
void jogarJogoDoBixo(apostador apostadores[], jogo jogos[]) {

    if (apostador_atual == 0 || apostadores[apostador_atual - 1].nome[0] == '\0') { //verificar se existem apostadores
        printf("Nenhum apostador atual.\n");
        return;
    }
    printf("Bem vindo, %s\n", apostadores[apostador_atual - 1].nome);
    Animal animais[NUM_ANIMAIS];

    inicializarAnimais(animais);    //inicializar array de animais

    srand(time(NULL));
    int numeroSorteado = rand() % NUM_ANIMAIS + 1;  //sortear numero aleatorio

    printf("Digite o numero do animal que voce quer apostar (1 a 25): ");   //escolher o numero de animal para a aposta
    int numeroApostado;
    scanf("%d", &numeroApostado);
    if (numeroApostado < 1 || numeroApostado > 25) {    //verificar se o numero esta entre 1 e 25 (valido)
        printf("Numero invalido. Tente novamente.\n");
        return;
    }
    printf("Digite quanto voce quer apostar (minimo %.2f): ", jogos[0].valor_minimo_aposta);    //escolher valor da aposta
    float valorApostado;
    scanf("%f", &valorApostado);
    if (valorApostado < jogos[0].valor_minimo_aposta) { //verificar se o valor é maior ou igual ao minimo do jogo
        printf("O valor minimo da aposta eh de %.2f. Tente novamente.\n", jogos[0].valor_minimo_aposta);
        return;
    }
    if (valorApostado > apostadores[apostador_atual - 1].saldo) {   //verificar se o saldo do apostador é suficiente
        printf("Saldo insuficiente. Seu saldo eh de %.2f. Tente novamente.\n", apostadores[apostador_atual - 1].saldo);
        return;
    }
    float premio_acumulado = jogos[0].premio_total += valorApostado;    //acumular valor apostado no total
    float saldo_atual = apostadores[apostador_atual - 1].saldo; //guardar o saldo atual

    printf("Animal sorteado: %s (Numero %d)\n", animais[numeroSorteado - 1].nome, numeroSorteado);  //mostrar qual animal foi sorteado

    FILE *file = fopen("historico_aposta.txt", "a");    //adicionar ao historico de apostas o que ocorreu
    if (numeroApostado == numeroSorteado) {     //se o numero apostado for igual o sorteado, ganhou
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
            fprintf(file, "Saldo atual: %.2f\n", saldo_atual += valorApostado);
            fprintf(file, "\n");
            fclose(file);
        }
    } else {    //se não, perdeu
        jogos[0].premio_total = premio_acumulado;   //atualizar premio acumulado
        printf("Que pena! Voce perdeu. Tente novamente.\n");
        fprintf(file, "Apostador: %s\n", apostadores[apostador_atual - 1].nome);
        fprintf(file, "Jogo: %s\n", jogos[0].nome);
        fprintf(file, "Animal apostado: %s\n", animais[numeroApostado - 1].nome);
        fprintf(file, "Animal sorteado: %s\n", animais[numeroSorteado - 1].nome);
        fprintf(file, "Valor apostado: %.2f\n", valorApostado);
        fprintf(file, "Premio acumulado: %.2f\n", premio_acumulado);
        fprintf(file, "Saldo atual: %.2f\n", saldo_atual -= valorApostado);
        fprintf(file, "\n");
        fclose(file);
        jogos[0].premio_total = premio_acumulado;   //atualizar premio acumulado
    }
    printf("Pressione Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
    getchar(); // Captura o Enter que ficou no buffer do scanf
}

//falta atualizar o saldo do apostador após o jogo

//jogar jogo das cores
void jogo_das_cores(apostador apostadores[], jogo jogos[]) {
    if (apostador_atual == 0 || apostadores[apostador_atual - 1].nome[0] == '\0') {     //verificar se existem apostadores
        printf("Nenhum apostador atual.\n");
        return;
    }
    printf("Bem vindo ao jogo das cores %s\n", apostadores[apostador_atual - 1].nome);

    printf("Digite quanto voce quer apostar (minimo %.2f): ", jogos[1].valor_minimo_aposta);    //escolher valor da aposta
    float valorApostado;
    scanf("%f", &valorApostado);
    if (valorApostado < jogos[1].valor_minimo_aposta) { //verificar se o valor é maior ou igual ao minimo do jogo
        printf("O valor minimo da aposta eh de %.2f. Tente novamente.\n", jogos[1].valor_minimo_aposta);
        return;
    }
    if (valorApostado > apostadores[apostador_atual - 1].saldo) {   //verificar se o saldo do apostador é suficiente
        printf("Saldo insuficiente. Seu saldo eh de %.2f. Tente novamente. \n", apostadores[apostador_atual - 1].saldo);
        return;
    }
    float premio_acumulado = jogos[1].premio_total += valorApostado;    //acumular valor apostado no total
    float saldo_atual = apostadores[apostador_atual - 1].saldo; //guardar o saldo atual

    printf("Voce tem apenas uma chance de acertar a cor unica que completara os 3 slots!\n");

    const char *cores[] = {"vermelho", "azul", "verde"};
    printf("Escolha umas das cores seguintes: vermelho, azul, verde\n");
    printf("Digite a cor escolhida: ");
    char cor_escolhida[10];
    int cor_esc; //pra usar no arquivo
    scanf("%s", cor_escolhida);     //informar a cor (vermelho, azul ou verde)
    if (strcmp(cor_escolhida, cores[0]) == 0) {     //checar qual é a cor e se ela é válida
        printf("Voce escolheu a cor vermelha\n");
        cor_esc = 0;
    } else if (strcmp(cor_escolhida, cores[1]) == 0) {
        printf("Voce escolheu a cor azul\n");
        cor_esc = 1;
    } else if (strcmp(cor_escolhida, cores[2]) == 0) {
        printf("Voce escolheu a cor verde\n");
        cor_esc = 2;
    } else {
        printf("Cor invalida. Tente novamente.\n");
        return;
    }

    printf("Atencao, se o resultado for 0 0 0 sera vermelho, 1 1 1 sera azul e 2 2 2 sera verde.\n");

    srand(time(NULL));      //sortear a sequência de cores
    int vet_verm[3] = {0, 0, 0};
    int vet_azul[3] = {1, 1, 1};
    int vet_verde[3] = {2, 2, 2};
    int roleta[3] = {rand() % 3, rand() % 3, rand() % 3};
    FILE *file = fopen("historico_aposta.txt", "a");    //adicionar ao historico de apostas o que ocorreu
    //verificar se é igual a aposta, se for ganhou, se não perdeu
    if (roleta[0] == 0 && roleta[1] == 0 && roleta[2] == 0 && strcmp(cor_escolhida, cores[0]) == 0) {
        printf("Voce ganhou!\n");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo informacoes_apostadores.txt\n");
        } else {
            fprintf(file, "Apostador: %s\n", apostadores[apostador_atual - 1].nome);
            fprintf(file, "Jogo: %s\n", jogos[1].nome);
                fprintf(file, "Cor escolhida: %s (%d %d %d)\n", cores[cor_esc], cor_esc, cor_esc,cor_esc);
                fprintf(file, "Cores sorteadas: %d %d %d \n", roleta[0], roleta[1], roleta[2]);
                fprintf(file, "Valor apostado: %.2f\n", valorApostado);
                fprintf(file, "Premio acumulado: %.2f\n", premio_acumulado);
                fprintf(file, "Saldo atual: %.2f\n", saldo_atual += valorApostado);
                fprintf(file, "\n");
                fclose(file);
            }
    } else if (roleta[0] == 1 && roleta[1] == 1 && roleta[2] == 1 && strcmp(cor_escolhida, cores[1]) == 0) {
        printf("Voce ganhou!\n");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo informacoes_apostadores.txt\n");
        } else {
            fprintf(file, "Apostador: %s\n", apostadores[apostador_atual - 1].nome);
            fprintf(file, "Jogo: %s\n", jogos[1].nome);
                fprintf(file, "Cor escolhida: %s (%d %d %d)\n", cores[cor_esc], cor_esc, cor_esc,cor_esc);
                fprintf(file, "Cores sorteadas: %d %d %d \n", roleta[0], roleta[1], roleta[2]);
                fprintf(file, "Valor apostado: %.2f\n", valorApostado);
                fprintf(file, "Premio acumulado: %.2f\n", premio_acumulado);
                fprintf(file, "Saldo atual: %.2f\n", saldo_atual += valorApostado);
                fprintf(file, "\n");
                fclose(file);
            }
    } else if (roleta[0] == 2 && roleta[1] == 2 && roleta[2] == 2 && strcmp(cor_escolhida, cores[2]) == 0) {
        printf("Voce ganhou!\n");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo informacoes_apostadores.txt\n");
        } else {
            fprintf(file, "Apostador: %s\n", apostadores[apostador_atual - 1].nome);
            fprintf(file, "Jogo: %s\n", jogos[1].nome);
                fprintf(file, "Cor escolhida: %s (%d %d %d)\n", cores[cor_esc], cor_esc, cor_esc,cor_esc);
                fprintf(file, "Cores sorteadas: %d %d %d \n", roleta[0], roleta[1], roleta[2]);
                fprintf(file, "Valor apostado: %.2f\n", valorApostado);
                fprintf(file, "Premio acumulado: %.2f\n", premio_acumulado);
                fprintf(file, "Saldo atual: %.2f\n", saldo_atual += valorApostado);
                fprintf(file, "\n");
                fclose(file);
            }
    } else {
        printf("Voce perdeu!\n");
        printf("A cor sorteada foi: ");

        for (int i = 0; i < 3; i++) {
            printf("%d", roleta[i]);
        }
        printf("\n");
        jogos[1].premio_total = premio_acumulado;   //atualizar premio acumulado
        if (file == NULL) {
            printf("Erro ao abrir o arquivo informacoes_apostadores.txt\n");
        } else {
            fprintf(file, "Apostador: %s\n", apostadores[apostador_atual - 1].nome);
            fprintf(file, "Jogo: %s\n", jogos[1].nome);
                fprintf(file, "Cor escolhida: %s (%d %d %d)\n", cores[cor_esc], cor_esc, cor_esc,cor_esc);
                fprintf(file, "Cores sorteadas: %d %d %d \n", roleta[0], roleta[1], roleta[2]);
                fprintf(file, "Valor apostado: %.2f\n", valorApostado);
                fprintf(file, "Premio acumulado: %.2f\n", premio_acumulado);
                fprintf(file, "Saldo atual: %.2f\n", saldo_atual -= valorApostado);
                fprintf(file, "\n");
                fclose(file);
            }
    }
    printf("Pressione Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
    getchar(); // Captura o Enter que ficou no buffer do scanf
}

//verificar o saldo a partir do arquivo informacoes_apostadores
void verificarSaldo() {
    char nome[50];
    char nomes[50];
    float saldo;
    int found = 0;
    FILE *file = fopen("informacoes_apostadores.txt", "r");
    printf("Digite o nome do apostador para verificar o saldo: ");  //digitar o nome do apostador que quer ver o saldo
    scanf("%s", nome);
    if (file == NULL) {
        printf("Arquivo ainda nao foi criado\n");
    } else {
        while (fscanf(file, "Nome: %s\n", nomes) != EOF) {  //verificar se o apostador existe e mostrar
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
    printf("Pressione tecla Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
    getchar(); // Captura o Enter que ficou no buffer do scanf
}

//adicionar saldo ao apoostador
void adicionarSaldo(apostador apostadores[]) {
    char nome[50];
    float saldo, valor_adicionado;
    int found = 0;      //verificar se encontrou o usuário ou não
    
    printf("Infome o nome do apostador: ");
    scanf("%s", nome);
    
    //ler arquivo informacoes_apostadores
    FILE *file = fopen("informacoes_apostadores.txt", "r");
    if (file == NULL) {
        printf("Arquivo ainda não foi criado.\n");
        return;
    }
    
    //criar arquivo temporário para adicionar as informações
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(file);
        return;
    }
    
    char nome_lido[50];
    int idade;
    while (fscanf(file, "Nome: %s\n", nome_lido) != EOF) {
        fscanf(file, "Idade: %d\n", &idade);
        fscanf(file, "Saldo: %f\n", &saldo);

        //verificar se o nome é o mesmo do lido
        if (strcmp(nome, nome_lido) == 0) {
            found = 1;  //atualizar encontrado pra 1
            printf("Saldo atual: %.2f\n", saldo);
            printf("Digite o valor que deseja adicionar: ");
            scanf("%f", &valor_adicionado);
            
            //verificar se o valor adicionado é maior que 0
            if (valor_adicionado <= 0) {
                printf("O valor deve ser maior que zero.\n");
            } else {
                saldo += valor_adicionado;
                printf("Novo saldo: %.2f\n", saldo);
            }
        }
        //escrever novas informações no arquivo temporário
        fprintf(tempFile, "Nome: %s\nIdade: %d\nSaldo: %.2f\n\n", nome_lido, idade, saldo);
    }

    fclose(file);
    fclose(tempFile);
    
    //remover o arquivo antigo e renomear o temporário
    remove("informacoes_apostadores.txt");
    rename("temp.txt", "informacoes_apostadores.txt");

    //se não tiver encontrado, avisar
    if (!found) {
        printf("Apostador não encontrado.\n");
        return;
    }
}

//jogar aposta, escolher um dos jogos ou voltar ao menu
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

// verificar histórico de apostas

void historicoApostas() {
    char nomeJogador[50];
    FILE *file = fopen("historico_aposta.txt", "r"); // abrir aqruivo de historico
    
    if (file == NULL) {  // se naõ ha historico de apostas
        printf("Não há historico de apostas!\n");
        return;
    }

    printf("Digite o nome do jogador: ");
    fgets(nomeJogador, sizeof(nomeJogador), stdin);
    nomeJogador[strcspn(nomeJogador, "\n")] = 0;  // Remove o caractere de nova linha, se presente

    char linha[200];
    int encontrouApostas = 0;

    printf("\nHistórico de apostas de %s:\n\n", nomeJogador);

    // Ler e verificar cada linha do arquivo
    while (fgets(linha, sizeof(linha), file) != NULL) {
        // Verificar se o nome do jogador aparece no início da linha
        if (strncmp(linha, nomeJogador, strlen(nomeJogador)) == 0) {
            printf("%s", linha);
            encontrouApostas = 1;
        }
    }

    if (!encontrouApostas) {
        printf("Nenhuma aposta encontrada para o jogador %s.\n", nomeJogador);
    }

    fclose(file);
    printf("\nPressione tecla Enter para voltar ao menu...");
    getchar(); // Espera o usuário pressionar Enter
}
