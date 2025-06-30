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
#include "sdc/sdc_hci_cmd_le.h"

#define BLE_HCI_UART_H4_NONE 0x00
#define BLE_HCI_UART_H4_CMD 0x01
#define BLE_HCI_UART_H4_ACL 0x02
#define BLE_HCI_UART_H4_SCO 0x03
#define BLE_HCI_UART_H4_EVT 0x04
#define BLE_HCI_UART_H4_ISO 0x05

int ble_transport_to_ll_iso_impl(struct os_mbuf *om) {
  /* From my understanding, os_mbuf works similarly to an arena allocator. I iterate over all the
   * nodes and copy them to a continuous chunk of memory */
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
  struct ble_hci_cmd *cmd = (struct ble_hci_cmd *)buf;

  uint16_t opcode = cmd->opcode;
  uint8_t ogf = opcode >> 10;
  uint8_t ocf = opcode & 0x3FF;

  uint8_t data_length = cmd->length;
  void *data = (void *)(cmd->data);

  uint8_t err = 0;

  switch (ogf) {
    case 0x01:
      switch (ocf) {
        case BLE_HCI_OCF_DISCONNECT_CMD:

          break;

        case BLE_HCI_OCF_RD_REM_VER_INFO:

          break;

        default:

          break;
      }
      break;

    case 0x02:
      /*There doesn't seem to be anything for this ogf*/
      break;

    case 0x03:
      switch (ocf) {
        case BLE_HCI_OCF_CB_SET_EVENT_MASK:
          err = sdc_hci_cmd_le_set_event_mask(data);
<<<<<<< HEAD
        break;
      
      case BLE_HCI_OCF_CB_RESET:
          
        break;
=======
          break;

        case BLE_HCI_OCF_CB_RESET:
>>>>>>> 226fc6827e443890e1192f171ba38b91e68c17c1

          break;

<<<<<<< HEAD
        b eak;
=======
        case BLE_HCI_OCF_CB_READ_TX_PWR:
>>>>>>> 226fc6827e443890e1192f171ba38b91e68c17c1

          break;

        case BLE_HCI_OCF_CB_SET_CTLR_TO_HOST_FC:

          break;

        case BLE_HCI_OCF_CB_HOST_BUF_SIZE:

          break;

        case BLE_HCI_OCF_CB_HOST_NUM_COMP_PKTS:

          break;

        case BLE_HCI_OCF_CB_SET_EVENT_MASK2:

          break;

        case BLE_HCI_OCF_CB_RD_AUTH_PYLD_TMO:

          break;

        case BLE_HCI_OCF_CB_WR_AUTH_PYLD_TMO:

          break;

        default:

          break;
      }
      break;

    case 0x04:
      switch (ocf) {
        case BLE_HCI_OCF_IP_RD_LOCAL_VER:

          break;

        case BLE_HCI_OCF_IP_RD_LOC_SUPP_CMD:

          break;

        case BLE_HCI_OCF_IP_RD_LOC_SUPP_FEAT:

          break;

        case BLE_HCI_OCF_IP_RD_BUF_SIZE:

          break;

        case BLE_HCI_OCF_IP_RD_BD_ADDR:

          break;

        default:

          break;
      }
      break;

    case 0x05:
      switch (ocf) {
        case BLE_HCI_OCF_RD_RSSI:

          break;

        default:

          break;
      }
      break;

    case 0x06:
      /*There doesn't seem to be anything for this ogf*/
      break;

    case 0x08:
      switch (ocf) {
        case BLE_HCI_OCF_LE_SET_EVENT_MASK:

          break;

        case BLE_HCI_OCF_LE_RD_BUF_SIZE:

          break;

        case BLE_HCI_OCF_LE_RD_BUF_SIZE_V2:
<<<<<<< HEAD
            err = sdc_hci_cmd_le_read_buffer_size(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_LOC_SUPP_FEAT:
            err = sdc_hci_cmd_le_read_local_supported_features(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_SET_RAND_ADDR:
            err = sdc_hci_cmd_le_set_random_address(data);
          break;

        case BLE_HCI_OCF_LE_SET_ADV_PARAMS:
            err = sdc_hci_cmd_le_set_adv_params(data);
          break;

        case BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR:
            err = sdc_hci_cmd_le_read_adv_physical_channel_tx_power(data);
          break;

        case BLE_HCI_OCF_LE_SET_ADV_DATA:
            err = sdc_hci_cmd_le_set_adv_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA:
            err = sdc_hci_cmd_le_set_scan_response_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_ADV_ENABLE:
            err = sdc_hci_cmd_le_set_adv_enable(data);
          break;

        case BLE_HCI_OCF_LE_SET_SCAN_PARAMS:
            err = sdc_hci_cmd_le_set_scan_params(data);
          break;

        case BLE_HCI_OCF_LE_SET_SCAN_ENABLE:
            err = sdc_hci_cmd_le_set_scan_enable(data);
          break;

        case BLE_HCI_OCF_LE_CREATE_CONN:
            err = sdc_hci_cmd_le_create_conn(data);
          break;

        case BLE_HCI_OCF_LE_CREATE_CONN_CANCEL:
            err = sdc_hci_cmd_le_create_conn_cancel();
          break;

        case BLE_HCI_OCF_LE_RD_WHITE_LIST_SIZE:
            err = sdc_hci_cmd_le_read_filter_accept_list_size(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_CLEAR_WHITE_LIST:
            err = sdc_hci_cmd_le_clear_filter_accept_list();
          break;

        case BLE_HCI_OCF_LE_ADD_WHITE_LIST:
            err = sdc_hci_cmd_le_add_device_to_filter_accept_list(data);
          break;

        case BLE_HCI_OCF_LE_RMV_WHITE_LIST:
            err = sdc_hci_cmd_le_remove_device_from_filter_accept_list(data);
          break;

        case BLE_HCI_OCF_LE_CONN_UPDATE:
            err = sdc_hci_cmd_le_conn_update(data);
          break;

        case BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS:
            err = sdc_hci_cmd_le_set_host_channel_classification(data);
          break;

        case BLE_HCI_OCF_LE_RD_CHAN_MAP:
            err = sdc_hci_cmd_le_read_channel_map(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_REM_FEAT:
            err = sdc_hci_cmd_le_read_remote_features(data);
          break;

        case BLE_HCI_OCF_LE_ENCRYPT:
            err = sdc_hci_cmd_le_encrypt(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RAND:
            err = sdc_hci_cmd_le_rand(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_START_ENCRYPT:
            err = sdc_hci_cmd_le_enable_encryption(data);
          break;

        case BLE_HCI_OCF_LE_LT_KEY_REQ_REPLY:
            err = sdc_hci_cmd_le_long_term_key_request_reply(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_LT_KEY_REQ_NEG_REPLY:
            err = sdc_hci_cmd_le_long_term_key_request_negative_reply(data, NULL) /*Ouput parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_SUPP_STATES:
            err = sdc_hci_cmd_le_read_supported_states(data);
=======
          err = sdc_hci_cmd_le_read_buffer_size(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_LOC_SUPP_FEAT:
          err = sdc_hci_cmd_le_read_local_supported_features(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_SET_RAND_ADDR:
          err = sdc_hci_cmd_le_set_random_address(data);
          break;

        case BLE_HCI_OCF_LE_SET_ADV_PARAMS:
          err = sdc_hci_cmd_le_set_adv_params(data);
          break;

        case BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR:
          err = sdc_hci_cmd_le_read_adv_physical_channel_tx_power(data);
          break;

        case BLE_HCI_OCF_LE_SET_ADV_DATA:
          err = sdc_hci_cmd_le_set_adv_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA:
          err = sdc_hci_cmd_le_set_scan_response_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_ADV_ENABLE:
          err = sdc_hci_cmd_le_set_adv_enable(data);
          break;

        case BLE_HCI_OCF_LE_SET_SCAN_PARAMS:
          err = sdc_hci_cmd_le_set_scan_params(data);
          break;

        case BLE_HCI_OCF_LE_SET_SCAN_ENABLE:
          err = sdc_hci_cmd_le_set_scan_enable(data);
          break;

        case BLE_HCI_OCF_LE_CREATE_CONN:
          err = sdc_hci_cmd_le_create_conn(data);
          break;

        case BLE_HCI_OCF_LE_CREATE_CONN_CANCEL:
          err = sdc_hci_cmd_le_create_conn_cancel();
          break;

        case BLE_HCI_OCF_LE_RD_WHITE_LIST_SIZE:
          err = sdc_hci_cmd_le_read_filter_accept_list_size(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_CLEAR_WHITE_LIST:
          err = sdc_hci_cmd_le_clear_filter_accept_list();
          break;

        case BLE_HCI_OCF_LE_ADD_WHITE_LIST:
          err = sdc_hci_cmd_le_add_device_to_filter_accept_list(data);
          break;

        case BLE_HCI_OCF_LE_RMV_WHITE_LIST:
          err = sdc_hci_cmd_le_remove_device_from_filter_accept_list(data);
          break;

        case BLE_HCI_OCF_LE_CONN_UPDATE:
          err = sdc_hci_cmd_le_conn_update(data);
          break;

        case BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS:
          err = sdc_hci_cmd_le_set_host_channel_classification(data);
          break;

        case BLE_HCI_OCF_LE_RD_CHAN_MAP:
          err = sdc_hci_cmd_le_read_channel_map(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_REM_FEAT:
          err = sdc_hci_cmd_le_read_remote_features(data);
          break;

        case BLE_HCI_OCF_LE_ENCRYPT:
          err = sdc_hci_cmd_le_encrypt(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RAND:
          err = sdc_hci_cmd_le_rand(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_START_ENCRYPT:
          err = sdc_hci_cmd_le_enable_encryption(data);
          break;

        case BLE_HCI_OCF_LE_LT_KEY_REQ_REPLY:
          err = sdc_hci_cmd_le_long_term_key_request_reply(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_LT_KEY_REQ_NEG_REPLY:
          err = sdc_hci_cmd_le_long_term_key_request_negative_reply(data,
                                                                    NULL); /*Ouput parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_SUPP_STATES:
          err = sdc_hci_cmd_le_read_supported_states(data);
>>>>>>> c8cc2c9ed1924ecb143c1929c553108059caae57
          break;

        case BLE_HCI_OCF_LE_RX_TEST:
            
          break;

        case BLE_HCI_OCF_LE_TX_TEST:

          break;

        case BLE_HCI_OCF_LE_TEST_END:

          break;

        case BLE_HCI_OCF_LE_REM_CONN_PARAM_RR:

          break;

        case BLE_HCI_OCF_LE_REM_CONN_PARAM_NRR:

          break;

        case BLE_HCI_OCF_LE_SET_DATA_LEN:
<<<<<<< HEAD
            err = sdc_hci_cmd_le_set_data_length(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_SUGG_DEF_DATA_LEN:
            err = sdc_hci_cmd_le_read_suggested_default_data_length(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_WR_SUGG_DEF_DATA_LEN:
            err = sdc_hci_cmd_le_write_suggested_default_data_length(data);
=======
          err = sdc_hci_cmd_le_set_data_length(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_SUGG_DEF_DATA_LEN:
          err = sdc_hci_cmd_le_read_suggested_default_data_length(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_WR_SUGG_DEF_DATA_LEN:
          err = sdc_hci_cmd_le_write_suggested_default_data_length(data);
>>>>>>> c8cc2c9ed1924ecb143c1929c553108059caae57
          break;

        case BLE_HCI_OCF_LE_RD_P256_PUBKEY:

          break;

        case BLE_HCI_OCF_LE_GEN_DHKEY:

          break;

        case BLE_HCI_OCF_LE_ADD_RESOLV_LIST:
<<<<<<< HEAD
            err = sdc_hci_cmd_le_add_device_to_resolving_list(data);
          break;

        case BLE_HCI_OCF_LE_RMV_RESOLV_LIST:
            err = sdc_hci_cmd_le_remove_device_from_resolving_list(data);
          break;

        case BLE_HCI_OCF_LE_CLR_RESOLV_LIST:
            err = sdc_hci_cmd_le_clear_resolving_list();
          break;

        case BLE_HCI_OCF_LE_RD_RESOLV_LIST_SIZE:
            err = sdc_hci_cmd_le_read_resolving_list_size(data);
=======
          err = sdc_hci_cmd_le_add_device_to_resolving_list(data);
          break;

        case BLE_HCI_OCF_LE_RMV_RESOLV_LIST:
          err = sdc_hci_cmd_le_remove_device_from_resolving_list(data);
          break;

        case BLE_HCI_OCF_LE_CLR_RESOLV_LIST:
          err = sdc_hci_cmd_le_clear_resolving_list();
          break;

        case BLE_HCI_OCF_LE_RD_RESOLV_LIST_SIZE:
          err = sdc_hci_cmd_le_read_resolving_list_size(data);
>>>>>>> c8cc2c9ed1924ecb143c1929c553108059caae57
          break;

        case BLE_HCI_OCF_LE_RD_PEER_RESOLV_ADDR:

          break;

        case BLE_HCI_OCF_LE_RD_LOCAL_RESOLV_ADDR:

          break;

        case BLE_HCI_OCF_LE_SET_ADDR_RES_EN:
<<<<<<< HEAD
            err = sdc_hci_cmd_le_set_address_resolution_enable(data);
          break;

        case BLE_HCI_OCF_LE_SET_RPA_TMO:
            err = sdc_hci_cmd_le_set_resolvable_private_address_timeout(data);
          break;

        case BLE_HCI_OCF_LE_RD_MAX_DATA_LEN:
            err = sdc_hci_cmd_le_read_max_data_length(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_PHY:
           err = sdc_hci_cmd_le_read_phy(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_SET_DEFAULT_PHY:
            err = sdc_hci_cmd_le_set_default_phy(data);
          break;

        case BLE_HCI_OCF_LE_SET_PHY:
            err = sdc_hci_cmd_le_set_phy(data);
=======
          err = sdc_hci_cmd_le_set_address_resolution_enable(data);
          break;

        case BLE_HCI_OCF_LE_SET_RPA_TMO:
          err = sdc_hci_cmd_le_set_resolvable_private_address_timeout(data);
          break;

        case BLE_HCI_OCF_LE_RD_MAX_DATA_LEN:
          err = sdc_hci_cmd_le_read_max_data_length(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_PHY:
          err = sdc_hci_cmd_le_read_phy(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_SET_DEFAULT_PHY:
          err = sdc_hci_cmd_le_set_default_phy(data);
          break;

        case BLE_HCI_OCF_LE_SET_PHY:
          err = sdc_hci_cmd_le_set_phy(data);
>>>>>>> c8cc2c9ed1924ecb143c1929c553108059caae57
          break;

        case BLE_HCI_OCF_LE_RX_TEST_V2:

          break;

        case BLE_HCI_OCF_LE_TX_TEST_V2:

          break;

        case BLE_HCI_OCF_LE_SET_ADV_SET_RND_ADDR:
<<<<<<< HEAD
            err = sdc_hci_cmd_le_set_adv_set_random_address(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM:
            err = sdc_hci_cmd_le_set_ext_adv_params(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_DATA:
            err = sdc_hci_cmd_le_set_ext_adv_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA:
            err = sdc_hci_cmd_le_set_ext_scan_response_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE:
            err = sdc_hci_cmd_le_set_ext_adv_enable(data);
          break;

        case BLE_HCI_OCF_LE_RD_MAX_ADV_DATA_LEN:
            err = sdc_hci_cmd_le_read_max_adv_data_length(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_NUM_OF_ADV_SETS:
            err = sdc_hci_cmd_le_read_number_of_supported_adv_sets(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_REMOVE_ADV_SET:
            err = sdc_hci_cmd_le_remove_adv_set(data);
          break;

        case BLE_HCI_OCF_LE_CLEAR_ADV_SETS:
            err = sdc_hci_cmd_le_clear_adv_sets();
          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS:
            err = sdc_hci_cmd_le_set_periodic_adv_params(data);
          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA:
            err = sdc_hci_cmd_le_set_periodic_adv_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE:
            err = sdc_hci_cmd_le_set_periodic_adv_enable(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM:
            err = sdc_hci_cmd_le_set_ext_scan_params(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE:
            err = sdc_hci_cmd_le_set_ext_scan_enable(data);
          break;

        case BLE_HCI_OCF_LE_EXT_CREATE_CONN:
            err = sdc_hci_cmd_le_ext_create_conn(data);
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC:
            err = sdc_hci_cmd_le_periodic_adv_create_sync(data);
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL:
            err = sdc_hci_cmd_le_periodic_adv_create_sync_cancel();
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC:
            err = sdc_hci_cmd_le_periodic_adv_terminate_sync(data);
=======
          err = sdc_hci_cmd_le_set_adv_set_random_address(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM:
          err = sdc_hci_cmd_le_set_ext_adv_params(data, NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_DATA:
          err = sdc_hci_cmd_le_set_ext_adv_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA:
          err = sdc_hci_cmd_le_set_ext_scan_response_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE:
          err = sdc_hci_cmd_le_set_ext_adv_enable(data);
          break;

        case BLE_HCI_OCF_LE_RD_MAX_ADV_DATA_LEN:
          err = sdc_hci_cmd_le_read_max_adv_data_length(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_NUM_OF_ADV_SETS:
          err = sdc_hci_cmd_le_read_number_of_supported_adv_sets(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_REMOVE_ADV_SET:
          err = sdc_hci_cmd_le_remove_adv_set(data);
          break;

        case BLE_HCI_OCF_LE_CLEAR_ADV_SETS:
          err = sdc_hci_cmd_le_clear_adv_sets();
          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS:
          err = sdc_hci_cmd_le_set_periodic_adv_params(data);
          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA:
          err = sdc_hci_cmd_le_set_periodic_adv_data(data);
          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE:
          err = sdc_hci_cmd_le_set_periodic_adv_enable(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM:
          err = sdc_hci_cmd_le_set_ext_scan_params(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE:
          err = sdc_hci_cmd_le_set_ext_scan_enable(data);
          break;

        case BLE_HCI_OCF_LE_EXT_CREATE_CONN:
          err = sdc_hci_cmd_le_ext_create_conn(data);
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC:
          err = sdc_hci_cmd_le_periodic_adv_create_sync(data);
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL:
          err = sdc_hci_cmd_le_periodic_adv_create_sync_cancel();
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC:
          err = sdc_hci_cmd_le_periodic_adv_terminate_sync(data);
>>>>>>> c8cc2c9ed1924ecb143c1929c553108059caae57
          break;

        case BLE_HCI_OCF_LE_ADD_DEV_TO_PERIODIC_ADV_LIST:
          err = sdc_hci_cmd_le_add_device_to_periodic_adv_list(data);
          break;

        case BLE_HCI_OCF_LE_REM_DEV_FROM_PERIODIC_ADV_LIST:
          err = sdc_hci_cmd_le_remove_device_from_periodic_adv_list(data);
          break;

        case BLE_HCI_OCF_LE_CLEAR_PERIODIC_ADV_LIST:
          err = sdc_hci_cmd_le_clear_periodic_adv_list();
          break;

        case BLE_HCI_OCF_LE_RD_PERIODIC_ADV_LIST_SIZE:
          err = sdc_hci_cmd_le_read_periodic_adv_list_size(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_TRANSMIT_POWER:
          err = sdc_hci_cmd_le_read_transmit_power(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_RD_RF_PATH_COMPENSATION:
          err = sdc_hci_cmd_le_read_rf_path_compensation(NULL); /*Output parameter here*/
          break;

        case BLE_HCI_OCF_LE_WR_RF_PATH_COMPENSATION:
          err = sdc_hci_cmd_le_write_rf_path_compensation(data);
          break;

        case BLE_HCI_OCF_LE_SET_PRIVACY_MODE:

          break;

        case BLE_HCI_OCF_LE_RX_TEST_V3:

          break;

        case BLE_HCI_OCF_LE_TX_TEST_V3:

          break;

        case BLE_HCI_OCF_LE_SET_CONNLESS_CTE_TX_PARAMS:
          err = sdc_hci_cmd_le_set_connless_cte_transmit_params(data);
          break;

        case BLE_HCI_OCF_LE_SET_CONNLESS_CTE_TX_ENABLE:
          err = sdc_hci_cmd_le_set_connless_cte_transmit_enable(data);
          break;

        case BLE_HCI_OCF_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_CONN_CTE_RX_PARAMS:

          break;

        case BLE_HCI_OCF_LE_SET_CONN_CTE_TX_PARAMS:
          err = sdc_hci_cmd_le_set_conn_cte_transmit_params(data, NULL);
          break;

        case BLE_HCI_OCF_LE_SET_CONN_CTE_REQ_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_CONN_CTE_RESP_ENABLE:
          err = sdc_hci_cmd_le_conn_cte_response_enable(data, NULL);
          break;

        case BLE_HCI_OCF_LE_RD_ANTENNA_INFO:
          err = sdc_hci_cmd_le_read_antenna_information(data);
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_RECEIVE_ENABLE:
          err = sdc_hci_cmd_le_set_periodic_adv_receive_enable(data);
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER:
          err = sdc_hci_cmd_le_periodic_adv_sync_transfer(data, NULL);
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_SET_INFO_TRANSFER:
          err = sdc_hci_cmd_le_periodic_adv_set_info_transfer(data, NULL);
          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER_PARAMS:
          err = sdc_hci_cmd_le_set_periodic_adv_sync_transfer_params(data, NULL);
          break;

        case BLE_HCI_OCF_LE_SET_DEFAULT_SYNC_TRANSFER_PARAMS:
          err = sdc_hci_cmd_le_set_default_periodic_adv_sync_transfer_params(data);
          break;

        case BLE_HCI_OCF_LE_GENERATE_DHKEY_V2:

          break;

        case BLE_HCI_OCF_LE_MODIFY_SCA:

          break;

        case BLE_HCI_OCF_LE_READ_ISO_TX_SYNC:
          err = sdc_hci_cmd_le_read_iso_tx_sync(data, NULL);
          break;

        case BLE_HCI_OCF_LE_SET_CIG_PARAMS:
          err = sdc_hci_cmd_le_set_cig_params(data, NULL);
          break;

        case BLE_HCI_OCF_LE_SET_CIG_PARAMS_TEST:
          err = sdc_hci_cmd_le_set_cig_params_test(data, NULL);
          break;

        case BLE_HCI_OCF_LE_CREATE_CIS:
          err = sdc_hci_cmd_le_create_cis(data);
          break;

        case BLE_HCI_OCF_LE_REMOVE_CIG:
          err = sdc_hci_cmd_le_remove_cig(data, NULL);
          break;

        case BLE_HCI_OCF_LE_ACCEPT_CIS_REQ:
          err = sdc_hci_cmd_le_accept_cis_request(data);
          break;

        case BLE_HCI_OCF_LE_REJECT_CIS_REQ:
          err = sdc_hci_cmd_le_reject_cis_request(data, NULL);
          break;

        case BLE_HCI_OCF_LE_CREATE_BIG:
          err = sdc_hci_cmd_le_create_big(data);
          break;

        case BLE_HCI_OCF_LE_CREATE_BIG_TEST:
          err = sdc_hci_cmd_le_create_big_test(data);
          break;

        case BLE_HCI_OCF_LE_TERMINATE_BIG:
          err = sdc_hci_cmd_le_terminate_big(data);
          break;

        case BLE_HCI_OCF_LE_BIG_CREATE_SYNC:
          err = sdc_hci_cmd_le_big_create_sync(data);
          break;

        case BLE_HCI_OCF_LE_BIG_TERMINATE_SYNC:
          err = sdc_hci_cmd_le_big_terminate_sync(data, NULL);
          break;

        case BLE_HCI_OCF_LE_REQ_PEER_SCA:
          err = sdc_hci_cmd_le_request_peer_sca(data);
          break;

        case BLE_HCI_OCF_LE_SETUP_ISO_DATA_PATH:
          err = sdc_hci_cmd_le_setup_iso_data_path(data, NULL);
          break;

        case BLE_HCI_OCF_LE_REMOVE_ISO_DATA_PATH:
          err = sdc_hci_cmd_le_remove_iso_data_path(data, NULL);
          break;

        case BLE_HCI_OCF_LE_ISO_TRANSMIT_TEST:

          break;

        case BLE_HCI_OCF_LE_ISO_RECEIVE_TEST:
          err = sdc_hci_cmd_le_iso_receive_test(data, NULL);
          break;

        case BLE_HCI_OCF_LE_ISO_READ_TEST_COUNTERS:

          break;

        case BLE_HCI_OCF_LE_ISO_TEST_END:
          err = sdc_hci_cmd_le_iso_test_end(data, NULL);
          break;

        case BLE_HCI_OCF_LE_SET_HOST_FEATURE:

          break;

        case BLE_HCI_OCF_LE_READ_ISO_LINK_QUALITY:

          break;

        case BLE_HCI_OCF_LE_ENH_READ_TRANSMIT_POWER_LEVEL:
          err = sdc_hci_cmd_le_enhanced_read_transmit_power_level(data, NULL);
          break;

        case BLE_HCI_OCF_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL:

          break;

        case BLE_HCI_OCF_LE_SET_PATH_LOSS_REPORT_PARAM:
          err = sdc_hci_cmd_le_set_path_loss_reporting_params(data, NULL);
          break;

        case BLE_HCI_OCF_LE_SET_PATH_LOSS_REPORT_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_TRANS_PWR_REPORT_ENABLE:
          err = sdc_hci_cmd_le_set_transmit_power_reporting_enable(data, NULL);
          break;

        case BLE_HCI_OCF_LE_SET_DEFAULT_SUBRATE:
          err = sdc_hci_cmd_le_set_default_subrate(data);
          break;

        case BLE_HCI_OCF_LE_SUBRATE_REQ:
          err = sdc_hci_cmd_le_subrate_request(data);
          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM_V2:
          err = sdc_hci_cmd_le_set_ext_adv_params_v2(data, NULL);
          break;

        case BLE_HCI_OCF_LE_CS_RD_LOC_SUPP_CAP:
          err = sdc_hci_cmd_le_cs_read_local_supported_capabilities(data);
          break;

        case BLE_HCI_OCF_LE_CS_RD_REM_SUPP_CAP:
          err = sdc_hci_cmd_le_cs_read_remote_supported_capabilities(data);
          break;

        case BLE_HCI_OCF_LE_CS_WR_CACHED_REM_SUPP_CAP:
          err = sdc_hci_cmd_le_cs_write_cached_remote_supported_capabilities(data, NULL);
          break;

        case BLE_HCI_OCF_LE_CS_SEC_ENABLE:
          err = sdc_hci_cmd_le_cs_security_enable(data);
          break;

        case BLE_HCI_OCF_LE_CS_SET_DEF_SETTINGS:
          err = sdc_hci_cmd_le_cs_set_default_settings(data, NULL);
          break;

        case BLE_HCI_OCF_LE_CS_RD_REM_FAE:
          err = sdc_hci_cmd_le_cs_read_remote_fae_table(data);
          break;

        case BLE_HCI_OCF_LE_CS_WR_CACHED_REM_FAE:
          err = sdc_hci_cmd_le_cs_write_cached_remote_fae_table(data, NULL);
          break;

        case BLE_HCI_OCF_LE_CS_CREATE_CONFIG:
          err = sdc_hci_cmd_le_cs_create_config(data);
          break;

        case BLE_HCI_OCF_LE_CS_REMOVE_CONFIG:
          err = sdc_hci_cmd_le_cs_remove_config(data);
          break;

        case BLE_HCI_OCF_LE_CS_SET_CHAN_CLASS:
          err = sdc_hci_cmd_le_cs_set_channel_classification(data);
          break;

        case BLE_HCI_OCF_LE_CS_SET_PROC_PARAMS:
          err = sdc_hci_cmd_le_cs_set_procedure_params(data, NULL);
          break;

        case BLE_HCI_OCF_LE_CS_PROC_ENABLE:
          err = sdc_hci_cmd_le_cs_procedure_enable(data);
          break;

        case BLE_HCI_OCF_LE_CS_TEST:
          err = sdc_hci_cmd_le_cs_test(data);
          break;

        case BLE_HCI_OCF_LE_CS_TEST_END:

          break;

        default:

          break;
      }
      break;

    case 0x3F:

      switch (ocf) {
        case BLE_HCI_OCF_VS_RD_STATIC_ADDR:

          break;

        case BLE_HCI_OCF_VS_SET_TX_PWR:

          break;

        case BLE_HCI_OCF_VS_CSS_CONFIGURE:

          break;

        case BLE_HCI_OCF_VS_CSS_ENABLE:

          break;

        case BLE_HCI_OCF_VS_CSS_SET_NEXT_SLOT:

          break;

        case BLE_HCI_OCF_VS_CSS_SET_CONN_SLOT:

          break;

        case BLE_HCI_OCF_VS_CSS_READ_CONN_SLOT:

          break;

        case BLE_HCI_OCF_VS_SET_DATA_LEN:

          break;

        case BLE_HCI_OCF_VS_SET_ANTENNA:

          break;

        case BLE_HCI_OCF_VS_SET_LOCAL_IRK:

          break;

        case BLE_HCI_OCF_VS_SET_SCAN_CFG:

          break;

        default:

          break;
      }

      break;

    default:

        break;
  }
  return -err;
}
