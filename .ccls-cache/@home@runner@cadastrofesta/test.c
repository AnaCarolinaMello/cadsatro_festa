#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "mockClient.h"
#include "mockSupplier.h"
#include "mockEvent.h"
#include "mockContract.h"
#include "mockPaymentData.h"

void test_all_functions() {
  test_all_client_functions();
  test_all_supplier_functions();
  test_all_event_functions();
  test_all_contract_functions();
  test_all_payment_data_functions();
}
