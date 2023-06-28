#if !defined(CLIENT_H)
#define CLIENT_H
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_client_db();
void start_clients_operation();
void include_client(FILE *f);
int locate_client(FILE *f, int codigo);
int check_client_desactive(FILE *f);
void change_client(FILE *f);
void delete_client(FILE *f);
void view_clients(FILE *f);
void view_clients_id(FILE *f);
void view_clients_name(FILE *f);

#endif
