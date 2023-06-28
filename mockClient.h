#if !defined(MOCKCLIENT_H)
#define MOCKCLIENT_H
#include "munit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_client_functions();
int test_open_client_db(int n);
int test_start_clients_operation(int op);
int test_locate_client(int codigo);
int test_check_client_desactive(int active[10]);
int test_include_client(int posicaoDesactive, int posicao);
int test_change_client(int posicao);
int test_delete_client(int posicao, int certeza);
int test_delete_event_by_client(int codigoC, int active[10], int events[10]);
int test_view_clients(int active[10]);
int test_view_clients_id(int active[10], int client[10], int codigo);
int test_view_clients_name(int active[10], char nome[100]);

#endif