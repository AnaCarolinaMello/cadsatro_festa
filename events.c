#include "events.h"
#include "client.h"
#include "supplier.h"
#include "contract.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Event {
  int codigo;
  int convidados;
  char data[100];
  char diaSemana[100];
  char horarioInicio[100];
  char horarioFim[100];
  char tema[100];
  int active;
  // Referências
  int codigoCliente;
  int codigoFornecedor;
};
typedef struct Event event;

// Função para abrir o arquivo ou cria-lo caso não exista
FILE *open_event_db() {
  setlocale(LC_ALL, "portuguese");
  FILE *f;
  if ((f = fopen("events.dat", "r+b")) == NULL) {
    if ((f = fopen("events.dat", "w+b")) == NULL) {
      printf("Erro na criação do arquivo!!");
      return NULL;
    }
  }
  return f;
}

void start_events_operation() {
  FILE *f = open_event_db();
  int option;
  do {
    printf("Escolha: \n");
    printf("1 - Incluir festa\n");
    printf("2 - Listar festas por cliente\n");
    printf("3 - Listar festas por data\n");
    printf("4 - Alterar dados da festa\n");
    printf("5 - Deletar a festa\n");
    printf("6 - Voltar ao menu principal\n");
    scanf("%d", &option);
    printf("\n");
    switch (option) {
    case 1:
      include_events(f);
      break;
    case 2:
      view_events_client(f);
      break;
    case 3:
      view_events_date(f);
      break;
    case 4:
      change_event(f);
      break;
    case 5:
      delete_event(f);
      break;
    }
  } while (option != 6);
  fclose(f);
}

int locate_event(FILE *f, int codigo) {
  int posicao = -1, find = 0;
  event e;
  fseek(f, 0, SEEK_SET);
  fread(&e, sizeof(e), 1, f);
  while (!feof(f) && !find) {
    posicao++;
    if (e.codigo == codigo) {
      find = 1;
    }
    fread(&e, sizeof(e), 1, f);
  }
  if (find) {
    return posicao;
  } else {
    return -1;
  }
}

int check_event_day_time(FILE *f, char data[100], char horaInicio[100], int codigo){
  char dayDatabase[3], monthDatabase[3], yearDatabase[5], day[3], month[3], year[5], hora[3], horaFim[3];
  event e;
  fseek(f, 0, SEEK_SET);
  fread(&e, sizeof(e), 1, f);
  while (!feof(f)) {
    if(e.codigo != codigo){
      e.data[strcspn(e.data, "\n")] = '\0';
      data[strcspn(data, "\n")] = '\0';
      sscanf(e.data, "%2s/%2s/%2s", dayDatabase, monthDatabase, yearDatabase);
      sscanf(data, "%2s/%2s/%2s", day, month, year);
      int dayDatabaseInt = atoi(dayDatabase);
      int monthDatabaseInt = atoi(monthDatabase);
      int yearDatabaseInt = atoi(yearDatabase);
      int dayInt = atoi(day);
      int monthInt = atoi(month);
      int yearInt = atoi(year);
  
      e.horarioFim[strcspn(e.horarioFim, "\n")] = '\0';
      horaInicio[strcspn(horaInicio, "\n")] = '\0';
      sscanf(e.horarioFim, "%2s:", horaFim);
      sscanf(horaInicio, "%2s:", hora);
      int horaIncioInt = atoi(hora);
      int horaFimInt = atoi(horaFim);
      if ((horaFimInt >= horaIncioInt) && (dayDatabaseInt == dayInt && monthDatabaseInt == monthInt && yearDatabaseInt == yearInt)) {
       return 0;
      }
    }
    fread(&e, sizeof(e), 1, f);
  }
  return 1;
}

int check_event_desactive(FILE *f) {
  int posicao = -1, achou = 0;
  event e;
  fseek(f, 0, SEEK_SET);
  fread(&e, sizeof(e), 1, f);
  while (!feof(f) && !achou) {
    posicao++;
    if (!e.active) {
      achou = 1;
    }
    fread(&e, sizeof(e), 1, f);
  }
  if (achou) {
    return posicao;
  } else {
    return -1;
  }
}

int convertMinutes(char *horario){
  int horas, minutos;
  sscanf(horario, "%d:%d", &horas, &minutos);
  return horas * 60 + minutos;
}

int verificar_duracao(char horaInicio[100], char horaFim[100]){
  int inicioMinutos = convertMinutes(horaInicio);
  int fimMinutos = convertMinutes(horaFim);

  int duracao = (fimMinutos - inicioMinutos + 1440) % 1440;

  if(duracao > 240){
    return 1;
  }else{
    return 0;
  }
}

void include_events(FILE *f) {
  FILE *c = open_client_db(), *s = open_supplier_db();
  event e;
  int posicao, posicaoDesactive;
  char dia[] = "sabado", dia2[] = "sab";
  printf("Digite o código da festa\n");
  scanf("%d", &e.codigo);
  posicao = locate_event(f, e.codigo);
  posicaoDesactive = check_event_desactive(f);
  // Verifica se existe uma posição com uma festa deletada
  if(posicaoDesactive == -1){
    // Verifica se o código digitado existe uma festa ativa
    if (posicao == -1) {
      printf("Digite o código do cliente\n");
      scanf("%d", &e.codigoCliente);
      // Verifica se o código do cliente existe
      if (locate_client(c, e.codigoCliente) == -1) {
        printf("Código %d do cliente não existe\n", e.codigoCliente);
        return;
      }
      printf("Digite o código do fornecedor\n");
      scanf("%d", &e.codigoFornecedor);
      // Verifica se o código do fornecedor existe
      if (locate_supplier(s, e.codigoFornecedor) == -1) {
        printf("Código %d do fornecedor não existe\n", e.codigoFornecedor);
        return;
      }
      printf("Digite o número de convidados\n");
      scanf("%d", &e.convidados);
      printf("Digite a data da festa\n");
      scanf(" %[^\n]", e.data);
      printf("Digite o dia da semana\n");
      scanf(" %[^\n]", e.diaSemana);
      //Verificação para caso dia da festa seja sábado
      if(strcmp(e.diaSemana, "Sabado") || strcmp(e.diaSemana, "Sábado") || strcmp(e.diaSemana, "sabado")  || strcmp(e.diaSemana, "sábado") || strcmp(e.diaSemana, "Sab")  || strcmp(e.diaSemana, "Sáb") || strcmp(e.diaSemana, "sab") || strcmp(e.diaSemana, "sáb")){
        printf("Selecione um dos horários:\n");
        printf("1 - 12:00 às 16:00\n");
        printf("2 - 18:00 às 22:00\n");
        int option;
        scanf("%d", &option);
        switch(option){
          case 1:
            strcpy(e.horarioInicio, "12:00");
            strcpy(e.horarioFim, "16:00");
            break;
          case 2:
            strcpy(e.horarioInicio, "18:00");
            strcpy(e.horarioFim, "22:00");
            break;
          default:
          return;
        }
      }else{
      printf("Digite o horário de inicio\n");
      scanf(" %[^\n]", e.horarioInicio);
      printf("Digite o horário de fim\n");
      scanf(" %[^\n]", e.horarioFim);
      }
      if(verificar_duracao(e.horarioInicio, e.horarioFim)){
        printf("A festa não pode ter mais de 4 horas\n\n");
        return;
      }
      if(!check_event_day_time(f, e.data, e.horarioInicio, e.codigo)){
        printf("Festa já cadastrada nesse dia e horário\n\n");
        return;
      }
      printf("Digite o tema da festa:\n");
      scanf(" %[^\n]", e.tema);
      printf("\n");
      e.active = 1;
      // Cria o contrato a partir da festa 
      create_contract(e.diaSemana, e.convidados, e.codigo);
      fseek(f, 0, SEEK_END);
      fwrite(&e, sizeof(e), 1, f);
      fflush(f);
    } else {
      printf("Código %d já existe no arquivo. Inclusão não realizada!\n",
             e.codigo);
    }
  } else {
    // Verifica se o código digitado existe uma festa ativa
    if(posicao == -1){
      printf("Digite o código do cliente\n");
      scanf("%d", &e.codigoCliente);
      // Verifica se o código do cliente existe
      if (locate_client(c, e.codigoCliente) == -1) {
        printf("Código %d do cliente não existe\n", e.codigoCliente);
        return;
      }
      printf("Digite o código do fornecedor\n");
      scanf("%d", &e.codigoFornecedor);
      // Verifica se o código do fornecedor existe
      if (locate_supplier(c, e.codigoFornecedor) == -1) {
        printf("Código %d do fornecedor não existe\n", e.codigoFornecedor);
        return;
      }
      printf("Digite o número de convidados\n");
      scanf("%d", &e.convidados);
      printf("Digite a data da festa\n");
      scanf(" %[^\n]", e.data);
      printf("Digite o dia da semana\n");
      scanf(" %[^\n]", e.diaSemana);
      //Verificação para caso dia da festa seja sábado
      if(strcmp(e.diaSemana, "Sabado") || strcmp(e.diaSemana, "Sábado") || strcmp(e.diaSemana, "sabado")  || strcmp(e.diaSemana, "sábado") || strcmp(e.diaSemana, "Sab")  || strcmp(e.diaSemana, "Sáb") || strcmp(e.diaSemana, "sab") || strcmp(e.diaSemana, "sáb")){
        printf("Selecione um dos horários:\n");
        printf("1 - 12 às 16\n");
        printf("2 - 18 às 22\n");
        int option;
        scanf("%d", &option);
        switch(option){
          case 1:
            strcpy(e.horarioInicio, "12");
            strcpy(e.horarioFim, "16");
            break;
          case 2:
            strcpy(e.horarioInicio, "18");
            strcpy(e.horarioFim, "22");
            break;
          default:
          return;
        }
      }else{
      printf("Digite o horário de inicio\n");
      scanf(" %[^\n]", e.horarioInicio);
      printf("Digite o horário de fim\n");
      scanf(" %[^\n]", e.horarioFim);
      }
      //verifica a duração máxima da festa
      if(verificar_duracao(e.horarioInicio, e.horarioFim)){
        printf("A festa não pode ter mais de 4 horas\n\n");
        return;
      }
      //verifica se já existe uma festa nesse horário
      if(!check_event_day_time(f, e.data, e.horarioInicio, e.codigo)){
        printf("Festa já cadastrada nesse dia e horário\n\n");
        return;
      }
      printf("Digite o tema da festa:\n");
      scanf(" %[^\n]", e.tema);
      printf("\n");
      e.active = 1;
      // Cria um contrato a partir da festa
      create_contract(e.diaSemana, e.convidados, e.codigo);
      fseek(f, sizeof(e) * (posicaoDesactive), SEEK_SET);
      fwrite(&e, sizeof(e), 1, f);
      fflush(f);
    } else {
      printf("Código %d já existe no arquivo. Inclusão não realizada!\n",
             e.codigo);
    }
  }
}

void change_event(FILE *f) {
  FILE *c = open_client_db(), *s = open_supplier_db();
  int codigo, posicao;
  char dia[] = "sabado", dia2[] = "sab";
  event e;
  printf("Diga qual o codigo da festa para alterar\n");
  scanf("%d", &codigo);
  posicao = locate_event(f, codigo);
  if (posicao != -1) {
    fseek(f, sizeof(e) * (posicao), SEEK_SET);
    fread(&e, sizeof(e), 1, f);
    printf("Código................: %d\n", e.codigo);
    printf("Convidados............: %d\n", e.convidados);
    printf("Data..................: %s\n", e.data);
    printf("Dia da semana.........: %s\n", e.diaSemana);
    printf("Horário...............: %s-%s\n", e.horarioInicio, e.horarioFim);
    printf("Tema..................: %s\n", e.tema);
    printf("Código do cliente.....: %d\n", e.codigoCliente);
    printf("Código do fornecedor..: %d\n", e.codigoFornecedor);
    view_contract_by_event(e.codigo);
    printf("Digite o código do cliente\n");
    scanf("%d", &e.codigoCliente);
    if (locate_client(c, e.codigoCliente) == -1) {
      printf("Código %d do cliente não existe\n", e.codigoCliente);
      return;
    }
    printf("Digite o código do fornecedor\n");
    scanf("%d", &e.codigoFornecedor);
    if (locate_supplier(s, e.codigoFornecedor) == -1) {
      printf("Código %d do fornecedor não existe\n", e.codigoFornecedor);
      return;
    }
    printf("Digite o numero de convidados\n");
    scanf("%d", &e.convidados);
    printf("Digite a data da festa\n");
    scanf(" %[^\n]", e.data);
    printf("Digite o dia da semana\n");
    scanf(" %[^\n]", e.diaSemana);
    if(strcmp(e.diaSemana, "Sabado") || strcmp(e.diaSemana, "Sábado") || strcmp(e.diaSemana, "sabado")  || strcmp(e.diaSemana, "sábado") || strcmp(e.diaSemana, "Sab")  || strcmp(e.diaSemana, "Sáb") || strcmp(e.diaSemana, "sab") || strcmp(e.diaSemana, "sáb")){
      printf("Selecione um dos horários:\n");
      printf("1 - 12:00 às 16:00\n");
      printf("2 - 18:00 às 22:00\n");
      int option;
      scanf("%d", &option);
      switch(option){
        case 1:
          strcpy(e.horarioInicio, "12:00");
          strcpy(e.horarioFim, "16:00");
          break;
        case 2:
          strcpy(e.horarioInicio, "18:00");
          strcpy(e.horarioFim, "22:00");
          break;
        default:
        return;
      }
    }else{
      printf("Digite o horário de inicio\n");
      scanf(" %[^\n]", e.horarioInicio);
      printf("Digite o horário de fim\n");
      scanf(" %[^\n]", e.horarioFim);
      if(verificar_duracao(e.horarioInicio, e.horarioFim)){
        printf("A festa não pode ter mais de 4 horas\n\n");
        return;
      }
    }
    if(!check_event_day_time(f, e.data, e.horarioInicio, e.codigo)){
      printf("Festa já cadastrada nesse dia e horário\n\n");
      return;
    }
    printf("Digite o tema da festa:\n");
    scanf(" %[^\n]", e.tema);
    printf("\n");
    change_contract_by_event(e.codigo, e.diaSemana, e.convidados);
    fseek(f, sizeof(e) * (posicao), SEEK_SET);
    fwrite(&e, sizeof(e), 1, f);
    fflush(f);
  } else {
    printf("Festa %d não encontrada\n", codigo);
  }
}

void delete_event(FILE *f) {
  int codigo, posicao, certeza;
  event e;
  printf("Diga qual o codigo da festa\n");
  scanf("%d", &codigo);
  posicao = locate_event(f, codigo);
  if (posicao != -1) {
    printf("Tem certeza que deseja deletar essa festa? Ela será "
           "permanentemente deletada(sim - 1, não - 0)\n");
    scanf("%d", &certeza);
    printf("\n");
    if (certeza) {
      fseek(f, sizeof(e) * (posicao), SEEK_SET);
      fread(&e, sizeof(e), 1, f);
      e.active = 0;
      delete_contract(e.codigo);
      fseek(f, sizeof(e) * (posicao), SEEK_SET);
      fwrite(&e, sizeof(e), 1, f);
      fflush(f);
    }
  } else {
    printf("Festa %d não encontrada\n", codigo);
  }
}

void delete_event_by_client(int codigoC) {
  FILE *f = open_event_db();
  int posicao;
  event e;
  fseek(f, 0, SEEK_SET);
  fread(&e, sizeof(e), 1, f);
  while (!feof(f)) {
    if (e.active && e.codigoCliente == codigoC) {
      posicao = locate_event(f, e.codigo);
      delete_contract(e.codigo);
      e.active = 0;
      fseek(f, sizeof(e) * (posicao), SEEK_SET);
      fwrite(&e, sizeof(e), 1, f);
      fflush(f);
    }
    fread(&e, sizeof(e), 1, f);
  }
}

void delete_event_by_supplier(int codigoF) {
  FILE *f = open_event_db();
  int posicao;
  event e;
  fseek(f, 0, SEEK_SET);
  fread(&e, sizeof(e), 1, f);
  while (!feof(f)) {
    if (e.active && e.codigoFornecedor == codigoF) {
      posicao = locate_event(f, e.codigo);
      delete_contract(e.codigo);
      e.active = 0;
      fseek(f, sizeof(e) * (posicao), SEEK_SET);
      fwrite(&e, sizeof(e), 1, f);
      fflush(f);
    }
    fread(&e, sizeof(e), 1, f);
  }
}

void view_events_client(FILE *f) {
  int codigoCliente, posicao, certeza;
  event e;
  printf("Qual o código do cliente\n");
  scanf("%d", &codigoCliente);
  printf("\n");
  fseek(f, 0, SEEK_SET);
  fread(&e, sizeof(e), 1, f);
  while (!feof(f)) {
    if (e.active && e.codigoCliente == codigoCliente) {
      printf("Código................: %d\n", e.codigo);
      printf("Convidados............: %d\n", e.convidados);
      printf("Data..................: %s\n", e.data);
      printf("Dia da semana.........: %s\n", e.diaSemana);
      printf("Horário...............: %s-%s\n", e.horarioInicio, e.horarioFim);
      printf("Tema..................: %s\n", e.tema);
      printf("Código do cliente.....: %d\n", e.codigoCliente);
      printf("Código do fornecedor..: %d\n", e.codigoFornecedor);
      view_contract_by_event(e.codigo);
      printf("\n");
    }
    fread(&e, sizeof(e), 1, f);
  }
}

void view_events_date(FILE *f) {
  int posicao;
  char date[100], dayDatabase[3], monthDatabase[3], yearDatabase[5], day[3], month[3], year[5];
  event e;
  printf("Qual a data da festa\n");
  scanf(" %s", date);
  printf("\n");
  fseek(f, 0, SEEK_SET);
  fread(&e, sizeof(e), 1, f);
  while (!feof(f)) {
    e.data[strcspn(e.data, "\n")] = '\0';
    date[strcspn(date, "\n")] = '\0';
    sscanf(e.data, "%2s/%2s/%2s", dayDatabase, monthDatabase, yearDatabase);
    sscanf(date, "%2s/%2s/%2s", day, month, year);
    int dayDatabaseInt = atoi(dayDatabase);
    int monthDatabaseInt = atoi(monthDatabase);
    int yearDatabaseInt = atoi(yearDatabase);
    int dayInt = atoi(day);
    int monthInt = atoi(month);
    int yearInt = atoi(year);
    
    if (e.active && (dayDatabaseInt == dayInt && monthDatabaseInt == monthInt && yearDatabaseInt == yearInt)) {
      printf("Código................: %d\n", e.codigo);
      printf("Convidados............: %d\n", e.convidados);
      printf("Data..................: %s\n", e.data);
      printf("Dia da semana.........: %s\n", e.diaSemana);
      printf("Horário...............: %s-%s\n", e.horarioInicio, e.horarioFim);
      printf("Tema..................: %s\n", e.tema);
      printf("Código do cliente.....: %d\n", e.codigoCliente);
      printf("Código do fornecedor..: %d\n", e.codigoFornecedor);
      view_contract_by_event(e.codigo);
      printf("\n");
    }
    fread(&e, sizeof(e), 1, f);
  }
}
