/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdlib.h>

/* BLE */
#include "nimble/ble.h"
#include "nimble/hci_common.h"
#include "nimble/nimble_npl.h"
#include "nimble/nimble_opt.h"
#include "nimble/transport.h"
#include "sdc/sdc.h"
#include "sdc/sdc_hci.h"

#define BLE_HCI_UART_H4_NONE 0x00
#define BLE_HCI_UART_H4_CMD 0x01
#define BLE_HCI_UART_H4_ACL 0x02
#define BLE_HCI_UART_H4_SCO 0x03
#define BLE_HCI_UART_H4_EVT 0x04
#define BLE_HCI_UART_H4_ISO 0x05

int ble_transport_to_ll_iso_impl(struct os_mbuf *om) {
  /* From my understanding, os_mbuf works similarly to an arena allocator. I iterate over all the
   * nodes and copy them to a contiguous chunk of memory */
  size_t len = 1;
  for (struct os_mbuf *m = om; m; m = SLIST_NEXT(m, om_next)) {
    len += m->om_len;
  }

  uint8_t *buf = malloc(len);  // TODO: Don't use malloc

  buf[0] = BLE_HCI_UART_H4_ISO;

  size_t i = 1;
  for (struct os_mbuf *m = om; m; m = SLIST_NEXT(m, om_next)) {
    memcpy(&buf[i], m->om_data, m->om_len);
    i += m->om_len;
  }

  int32_t err = sdc_hci_iso_data_put(buf);

  free(buf);

  return err;
}

static void fault_handler_(const char *file, const uint32_t line) {
  return;  // TODO: Log error
}

static void sdc_callback_(void) { return; }

void ble_transport_ll_init(void) {
  int32_t err;

  err = sdc_init(fault_handler_);
  if (err < 0) {
    return;
  }

  err = sdc_cfg_set(SDC_DEFAULT_RESOURCE_CFG_TAG, SDC_CFG_TYPE_NONE, NULL);
  if (err < 0) {
    return;
  }

  uint8_t *mem = malloc(err);
  err = sdc_enable(sdc_callback_, mem);

  free(mem);

  return;
}

int ble_transport_to_ll_acl_impl(struct os_mbuf *om) {
  size_t len = 1;
  for (struct os_mbuf *m = om; m; m = SLIST_NEXT(m, om_next)) {
    len += m->om_len;
  }

  uint8_t *buf = malloc(len);

  buf[0] = BLE_HCI_UART_H4_ACL;

  size_t i = 1;
  for (struct os_mbuf *m = om; m; m = SLIST_NEXT(m, om_next)) {
    memcpy(&buf[i], m->om_data, m->om_len);
    i += m->om_len;
  }

  int32_t err = sdc_hci_data_put(buf);

  free(buf);

  return err;
}

int ble_transport_to_ll_cmd_impl(void *buf) {
  return 0;  // How do I implement this?
}
