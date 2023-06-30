#include "paymentData.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Value{
  int convidados;
  int dia;
  float valor;
};
struct PaymentCheck{
  char formaDePagamento[50];
  int desconto;
};
typedef struct Value value;
typedef struct PaymentCheck paymentCheck;

void include_paymentCheck(FILE *f);
void include_valuePay(FILE *f);

FILE *open_value_db() {
  setlocale(LC_ALL, "portuguese");
  FILE *f;
  if ((f = fopen("payValues.dat", "r+b")) == NULL) {
    if ((f = fopen("payValues.dat", "w+b")) == NULL) {
      printf("Erro na criação do arquivo!!");
      return NULL;
    }
    include_valuePay(f);
  }
  return f;
}

FILE *open_paymentCheck_db() {
  setlocale(LC_ALL, "portuguese");
  FILE *f;
  if ((f = fopen("paymentCheck.dat", "r+b")) == NULL) {
    if ((f = fopen("paymentCheck.dat", "w+b")) == NULL) {
      printf("Erro na criação do arquivo!!");
      return NULL;
    }
    include_paymentCheck(f);
  }
  return f;
}

void start_payment_operation() {
  FILE *v = open_value_db();
  FILE *p = open_paymentCheck_db();
  int option;
  do {
    printf("Escolha: \n");
    printf("1 - Consultar valores\n");
    printf("2 - Consultar desconto\n");
    printf("3 - Voltar ao menu principal\n");
    scanf("%d", &option);
    printf("\n");
    switch (option) {
    case 1:
      view_pay_values(v);
      break;
    case 2:
      view_payment_desconts(p);
      break;
    }
  } while (option != 3);
  fclose(v);
  fclose(p);
}

void include_paymentCheck(FILE *f){
  paymentCheck p;
  strcpy(p.formaDePagamento, "À vista");
  p.desconto = 10;
  fseek(f, 0, SEEK_END);
  fwrite(&p, sizeof(p), 1, f);
  fflush(f);
  strcpy(p.formaDePagamento, "Duas vezes");
  p.desconto = 5;
  fseek(f, 0, SEEK_END);
  fwrite(&p, sizeof(p), 1, f);
  fflush(f);
  strcpy(p.formaDePagamento, "Três vezes");
  p.desconto = 2;
  fseek(f, 0, SEEK_END);
  fwrite(&p, sizeof(p), 1, f);
  fflush(f);
  strcpy(p.formaDePagamento, "Quatro vezes ou mais");
  p.desconto = 0;
  fseek(f, 0, SEEK_END);
  fwrite(&p, sizeof(p), 1, f);
  fflush(f);
}

void include_valuePay(FILE *f){
  value v;
  v.convidados = 30;
  v.valor = 1899.00;
  v.dia = 1;
  fseek(f, 0, SEEK_END);
  fwrite(&v, sizeof(v), 1, f);
  fflush(f);
  v.convidados = 30;
  v.valor = 2099.00;
  v.dia = 2;
  fseek(f, 0, SEEK_END);
  fwrite(&v, sizeof(v), 1, f);
  fflush(f);
  v.convidados = 50;
  v.valor = 2199.00;
  v.dia = 1;
  fseek(f, 0, SEEK_END);
  fwrite(&v, sizeof(v), 1, f);
  fflush(f);
  v.convidados = 50;
  v.valor = 2299.00;
  v.dia = 2;
  fseek(f, 0, SEEK_END);
  fwrite(&v, sizeof(v), 1, f);
  fflush(f);
  v.convidados = 80;
  v.valor = 3199.00;
  v.dia = 1;
  fseek(f, 0, SEEK_END);
  fwrite(&v, sizeof(v), 1, f);
  fflush(f);
  v.convidados = 80;
  v.valor = 3499.00;
  v.dia = 2;
  fseek(f, 0, SEEK_END);
  fwrite(&v, sizeof(v), 1, f);
  fflush(f);
  v.convidados = 100;
  v.valor = 3799.00;
  v.dia = 1;
  fseek(f, 0, SEEK_END);
  fwrite(&v, sizeof(v), 1, f);
  fflush(f);
  v.convidados = 100;
  v.valor = 3999.00;
  v.dia = 2;
  fseek(f, 0, SEEK_END);
  fwrite(&v, sizeof(v), 1, f);
  fflush(f);
}

void view_payment_desconts(FILE *f) {
  paymentCheck p;
  fseek(f, 0, SEEK_SET);
  fread(&p, sizeof(p), 1, f);
  printf("Forma de pagamento          Desconto\n\n");
  while (!feof(f)) {
      if(p.desconto == 10){
        printf("%s                        %d%%\n", p.formaDePagamento, p.desconto);
      }else if(p.desconto == 0){
        printf("%s           %d%%\n", p.formaDePagamento, p.desconto);
      }else{
        printf("%s                     %d%%\n", p.formaDePagamento, p.desconto);
      }
      printf("\n");
    fread(&p, sizeof(p), 1, f);
  }
}

void view_pay_values(FILE *f) {
  value v;
  char weekday[50] = "Segunda a quinta", weekend[50] = "Sexta a domingo";
  fseek(f, 0, SEEK_SET);
  fread(&v, sizeof(v), 1, f);
  printf("Convidados           Dia da semana                Preço\n\n");
  while (!feof(f)) {
    if(v.dia == 1){
      if(v.convidados == 100){
        printf("%d                 %s            R$%.2f\n", v.convidados, weekday, v.valor);
      }else{
        printf("%d                  %s            R$%.2f\n", v.convidados, weekday, v.valor);
      }
    }else{
      if(v.convidados == 100){
        printf("%d                 %s             R$%.2f\n", v.convidados, weekend, v.valor);
      }else{
        printf("%d                  %s             R$%.2f\n", v.convidados, weekend, v.valor);
      }
    }
    fread(&v, sizeof(v), 1, f);
  }
}

int get_discount(int metodo){
  if(metodo == 1){
    return 10;
  }else if(metodo == 2){
    return 5;
  }else if(metodo == 3){
    return 2;
  }else if(metodo == 4){
    return 0;
  }
  return 0;
}

float get_price(int convidados, int dia){
  value v;
  FILE *f = open_value_db();
  fseek(f, 0, SEEK_SET);
  fread(&v, sizeof(v), 1, f);
  while (!feof(f)) {
    if(v.dia == dia){
      if(convidados < 30 && v.convidados == 30){
        return v.valor;
      }else if(convidados >= 30 && convidados < 50 && v.convidados == 30){
        return v.valor;
      }else if(convidados >= 50 && convidados < 80 && v.convidados == 50){
        return v.valor;
      }else if(convidados >= 80 && convidados < 100 && v.convidados == 80){
        return v.valor;
      }else if(convidados >= 100 && v.convidados == 100){
        return v.valor;
      }
    }
    fread(&v, sizeof(v), 1, f);
  }
  return 0;
}


int check_day(char dia[100]){
  if(!strcmp(dia, "Segunda") || !strcmp(dia, "segunda") || !strcmp(dia, "Seg") || !strcmp(dia, "seg")){
    return 1;
  }else if(!strcmp(dia, "Terca") || !strcmp(dia, "Terça") || !strcmp(dia, "terca")  || !strcmp(dia, "terça") || !strcmp(dia, "Ter")  || !strcmp(dia, "ter")){
    return 1;
  }else if(!strcmp(dia, "Quarta") || !strcmp(dia, "quarta") || !strcmp(dia, "qua") || !strcmp(dia, "qua")){
    return 1;
  }else if(!strcmp(dia, "Quinta") || !strcmp(dia, "quinta") || !strcmp(dia, "Qui") || !strcmp(dia, "qui")){
    return 1;
  }else if(!strcmp(dia, "Sexta") || !strcmp(dia, "sexta") || !strcmp(dia, "Sex") || !strcmp(dia, "sex")){
    return 2;
  }else if(!strcmp(dia, "Sabado") || !strcmp(dia, "Sábado") || !strcmp(dia, "sabado")  || !strcmp(dia, "sábado") || !strcmp(dia, "Sab")  || !strcmp(dia, "Sáb") || !strcmp(dia, "sab") || !strcmp(dia, "sáb")){
    return 2;
  }else if(!strcmp(dia, "Domingo") || !strcmp(dia, "domingo") || !strcmp(dia, "Dom") || !strcmp(dia, "dom")){
    return 2;
  }
  return 2;
}