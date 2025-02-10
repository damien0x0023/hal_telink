/******************************************************************************
 * Copyright (c) 2023 Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

#include "tl_rf_power.h"
#if CONFIG_SOC_RISCV_TELINK_B91 || CONFIG_SOC_RISCV_TELINK_B92
 #include <rf.h>
#elif CONFIG_SOC_RISCV_TELINK_B95 || CONFIG_SOC_RISCV_TELINK_TL321X || CONFIG_SOC_RISCV_TELINK_TL721X
#include <rf_common.h>
#endif

#if CONFIG_SOC_RISCV_TELINK_B91
/* TX power B91 lookup table */
const uint8_t tl_tx_pwr_lt[] = {
	RF_POWER_N30dBm,        /* -30.0 dBm: -30 */
	RF_POWER_N30dBm,        /* -30.0 dBm: -29 */
	RF_POWER_N30dBm,        /* -30.0 dBm: -28 */
	RF_POWER_N30dBm,        /* -30.0 dBm: -27 */
	RF_POWER_N30dBm,        /* -30.0 dBm: -26 */
	RF_POWER_N23p54dBm,     /* -23.5 dBm: -25 */
	RF_POWER_N23p54dBm,     /* -23.5 dBm: -24 */
	RF_POWER_N23p54dBm,     /* -23.5 dBm: -23 */
	RF_POWER_N23p54dBm,     /* -23.5 dBm: -22 */
	RF_POWER_N23p54dBm,     /* -23.5 dBm: -21 */
	RF_POWER_N17p83dBm,     /* -17.8 dBm: -20 */
	RF_POWER_N17p83dBm,     /* -17.8 dBm: -19 */
	RF_POWER_N17p83dBm,     /* -17.8 dBm: -18 */
	RF_POWER_N17p83dBm,     /* -17.8 dBm: -17 */
	RF_POWER_N17p83dBm,     /* -17.8 dBm: -16 */
	RF_POWER_N12p06dBm,     /* -12.0 dBm: -15 */
	RF_POWER_N12p06dBm,     /* -12.0 dBm: -14 */
	RF_POWER_N12p06dBm,     /* -12.0 dBm: -13 */
	RF_POWER_N12p06dBm,     /* -12.0 dBm: -12 */
	RF_POWER_N12p06dBm,     /* -12.0 dBm: -11 */
	RF_POWER_N8p78dBm,      /*  -8.7 dBm: -10 */
	RF_POWER_N8p78dBm,      /*  -8.7 dBm:  -9 */
	RF_POWER_N8p78dBm,      /*  -8.7 dBm:  -8 */
	RF_POWER_N6p54dBm,      /*  -6.5 dBm:  -7 */
	RF_POWER_N6p54dBm,      /*  -6.5 dBm:  -6 */
	RF_POWER_N4p77dBm,      /*  -4.7 dBm:  -5 */
	RF_POWER_N4p77dBm,      /*  -4.7 dBm:  -4 */
	RF_POWER_N3p37dBm,      /*  -3.3 dBm:  -3 */
	RF_POWER_N2p01dBm,      /*  -2.0 dBm:  -2 */
	RF_POWER_N1p37dBm,      /*  -1.3 dBm:  -1 */
	RF_POWER_P0p01dBm,      /*   0.0 dBm:   0 */
	RF_POWER_P0p80dBm,      /*   0.8 dBm:   1 */
	RF_POWER_P2p32dBm,      /*   2.3 dBm:   2 */
	RF_POWER_P3p25dBm,      /*   3.2 dBm:   3 */
	RF_POWER_P4p35dBm,      /*   4.3 dBm:   4 */
	RF_POWER_P5p68dBm,      /*   5.6 dBm:   5 */
	RF_POWER_P5p68dBm,      /*   5.6 dBm:   6 */
	RF_POWER_P6p98dBm,      /*   6.9 dBm:   7 */
	RF_POWER_P8p05dBm,      /*   8.0 dBm:   8 */
	RF_POWER_P9p11dBm,      /*   9.1 dBm:   9 */
};

#elif CONFIG_SOC_RISCV_TELINK_B92 || CONFIG_SOC_RISCV_TELINK_B95
/* TX power B92 and B95 lookup table */
const uint8_t tl_tx_pwr_lt[] = {
	RF_POWER_N30dBm,        /* -30.0 dBm: -30 */
	RF_POWER_N30dBm,        /* -30.0 dBm: -29 */
	RF_POWER_N30dBm,        /* -30.0 dBm: -28 */
	RF_POWER_N30dBm,        /* -30.0 dBm: -27 */
	RF_POWER_N30dBm,        /* -30.0 dBm: -26 */
	RF_POWER_N22p53dBm,     /* -22.5 dBm: -25 */
	RF_POWER_N22p53dBm,     /* -22.5 dBm: -24 */
	RF_POWER_N22p53dBm,     /* -22.5 dBm: -23 */
	RF_POWER_N22p53dBm,     /* -22.5 dBm: -22 */
	RF_POWER_N22p53dBm,     /* -22.5 dBm: -21 */
	RF_POWER_N13p42dBm,     /* -13.5 dBm: -20 */
	RF_POWER_N13p42dBm,     /* -13.5 dBm: -19 */
	RF_POWER_N13p42dBm,     /* -13.5 dBm: -18 */
	RF_POWER_N13p42dBm,     /* -13.5 dBm: -17 */
	RF_POWER_N13p42dBm,     /* -13.5 dBm: -16 */
	RF_POWER_N9p03dBm,      /* -9.0 dBm: -15 */
	RF_POWER_N9p03dBm,      /* -9.0 dBm: -14 */
	RF_POWER_N9p03dBm,      /* -9.0 dBm: -13 */
	RF_POWER_N9p03dBm,      /* -9.0 dBm: -12 */
	RF_POWER_N9p03dBm,      /* -9.0 dBm: -11 */
	RF_POWER_N5p94dBm,      /*  -5.9 dBm: -10 */
	RF_POWER_N5p94dBm,      /*  -5.9 dBm:  -9 */
	RF_POWER_N5p94dBm,      /*  -5.9 dBm:  -8 */
	RF_POWER_N5p94dBm,      /*  -5.9 dBm:  -7 */
	RF_POWER_N5p94dBm,      /*  -5.9 dBm:  -6 */
	RF_POWER_N3p95dBm,      /*  -4.0 dBm:  -5 */
	RF_POWER_N3p95dBm,      /*  -4.0 dBm:  -4 */
	RF_POWER_N2p51dBm,      /*  -2.5 dBm:  -3 */
	RF_POWER_N1p52dBm,      /*  -1.5 dBm:  -2 */
	RF_POWER_N0p43dBm,      /*  -0.5 dBm:  -1 */
	RF_POWER_P0p01dBm,      /*   0.0 dBm:   0 */
	RF_POWER_P1p03dBm,      /*   1.0 dBm:   1 */
	RF_POWER_P1p62dBm,      /*   1.6 dBm:   2 */
	RF_POWER_P2p51dBm,      /*   2.5 dBm:   3 */
	RF_POWER_P3p51dBm,      /*   3.5 dBm:   4 */
	RF_POWER_P4p61dBm,      /*   4.6 dBm:   5 */
	RF_POWER_P5p21dBm,      /*   5.2 dBm:   6 */
	RF_POWER_P7p00dBm,      /*   7.0 dBm:   7 */
	RF_POWER_P8p25dBm,      /*   8.2 dBm:   8 */
	RF_POWER_P9p90dBm,      /*   9.9 dBm:   9 */
};

#elif CONFIG_SOC_RISCV_TELINK_TL321X
/* TX power TL321X lookup table */
const uint8_t tl_tx_pwr_lt[] = {
	/*VANT*/
	RF_POWER_N51p19dBm,   /**< -50.2 dbm: -20 */
	RF_POWER_N25p53dBm,   /**< -25.5 dbm: -19 */
	RF_POWER_N20p01dBm,   /**< -20.0 dbm: -18 */
	RF_POWER_N14p63dBm,   /**< -14.6 dbm: -17 */
	RF_POWER_N12p66dBm,   /**< -12.6 dbm: -16 */
	RF_POWER_N11p28dBm,   /**< -11.3 dbm: -15 */
	RF_POWER_N9p92dBm,    /**<  -9.9 dbm: -14 */
	RF_POWER_N8p88dBm,    /**<  -8.8 dbm: -13 */
	RF_POWER_N7p86dBm,    /**<  -7.8 dbm: -12 */
	RF_POWER_N6p25dBm,    /**<  -6.2 dbm: -11 */
	RF_POWER_N5p58dBm,    /**<  -5.6 dbm: -10 */
	RF_POWER_N4p92dBm,    /**<  -4.9 dbm:  -9 */
	RF_POWER_N4p36dBm,    /**<  -4.4 dbm:  -8 */
	RF_POWER_N3p37dBm,    /**<  -3.3 dbm:  -7 */
	RF_POWER_N2p80dBm,    /**<  -2.8 dbm:  -6 */
	RF_POWER_N2p00dBm,    /**<  -2.0 dbm:  -5 */
	RF_POWER_N1p60dBm,    /**<  -1.6 dbm:  -4 */
	RF_POWER_N1p22dBm,    /**<  -1.2 dbm:  -3 */
	RF_POWER_N0p54dBm,    /**<  -0.5 dbm:  -2 */
	RF_POWER_N0p22dBm,    /**<  -0.2 dbm:  -1 */
	RF_POWER_P0p08dBm,    /**<   0.0 dbm:   0 */
	RF_POWER_P0p91dBm,    /**<   0.9 dbm:   1 */
	RF_POWER_P2p03dBm,    /**<   2.0 dbm:   2 */
	RF_POWER_P3p03dBm,    /**<   3.0 dbm:   3 */
	RF_POWER_P4p13dBm,    /**<   4.0 dbm:   4*/
	RF_POWER_P5p00dBm,    /**<   5.0 dbm:   5 */

	/*VBAT*/
	RF_POWER_P6p05dBm,    /**<   6.0 dbm:   6 */
	RF_POWER_P6p97dBm,    /**<   7.0 dbm:   7 */
	RF_POWER_P8p03dBm,    /**<   8.0 dbm:   8 */
	RF_POWER_P9p10dBm,    /**<   9.0 dbm:   9 */
	RF_POWER_P10p00dBm,   /**<  10.0 dbm:  10 */
	RF_POWER_P11p16dBm,   /**<  11.1 dbm:  11 */
};


#elif CONFIG_SOC_RISCV_TELINK_TL721X
/* TX power TL721X lookup table */
const uint8_t tl_tx_pwr_lt[] = {
	RF_POWER_N41p80dBm,   /**< -41.8 dbm: -22 */
	RF_POWER_N25p19dBm,   /**< -25.2 dbm: -21 */
	RF_POWER_N20p22dBm,   /**< -20.2 dbm: -20 */
	RF_POWER_N16p82dBm,   /**< -16.8 dbm: -19 */
	RF_POWER_N14p64dBm,   /**< -14.6 dbm: -18 */
	RF_POWER_N12p72dBm,   /**< -12.7 dbm: -17 */
	RF_POWER_N11p42dBm,   /**< -11.4 dbm: -16 */
	RF_POWER_N10p09dBm,   /**< -10.1 dbm: -15 */
	RF_POWER_N9p14dBm,    /**<  -9.1 dbm: -14 */
	RF_POWER_N8p14dBm,    /**<  -8.1 dbm: -13 */
	RF_POWER_N7p39dBm,    /**<  -7.4 dbm: -12 */
	RF_POWER_N6p61dBm,    /**<  -6.6 dbm: -11 */
	RF_POWER_N6p03dBm,    /**<  -6.0 dbm: -10 */
	RF_POWER_N5p38dBm,    /**<  -5.4 dbm:  -9 */
	RF_POWER_N4p90dBm,    /**<  -5.0 dbm:  -8 */
	RF_POWER_N4p36dBm,    /**<  -4.4 dbm:  -7 */
	RF_POWER_N3p89dBm,    /**<  -3.8 dbm:  -6 */
	RF_POWER_N3p08dBm,    /**<  -3.0 dbm:  -5 */
	RF_POWER_N2p30dBm,    /**<  -2.3 dbm:  -4 */
	RF_POWER_N1p67dBm,    /**<  -1.7 dbm:  -3 */
	RF_POWER_N1p08dBm,    /**<  -1.0 dbm   -2 */
	RF_POWER_N0p66dBm,    /**<  -0.7 dbm:  -1 */
	RF_POWER_P0p03dBm,    /**<   0.0 dbm:   0 */
	RF_POWER_P0p66dBm,    /**<   0.7 dbm:   1 */
	RF_POWER_P0p98dBm,    /**<   1.0 dbm:   2 */
	RF_POWER_P1p65dBm,    /**<   1.6 dbm:   3 */
	RF_POWER_P2p00dBm,    /**<   2.0 dbm:   4 */
	RF_POWER_P2p43dBm,    /**<   2.4 dbm:   5 */
	RF_POWER_P2p73dBm,    /**<   2.7 dbm:   6 */
	RF_POWER_P3p04dBm,    /**<   3.0 dbm:   7*/
	RF_POWER_P3p32dBm,    /**<   3.3 dbm:   8 */
	RF_POWER_P3p48dBm,    /**<   3.5 dbm:   9 */
	RF_POWER_P3p82dBm,    /**<   3.8 dbm:  10 */
	RF_POWER_P4p09dBm,    /**<   4.1 dbm:  11 */
	RF_POWER_P4p79dBm,    /**<   4.8 dbm:  12 */
	RF_POWER_P5p53dBm,    /**<   5.5 dbm:  13 */
	RF_POWER_P6p08dBm,    /**<   6.0 dbm:  14 */
	RF_POWER_P6p71dBm,    /**<   6.7 dbm:  15 */
	RF_POWER_P7p52dBm,    /**<   7.5 dbm:  16 */
	RF_POWER_P8p06dBm,    /**<   8.0 dbm:  17 */
	RF_POWER_P8p55dBm,    /**<   8.4 dbm:  18 */
	RF_POWER_P9p10dBm,    /**<   9.1 dbm:  19 */
	RF_POWER_P9p45dBm,    /**<   9.5 dbm:  20 */
	RF_POWER_P10p00dBm,   /**<  10.0 dbm:  21 */
};

#endif
