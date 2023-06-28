#if !defined(MOCKPAYMENTDATA_H)
#define MOCKPAYMENTDATA_H
#include "munit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_payment_data_functions();
int test_open_value_db(int n);
int test_open_paymentCheck_db(int n);
int test_start_payment_operation(int op);
int test_view_payment_desconts(int desconto);
int test_view_pay_values(int dia, int convidados);

#endif