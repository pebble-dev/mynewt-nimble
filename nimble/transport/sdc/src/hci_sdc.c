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
      switch (ocf)
      {
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
      switch (ocf)
      {
      case BLE_HCI_OCF_CB_SET_EVENT_MASK:
          err = sdc_hci_cmd_le_set_event_mask(data);
        break;
      
      case BLE_HCI_OCF_CB_RESET:

        break;

      case BLE_HCI_OCF_CB_READ_TX_PWR:

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
      switch (ocf)
      {
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
      switch (ocf)
      {
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
      switch (ocf)
      {
        case BLE_HCI_OCF_LE_SET_EVENT_MASK:

          break;

        case BLE_HCI_OCF_LE_RD_BUF_SIZE:

          break;

        case BLE_HCI_OCF_LE_RD_BUF_SIZE_V2:

          break;

        case BLE_HCI_OCF_LE_RD_LOC_SUPP_FEAT:

          break;

        case BLE_HCI_OCF_LE_SET_RAND_ADDR:

          break;

        case BLE_HCI_OCF_LE_SET_ADV_PARAMS:

          break;

        case BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR:

          break;

        case BLE_HCI_OCF_LE_SET_ADV_DATA:

          break;

        case BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA:

          break;

        case BLE_HCI_OCF_LE_SET_ADV_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_SCAN_PARAMS:

          break;
        
        case BLE_HCI_OCF_LE_SET_SCAN_ENABLE:

          break;

        case BLE_HCI_OCF_LE_CREATE_CONN:

          break;

        case BLE_HCI_OCF_LE_CREATE_CONN_CANCEL:

          break;

        case BLE_HCI_OCF_LE_RD_WHITE_LIST_SIZE:

          break;

        case BLE_HCI_OCF_LE_CLEAR_WHITE_LIST:

          break;

        case BLE_HCI_OCF_LE_ADD_WHITE_LIST:

          break;

        case BLE_HCI_OCF_LE_RMV_WHITE_LIST:

          break;
        
        case BLE_HCI_OCF_LE_CONN_UPDATE:

          break;

        case BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS:

          break;

        case BLE_HCI_OCF_LE_RD_CHAN_MAP:

          break;

        case BLE_HCI_OCF_LE_RD_REM_FEAT:

          break;
      
        case BLE_HCI_OCF_LE_ENCRYPT:

          break;

        case BLE_HCI_OCF_LE_RAND:

          break;

        case BLE_HCI_OCF_LE_START_ENCRYPT:

          break;
        
        case BLE_HCI_OCF_LE_LT_KEY_REQ_REPLY:

          break;

        case BLE_HCI_OCF_LE_LT_KEY_REQ_NEG_REPLY:

          break;

        case BLE_HCI_OCF_LE_RD_SUPP_STATES:

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

          break;

        case BLE_HCI_OCF_LE_RD_SUGG_DEF_DATA_LEN:

          break;

        case BLE_HCI_OCF_LE_WR_SUGG_DEF_DATA_LEN:

          break;

        case BLE_HCI_OCF_LE_RD_P256_PUBKEY:

          break;

        case BLE_HCI_OCF_LE_GEN_DHKEY:

          break;

        case BLE_HCI_OCF_LE_ADD_RESOLV_LIST:

          break;

        case BLE_HCI_OCF_LE_RMV_RESOLV_LIST:

          break;

        case BLE_HCI_OCF_LE_CLR_RESOLV_LIST:

          break;

        case BLE_HCI_OCF_LE_RD_RESOLV_LIST_SIZE:

          break;

        case BLE_HCI_OCF_LE_RD_PEER_RESOLV_ADDR:

          break;

        case BLE_HCI_OCF_LE_RD_LOCAL_RESOLV_ADDR:

          break;

        case BLE_HCI_OCF_LE_SET_ADDR_RES_EN:

          break;

        case BLE_HCI_OCF_LE_SET_RPA_TMO:

          break;

        case BLE_HCI_OCF_LE_RD_MAX_DATA_LEN:

          break;

        case BLE_HCI_OCF_LE_RD_PHY:

          break;

        case BLE_HCI_OCF_LE_SET_DEFAULT_PHY:

          break;

        case BLE_HCI_OCF_LE_SET_PHY:

          break;

        case BLE_HCI_OCF_LE_RX_TEST_V2:

          break;

        case BLE_HCI_OCF_LE_TX_TEST_V2:

          break;

        case BLE_HCI_OCF_LE_SET_ADV_SET_RND_ADDR:

          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM:

          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_DATA:

          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA:

          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE:

          break;

        case BLE_HCI_OCF_LE_RD_MAX_ADV_DATA_LEN:

          break;

        case BLE_HCI_OCF_LE_RD_NUM_OF_ADV_SETS:

          break;

        case BLE_HCI_OCF_LE_REMOVE_ADV_SET:

          break;
        
        case BLE_HCI_OCF_LE_CLEAR_ADV_SETS:

          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS:

          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA:

          break;

        case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM:

          break;

        case BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE:

          break;

        case BLE_HCI_OCF_LE_EXT_CREATE_CONN:

          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC:

          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL:

          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC:

          break;

        case BLE_HCI_OCF_LE_ADD_DEV_TO_PERIODIC_ADV_LIST:

          break;

        case BLE_HCI_OCF_LE_REM_DEV_FROM_PERIODIC_ADV_LIST:

          break;

        case BLE_HCI_OCF_LE_CLEAR_PERIODIC_ADV_LIST:

          break;

        case BLE_HCI_OCF_LE_RD_PERIODIC_ADV_LIST_SIZE:

          break;

        case BLE_HCI_OCF_LE_RD_TRANSMIT_POWER:

          break;

        case BLE_HCI_OCF_LE_RD_RF_PATH_COMPENSATION:

          break;

        case BLE_HCI_OCF_LE_WR_RF_PATH_COMPENSATION:

          break;

        case BLE_HCI_OCF_LE_SET_PRIVACY_MODE:

          break;

        case BLE_HCI_OCF_LE_RX_TEST_V3:

          break;

        case BLE_HCI_OCF_LE_TX_TEST_V3:

          break;

        case BLE_HCI_OCF_LE_SET_CONNLESS_CTE_TX_PARAMS:

          break;

        case BLE_HCI_OCF_LE_SET_CONNLESS_CTE_TX_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_CONN_CTE_RX_PARAMS:

          break;

        case BLE_HCI_OCF_LE_SET_CONN_CTE_TX_PARAMS:

          break;

        case BLE_HCI_OCF_LE_SET_CONN_CTE_REQ_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_CONN_CTE_RESP_ENABLE:

          break;

        case BLE_HCI_OCF_LE_RD_ANTENNA_INFO:

          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_RECEIVE_ENABLE:

          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER:

          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_SET_INFO_TRANSFER:

          break;

        case BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER_PARAMS:

          break;

        case BLE_HCI_OCF_LE_SET_DEFAULT_SYNC_TRANSFER_PARAMS:

          break;

        case BLE_HCI_OCF_LE_GENERATE_DHKEY_V2:

          break;

        case BLE_HCI_OCF_LE_MODIFY_SCA:

          break;

        case BLE_HCI_OCF_LE_READ_ISO_TX_SYNC:

          break;

        case BLE_HCI_OCF_LE_SET_CIG_PARAMS:

          break;

        case BLE_HCI_OCF_LE_SET_CIG_PARAMS_TEST:

          break;

        case BLE_HCI_OCF_LE_CREATE_CIS:

          break;

        case BLE_HCI_OCF_LE_REMOVE_CIG:

          break;

        case BLE_HCI_OCF_LE_ACCEPT_CIS_REQ:

          break;

        case BLE_HCI_OCF_LE_REJECT_CIS_REQ:

          break;

        case BLE_HCI_OCF_LE_CREATE_BIG:

          break;

        case BLE_HCI_OCF_LE_CREATE_BIG_TEST:

          break;

        case BLE_HCI_OCF_LE_TERMINATE_BIG:

          break;

        case BLE_HCI_OCF_LE_BIG_CREATE_SYNC:

          break;

        case BLE_HCI_OCF_LE_BIG_TERMINATE_SYNC:

          break;

        case BLE_HCI_OCF_LE_REQ_PEER_SCA:

          break;

        case BLE_HCI_OCF_LE_SETUP_ISO_DATA_PATH:

          break;

        case BLE_HCI_OCF_LE_REMOVE_ISO_DATA_PATH:

          break;

        case BLE_HCI_OCF_LE_ISO_TRANSMIT_TEST:

          break;

        case BLE_HCI_OCF_LE_ISO_RECEIVE_TEST:

          break;

        case BLE_HCI_OCF_LE_ISO_READ_TEST_COUNTERS:

          break;

        case BLE_HCI_OCF_LE_ISO_TEST_END:

          break;

        case BLE_HCI_OCF_LE_SET_HOST_FEATURE:

          break;

        case BLE_HCI_OCF_LE_READ_ISO_LINK_QUALITY:

          break;

        case BLE_HCI_OCF_LE_ENH_READ_TRANSMIT_POWER_LEVEL:

          break;

        case BLE_HCI_OCF_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL:

          break;

        case BLE_HCI_OCF_LE_SET_PATH_LOSS_REPORT_PARAM:

          break;

        case BLE_HCI_OCF_LE_SET_PATH_LOSS_REPORT_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_TRANS_PWR_REPORT_ENABLE:

          break;

        case BLE_HCI_OCF_LE_SET_DEFAULT_SUBRATE:

          break;

        case BLE_HCI_OCF_LE_SUBRATE_REQ:

          break;

        case BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM_V2:

          break;

        case BLE_HCI_OCF_LE_CS_RD_LOC_SUPP_CAP:

          break;

        case BLE_HCI_OCF_LE_CS_RD_REM_SUPP_CAP:

          break;

        case BLE_HCI_OCF_LE_CS_WR_CACHED_REM_SUPP_CAP:

          break;

        case BLE_HCI_OCF_LE_CS_SEC_ENABLE:

          break;

        case BLE_HCI_OCF_LE_CS_SET_DEF_SETTINGS:

          break;

        case BLE_HCI_OCF_LE_CS_RD_REM_FAE:

          break;

        case BLE_HCI_OCF_LE_CS_WR_CACHED_REM_FAE:

          break;

        case BLE_HCI_OCF_LE_CS_CREATE_CONFIG:

          break;

        case BLE_HCI_OCF_LE_CS_REMOVE_CONFIG:

          break;

        case BLE_HCI_OCF_LE_CS_SET_CHAN_CLASS:

          break;
        
        case BLE_HCI_OCF_LE_CS_SET_PROC_PARAMS:

          break;

        case BLE_HCI_OCF_LE_CS_PROC_ENABLE:

          break;

        case BLE_HCI_OCF_LE_CS_TEST:

          break;

        case BLE_HCI_OCF_LE_CS_TEST_END:

          break;

        default:

          break;
      }
      break;

    case 0x3F:

      switch (ocf)
      {
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

        case BLE_HCI_VS_SET_SCAN_CFG_FLAG_NO_LEGACY:

          break;

        case BLE_HCI_VS_SET_SCAN_CFG_FLAG_NO_EXT:

          break;

        case BLE_HCI_VS_SET_SCAN_CFG_FLAG_RSSI_FILTER:

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

}
