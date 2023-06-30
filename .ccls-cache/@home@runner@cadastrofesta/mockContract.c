#include "munit.h"
#include "mockContract.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_contract_functions(){
  // test_open_contract_db
  munit_assert_int(test_open_contract_db(0), ==, 0);
  munit_assert_int(test_open_contract_db(1), ==, 1);
  // test_start_contract_operation
  munit_assert_int(test_start_contract_operation(1), ==, 1);
  munit_assert_int(test_start_contract_operation(2), ==, 2);
  munit_assert_int(test_start_contract_operation(3), ==, 3);
  munit_assert_int(test_start_contract_operation(4), ==, 4);
  munit_assert_int(test_start_contract_operation(5), ==, 5);
  // test_locate_contract
  munit_assert_int(test_locate_contract(-1), ==, -1);
  munit_assert_int(test_locate_contract(1), ==, 3);
  munit_assert_int(test_locate_contract(5), ==, -1);
  // test_check_contract_desactive
  int active1[10] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 0};
  int active2[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  munit_assert_int(test_check_contract_desactive(active1), ==, 5);
  munit_assert_int(test_check_contract_desactive(active2), ==, -1);
  // test_create_contract
  munit_assert_int(test_create_contract(1, 1, -1), ==, 0);
  munit_assert_int(test_create_contract(1, -1, -1), ==, 11);
  munit_assert_int(test_create_contract(2, -1, -1), ==, 12);
  munit_assert_int(test_create_contract(3, -1, -1), ==, 13);
  munit_assert_int(test_create_contract(4, -1, -1), ==, 14);
  munit_assert_int(test_create_contract(5, -1, -1), ==, 15);
  munit_assert_int(test_create_contract(1, 1, 1), ==, 2);
  munit_assert_int(test_create_contract(1, -1, 1), ==, 31);
  munit_assert_int(test_create_contract(2, -1, 1), ==, 32);
  munit_assert_int(test_create_contract(3, -1, 1), ==, 33);
  munit_assert_int(test_create_contract(4, -1, 1), ==, 34);
  munit_assert_int(test_create_contract(5, -1, 1), ==, 35);
  // test_change_contract_by_event
munit_assert_int(test_change_contract_by_event(1, 1), ==, 0);
  munit_assert_int(test_change_contract_by_event(1, -1), ==, 11);
  munit_assert_int(test_change_contract_by_event(2, -1), ==, 12);
  munit_assert_int(test_change_contract_by_event(3, -1), ==, 13);
  munit_assert_int(test_change_contract_by_event(4, -1), ==, 14);
  munit_assert_int(test_change_contract_by_event(5, -1), ==, 15);
  // test_delete_contract_by_event
  int contract[10] = {1, 2, 3, 4, 1, 4, 5, 1, 2, 4};
  int active3[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  munit_assert_int(test_delete_contract_by_event(1, active1, contract), ==, 1);
  munit_assert_int(test_delete_contract_by_event(1, active3, contract), ==, 0);
  // test_view_contract_by_event
  munit_assert_int(test_view_contract_by_event(contract, active1, 1), ==, 1);
  munit_assert_int(test_view_contract_by_event(contract, active3, 1), ==, 0);
  // test_view_contract
  munit_assert_int(test_view_contract(contract, active1, 1), ==, 1);
  munit_assert_int(test_view_contract(contract, active3, 1), ==, 0);
  // test_pay_contract
  munit_assert_int(test_pay_contract(-1), ==, 0);
  munit_assert_int(test_pay_contract(1), ==, 1);
  // test_cancel_contract
  munit_assert_int(test_cancel_contract(-1, 1), ==, 0);
  munit_assert_int(test_cancel_contract(1, 1), ==, 1);
  munit_assert_int(test_cancel_contract(1, 0), ==, 2);
}

int test_open_contract_db(int n) {
  setlocale(LC_ALL, "portuguese");
  char name[100];
  FILE *f;
  if(n >= 1){
    strcpy(name, "contracts.dat");
  }
  if(n >= 1){
    if ((f = fopen(name, "r+b")) == NULL) {
      if ((f = fopen(name, "w+b")) == NULL) {
        printf("Erro na criação do arquivo!!");
        fclose(f);
        return 0;
      }
    }
  }else{
    return 0;
  }
  fclose(f);
  return 1;
}

int test_start_contract_operation(int op) {
  switch (op) {
  case 1:
    return 1;
    break;
  case 2:
    return 2;
    break;
  case 3:
    return 3;
    break;
  case 4:
    return 4;
    break;
  default:
    return 5;
    break;
  }
}

int test_locate_contract(int codigo) {
  int posicao = -1, achou = 0, i = 0;
  int s[10] = {2, 4, 3, 1, 7, 5, 6, 8, 9, 10};
  int active[10] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 0}; // 10 e 5 desactive
  while (i < 10 && !achou) {
    posicao++;
    if (s[i] == codigo && active[i]) {
      achou = 1;
    }
    i++;
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

int test_check_contract_desactive(int active[10]) { 
  int posicao = -1, achou = 0, i = 0;
  while (i < 10 && !achou) {
    posicao++;
    if (!active[i]) {
      achou = 1;
    }
    i++;
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

int test_create_contract(int formaPagamento, int posicao, int posicaoDesactive){
  if(posicaoDesactive == -1){
    if(posicao != -1){
      return 0;
    }else{
      if(formaPagamento == 1){
        return 11;
      }else if(formaPagamento == 2){
        return 12;
      }else if(formaPagamento == 3){
        return 13;
      }else if(formaPagamento == 4){
        return 14;
      }else{
        return 15;
      }
      }
  }else{
    if(posicao != -1){
      return 2;
    }else{
      if(formaPagamento == 1){
        return 31;
      }else if(formaPagamento == 2){
        return 32;
      }else if(formaPagamento == 3){
        return 33;
      }else if(formaPagamento == 4){
        return 34;
      }else{
        return 35;
      }
    }
  }
  return 100;
}

int test_change_contract_by_event(int formaPagamento, int posicao){
  if(posicao != -1){
      return 0;
  }else{
    if(formaPagamento == 1){
      return 11;
    }else if(formaPagamento == 2){
      return 12;
    }else if(formaPagamento == 3){
      return 13;
    }else if(formaPagamento == 4){
      return 14;
    }else{
      return 15;
    }
  }
}

int test_delete_contract_by_event(int codigoF, int active[10], int contract[10]) {
  int i = 0;
  while (i < 10) {
    if (active[i] && contract[i] == codigoF) {
      return 1;
    }
    i++;
  }
  return 0;
}

int test_view_contract_by_event(int active[10], int contract[10], int codigoF) {
  int i = 0;
  while (i < 10) {
    if (active[i] && codigoF == contract[i]) {
      return 1;
    }
    i++;
  }
  return 0;
}

int test_view_contract(int active[10], int contract[10], int codigoC) {
  int i = 0;
  while (i < 10) {
    if (active[i] && codigoC == contract[i]) {
      return 1;
    }
    i++;
  }
  return 0;
}

int test_pay_contract(int posicao){
  if(posicao != -1){
    return 1;
  }else{
    return 0;
  }
}

int test_cancel_contract(int posicao, int certeza){
  if(posicao != -1){
    if(certeza){
      return 1;
    }else{
      return 2;
    }
  }else{
    return 0;
  }
}