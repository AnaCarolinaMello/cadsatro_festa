#include "mockEvent.h"
#include "events.h"
#include "munit.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_all_event_functions() {
  // test_open_event_db
  munit_assert_int(test_open_event_db(0), ==, 0);
  munit_assert_int(test_open_event_db(1), ==, 1);
  //test_check_event_day_time
  char dia[100] = "12/12/12";
  char hora[100] = "18:00";
  munit_assert_int(test_check_event_day_time(dia, hora, 2), ==, 0);
  munit_assert_int(test_check_event_day_time(dia, hora, 1), ==, 1);
  strcpy(hora, "20:00");
  munit_assert_int(test_check_event_day_time(dia, hora, 2), ==, 1);
  strcpy(dia, "13/12/12");
  strcpy(hora, "18:00");
  munit_assert_int(test_check_event_day_time(dia, hora, 2), ==, 1);
  // test_start_events_operation
  munit_assert_int(test_start_events_operation(1), ==, 1);
  munit_assert_int(test_start_events_operation(2), ==, 2);
  munit_assert_int(test_start_events_operation(3), ==, 3);
  munit_assert_int(test_start_events_operation(4), ==, 4);
  munit_assert_int(test_start_events_operation(5), ==, 5);
  munit_assert_int(test_start_events_operation(6), ==, 6);
  // test_locate_event
  munit_assert_int(test_locate_event(-1), ==, -1);
  munit_assert_int(test_locate_event(1), ==, 3);
  munit_assert_int(test_locate_event(5), ==, -1);
  // test_check_event_desactive
  int active1[10] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 0};
  int active2[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  munit_assert_int(test_check_event_desactive(active1), ==, 5);
  munit_assert_int(test_check_event_desactive(active2), ==, -1);
  // test_verificar_duracao
  char horaI[100] = "18:00", horaF[100] = "22:00";
  munit_assert_int(test_verificar_duracao(horaI, horaF), ==, 0);
  strcpy(horaF, "23:00");
  munit_assert_int(test_verificar_duracao(horaI, horaF), ==, 1);
  // test_include_event
  munit_assert_int(test_include_event(-1, -1, 0, 1), ==, 1);
  munit_assert_int(test_include_event(-1, -1, 0, 2), ==, 2);
  munit_assert_int(test_include_event(-1, -1, 0, 3), ==, 3);
  munit_assert_int(test_include_event(-1, -1, 1, 3), ==, 4);
  munit_assert_int(test_include_event(-1, -1, 1, 3), ==, 4);
  munit_assert_int(test_include_event(-1, 1, 1, 3), ==, 5);
  munit_assert_int(test_include_event(1, -1, 0, 1), ==, 6);
  munit_assert_int(test_include_event(1, -1, 0, 2), ==, 7);
  munit_assert_int(test_include_event(1, -1, 0, 3), ==, 8);
  munit_assert_int(test_include_event(1, -1, 1, 3), ==, 9);
  munit_assert_int(test_include_event(1, 1, 1, 3), ==, 10);
  // test_change_event
  munit_assert_int(test_change_event(-1, 0, 1), ==, 0);
  munit_assert_int(test_change_event(1, 0, 1), ==, 1);
  munit_assert_int(test_change_event(1, 0, 2), ==, 2);
  munit_assert_int(test_change_event(1, 0, 3), ==, 3);
  munit_assert_int(test_change_event(1, 1, 3), ==, 4);
  // test_delete_event
  munit_assert_int(test_delete_event(1, 1), ==, 1);
  munit_assert_int(test_delete_event(1, 0), ==, 2);
  munit_assert_int(test_delete_event(-1, -1), ==, 0);
  // test_delete_event_by_client
  int active3[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int events[10] = {1, 2, 3, 4, 1, 4, 5, 1, 2, 4};
  munit_assert_int(test_delete_event_by_client(1, active2, events), ==, 1);
  munit_assert_int(test_delete_event_by_client(7, active2, events), ==, 0);
  munit_assert_int(test_delete_event_by_client(7, active3, events), ==, 0);
  // test_delete_event_by_supplier
  munit_assert_int(test_delete_event_by_supplier(1, active2, events), ==, 1);
  munit_assert_int(test_delete_event_by_supplier(7, active2, events), ==, 0);
  munit_assert_int(test_delete_event_by_supplier(7, active3, events), ==, 0);
  // test_view_events_client
  munit_assert_int(test_view_events_client(active2, events, 1), ==, 1);
  munit_assert_int(test_view_events_client(active3, events, 1), ==, 0);
  // test_view_events_date
  char data1[100] = "12/12/2023";
  char data2[100] = "13/10/2023";
  munit_assert_int(test_view_events_date(data1, active2), ==, 1);
  munit_assert_int(test_view_events_date(data2, active2), ==, 0);
  munit_assert_int(test_view_events_date(data2, active3), ==, 0);
}

int test_open_event_db(int n) {
  setlocale(LC_ALL, "portuguese");
  char name[100];
  FILE *f;
  if (n >= 1) {
    strcpy(name, "events.dat");
  }
  if (n >= 1) {
    if ((f = fopen(name, "r+b")) == NULL) {
      if ((f = fopen(name, "w+b")) == NULL) {
        printf("Erro na criação do arquivo!!");
        fclose(f);
        return 0;
      }
    }
  } else {
    return 0;
  }
  fclose(f);
  return 1;
}

int test_check_event_day_time(char data[100], char horaInicio[100], int codigo){
  int codigoC = 1;
  char mockData[20] = "12/12/12";
  char mockHora[20] = "19:00";
  char dayDatabase[3], monthDatabase[3], yearDatabase[5], day[3], month[3], year[5], hora[3], horaFim[3];
  if(codigo != codigoC){
    mockData[strcspn(mockData, "\n")] = '\0';
    data[strcspn(data, "\n")] = '\0';
    sscanf(mockData, "%2s/%2s/%2s", dayDatabase, monthDatabase, yearDatabase);
    sscanf(data, "%2s/%2s/%2s", day, month, year);
    int dayDatabaseInt = atoi(dayDatabase);
    int monthDatabaseInt = atoi(monthDatabase);
    int yearDatabaseInt = atoi(yearDatabase);
    int dayInt = atoi(day);
    int monthInt = atoi(month);
    int yearInt = atoi(year);
  
    mockHora[strcspn(mockHora, "\n")] = '\0';
    horaInicio[strcspn(horaInicio, "\n")] = '\0';
    sscanf(mockHora, "%2s:", horaFim);
    sscanf(horaInicio, "%2s:", hora);
    int horaIncioInt = atoi(hora);
    int horaFimInt = atoi(horaFim);
    if ((horaFimInt >= horaIncioInt) && (dayDatabaseInt == dayInt && monthDatabaseInt == monthInt && yearDatabaseInt == yearInt)) {
     return 0;
    }
  }
  return 1;
}

int test_start_events_operation(int op) {
  switch (op) {
  case 1:
    return 1;
    break;
  case 2:
    return 2;
    break;
  case 3:
    return 3;
    break;
  case 4:
    return 4;
    break;
  case 5:
    return 5;
    break;
  default:
    return 6;
    break;
  }
}

int test_locate_event(int codigo) {
  int posicao = -1, achou = 0, i = 0;
  int e[10] = {2, 4, 3, 1, 7, 5, 6, 8, 9, 10};
  int active[10] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 0}; // 10 e 5 desactive
  while (i < 10 && !achou) {
    posicao++;
    if (e[i] == codigo && active[i]) {
      achou = 1;
    }
    i++;
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

int test_check_event_desactive(int active[10]) {
  int posicao = -1, achou = 0, i = 0;
  while (i < 10 && !achou) {
    posicao++;
    if (!active[i]) {
      achou = 1;
    }
    i++;
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

int test_verificar_duracao(char horaInicio[100], char horaFim[100]){
  int inicioMinutos = convertMinutes(horaInicio);
  int fimMinutos = convertMinutes(horaFim);

  int duracao = (fimMinutos - inicioMinutos + 1440) % 1440;

  if(duracao > 240){
    return 1;
  }else{
    return 0;
  }
}

int test_include_event(int posicaoDesactive, int posicao, int dia, int escolha) {
  if (posicaoDesactive == -1) {
    if (posicao == -1) {
      if(dia == 0){
        switch(escolha){
          case 1:
            return 1;
            break;
          case 2:
            return 2;
            break;
          default:
          return 3;
        }
      }
      return 4;
    } else {
      return 5;
    }
  } else {
    if (posicao == -1) {
      if(dia == 0){
        switch(escolha){
          case 1:
            return 6;
            break;
          case 2:
            return 7;
            break;
          default:
          return 8;
        }
      }
      return 9;
    } else {
      return 10;
    }
  }
}

int test_change_event(int posicao, int dia, int escolha) {
  if (posicao != -1) {
    if(dia == 0){
      switch(escolha){
        case 1:
          return 1;
          break;
        case 2:
          return 2;
          break;
        default:
        return 3;
      }
    }
    return 4;
  } else {
    return 0;
  }
}

int test_delete_event(int posicao, int certeza) {
  if (posicao != -1) {
    if (certeza) {
      return 1;
    }
    return 2;
  } else {
    return 0;
  }
}

int test_delete_event_by_client(int codigoC, int active[10], int events[10]) {
  int i = 0;
  while (i < 10) {
    if (active[i] && events[i] == codigoC) {
      return 1;
    }
    i++;
  }
  return 0;
}

int test_delete_event_by_supplier(int codigoF, int active[10], int events[10]) {
  int i = 0;
  while (i < 10) {
    if (active[i] && events[i] == codigoF) {
      return 1;
    }
    i++;
  }
  return 0;
}

int test_view_events_client(int active[10], int events[10], int clienteId) {
  int i = 0;
  while (i < 10) {
    if (active[i] && clienteId == events[i]) {
      return 1;
    }
    i++;
  }
  return 0;
}

int test_view_events_date(char data[100], int active[10]) {
  int posicao, i = 0;
  char data2[100] = "12/12/2023";
  char dayDatabase[3], monthDatabase[3], yearDatabase[5], day[3], month[3], year[5];
  data2[strcspn(data2, "\n")] = '\0';
  data[strcspn(data, "\n")] = '\0';
  sscanf(data2, "%2s/%2s/%2s", dayDatabase, monthDatabase, yearDatabase);
  sscanf(data, "%2s/%2s/%2s", day, month, year);
  int dayDatabaseInt = atoi(dayDatabase);
  int monthDatabaseInt = atoi(monthDatabase);
  int yearDatabaseInt = atoi(yearDatabase);
  int dayInt = atoi(day);
  int monthInt = atoi(month);
  int yearInt = atoi(year);
  while(i < 10){
    if (active && (dayDatabaseInt == dayInt && monthDatabaseInt == monthInt && yearDatabaseInt == yearInt)) {
      return 1;
    }
    i++;
  }
  return 0;
}