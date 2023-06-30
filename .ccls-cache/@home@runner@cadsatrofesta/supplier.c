#include "supplier.h"
#include "events.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Supplier {
  int codigo;
  char nome[100];
  char telefone[50];
  char email[50];
  int active;
};
typedef struct Supplier supplier;

FILE *open_supplier_db() {
  setlocale(LC_ALL, "portuguese");
  FILE *f;
  if ((f = fopen("suppliers.dat", "r+b")) == NULL) {
    if ((f = fopen("suppliers.dat", "w+b")) == NULL) {
      printf("Erro na criação do arquivo!!");
      return NULL;
    }
  }
  return f;
}

void start_supplier_operation() {
  FILE *f = open_supplier_db();
  int op;
  do {
    printf("Escolha:\n");
    printf("1 - Incluir fornecedor\n");
    printf("2 - Alterar fornecedor\n");
    printf("3 - Listar fornecedores\n");
    printf("4 - Deletar fornecedor\n");
    printf("5 - Voltar ao menu principal\n");
    scanf("%d", &op);
    printf("\n");
    switch (op) {
    case 1:
      include_supplier(f);
      break;
    case 2:
      change_supplier(f);
      break;
    case 3:
      view_suppliers(f);
      system("pause");
      break;
    case 4:
      delete_supplier(f);
      break;
    }
  } while (op != 5);
  fclose(f);
}

int locate_supplier(FILE *f, int codigo) {
  int posicao = -1, achou = 0;
  supplier s;
  fseek(f, 0, SEEK_SET);
  fread(&s, sizeof(s), 1, f);
  while (!feof(f) && !achou) {
    posicao++;
    if (s.codigo == codigo && s.active) {
      achou = 1;
    }
    fread(&s, sizeof(s), 1, f);
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

int check_supplier_desactive(FILE *f) {
  int posicao = -1, achou = 0;
  supplier s;
  fseek(f, 0, SEEK_SET);
  fread(&s, sizeof(s), 1, f);
  while (!feof(f) && !achou) {
    posicao++;
    if (!s.active) {
      achou = 1;
    }
    fread(&s, sizeof(s), 1, f);
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

void include_supplier(FILE *f) {
  supplier s;
  int posicao, posicaoDesactive;
  printf("Digite o código do fornecedor\n");
  scanf("%d", &s.codigo);
  posicao = locate_supplier(f, s.codigo);
  posicaoDesactive = check_supplier_desactive(f);
  if (posicaoDesactive == -1) {
    if (posicao == -1) {
      printf("Digite o nome do fornecedor\n");
      scanf(" %[^\n]", s.nome);
      printf("Digite o telefone do fornecedor\n");
      scanf(" %[^\n]", s.telefone);
      printf("Digite o e-mail do fornecedor\n");
      scanf(" %[^\n]", s.email);
      printf("\n");
      s.active = 1;
      fseek(f, 0, SEEK_END);
      fwrite(&s, sizeof(s), 1, f);
      fflush(f);
    } else {
      printf("Código %d já existe no arquivo. Inclusão não realizada!\n",
             s.codigo);
    }
  } else {
    if(posicao == -1){
      printf("Digite o nome do fornecedor\n");
      scanf(" %[^\n]", s.nome);
      printf("Digite o telefone do fornecedor\n");
      scanf(" %[^\n]", s.telefone);
      printf("Digite o e-mail do fornecedor\n");
      scanf(" %[^\n]", s.email);
      printf("\n");
      s.active = 1;
      fseek(f, sizeof(s) * (posicaoDesactive), SEEK_SET);
      fwrite(&s, sizeof(s), 1, f);
      fflush(f);
    } else {
      printf("Código %d já existe no arquivo. Inclusão não realizada!\n",
             s.codigo);
    }
  }
}

void change_supplier(FILE *f) {
  int codigo, posicao;
  supplier s;
  printf("Diga qual o codigo do fornecedor para alterar\n");
  scanf("%d", &codigo);
  posicao = locate_supplier(f, codigo);
  if (posicao != -1) {
    fseek(f, sizeof(s) * (posicao), SEEK_SET);
    fread(&s, sizeof(s), 1, f);
    printf("Código.............: %d \n", s.codigo);
    printf("Nome...............: %s \n", s.nome);
    printf("Telefone...........: %s\n", s.telefone);
    printf("Data de nascimento.: %s\n", s.email);
    printf("Digite o nome do fornecedor\n");
    scanf(" %[^\n]", s.nome);
    printf("Digite o telefone do fornecedor\n");
    scanf(" %[^\n]", s.telefone);
    printf("Digite o e-mail do fornecedor\n");
    scanf(" %[^\n]", s.email);
    printf("\n");
    fseek(f, sizeof(s) * (posicao), SEEK_SET);
    fwrite(&s, sizeof(s), 1, f);
    fflush(f);
  } else {
    printf("Fonecedor %d não encontrado\n", codigo);
  }
}

void delete_supplier(FILE *f) {
  int codigo, posicao, certeza;
  supplier s;
  printf("Diga qual o codigo do fornecedor\n");
  scanf("%d", &codigo);
  posicao = locate_supplier(f, codigo);
  if (posicao != -1) {
    printf("Tem certeza que deseja deletar esse fornecedor? Ele será "
           "permanentemente deletado(sim - 1, não - 0)\n");
    scanf("%d", &certeza);
    printf("\n");
    if (certeza) {
      fseek(f, sizeof(s) * (posicao), SEEK_SET);
      fread(&s, sizeof(s), 1, f);
      delete_event_by_supplier(s.codigo);
      s.active = 0;
      fseek(f, sizeof(s) * (posicao), SEEK_SET);
      fwrite(&s, sizeof(s), 1, f);
      fflush(f);
    }
  } else {
    printf("Fonecedor %d não encontrado\n", codigo);
  }
}

void view_suppliers(FILE *f) {
  supplier s;
  fseek(f, 0, SEEK_SET);
  fread(&s, sizeof(s), 1, f);
  while (!feof(f)) {
    if (s.active) {
      printf("Código.............: %d \n", s.codigo);
      printf("Nome...............: %s \n", s.nome);
      printf("Telefone...........: %s\n", s.telefone);
      printf("Data de nascimento.: %s\n", s.email);
      printf("\n");
    }
    fread(&s, sizeof(s), 1, f);
  }
}