/*Bibliotecas utilizadas:*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Struct criada para criar um ponteiro para armazenar os registros*/
typedef struct {
	int id;
	char login[15];
	char password[30];
	char gender;
	double salary;
}registro;

/*Struct criada para ler os comandos dados pelo usuário*/
typedef struct {
  int comando;	/*1=ADD, 2=SEARCH, 3=DEL*/

	/*variaveis para ADD*/
  int   add_id;
  char  add_login[15];
  char  add_password[30];
  char  add_gender;
  double add_salary;

	/*variaveis para SEARCH e DEL*/
	char chave[20]; /*item do registro que será lido para ser utilizado nas funções DEL e SEARCH como parâmetro de busca(exemplo: 2 ”gender” ”M”, significa que "gender" será o item do registro que será usado como parâmetro)*/ 
	char valorStr[30];/*valor do item do registro que será lido (usarei atof() para converter os valores de id e de salary)*/
}Comando;


void PROCESS_COMMAND(Comando *cmd, registro *pessoas, int *count){ /*Função para ler os comandos do usuário e "escolher" qual executar*/
	
	while (scanf("%d", &cmd->comando) == 1){
		int total_removed = 0; /*Variável que armazena a quantidade total de registros removidos*/
		if(cmd->comando == 1){
		/*Estrutura da função ADD: 1 <id> "login" "password" "gender" <salary>*/
		/*Scanf que lê todos os valores dados pelo usuário para adicionar 1 registro*/
    scanf(" %d \"%[^\"]\" \"%[^\"]\" \"%c\" %lf", &cmd->add_id, cmd->add_login, cmd->add_password, &cmd->add_gender, &cmd->add_salary); 
		/*Chamada da função ADD que adiciona um novo registro, mas se falhar escreve "Sem espaço para inserção.".*/
    ADD(pessoas, count, cmd->add_id, cmd->add_login, cmd->add_password, cmd->add_gender, cmd->add_salary);
		/*Verifica se o comando será DEL ou SEARCH para ler a chave que será usado como parâmetro para pesquisa ou para deletar um registro.*/
	} else if(cmd->comando == 2 || cmd->comando == 3){
		/*Estrutua das funções SEARCH e DEL: "2 ou 3" <chave>(que pode ser qualquer item do registro(ex: id, login, gender, etc))*/
		/*Scanf que lê o item (chave) */
		scanf(" \"%[^\"]\" ", cmd->chave);
		if (strcmp(cmd->chave, "id") == 0) {
			/*Variável para ler int*/
    	int v;
    	scanf("%d", &v);
			/*Essa função sprintf transforma o valor (neste caso, nos casos de int e float)para uma string, para que possam ser comparados aos valores já registrados.*/
    	sprintf(cmd->valorStr, "%d", v);
		} 
		else if (strcmp(cmd->chave, "salary") == 0) {
			/*Variável para ler float*/
    	double d;
    	scanf("%lf", &d);
    	sprintf(cmd->valorStr, "%.2f", d);
	}
		else {
    /*Já aqui a leitura é feita normalmente para login, password, gender que vêm entre aspas*/
    scanf(" \"%[^\"]\"", cmd->valorStr);
	}
		/*Verifica se o comando será 2(SEARCH) ou 3(DEL)*/
		if (cmd->comando == 2) {
				/*Chamada da função SEARCH que procura por um registro através de um parâmetro e o imprime na tela, mas se falhar escreve "Nada encontrado.".*/
        SEARCH(pessoas, count, cmd->chave, cmd->valorStr);
    } else if(cmd->comando == 3){
			/*Variável que armazena a quantidade de itens removidos em uma única chamada da função*/
    		int removed = 0;
				/*Adiciona o resultado da chamadada da função DEL à removed.*/
				removed += DEL(pessoas, count, cmd->chave, cmd->valorStr);
    		if (removed > 0) {
					/*Adiciona a quantidade de uma chamada para salvar no total*/
					total_removed += removed;
					printf("%d registro(s) removido(s).\n", total_removed);
    		}
		}

		}
	}
}

/*Função de adicionar novos registros, que se não achar espaço vazio e/ou for ultrapassar o limite de 1000 registros imprime "Sem espaço para inserção.".*/
void ADD(registro *pessoas, int *count, int id, char* login, char* password, char gender, double salary){ 
    // 1) tenta achar um slot livre (id==0)
    for (int i = 0; i < 1000; i++) {
        if (pessoas[i].id == 0) {
            /* Achei! insiro aqui e retorno.*/
            pessoas[i].id      = id;
            strcpy(pessoas[i].login,    login);
            strcpy(pessoas[i].password, password);
            pessoas[i].gender  = gender;
            pessoas[i].salary  = salary;
						(*count)++;
						printf("Registro inserido.\n");
            return;
        }
    }
		/*Se não achar, imprime*/
			printf("Sem espaço para inserção.\n");
		
}


/*Busca em `pessoas[0..count-1]` todos os registros cujo campo `chave` coincide com `valorStr`. Se achar, imprime-os; senão, imprime "Nada encontrado."*/
void SEARCH(registro *pessoas, int *count, const char *chave, const char *valorStr) {
		int found = 0;
		/*Loop que procura algum registro cujo campo `chave` coincide com `valorStr`.*/
    for (int i = 0; i < 1000; i++) {
        if (pessoas[i].id == 0) continue;
				/*Variável int que funciona como boolean e armazena se ouve alguma correspondência(Se sim, imprime todo o registro no formato do arquivo JSON, senão, imprime "Nada encontrado").*/
        int match = 0;

				/* Nos if’s, comparamos a string `chave` (por exemplo "id", "login", "gender", etc.) com o nome de cada campo do registro e, se coincidir, testamos se o valor desse campo em `pessoas[i]` bate com `valorStr`.  
   			Em caso afirmativo, `match` recebe 1 para indicar que houve correspondência. */
        if (strcmp(chave, "id") == 0) {
            match = (pessoas[i].id == atoi(valorStr));
        } else if (strcmp(chave, "login") == 0) {
            match = (strcmp(pessoas[i].login, valorStr) == 0);
        } else if (strcmp(chave, "password") == 0) {
            match = (strcmp(pessoas[i].password, valorStr) == 0);
        } else if (strcmp(chave, "gender") == 0) {
            match = (pessoas[i].gender == valorStr[0]);
        } else if (strcmp(chave, "salary") == 0) {
            match = (pessoas[i].salary == atof(valorStr));
        } else {
            break;
        }
				/*match == 1 significa que deve imprimir esse match*/
        if (match) {
            found++;
            printf("{\n");
            printf("    \"id\": %d,\n",        pessoas[i].id);
            printf("    \"login\": \"%s\",\n", pessoas[i].login);
            printf("    \"password\": \"%s\",\n", pessoas[i].password);
            printf("    \"gender\": \"%c\",\n", pessoas[i].gender);
            printf("    \"salary\": %.2f\n",   pessoas[i].salary);
            printf("}\n");
        }
    }
		/*Se não for encontrado nenhum match, imprime "Nada encontrado.".*/
    if (found == 0) {
        printf("Nada encontrado.\n");
    }
}


/* Função de remoção: percorre o array `pessoas` e, para cada registro cujo campo indicado por `chave` (id, login, password, gender ou salary) coincida com `valorStr`, zera o struct, decrementa `*count` e acumula em `removed_now`.  
Retorna a quantidade total de registros removidos. */
int DEL(registro* pessoas, int *count, const char* chave, const char* valorStr) {
	/*Variável que armazena quantos registros foram removidos*/
  int removed_now = 0;
	for (int i = 0; i < 1000; i++) {
			/*Checa se a posição está vazia*/
      if (pessoas[i].id == 0) continue;

      /* Dentro dos if’s de maior escopo, verifica se a `chave` corresponde a um campo válido (como "id", "login", etc.) e, se sim, compara o valor desse campo no registro atual com `valorStr`.  
   		Se houver coincidência, o registro é zerado e contabilizado como removido. */
			if (strcmp(chave, "id") == 0) { 
          int id = atoi(valorStr);
          if (pessoas[i].id == id) {
							/*Zera o struct naquela posição*/
              pessoas[i] = (registro){0};
							/*Incremenda a variável que armazena quantos registros foram removidos*/ 
              removed_now++;
							/*Decrementa o valor de x que foi passapo por referência em PROCCES_COMMAND e novamente em DEL*/
							(*count)--;
						}
      } else if (strcmp(chave, "salary") == 0) {
          double salario = atof(valorStr);
          if (pessoas[i].salary == salario) {
              pessoas[i] = (registro){0};
              removed_now++;
							(*count)--;
						}
      } else if (strcmp(chave, "login") == 0) {
          if (strcmp(pessoas[i].login, valorStr) == 0) {
              pessoas[i] = (registro){0};
              removed_now++;
							(*count)--;
						}
      }else if (strcmp(chave, "password") == 0) {
    		if (strcmp(pessoas[i].password, valorStr) == 0) {
      				pessoas[i] = (registro){0};
              removed_now++;
							(*count)--;
						}
			}else if (strcmp(chave, "gender") == 0) {
    		if (pessoas[i].gender == valorStr[0]) { 
      				pessoas[i] = (registro){0};
              removed_now++;
							(*count)--;
						}
				}

    }
		/*Se falhar em remover e/ou não for uma remoção válida, imprime "Remoção inválida".*/
		if (removed_now == 0) {
			printf("Remoção inválida.\n");
		}
		/*Retorna quantos registros foram excluídos para soma e salvar em total_removed.*/
		return removed_now;
}



int main(){
	/*Variável que armazena o nome do arquivo JSON*/
	char arq_name[50];
	/*Leitura e abertura para leitura com "r" do arquivo JSON*/
	scanf("%s", arq_name);
	FILE* arquivo = fopen(arq_name, "r");
	/*Checa se não ouve nenhum erro ao abrir o arquivo.*/
  if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
	/*Aloca e já zera tudo com calloc (id=0, login[0]='\0', salary=0.0, etc...)*/
	registro* pessoas = calloc(1000, sizeof(registro));
	/*Variável que armazena quantos registros foram lidos do arquivo JSON e que continua armazenando quantos registros há salvos*/
	int x = 0;

	char linha[256];
	/* Lê o arquivo linha por linha usando `fgets`, procurando por blocos de registros em formato JSON.  
   Quando encontra uma linha com `{`, inicia a leitura de um novo registro. */
  while (fgets(linha, sizeof(linha), arquivo)){

		/*strstr faz com que ache todos os "{" no arquivo JSON*/
		if (strstr(linha, "{")) {
			registro pessoa ={0};
      
			/* Faz a leitura das variáveis do registro linha por linha até encontrar "}", 
			indicando o fim do bloco do registro atual no JSON. */
		while (fgets(linha, sizeof(linha), arquivo) && !strstr(linha, "}")){
			char var[20];
			sscanf(linha, " \" %[^\" ] \"", var);
			if(strcmp(var, "id")==0){
				sscanf(linha, " \"id\" : %d", &pessoa.id);
			} else if(strcmp(var,"login")==0){
				sscanf(linha, " \"login\" : \"%[^\"]\"", pessoa.login);  // Lê o login
      } else if(strcmp(var,"password")==0){
				sscanf(linha, " \"password\" : \"%[^\"]\"", pessoa.password);  // Lê a senha
			} else if(strcmp(var,"gender")==0){
				sscanf(linha, " \"gender\" : \"%c\"", &pessoa.gender);
			} else if(strcmp(var,"salary")==0){
        sscanf(linha, " \"salary\" : %lf", &pessoa.salary);
		  }
		}

		/* Incrementa o contador `x` e armazena o registro lido na próxima posição livre do vetor `pessoas[]`.  
   	Isso garante que todos os registros válidos sejam salvos sequencialmente. */
    pessoas[x++] = pessoa;
		}
	}
	/*Imprime quantos registros foram lidos*/
	printf("%d registro(s) lido(s).\n", x);
	/*Fecha o arquivo.*/
    fclose(arquivo);

	/*Declara e inicializa a struct `cmd` com zeros para garantir que todos os campos comecem limpos,  
  evitando lixo de memória nas leituras dos comandos. */	
	Comando cmd = {0};
	
	/* Chama a função que processa os comandos digitados pelo usuário (ADD, SEARCH ou DEL),  
  usando o vetor `pessoas` e o contador `x` que rastreia a quantidade de registros ativos.*/
	PROCESS_COMMAND(&cmd, pessoas, &x);

	/* Libera a memória alocada dinamicamente para o vetor `pessoas`,  
   evitando vazamento de memória ao final do programa. */
	free(pessoas);
	return 0;
}