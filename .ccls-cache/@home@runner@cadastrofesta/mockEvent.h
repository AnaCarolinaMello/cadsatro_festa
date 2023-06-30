#if !defined(MOCKEVENT_H)
#define MOCKEVENT_H
#include "munit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_event_functions();
int test_open_event_db(int n);
int test_check_event_day_time(char data[100], char horaInicio[100], int codigo);
int test_start_events_operation(int op);
int test_locate_event(int codigo);
int test_check_event_desactive(int active[10]);
int test_include_event(int posicaoDesactive, int posicao, int dia, int escolha);int test_change_event(int posicao, int dia, int escolha);
int test_delete_event(int posicao, int certeza);
int test_view_events_client(int active[10],int events[10], int clienteId);
int test_view_events_date(char data[100], int active[10]);
int test_delete_event_by_supplier(int codigoF, int active[10], int events[10]);

#endif