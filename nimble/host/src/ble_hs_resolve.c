/*
 * Copyright 2025 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "host/ble_hs.h"
#include "host/ble_store.h"
#include "ble_sm_alg_priv.h"

typedef struct ble_hs_resolve_iterator_context {
  uint8_t *peer_addr;
  uint8_t *peer_addr_type;
  uint8_t *peer_rpa_addr;
  bool resolved;
} ble_hs_resolve_iterator_context;

static int prv_irk_can_resolve_addr(uint8_t *addr, uint8_t *irk) {
  int rc;
  struct ble_encryption_block ecb = {0};

  memcpy(ecb.key, irk, 16);
  memset(ecb.plain_text, 0, 16);
  memcpy(ecb.plain_text, addr+3, 3);

  // this internally flips the byte order of the key/plain text and resulting cipher text
  rc = ble_sm_alg_encrypt(ecb.key, ecb.plain_text, ecb.cipher_text);
  if (rc != 0) {
    return rc;
  }

  if (memcmp(ecb.cipher_text, addr, 3) == 0) {
    rc = 0;
  } else {
    rc = BLE_HS_ENOENT;
  }

  return rc;
}

static int prv_iterate_cb(int obj_type, union ble_store_value *val, void *cookie) {
  ble_hs_resolve_iterator_context *context = (ble_hs_resolve_iterator_context *)cookie;

  if (val->sec.irk_present && prv_irk_can_resolve_addr(context->peer_addr, val->sec.irk) == 0) {
    memcpy(context->peer_rpa_addr, context->peer_addr, BLE_DEV_ADDR_LEN);
    memcpy(context->peer_addr, &val->sec.peer_addr.val, BLE_DEV_ADDR_LEN);
    *context->peer_addr_type = BLE_ADDR_PUBLIC_ID;
    context->resolved = true;
    return 1;
  }

  return 0;
}

void ble_hs_resolve_update_peer_id_addr(uint8_t *peer_addr, uint8_t *peer_addr_type, uint8_t *peer_rpa_addr) {
  ble_hs_resolve_iterator_context context = {
    .peer_addr = peer_addr,
    .peer_addr_type = peer_addr_type,
    .peer_rpa_addr = peer_rpa_addr,
    .resolved = false,
  };
  ble_store_iterate(BLE_STORE_OBJ_TYPE_PEER_SEC, prv_iterate_cb, &context);
}
