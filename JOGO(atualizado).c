#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#define ARQ_RANKING "ranking.txt"

// Uso do ifdef para funcionar tanto no Windows quanto no Linux
#ifdef _WIN32
	#include <windows.h>
	void limparTela(){
		system("cls");
	}
	void esperar(int segundos){
		Sleep(segundos * 1000);
	}

#else
	#include <unistd.h>
	void limparTela(){
		system("clear");
	}
	void esperar(int segundos){
		sleep(segundos);
	}
#endif

//DEFINIÇÃO DE ESTRUTURAS E ENUMERAÇÃO DAS FICHAS!!!

// vou usar os nomes em capsLock  para indicar que são constantes. Descobri que é uma convenção
// Para enumerações também é convenção usar 'Enum'
typedef enum {
	VAZIO = 0,
	NORMAL = 1,
	PORTAL = 2,
	EXPLOSIVA = 3
} TipoFicha;

// Vamos precisar de um struct que represente uma única celula do jogo 
// para saber de quem é a peça que está ali. Vai ajudar muito na portal e na explosiva
typedef struct {
	int jogador; // 0 = vazio, 1 = jogador1/PC1, 2 = Jogador2/PC2
	TipoFicha tipo; // usado para saber qual o poder da peça
} Casa;

// Precisamos de um struct para saber que tipo de peça e quantas dessas peças cada jogador tem
typedef struct{
	int qtdPortal;
	int qtdExplosiva;
} Inventario;

//struct do ranking com o nome do jogador e o numero de jogadas
typedef struct{
	char nome[50];
	int jogadas;
} Ranking;

//LIMPEZA DE CARACTERES RESIDUAIS
// Para impedir que entradas inválidas sejam lidass
void limparBuffer(){
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}

// IMPRESSÃO DA MATRIZ DO JOGO
void imprimirTabuleiro(Casa mat[6][7]){
	int i, j;
	
	printf("\n");
	
	printf("   ");
	for(j = 0; j < 7; j++){
		printf(" %d ", j+1);
	}
	
	printf("\n");
	
	printf("===================================\n");
	
	for(i = 0; i < 6; i++){
		printf("   |");
		
		for(j = 0; j < 7; j++){

			if(mat[i][j].jogador == 0){
				printf(" . ");
			} else{
				// aqui vamos definir o caractere base (X ou O) para os jogadores
				char simbolo = (mat[i][j].jogador == 1) ? 'X' : 'O';

				// Se for uma ficha especial, TEM que mudar a visualização!!!
				if(mat[i][j].tipo == PORTAL){
					printf("(%c)", simbolo);
				} else if(mat[i][j].tipo == EXPLOSIVA){
					printf("*%c*", simbolo);
				} else{
					printf(" %c ", simbolo);
				}

			} 
		}
		printf("|\n");
	}
	printf("===================================\n");
}

// precisamos de uma função auxiliar para verificar se a coordenada existe na matriz (se não, vai cair fora do jogo)
int posicaoValida(int i, int j){
	return (i >= 0 && i < 6 && j >= 0 && j <= 7);
}

// Função de explosão usando recursividade
// Quando uma bomba for explodir, chama essa função aqui
// Se atingir outra bomba na redondeza, vai chamar ela mesma de novo
void explodir(Casa mat[6][7], int r, int c){
	// a bomba some primeiro, pois nao faz sentido ela continuar ali depois de explodir
	mat[r][c].jogador = 0;
	mat[r][c].tipo = NORMAL;

	// vai percorrer as 8 casas em volta 
	for(int i = r - 1; i <= r + 1; i++){
		for(int j = c - 1; j <= c + 1; j++){
			// Se a posição existe E tem uma peça nela
			if(posicaoValida(i,j) && mat[i][j].jogador != 0){

				// verifica se a peça destruída é explosiva também
				// se for, explode ela tbm
				if(mat[i][j].tipo == EXPLOSIVA){
					explodir(mat, i, j);
				} else{
					// se for peça normal, apaga ela do tabuleiro
					mat[i][j].jogador = 0;
					mat[i][j].tipo = NORMAL;
				}
			}
		}
	}
	printf("\n=== OLHA A BOMBA, GALERA!! Explosao na linha %d, coluna %d ===\n", r+1,c+1);
	esperar(1); // pausa para a explosão acontecer e não ser instantânea
}

// agora precisamos criar essa função para verificar as explosões e fazer cair 
// precisa procurar as fichas explosivas, se encotrar vai explodir e fazer peças cairem 
void verificarExplosoes(Casa mat[6][7]){
	if(explodir(mat, i, j){

	}

}

// LÓGICA DA GRAVIDADE
// A função jogo vai ter alguns parâmetros, como matriz, a coluna e o jogador. Dentro dela, será aplicado um for
// Que serve pra verificar as linhas, para faze a verificação da última posição até a primeira
int jogo(Casa mat[6][7], int col, int jogador, TipoFicha tipoPeca){

	// lógica do portal
	for(int i = 5; i >= 0; i--){
		if(mat[i][col].jogador == 0){
			if(tipoPeca == PORTAL){
		// se tem uma peça embaixo (i+1) de onde a ficha vai cair (i) 
				if(i < 5 && mat[i+1][col].jogador != 0){
					// o portal vai sumir com a peça de baixo
					// Vai criar um buraco no lugar que tinha aquela peça
					mat[i+1][col].jogador = 0;
					mat[i+1][col].tipo = NORMAL;

					// a peça portal PRECISA sumir também, uma vez que já foi usada
					// Logo, a posição 'i' vai continuar vazia também e a 'i+1' também

					printf("\n=== ZAP! A ficha portal apagou a peca abaixo! ===\n");
					esperar(1);
					return 1; // jogada válida e concluída
				}
			}



			// coloca a peça no lugar
			mat[i][col].jogador = jogador;
			mat[i][col].tipo = tipoPeca;

			// temos que verificar se a jogada causou explosões
			verificarExplosoes(mat);

		return 1;
		}
	}
	return 0; // coluna cheia
}


// VERIFICAÇÃO DE VITÓRIA
// Essa função vai usar a lógica de verificar as linhas, colunas e diagonais
// Ela retorna 0 em caso de não haver vitória, 1 se o jogador 1 vencer e 2 se o jogador 2 vencer
int verificarVitoria(Casa matriz[6][7]){
	int i, j;

	for(i = 0; i < 6; i++){
		for(j = 0; j <= 3; j++){ // vai até a coluna 3 para evitar quebrar o código
			if(matriz[i][j].jogador != 0 && // verifica se não é vazio
				matriz[i][j].jogador == matriz[i][j+1].jogador &&
				matriz[i][j].jogador == matriz[i][j+2].jogador &&
				matriz[i][j].jogador == matriz[i][j+3].jogador){
					return matriz[i][j].jogador; // retorna quem ganhou
				}
		}
	}


	// Verificando a vertical
	for(j = 0; j < 7; j++){
		for(i = 0; i <= 2; i++){ // vai até a linha 2 (i+3) para evitar quebrar o código
			if(matriz[i][j].jogador != 0 &&
				matriz[i][j].jogador == matriz[i+1][j].jogador &&
				matriz[i][j].jogador == matriz[i+2][j].jogador &&
				matriz[i][j].jogador == matriz[i+3][j].jogador){
					return matriz[i][j].jogador;
				}
		}
	}

	// Verificando a diagonal principal
	for(i = 0; i <= 2; i++){
		for(j = 0; j <= 3; j++){
			if(matriz[i][j].jogador != 0 &&
				matriz[i][j].jogador == matriz[i+1][j+1].jogador &&
				matriz[i][j].jogador == matriz[i+2][j+2].jogador &&
				matriz[i][j].jogador == matriz[i+3][j+3].jogador) {
					return matriz[i][j].jogador;
				}
		}
	}

	// verificando a diagonal secundária
	for(j = 6; j >= 3; j--){
		for(i = 0; i <= 2; i++){
			if(matriz[i][j].jogador != 0 &&
				matriz[i][j].jogador == matriz[i+1][j-1].jogador &&
				matriz[i][j].jogador == matriz[i+2][j-2].jogador &&
				matriz[i][j].jogador == matriz[i+3][j-3].jogador) {
					return matriz[i][j].jogador;
				}
		}
	}

	return 0; // Se chegar aqui, significa que ninguém ganhou ainda

}
// LÓGICA DO RANKING (ler ranking --> salva atualizado --> decide se jogador entra ou nao)

//leitura do ranking
void carregarRanking(Ranking hall[3]){//recebe um vetor de 3 posições do tipo ranking
	FILE *f = fopen("ranking.txt", "r"); //"r" - modo leitura apenas

	if(f == NULL){ //se o arquivo não existir cria um ranking vazio
		for(int i = 0; i<3; i++){ //percorre as três posições
			hall[i].jogadas = 42; //numero de jogadas do campeão da posição i
			strcpy(hall[i].nome, "---");
		}
		return;
	}
	for(int i = 0; i<3; i++){ //fscanf para leitura formatada
		if(fscanf(f, "%s %d", hall[i].nome, &hall[i].jogadas)!= 2){//se o fscanf não leu 2 valores (nome e jogadas) então preenche com o padrão (---)
			hall[i].jogadas = 42;
			strcpy(hall[i].nome, "---");
		}
	}
	fclose(f);
}

void salvarRanking(Ranking hall[3]){
    FILE *f = fopen("ranking.txt", "w");//"w" apaga tudo e escreve novamente, garante atualização constante

    if(f == NULL){
        printf("Erro ao salvar ranking!\n");
        return;
    }

    for(int i = 0; i < 3; i++){
        fprintf(f, "%s %d\n", hall[i].nome, hall[i].jogadas); //salva já no formato nome + numero de jogadas
    }

    fclose(f);
}
 
void atualizarRanking(char nome[], int jogadas){ //parametros recebidos: nome do jogador que venceu e o numero de jogadas feitas
    Ranking hall[3];

    carregarRanking(hall);

    for(int i = 0; i < 3; i++){ //percorre as três posições
        if(jogadas < hall[i].jogadas){ //compara    
													
            for(int j = 2; j > i; j--){ //move os jogadores para baixo
                hall[j] = hall[j-1];
            }

            strcpy(hall[i].nome, nome); //insere o novo jogador na posição correta
            hall[i].jogadas = jogadas;

            salvarRanking(hall); //salva o ranking atualizado no arquivo

            printf("\nVocê entrou no Hall dos Campeões!\n");
            return;
        }
    }
}

void mostrarRanking(){
    Ranking hall[3];

    carregarRanking(hall);

    limparTela();

    printf("\n===== HALL DOS CAMPEÕES =====\n\n");

    for(int i = 0; i < 3; i++){
        printf("%dº Lugar: %s - %d jogadas\n", i+1, hall[i].nome, hall[i].jogadas);
    }

    printf("\nPressione ENTER para voltar...");
    getchar();
}

// MODO 1, JOGADOR X JOGADOR
void jogadorVsjogador(){

	int matrizJJ[6][7];
	int jogadorRodada = 1;
	int coluna;
	int validacao;
	int vencedor = 0; // vamos usar essa variável para armazenar o vencedor
	int totalJogadas = 0; // para contar o número de jogadas

	// Não tava funcionando só utilizando o imprimirtabuleiro, então eu inicializei outra matriz aqui dentro
	for(int i = 0; i<6; i++){
		for(int j = 0; j<7; j++){
			matrizJJ[i][j] = 0;
		}
	}

	// O loop do jogo vai rodar enquanto não tiver um vencedor e não tiver empate (jogadas < 42)
	while(vencedor == 0 && totalJogadas < 42){

		limparTela();
		imprimirTabuleiro(matrizJJ);

		//Essa parte serve para alertar qual jogador deve fazer sua jogada
		if(jogadorRodada == 1){
			printf("Jogador 1 (X), escolha a coluna: ");
		}
		else{
			printf("\nJogador 2 (O), escolha a coluna: ");
		}
		scanf("%d",&coluna);
		limparBuffer();

		// A verificação que eu tinha citado, para impedir que joguem em colunas que não existem
		if(coluna < 1 || coluna > 7){
			printf("Coluna inválida. Por favor, escolha uma coluna válida!\n");
			getchar(); getchar();
			// O continue é para não quebrar o jg por conta disso, e continuar a escolha da coluna em seguida
			continue;
		}

		//USO DA FUNÇÃO JOGO
		// Chama a função jogo com os parâmetros da função JogadorvsJogador
		// O parâmetro "coluna-1" está sendo usado porque o jogador escolhe colunas de 1 a 7 e...
		// os índices da matriz vão de 0 até 6, então é feita essa pequena subtração para poder ter uma verificação correta da posição
		validacao = jogo(matrizJJ,coluna-1,jogadorRodada, NORMAL);

		// VERIFICAÇÃO DA COLUNA
		if(!validacao){
			printf("Coluna já está cheia. Tente outra jogada!\n");
			getchar(); getchar();
			continue;
		}

		// Incrementa se a jogada for válida
		totalJogadas++;

		// CHAMANDO A FUNÇÃO VERIFICAR VITÓRIA
		vencedor = verificarVitoria(matrizJJ);

		// TROCA DE JOGADORES ( caso não haja vitória)
		// Se ninguém ganhou, a variável vencedor será 0
		// Se for 0, a gente troca o jogador da rodada
		// Se não for 0, significa que alguém ganhou e o loop vai terminar sem sequer passar por esse bloco
		if(vencedor == 0){
			if(jogadorRodada == 1){
				jogadorRodada = 2;
			} else {
				jogadorRodada = 1;
			}
		}
		if(vencedor != 0){
	        char nome[50];
      		printf("Digite seu nome: ");
    		gets(nome);
    		atualizarRanking(nome, totalJogadas);
}

	}

	limparTela();
	imprimirTabuleiro(matrizJJ);


	// código para comunicar aos jogadores o resultado da partida
	if(vencedor == 1){
		printf("\n==========================\n");
		printf("PARABENS! O JOGADOR 1 (X) VENCEU!\n");
		printf("==========================\n");
	} else if(vencedor == 2){
		printf("\n==========================\n");
		printf("PARABENS! O JOGADOR 2 (O) VENCEU!\n");
		printf("==========================\n");
	} else {
		printf("\n==========================\n");
		printf("EMPATE! NINGUEM VENCEU!\n");
		printf("==========================\n");
	}

	printf("\nPressione enter para voltar...\n");
	getchar(); getchar();

}

// Modo 2, JOGADOR X MAQUINA
void jogadorVsMaquina(){

	// A aplicação é quase idêntica ao que foi feito na função JogadorvsJogador
	// A principal diferença nessa função é o uso do Srand para gerar posições aleatórias da máquina
	// E uma nova verificação feita para impedir que a máquina escolha colunas já preenchidas

	srand(time(NULL));

	int matrizJM[6][7];
	int jogadorRodada = 1;
	int coluna;
	int validacao;
	int vencedor = 0; // vamos usar essa variável para armazenar o vencedor
	int totalJogadas = 0; // para contar o número de jogadas

	// Inicializando a Matriz
	for(int i = 0; i<6; i++){
		for(int j = 0; j<7; j++){
			matrizJM[i][j] = 0;
		}
	}

	// O loop do jogo vai rodar enquanto não tiver um vencedor e não tiver empate (jogadas < 42)
	while(vencedor == 0 && totalJogadas < 42){

		limparTela();
		imprimirTabuleiro(matrizJM);

		// Foi melhor definir como uma variável de fora, por que ela vira uma variável fixa
		int colunaAleatoria = rand()%7;

		// VERIFICAÇÃO DAS COLUNAS(Para a MAQUINA)
		// A verificação ocorre de maneira cíclica, a partir do valor sorteado, ele irá percorrer todas as colunas
		// A verificação ocorre até achar uma coluna com espaço disponível
		for(int i=0; i<7; i++){
			int posicao = (colunaAleatoria+i)%7; 

			// Verificação se a coluna está vazia
			// Caso o topo da coluna esteja vazio, ou seja, seja igual a zero
			// Então é liberada a jogada nesta posição
			if(matrizJM[0][posicao] == 0){
				coluna = posicao + 1; // Aqui é somado +1, porque converte o vetor de 1 até 7, em vez de 0 a 6
				break; 
			}

		}
		//Essa parte serve para alertar quem deve fazer a jogada
		if(jogadorRodada == 1){
			printf("Jogador 1 (X), escolha a coluna:\n ");
			scanf("%d",&coluna);
			limparBuffer();
		}
		else{
			printf("Turno da Maquina (O):\n");
		}

		// A verificação que eu tinha citado, para impedir que joguem em colunas que não existem
		if(coluna < 1 || coluna > 7){
			printf("Coluna inválida. Por favor, escolha uma coluna válida!\n");
			getchar(); getchar();
			// O continue é para não quebrar o jogo por conta disso, e continuar a escolha da coluna em seguida
			continue;
		}
		
		//USO DA FUNÇÃO JOGO
		// Chama a função jogo com os parâmetros da função JogadorvsJogador
		// O parâmetro "coluna-1" está sendo usado porque o jogador escolhe colunas de 1 a 7 e...
		// os índices da matriz vão de 0 até 6, então é feita essa pequena subtração para poder ter uma verificação correta da posição
		validacao = jogo(matrizJM,coluna-1,jogadorRodada, NORMAL);

		// VERIFICAÇÃO DA COLUNA
		if(!validacao){
			printf("Coluna já está cheia. Tente outra jogada!\n");
			getchar(); getchar();
			continue;
		}

		// Incrementa se a jogada for válida
		totalJogadas++;

		// CHAMANDO A FUNÇÃO VERIFICAR VITÓRIA
		vencedor = verificarVitoria(matrizJM);

		// TROCA DE JOGADORES ( caso não haja vitória)
		// Se ninguém ganhou, a variável vencedor será 0
		// Se for 0, a gente troca o jogador da rodada
		// Se não for 0, significa que alguém ganhou e o loop vai terminar sem sequer passar por esse bloco
		if(vencedor == 0){
			if(jogadorRodada == 1){
				jogadorRodada = 2;
			} else {
				jogadorRodada = 1;
			}
		}
	}
	limparTela();
	imprimirTabuleiro(matrizJM);


	// código para comunicar aos jogadores o resultado da partida
	if(vencedor == 1){
		printf("\n==========================\n");
		printf("PARABENS! O JOGADOR 1 (X) VENCEU!\n");
		printf("==========================\n");
	} else if(vencedor == 2){
		printf("\n==========================\n");
		printf("PARABENS! A Maquina (O) VENCEU!\n");
		printf("==========================\n");
	} else {
		printf("\n==========================\n");
		printf("EMPATE! NINGUEM VENCEU!\n");
		printf("==========================\n");
	}

	printf("\nPressione enter para voltar...\n");
	getchar(); getchar();

}

void MaquinaVsMaquina(){
	
	// Como eu citei anteriormente, a lógica também vai ser a mesma de JogadorvsJogador
	// A principal diferença dessa função, é que não há uso de scanf, já que todas as posições são sorteadas
	// A verificação das colunas é usada agora para ambas as máquinas, já que elas são os jogadores

	srand(time(NULL));

	int matrizMM[6][7];
	int jogadorRodada = 1;
	int coluna;
	int validacao;
	int vencedor = 0; 
	int totalJogadas = 0; 

	// Inicialização da matriz
	for(int i = 0; i<6; i++){
		for(int j = 0; j<7; j++){
			matrizMM[i][j] = 0;
		}
	}

	// O loop do jogo vai rodar enquanto não tiver um vencedor e não tiver empate (jogadas < 42)
	while(vencedor == 0 && totalJogadas < 42){
		
		limparTela();
		imprimirTabuleiro(matrizMM);


		esperar(1); // Pausa de 1 segundo para melhor visualização das jogadas

		int colunaAleatoria = rand()%7;

		// Verificação da coluna para as máquinas, como feito na função JogadorvsMaquina

		for(int i=0; i<7; i++){
			int posicao = (colunaAleatoria+i)%7;
			if(matrizMM[0][posicao] == 0){
				coluna = posicao + 1;
				break;
			}
		}

		if(jogadorRodada == 1){
			printf("Turno da Maquina 1 (X):\n ");
		}
		else{
			printf("Turno da Maquina 2 (O):\n");
		
		}
		// A verificação que eu tinha citado, para impedir que joguem em colunas que não existem
		if(coluna < 1 || coluna > 7){
			printf("Coluna inválida. Por favor, escolha uma coluna válida!\n");
			getchar(); getchar();
			// O continue é para não quebrar o jogo por conta disso, e continuar a escolha da coluna em seguida
			continue;
		}
		//USO DA FUNÇÃO JOGO
		// Chama a função jogo com os parâmetros da função JogadorvsJogador
		// O parâmetro "coluna-1" está sendo usado porque o jogador escolhe colunas de 1 a 7 e...
		// os índices da matriz vão de 0 até 6, então é feita essa pequena subtração para poder ter uma verificação correta da posição
		validacao = jogo(matrizMM,coluna-1,jogadorRodada, NORMAL);

		// Aqui é para testar a validação da função jogo, caso não tenha espaço na coluna, irá ter esse alerta
		if(!validacao){
			printf("Coluna já está cheia. Tente outra jogada!\n");
			getchar(); getchar();
			continue;
		}

		// Incrementa se a jogada for válida
		totalJogadas++;

		// CHAMANDO A FUNÇÃO VERIFICAR VITÓRIA
		vencedor = verificarVitoria(matrizMM);

		// TROCA DE JOGADORES ( caso não haja vitória)
		// Se ninguém ganhou, a variável vencedor será 0
		// Se for 0, a gente troca o jogador da rodada
		// Se não for 0, significa que alguém ganhou e o loop vai terminar sem sequer passar por esse bloco
		if(vencedor == 0){
			if(jogadorRodada == 1){
				jogadorRodada = 2;
			} else {
				jogadorRodada = 1;
			}
		}
	}

	limparTela();
	imprimirTabuleiro(matrizMM);


	// código para comunicar aos jogadores o resultado da partida
	if(vencedor == 1){
		printf("\n==========================\n");
		printf("PARABENS! A Maquina 1 (X) VENCEU!\n");
		printf("==========================\n");
	} else if(vencedor == 2){
		printf("\n==========================\n");
		printf("PARABENS! A Maquina 2 (O) VENCEU!\n");
		printf("==========================\n");
	} else {
		printf("\n==========================\n");
		printf("EMPATE! NINGUEM VENCEU!\n");
		printf("==========================\n");
	}

	printf("\nPressione enter para voltar...\n");
	getchar(); getchar();

}

int main(){
	
	int opcao = 0;
	int modoDeJogo = 0;
	
	//DO-WHILE PARA QUE O MENU APAREÇA AO MENOS UMA VEZ
	do {
		
		limparTela();
		printf("\n===========================\n");
		printf("\n    LIGUE 4 - GRUPO 2    \n");
		printf("\n===========================\n");
		printf(" 1 - Iniciar Jogo\n");
		printf(" 2 - Ranking (Em manutencao...)\n");
		printf(" 3 - Sair\n");
		printf("\n===========================\n");
		printf("\nEscolha uma opcao: ");
		
		if(scanf("%d", &opcao) == 0){
			opcao = -1; // Define uma opção inválida para acionar a mensagem de erro
		}
		limparBuffer();
		
		switch(opcao){
			case 1:
				
				printf("\n=== MODO DE JOGO ===\n");
				printf(" 1 - Jogador vs Jogador\n");
				printf(" 2 - Jogador vs Maquina\n");
				printf(" 3 -  Maquina vs Maquina\n");
				printf("Escolha: ");
				
				if(scanf("%d", &modoDeJogo) == 0){
					modoDeJogo = -1; // Define uma opção inválida para acionar a mensagem de erro
				}
				limparBuffer();
				
				if(modoDeJogo == 1){
					printf("\nIniciando partida: Amigo vs Amigo...\n");

					jogadorVsjogador();
				}
				else if(modoDeJogo == 2){
					printf("\nIniciando partida: Voce vs Maquina...\n");

					jogadorVsMaquina();

				} else if(modoDeJogo == 3){
					printf("\nIniciando partida: Maquina vs Maquina...\n");
					printf("Assistindo rinha das IAs...\n");
					esperar(2);
			
					MaquinaVsMaquina();
				}
				else{
					printf("\nModo inválido! Voltando...\n");
				}
				break;
			
			case 2:
				printf("\n===== RANKING =====\n");
				printf("Recurso indisponivel no momento por falta de conhecimento dos devs.\n");
				
				printf("Pressione enter para voltar...\n");
				getchar(); getchar();
				break;
				
			case 3:
				printf("\nSaindo do jogo... Ate mais!\n");
				break;
				
			default:
				printf("\nOpcao invalida! Tente novamente.\n");
				
				getchar(); getchar();	
		}
	} while (opcao != 3);
	
	return 0;
}