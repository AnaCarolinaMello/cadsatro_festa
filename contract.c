#include "contract.h"
#include "events.h"
#include "paymentData.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Contract {
  int codigo;
  float valorTotal;
  int desconto;
  float valorFinal;
  char formaPagamento[100];
  char status[50];
  int active;
  // Referências
  int codigoFesta;
};
typedef struct Contract contract;

FILE *open_contract_db() {
  setlocale(LC_ALL, "portuguese");
  FILE *f;
  if ((f = fopen("contracts.dat", "r+b")) == NULL) {
    if ((f = fopen("contracts.dat", "w+b")) == NULL) {
      printf("Erro na criação do arquivo!!");
      return NULL;
    }
  }
  return f;
}

void start_contract_operation() {
  FILE *f = open_contract_db();
  int option;
  do {
    printf("Escolha: \n");
    printf("1 - Consultar o contrato pela festa\n");
    printf("2 - Pagar um contatro\n");
    printf("3 - Cancelar um contatro\n");
    printf("4 - Ver contrato pelo código\n");
    printf("5 - Voltar ao menu principal\n");
    scanf("%d", &option);
    printf("\n");
    switch (option) {
    case 1:
      view_contract_by_events(f);
      break;
    case 2:
      pay_contract(f);
      break;
    case 3:
      cancel_contract(f);
      break;
    case 4:
      view_contract(f);
      break;
    }
  } while (option != 5);
  fclose(f);
}

int locate_contract(FILE *f, int codigo) {
  int posicao = -1, find = 0;
  contract c;
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f) && !find) {
    posicao++;
    if (c.codigo == codigo) {
      find = 1;
    }
    fread(&c, sizeof(c), 1, f);
  }
  if (find) {
    return posicao;
  } else {
    return -1;
  }
}

int check_contract_desactive(FILE *f) {
  int posicao = -1, achou = 0;
  contract c;
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

void create_contract(char diaSemana[100], int convidados, int codigoFesta){
  FILE *f = open_contract_db();
  contract c;
  float valor, valorFinal, descontoMul;
  int formaPagamento, posicao = 1, posicaoDesactive, dia, desconto;
  printf("Digite o código do contrato\n");
  scanf("%d", &c.codigo);
  posicaoDesactive = check_contract_desactive(f);
  if(posicaoDesactive == -1){
    while(posicao != -1){
      posicao = locate_contract(f, c.codigo);
      if(posicao != -1){
        printf("Código já existente, digite outro\n");
        scanf("%d", &c.codigo);
      }else{
        printf("De quantas vezes quer parcelar?\n");
        printf("1 - À vista\n");
        printf("2 - Duas vezes\n");
        printf("3 - Três vezes\n");
        printf("4 - Quatro vezes ou mais\n");
        scanf("%d", &formaPagamento);
        desconto = get_discount(formaPagamento);
        c.codigoFesta = codigoFesta;
        c.active = 1;
        strcpy(c.status, "A pagar");
        c.desconto = desconto;
        descontoMul = (100 - desconto) / 100.00;
        dia = check_day(diaSemana);
        valor = get_price(convidados, dia);
        valorFinal = valor * descontoMul;
        c.valorTotal = valor;
        c.valorFinal = valorFinal;
        if(formaPagamento == 1){
          strcpy(c.formaPagamento, "À vista");
        }else if(formaPagamento == 2){
          strcpy(c.formaPagamento, "Duas vezes");
        }else if(formaPagamento == 3){
          strcpy(c.formaPagamento, "Três vezes");
        }else if(formaPagamento == 4){
          strcpy(c.formaPagamento, "Quatro vezes ou mais");
        }else{
          strcpy(c.formaPagamento, "Quatro vezes ou mais");
        }
        fseek(f, 0, SEEK_END);
        fwrite(&c, sizeof(c), 1, f);
        fflush(f);
        printf("Contrato cadastrado\n\n");
      }
    }
  }else{
    while(posicao != -1){
      posicao = locate_contract(f, c.codigo);
      if(posicao != -1){
        printf("Código já existente, digite outro\n");
        scanf("%d", &c.codigo);
      }else{
        printf("De quantas vezes quer parcelar?\n");
        printf("1 - À vista\n");
        printf("2 - Duas vezes\n");
        printf("3 - Três vezes\n");
        printf("4 - Quatro vezes ou mais\n");
        scanf("%d", &formaPagamento);
        printf("\n");
        desconto = get_discount(formaPagamento);
        c.codigoFesta = codigoFesta;
        c.active = 1;
        strcpy(c.status, "A pagar");
        c.desconto = desconto;
        descontoMul = (100 - desconto) / 100.00;
        dia = check_day(diaSemana);
        valor = get_price(convidados, dia);
        valorFinal = valor * descontoMul;
        c.valorTotal = valor;
        c.valorFinal = valorFinal;
        if(formaPagamento == 1){
          strcpy(c.formaPagamento, "À vista");
        }else if(formaPagamento == 2){
          strcpy(c.formaPagamento, "Duas vezes");
        }else if(formaPagamento == 3){
          strcpy(c.formaPagamento, "Três vezes");
        }else if(formaPagamento == 4){
          strcpy(c.formaPagamento, "Quatro vezes ou mais");
        }else{
          strcpy(c.formaPagamento, "Quatro vezes ou mais");
        }
        fseek(f, sizeof(c) * (posicaoDesactive), SEEK_SET);
        fwrite(&c, sizeof(c), 1, f);
        fflush(f);
        printf("Contrato cadastrado\n\n");
      }
    }
  }
}

void delete_contract(int codigoF) {
  FILE *f = open_contract_db();
  int posicao;
  contract c;
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f)) {
    if (c.active && c.codigoFesta == codigoF) {
      posicao = locate_contract(f, c.codigo);
      c.active = 0;
      fseek(f, sizeof(c) * (posicao), SEEK_SET);
      fwrite(&c, sizeof(c), 1, f);
      fflush(f);
    }
    fread(&c, sizeof(c), 1, f);
  }
}


void view_contract_by_events(FILE *f) {
  contract c;
  int codigoF;
  printf("Qual o código da festa que deseja encontrar o contrato?\n");
  scanf("%d", &codigoF);
  printf("\n");
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f)) {
    if (c.active && c.codigoFesta == codigoF) {
      printf("Código................: %d \n", c.codigo);
      printf("Valor bruto...........: R$ %.2f \n", c.valorTotal);
      printf("Desconto..............: %d%% \n", c.desconto);
      printf("Valor final...........: R$ %.2f\n", c.valorFinal);
      printf("Status da pagamento...: %s\n", c.status);
      printf("Código da festa.......: %d \n", c.codigoFesta);
      printf("\n");
    }
    fread(&c, sizeof(c), 1, f);
  }
}

void view_contract(FILE *f) {
  contract c;
  int codigo;
  printf("Digite o código do contrato\n");
  scanf("%d", &codigo);
  printf("\n");
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f)) {
    if (c.active && c.codigo == codigo) {
      printf("Código................: %d \n", c.codigo);
      printf("Valor bruto...........: R$ %.2f \n", c.valorTotal);
      printf("Desconto..............: %d%% \n", c.desconto);
      printf("Valor final...........: R$ %.2f\n", c.valorFinal);
      printf("Status do pagamento...: %s\n", c.status);
      printf("Código da festa.......: %d \n", c.codigoFesta);
      printf("\n");
    }
    fread(&c, sizeof(c), 1, f);
  }
}

void view_contract_by_event(int codigoF) {
  FILE *f = open_contract_db();
  contract c;
  int codigo;
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f)) {
    if (c.active && c.codigoFesta == codigoF) {
      printf("Código do contrato....: %d \n", c.codigo);
      printf("Valor bruto...........: R$ %.2f \n", c.valorTotal);
      printf("Desconto..............: %d%% \n", c.desconto);
      printf("Valor final...........: R$ %.2f\n", c.valorFinal);
      printf("Status do pagamento...: %s\n", c.status);
      printf("\n");
    }
    fread(&c, sizeof(c), 1, f);
  }
}

void change_contract_by_event(int codigoF, char diaSemana[100], int convidados) {
  FILE *f = open_contract_db();
  contract c;
  int formaPagamento;
  int codigo, posicao, dia;
  float desconto, descontoMul, valor, valorFinal;
  fseek(f, 0, SEEK_SET);
  fread(&c, sizeof(c), 1, f);
  while (!feof(f)) {
    if (c.active && c.codigoFesta == codigoF) {
      posicao = locate_contract(f, c.codigo);
      fseek(f, sizeof(c) * (posicao), SEEK_SET);
      fread(&c, sizeof(c), 1, f);
      printf("De quantas vezes quer parcelar?\n");
        printf("1 - À vista\n");
        printf("2 - Duas vezes\n");
        printf("3 - Três vezes\n");
        printf("4 - Quatro vezes ou mais\n");
        scanf("%d", &formaPagamento);
        printf("\n");
        desconto = get_discount(formaPagamento);
        c.active = 1;
        strcpy(c.status, "A pagar");
        c.desconto = desconto;
        descontoMul = (100 - desconto) / 100.00;
        dia = check_day(diaSemana);
        valor = get_price(convidados, dia);
        valorFinal = valor * descontoMul;
        c.valorTotal = valor;
        c.valorFinal = valorFinal;
        if(formaPagamento == 1){
          strcpy(c.formaPagamento, "À vista");
        }else if(formaPagamento == 2){
          strcpy(c.formaPagamento, "Duas vezes");
        }else if(formaPagamento == 3){
          strcpy(c.formaPagamento, "Três vezes");
        }else if(formaPagamento == 4){
          strcpy(c.formaPagamento, "Quatro vezes ou mais");
        }else{
          strcpy(c.formaPagamento, "Quatro vezes ou mais");
        }
      strcpy(c.status, "A pagar");
      fseek(f, sizeof(c) * (posicao), SEEK_SET);
      fwrite(&c, sizeof(c), 1, f);
      fflush(f);
      return;
    }
    fread(&c, sizeof(c), 1, f);
  }
}

void pay_contract(FILE *f){
  contract c;
  int codigo, posicao;
  printf("Digite o código do contrato que deseja pagar\n");
  scanf("%d", &codigo);
  printf("\n");
  posicao = locate_contract(f, codigo);
  if(posicao != -1){
    fseek(f, sizeof(c) * (posicao), SEEK_SET);
    fread(&c, sizeof(c), 1, f);
    strcpy(c.status, "Pago");
    fseek(f, sizeof(c) * (posicao), SEEK_SET);
    fwrite(&c, sizeof(c), 1, f);
    fflush(f);
    printf("Contrato pago com sucesso\n");
  }else{
    printf("Código %d inexistente\n", codigo);
  }
}

void cancel_contract(FILE *f){
  contract c;
  int codigo, posicao, certeza;
  printf("Digite o código do contrato que deseja cancelar\n");
  scanf("%d", &codigo);
  printf("\n");
  posicao = locate_contract(f, codigo);
  if(posicao != -1){
    printf("Tem certeza que deseja deletar essa festa? Ela será "
           "permanentemente deletada(sim - 1, não - 0)\n");
    scanf("%d", &certeza);
    printf("\n");
    if (certeza) {
      fseek(f, sizeof(c) * (posicao), SEEK_SET);
      fread(&c, sizeof(c), 1, f);
      strcpy(c.status, "Cancelado");
      fseek(f, sizeof(c) * (posicao), SEEK_SET);
      fwrite(&c, sizeof(c), 1, f);
      fflush(f);
    }
  }else{
    printf("Código %d inexistente", codigo);
  }
}