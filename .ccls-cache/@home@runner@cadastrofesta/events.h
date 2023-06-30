#if !defined(EVENTS_H)
#define EVENTS_H
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *open_events_db();
void start_events_operation();
void include_events(FILE *f);
void view_events_client(FILE *f);
void view_events_date(FILE *f);
void change_event(FILE *f);
void delete_event(FILE *f);
void delete_event_by_client(int codigoC);
void delete_event_by_supplier(int codigoF);

#endif