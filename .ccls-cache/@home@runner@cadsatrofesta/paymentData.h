#if !defined(PAYMENTDATA_H)
#define PAYMENTDATA_H
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_value_db();
FILE *open_paymentCheck_db();
void start_payment_operation();
void view_payment_desconts(FILE *f);
void view_pay_values(FILE *f);
int get_descont(int metodo);
float get_price(int convidados, int dia);
int check_day(char dia[100]);

#endif