#include "client.h"
#include "events.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Client {
  int codigo;
  char nome[100];
  char endereco[100];
  char telefone[50];
  char dataNascimento[50];
  int active;
};
typedef struct Client client;

FILE *open_client_db() {
  setlocale(LC_ALL, "portuguese");
  FILE *f;
  if ((f = fopen("clientes.dat", "r+b")) == NULL) {
    if ((f = fopen("clientes.dat", "w+b")) == NULL) {
      printf("Erro na criação do arquivo!!");
      return NULL;
    }
  }
  return f;
}

void start_clients_operation() {
  FILE *f = open_client_db();
  int op;
  do {
    printf("Escolha:\n");
    printf("1 - Incluir cliente\n");
    printf("2 - Alterar cliente\n");
    printf("3 - Listar clientes\n");
    printf("4 - Deletar cliente\n");
    printf("5 - Voltar ao menu principal\n");
    scanf("%d", &op);
    printf("\n");
    switch (op) {
    case 1:
      include_client(f);
      break;
    case 2:
      change_client(f);
      break;
    case 3:
      view_clients(f);
      break;
    case 4:
      delete_client(f);
      break;
    }
  } while (op != 5);
  fclose(f);
}

int locate_client(FILE *f, int codigo) {
  int posicao = -1, achou = 0;
  client c;
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f) && !achou) {
    posicao++;
    if (c.codigo == codigo && c.active) {
      achou = 1;
    }
    fread(&c, sizeof(c), 1, f);
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

int check_client_desactive(FILE *f) {
  int posicao = -1, achou = 0;
  client c;
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f) && !achou) {
    posicao++;
    if (!c.active) {
      achou = 1;
    }
    fread(&c, sizeof(c), 1, f);
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

void include_client(FILE *f) {
  client c;
  int posicao, posicaoDesactive;
  printf("Digite o código do cliente\n");
  scanf("%d", &c.codigo);
  posicao = locate_client(f, c.codigo);
  posicaoDesactive = check_client_desactive(f);
  if (posicaoDesactive == -1) {
    if (posicao == -1) {
      printf("Digite o nome do cliente\n");
      scanf(" %[^\n]", c.nome);
      printf("Digite o endereço do cliente\n");
      scanf(" %[^\n]", c.endereco);
      printf("Digite o telefone do cliente\n");
      scanf(" %[^\n]", c.telefone);
      printf("Digite a data de nascimento do cliente\n");
      scanf(" %[^\n]", c.dataNascimento);
      printf("\n");
      c.active = 1;
      fseek(f, 0, SEEK_END);
      fwrite(&c, sizeof(c), 1, f);
      fflush(f);
    } else {
      printf("Código %d já existe no arquivo. Inclusão não realizada!\n",
             c.codigo);
    }
  } else {
    if(posicao == -1){
      printf("Digite o nome do cliente\n");
      scanf(" %[^\n]", c.nome);
      printf("Digite o endereço do cliente\n");
      scanf(" %[^\n]", c.endereco);
      printf("Digite o telefone do cliente\n");
      scanf(" %[^\n]", c.telefone);
      printf("Digite a data de nascimento do cliente\n");
      scanf(" %[^\n]", c.dataNascimento);
      printf("\n");
      c.active = 1;
      fseek(f, sizeof(c) * (posicaoDesactive), SEEK_SET);
      fwrite(&c, sizeof(c), 1, f);
      fflush(f);
    } else {
      printf("Código %d já existe no arquivo. Inclusão não realizada!\n",
             c.codigo);
    }
  }
}

void change_client(FILE *f) {
  int codigo, posicao;
  client c;
  printf("Diga qual o codigo do cliente para alterar\n");
  scanf("%d", &codigo);
  posicao = locate_client(f, codigo);
  if (posicao != -1) {
    fseek(f, sizeof(c) * (posicao), SEEK_SET);
    fread(&c, sizeof(c), 1, f);
    printf("Código.............: %d \n", c.codigo);
    printf("Nome...............: %s \n", c.nome);
    printf("Endereço...........: %s \n", c.endereco);
    printf("Telefone...........: %s\n", c.telefone);
    printf("Data de nascimento.: %s\n", c.dataNascimento);
    printf("Digite o nome do cliente\n");
    scanf(" %[^\n]", c.nome);
    printf("Digite o endereço do cliente\n");
    scanf(" %[^\n]", c.endereco);
    printf("Digite o telefone do cliente\n");
    scanf(" %[^\n]", c.telefone);
    printf("Digite a data de nascimento do cliente\n");
    scanf(" %[^\n]", c.dataNascimento);
    printf("\n");
    fseek(f, sizeof(c) * (posicao), SEEK_SET);
    fwrite(&c, sizeof(c), 1, f);
    fflush(f);
  } else {
    printf("Cliente %d não encontrado\n", codigo);
  }
}

void delete_client(FILE *f) {
  int codigo, posicao, certeza;
  client c;
  printf("Diga qual o codigo do cliente\n");
  scanf("%d", &codigo);
  posicao = locate_client(f, codigo);
  if (posicao != -1) {
    printf("Tem certeza que deseja deletar esse cliente? Ele será "
           "permanentemente deletado(sim - 1, não - 0)\n");
    scanf("%d", &certeza);
    printf("\n");
    if (certeza) {
      fseek(f, sizeof(c) * (posicao), SEEK_SET);
      fread(&c, sizeof(c), 1, f);
      delete_event_by_client(c.codigo);
      c.active = 0;
      fseek(f, sizeof(c) * (posicao), SEEK_SET);
      fwrite(&c, sizeof(c), 1, f);
      fflush(f);
    }
  } else {
    printf("Cliente %d não encontrado\n", codigo);
  }
}

void view_clients(FILE *f) {
  client c;
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f)) {
    if (c.active) {
      printf("Código.............: %d \n", c.codigo);
      printf("Nome...............: %s \n", c.nome);
      printf("Endereço...........: %s \n", c.endereco);
      printf("Telefone...........: %s\n", c.telefone);
      printf("Data de nascimento.: %s\n", c.dataNascimento);
      printf("\n");
    }
    fread(&c, sizeof(c), 1, f);
  }
}