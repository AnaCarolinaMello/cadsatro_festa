#include "client.h"
#include "events.h"
#include "supplier.h"
#include "contract.h"
#include "paymentData.h"
#include "test.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  // test_all_functions();
  setlocale(LC_ALL, "portuguese");
  int opcao;
  do {
    printf("Escolha:\n");
    printf("1 - Operações com clientes\n");
    printf("2 - Operações com fornecedores\n");
    printf("3 - Operações com festas\n");
    printf("4 - Operações com contratos\n");
    printf("5 - Consulte os valores, descontos e forma de pagamento disponíveis\n");
    printf("6 - Sair do sistema\n");
    scanf("%d", &opcao);
    printf("\n");
    if (opcao == 1) {
      start_clients_operation();
    } else if (opcao == 2) {
      start_supplier_operation();
    } else if (opcao == 3) {
      start_events_operation();
    } else if (opcao == 4) {
      start_contract_operation();
    } else if (opcao == 5) {
      start_payment_operation();
    }
  } while (opcao != 6);
  return 0;
}