/******************************************************************************
 * Copyright (c) 2024 Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *****************************************************************************/
#ifndef DRIVERS_TL321X_EXT_MISC_H_
#define DRIVERS_TL321X_EXT_MISC_H_

//#include "drivers/TL321X/reg_include/core_reg.h" //#include "nds_intrinsic.h"

#include "types.h"
#include "../compatibility_pack/cmpt.h"
#include "../lib/include/analog.h"
#include "../adc.h"
#include "../gpio.h"
#include "../lib/include/stimer.h"
#include "../lib/include/pm/pm.h"
#include "lib/include/rf/rf_common.h"
#include "../lib/include/trng/trng.h"
#include <stdbool.h>

/*
 * addr - only 0x00012 ~ 0x00021 can be used !!! */
#define write_log32(err_code)           write_sram32(0x00014, err_code)

/******************************* pke_start ******************************************************************/
#define ismemzero4(a, len)              uint32_BigNum_Check_Zero(a, len)    //For compatible with B91
/******************************* pke_end ********************************************************************/


/******************************* analog_start ******************************************************************/
#define analog_write                analog_write_reg8
#define analog_read                 analog_read_reg8
/******************************* analog_end ********************************************************************/



/******************************* core_start ******************************************************************/
#define irq_disable                 core_interrupt_disable
#define irq_enable                  core_interrupt_enable
#define irq_restore(en)             core_restore_interrupt(en)

/******************************* core_end ********************************************************************/




/******************************* efuse start *****************************************************************/

bool efuse_get_mac_address(u8* mac_read, int length);
/******************************* efuse end *******************************************************************/




/******************************* gpio start ******************************************************************/

#define reg_gpio_out    reg_gpio_out_set_clear
/**
 * @brief     This function read a pin's cache from the buffer.
 * @param[in] pin - the pin needs to read.
 * @param[in] p - the buffer from which to read the pin's level.
 * @return    the state of the pin.
 */
static inline unsigned int gpio_read_cache(gpio_pin_e pin, unsigned char *p)
{
    return p[pin>>8] & (pin & 0xff);
}

/**
 * @brief      This function read all the pins' input level.
 * @param[out] p - the buffer used to store all the pins' input level
 * @return     none
 */
static inline void gpio_read_all(unsigned char *p)
{
    //TODO: gpio_get_level_all(p);
    p[0] = reg_gpio_pa_in;
    p[1] = reg_gpio_pb_in;
    p[2] = reg_gpio_pc_in;
    p[3] = reg_gpio_pd_in;
    p[4] = reg_gpio_pe_in;
}

/**
 *  @brief  Define pull up or down types
 */
typedef enum {
    PM_PIN_UP_DOWN_FLOAT    = 0,
    PM_PIN_PULLUP_1M        = 1,
    PM_PIN_PULLDOWN_100K    = 2,
    PM_PIN_PULLUP_10K       = 3,
}gpio_pull_type;

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 */
void gpio_setup_up_down_resistor(gpio_pin_e gpio, gpio_pull_type up_down);
/******************************* gpio end ********************************************************************/




/******************************* rf tart **********************************************************************/
/**
 * @brief     This function serves to set BLE mode of RF.
 * @return    none.
 */
void rf_drv_ble_init(void);

#define RF_POWER_P3dBm   RF_POWER_INDEX_P2p99dBm
#define RF_POWER_P0dBm   RF_POWER_INDEX_N0p07dBm
#define RF_POWER_P9dBm   RF_POWER_INDEX_P8p95dBm

#if RF_THREE_CHANNEL_CALIBRATION

/**
 *  @brief      this function serve to set the TX power calibration.
 *  @param[in]  channel_power: channel power calibration of 40 channel.
 *  @return     none
*/
void rf_set_channel_power_calibration(unsigned char *channel_power);

/**
 *  @brief      this function serve to enable the rx timing sequence adjusted.
 *  @param[in]  enable: channel power calibration enable or disable.
 *  @return     none
*/
void rf_set_channel_power_enable(unsigned char enable);
#endif

/******************************* rf end  **********************************************************************/




/******************************* trng_start ******************************************************************/
#define rand                        trng_rand
#define random_generator_init       trng_init


/**
 * @brief     This function performs to generate a series of random numbers
 * @param[in]  len - data length
 * @param[out] data - data pointer
 * @return    none
 **/
void generateRandomNum(int len, unsigned char *data);

/******************************* trng_end ********************************************************************/


/******************************* stimer start ******************************************************************/
#define sleep_us(x)                 delay_us(x)
#define sleep_ms(x)                 delay_ms(x)
/******************************* stimer end ********************************************************************/


/******************************* usb_end *********************************************************************/
#define reg_usb_irq                 REG_ADDR8(0x100839)
/******************************* usb_end *********************************************************************/

/******************************* dma_start ***************************************************************/

//4(DMA_len) + 2(BLE header) + ISORxOct + 4(MIC) + 3(CRC) + 8(ExtraInfo)
#define     TLK_RF_RX_EXT_LEN       (21)

//10 = 4(DMA_len) + 2(BLE header) + 4(MIC)
#define     TLK_RF_TX_EXT_LEN       (10)

/**
 * @brief   RX Data buffer length = n + 21, then 16 Byte align
 *          n + 21 = 4(DMA_len) + 2(BLE header) + n + 4(MIC) + 3(CRC) + 8(ExtraInfo)
            RX buffer size must be be 16*n, due to MCU design
 */
#define     CAL_LL_COMMON_RX_FIFO_SIZE(n)       (((n + TLK_RF_RX_EXT_LEN) + 15) / 16 *16)
#define     CAL_LL_ACL_RX_FIFO_SIZE(maxRxOct)   CAL_LL_COMMON_RX_FIFO_SIZE(maxRxOct)
#define     CAL_LL_CIS_RX_FIFO_SIZE(cisRxPdu)   CAL_LL_COMMON_RX_FIFO_SIZE(cisRxPdu)

/**
 * @brief   ACL TX Data buffer length = maxTxOct + 10, then 16 Byte align
 *          maxTxOct + 10 = 4(DMA_len) + 2(BLE header) + maxTxOct + 4(MIC)
            TX buffer size must be be 16*n, due to MCU design
 */
#define     CAL_LL_ACL_TX_FIFO_SIZE(maxTxOct)   (((maxTxOct+TLK_RF_TX_EXT_LEN) + 15) / 16 *16)


/*HCI TX RX buffer len = uart_fifo+ dma 4byte */
#define     HCI_FIFO_SIZE(n)                    (((n+2+4) + 15) / 16 *16)





/*
 * @brief   ISO RX Data buffer length = ISORxOct + 21, then 16 Byte align
 *          ISORxOct + 21 = 4(DMA_len) + 2(BLE header) + ISORxOct + 4(MIC) + 3(CRC) + 8(ExtraInfo)
 *          RX buffer size must be be 16*n, due to MCU design
 */
#define     CAL_LL_ISO_RX_FIFO_SIZE(n)          (((n + TLK_RF_RX_EXT_LEN) + 15) / 16 * 16)


/*
 * @brief   ISO TX Data buffer length = ISOTxOct + 10, then 16 Byte align
 *          ISORxOct + 10 = 4(DMA_len) + 2(BLE header) + ISOTxOct + 4(MIC)
 *          TX buffer size must be be 16*n, due to MCU design
 */
#define     CAL_LL_ISO_TX_FIFO_SIZE(n)          (((n + TLK_RF_TX_EXT_LEN) + 15) / 16 * 16)


/*
* DMA_LEN(4B)+Hdr(2B)+PLD(251B)+MIC(4B)+CRC(3B)+TLK_PKT_INFO(12B)
*             **use 2B enough**
*/
#define     ISO_BIS_RX_PDU_SIZE_ALIGN16(n)          (((n + 25) + 15) / 16 * 16) //4+2+4+2+4+3+12

//12 = 4(struct bis_rx_pdu_tag  *next) + 4(u32 payloadNum) + 4(u32 idealPldAnchorTick) in bis_rx_pdu_t
#define     BIS_LL_RX_PDU_FIFO_SIZE(n)              (CAL_LL_ISO_RX_FIFO_SIZE(n) + 12)

/******************************* dma_end ********************************************************************/



/******************************* plic_start ******************************************************************/
enum{//todo: SunWei &YeYang
    FLD_IRQ_ZB_RT_EN = 15,
};
/******************************* plic_end ********************************************************************/


#endif /* DRIVERS_TL321X_EXT_MISC_H_ */
