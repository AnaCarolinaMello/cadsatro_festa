#if !defined(SUPPLIER_H)
#define SUPPLIER_H
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_supplier_db();
void start_supplier_operation();
void include_supplier(FILE *f);
int locate_supplier(FILE *f, int codigo);
int check_supplier_desactive(FILE *f);
void change_supplier(FILE *f);
void delete_supplier(FILE *f);
void view_suppliers(FILE *f);

#endif
