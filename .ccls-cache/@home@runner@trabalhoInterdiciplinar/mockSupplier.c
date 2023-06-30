#include "mockSupplier.h"
#include "supplier.h"
#include "munit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_supplier_functions() {
  //test_open_supplier_db
  munit_assert_int(test_open_supplier_db(0), ==, 0);
  munit_assert_int(test_open_supplier_db(1), ==, 1);
  //test_start_suppliers_operation
  munit_assert_int(test_start_suppliers_operation(1), ==, 1);
  munit_assert_int(test_start_suppliers_operation(2), ==, 2);
  munit_assert_int(test_start_suppliers_operation(3), ==, 3);
  munit_assert_int(test_start_suppliers_operation(4), ==, 4);
  munit_assert_int(test_start_suppliers_operation(5), ==, 5);
  //test_locate_supplier
  munit_assert_int(test_locate_supplier(-1), ==, -1);
  munit_assert_int(test_locate_supplier(1), ==, 3);
  munit_assert_int(test_locate_supplier(5), ==, -1);
  //test_check_supplier_desactive
  int active1[10] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 0};
  int active2[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  munit_assert_int(test_check_supplier_desactive(active1), ==, 5);
  munit_assert_int(test_check_supplier_desactive(active2), ==, -1);
  //test_include_supplier
  munit_assert_int(test_include_supplier(-1, 1), ==, 0);
  munit_assert_int(test_include_supplier(1, -1), ==, 2);
  munit_assert_int(test_include_supplier(1, 1), ==, 3);
  munit_assert_int(test_include_supplier(-1, -1), ==, 1);
  //test_change_supplier
  munit_assert_int(test_change_supplier(-1), ==, 0);
  munit_assert_int(test_change_supplier(1), ==, 1);
  //test_delete_supplier
  munit_assert_int(test_delete_supplier(1, 1), ==, 1);
  munit_assert_int(test_delete_supplier(1, 0), ==, 2);
  munit_assert_int(test_delete_supplier(-1, -1), ==, 0);
  //test_view_suppliers
  int active3[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  munit_assert_int(test_view_suppliers(active2), ==, 1);
  munit_assert_int(test_view_suppliers(active3), ==, 0);
  
}

int test_open_supplier_db(int n) {
  setlocale(LC_ALL, "portuguese");
  char name[100];
  FILE *f;
  if(n >= 1){
    strcpy(name, "suppliers.dat");
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

int test_start_suppliers_operation(int op) {
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

int test_locate_supplier(int codigo) {
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

int test_check_supplier_desactive(int active[10]) { 
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

int test_include_supplier(int posicaoDesactive, int posicao) {
  if (posicaoDesactive == -1) {
    if (posicao == -1) {
      return 1;
    } else {
      return 0;
    }
  } else {
    if (posicao == -1) {
      return 2;
    } else {
      return 3;
    }
  }
}

int test_change_supplier(int posicao) {
  if (posicao != -1) {
    return 1;
  } else {
    return 0;
  }
}

int test_delete_supplier(int posicao, int certeza) {
  if (posicao != -1) {
    if (certeza) {
      return 1;
    }
    return  2;
  } else {
    return 0;
  }
}

int test_view_suppliers(int active[10]) {
  int i = 0, *suppliersActive = malloc(10 * sizeof(int));
  while (i < 10) {
    if (active[i]) {
      return 1;
    }
    i++;
  }
  return 0;
}