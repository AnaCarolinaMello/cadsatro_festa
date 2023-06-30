#if !defined(MOCKSUPPLIER_H)
#define MOCKSUPPLIER_H
#include "munit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_supplier_functions();
int test_open_supplier_db(int n);
int test_start_suppliers_operation(int op);
int test_locate_supplier(int codigo);
int test_check_supplier_desactive(int active[10]);
int test_include_supplier(int posicaoDesactive, int posicao);
int test_change_supplier(int posicao);
int test_delete_supplier(int posicao, int certeza);
int test_view_suppliers(int active[10]);

#endif