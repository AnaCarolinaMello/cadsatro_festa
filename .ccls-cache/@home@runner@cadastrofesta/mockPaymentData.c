#include "mockPaymentData.h"
#include "paymentData.h"
#include "munit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_payment_data_functions(){
  // test_open_value_db
  munit_assert_int(test_open_value_db(0), ==, 0);
  munit_assert_int(test_open_value_db(1), ==, 1);
  // test_open_paymentCheck_db
  munit_assert_int(test_open_paymentCheck_db(0), ==, 0);
  munit_assert_int(test_open_paymentCheck_db(1), ==, 1);
  // test_start_payment_operation
  munit_assert_int(test_start_payment_operation(1), ==, 1);
  munit_assert_int(test_start_payment_operation(2), ==, 2);
  munit_assert_int(test_start_payment_operation(3), ==, 3);
  //test_view_payment_desconts
  munit_assert_int(test_view_payment_desconts(10), ==, 0);
  munit_assert_int(test_view_payment_desconts(0), ==, 1);
  munit_assert_int(test_view_payment_desconts(5), ==, 2);
  //test_view_pay_values
  munit_assert_int(test_view_pay_values(1, 100), ==, 0);
  munit_assert_int(test_view_pay_values(1, 50), ==, 1);
  munit_assert_int(test_view_pay_values(2, 100), ==, 2);
  munit_assert_int(test_view_pay_values(2, 50), ==, 3);
  // get_discount
  munit_assert_int(get_discount(1), ==, 10);
  munit_assert_int(get_discount(2), ==, 5);
  munit_assert_int(get_discount(3), ==, 2);
  munit_assert_int(get_discount(4), ==, 0);
  munit_assert_int(get_discount(5), ==, 0);
  // get_price
  munit_assert_long(get_price(20, 1), ==, 1899.00);
  munit_assert_long(get_price(20, 2), ==, 2099.00);
  munit_assert_long(get_price(40, 1), ==, 1899.00);
  munit_assert_long(get_price(40, 2), ==, 2099.00);
  munit_assert_long(get_price(60, 1), ==, 2199.00);
  munit_assert_long(get_price(60, 2), ==, 2299.00);
  munit_assert_long(get_price(90, 1), ==, 3199.00);
  munit_assert_long(get_price(90, 2), ==, 3499.00);
  munit_assert_long(get_price(100, 1), ==, 3799.00);
  munit_assert_long(get_price(100, 2), ==, 3999.00);
  munit_assert_long(get_price(110, 1), ==, 3799.00);
  munit_assert_long(get_price(110, 2), ==, 3999.00);
  // check_day
  char dia[100] = "Segunda";
  munit_assert_int(check_day(dia), ==, 1);
  strcpy(dia, "Terça");
  munit_assert_int(check_day(dia), ==, 1);
  strcpy(dia, "Quarta");
  munit_assert_int(check_day(dia), ==, 1);
  strcpy(dia, "Quinta");
  munit_assert_int(check_day(dia), ==, 1);
  strcpy(dia, "Sexta");
  munit_assert_int(check_day(dia), ==, 2);
  strcpy(dia, "Sábado");
  munit_assert_int(check_day(dia), ==, 2);
  strcpy(dia, "Domingo");
  munit_assert_int(check_day(dia), ==, 2);
  strcpy(dia, "Outro");
  munit_assert_int(check_day(dia), ==, 2);
}

int test_open_value_db(int n) {
  setlocale(LC_ALL, "portuguese");
  char name[100];
  FILE *f;
  if (n >= 1) {
    strcpy(name, "payValues.dat");
  }
  if (n >= 1) {
    if ((f = fopen(name, "r+b")) == NULL) {
      if ((f = fopen(name, "w+b")) == NULL) {
        printf("Erro na criação do arquivo!!");
        fclose(f);
        return 0;
      }
    }
  } else {
    return 0;
  }
  fclose(f);
  return 1;
}

int test_open_paymentCheck_db(int n) {
  setlocale(LC_ALL, "portuguese");
  char name[100];
  FILE *f;
  if (n >= 1) {
    strcpy(name, "paymentCheck.dat");
  }
  if (n >= 1) {
    if ((f = fopen(name, "r+b")) == NULL) {
      if ((f = fopen(name, "w+b")) == NULL) {
        printf("Erro na criação do arquivo!!");
        fclose(f);
        return 0;
      }
    }
  } else {
    return 0;
  }
  fclose(f);
  return 1;
}

int test_start_payment_operation(int op) {
  switch (op) {
  case 1:
    return 1;
    break;
  case 2:
    return 2;
    break;
  default:
    return 3;
    break;
  }
}

int test_view_payment_desconts(int desconto) {
  if(desconto == 10){
    return 0;
  }else if(desconto == 0){
    return 1;
  }else{
    return 2;
  }
}

int test_view_pay_values(int dia, int convidados) {
  if(dia == 1){
    if(convidados == 100){
      return 0;
    }else{
      return 1;
    }
  }else{
    if(convidados == 100){
     return 2;
    }else{
      return 3;
    }
  }
}