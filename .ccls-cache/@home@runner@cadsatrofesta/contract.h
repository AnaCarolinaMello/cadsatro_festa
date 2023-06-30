#if !defined(CONTRACT_H)
#define CONTRACT_H
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_contract_db();
void start_contract_operation();
void create_contract(char diaSemana[100], int convidados, int codigoFesta);
void view_contract_by_events(FILE *f);
void pay_contract(FILE *f);
void delete_contract(int codigoF);
void cancel_contract(FILE *f);
void view_contract(FILE *f);
void view_contract_by_event(int codigoF);

#endif