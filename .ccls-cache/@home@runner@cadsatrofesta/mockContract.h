#if !defined(MOCKCONTRACT_H)
#define MOCKCONTRACT_H
#include "munit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_contract_functions();
int test_open_contract_db(int n);
int test_start_contract_operation(int op);
int test_locate_contract(int codigo);
int test_check_contract_desactive(int active[10]);
int test_create_contract(int formaPagamento, int posicao, int posicaoDesactive);
int test_delete_contract_by_event(int codigoF, int active[10], int contract[10]);
int test_view_contract_by_event(int active[10], int contract[10], int codigoF);
int test_view_contract(int active[10], int contract[10], int codigoC);
int test_pay_contract(int posicao);
int test_cancel_contract(int posicao, int certeza);

#endif