/*!
    \file  gd32f103c_eval.c
    \brief firmware functions to manage leds, keys, COM ports
*/

/*
    Copyright (C) 2017 GigaDevice

    2015-11-16, V1.0.0, demo for GD32F10x
    2017-06-30, V2.0.0, demo for GD32F10x
*/

#include <gd32f10x.h>
//#include "gd32f103c_eval.h"

#define KEYn                             36U

#define KEY1_PIN                   GPIO_PIN_3
#define KEY1_GPIO_PORT             GPIOC
#define KEY1_GPIO_CLK              RCU_GPIOC

#define KEY2_PIN                   GPIO_PIN_4
#define KEY2_GPIO_PORT             GPIOC
#define KEY2_GPIO_CLK              RCU_GPIOC

#define KEY3_PIN                   GPIO_PIN_5
#define KEY3_GPIO_PORT             GPIOC
#define KEY3_GPIO_CLK              RCU_GPIOC

#define KEY4_PIN                   GPIO_PIN_6
#define KEY4_GPIO_PORT             GPIOC
#define KEY4_GPIO_CLK              RCU_GPIOC

#define KEY5_PIN                   GPIO_PIN_7
#define KEY5_GPIO_PORT             GPIOC
#define KEY5_GPIO_CLK              RCU_GPIOC

#define KEY6_PIN                   GPIO_PIN_8
#define KEY6_GPIO_PORT             GPIOC
#define KEY6_GPIO_CLK              RCU_GPIOC

#define KEY7_PIN                   GPIO_PIN_9
#define KEY7_GPIO_PORT             GPIOC
#define KEY7_GPIO_CLK              RCU_GPIOC

#define KEY8_PIN                   GPIO_PIN_10
#define KEY8_GPIO_PORT             GPIOC
#define KEY8_GPIO_CLK              RCU_GPIOC

#define KEY9_PIN                   GPIO_PIN_11
#define KEY9_GPIO_PORT             GPIOC
#define KEY9_GPIO_CLK              RCU_GPIOC

#define KEY10_PIN                   GPIO_PIN_12
#define KEY10_GPIO_PORT             GPIOC
#define KEY10_GPIO_CLK              RCU_GPIOC

#define KEY11_PIN                   GPIO_PIN_13
#define KEY11_GPIO_PORT             GPIOC
#define KEY11_GPIO_CLK              RCU_GPIOC

#define KEY12_PIN                   GPIO_PIN_14
#define KEY12_GPIO_PORT             GPIOC
#define KEY12_GPIO_CLK              RCU_GPIOC

#define KEY13_PIN                   GPIO_PIN_15
#define KEY13_GPIO_PORT             GPIOC
#define KEY13_GPIO_CLK              RCU_GPIOC

#define KEY14_PIN                   GPIO_PIN_0
#define KEY14_GPIO_PORT             GPIOD
#define KEY14_GPIO_CLK              RCU_GPIOD

#define KEY15_PIN                   GPIO_PIN_1
#define KEY15_GPIO_PORT             GPIOD
#define KEY15_GPIO_CLK              RCU_GPIOD

#define KEY16_PIN                   GPIO_PIN_2
#define KEY16_GPIO_PORT             GPIOD
#define KEY16_GPIO_CLK              RCU_GPIOD

#define KEY17_PIN                   GPIO_PIN_3
#define KEY17_GPIO_PORT             GPIOD
#define KEY17_GPIO_CLK              RCU_GPIOD

#define KEY18_PIN                   GPIO_PIN_4
#define KEY18_GPIO_PORT             GPIOD
#define KEY18_GPIO_CLK              RCU_GPIOD

#define KEY19_PIN                   GPIO_PIN_5
#define KEY19_GPIO_PORT             GPIOD
#define KEY19_GPIO_CLK              RCU_GPIOD

#define KEY20_PIN                   GPIO_PIN_6
#define KEY20_GPIO_PORT             GPIOD
#define KEY20_GPIO_CLK              RCU_GPIOD

#define KEY21_PIN                   GPIO_PIN_7
#define KEY21_GPIO_PORT             GPIOD
#define KEY21_GPIO_CLK              RCU_GPIOD

#define KEY22_PIN                   GPIO_PIN_8
#define KEY22_GPIO_PORT             GPIOD
#define KEY22_GPIO_CLK              RCU_GPIOD

#define KEY23_PIN                   GPIO_PIN_9
#define KEY23_GPIO_PORT             GPIOD
#define KEY23_GPIO_CLK              RCU_GPIOD

#define KEY24_PIN                   GPIO_PIN_10
#define KEY24_GPIO_PORT             GPIOD
#define KEY24_GPIO_CLK              RCU_GPIOD

#define KEY25_PIN                   GPIO_PIN_11
#define KEY25_GPIO_PORT             GPIOD
#define KEY25_GPIO_CLK              RCU_GPIOD

#define KEY26_PIN                   GPIO_PIN_12
#define KEY26_GPIO_PORT             GPIOD
#define KEY26_GPIO_CLK              RCU_GPIOD

#define KEY27_PIN                   GPIO_PIN_13
#define KEY27_GPIO_PORT             GPIOD
#define KEY27_GPIO_CLK              RCU_GPIOD

#define KEY28_PIN                   GPIO_PIN_14
#define KEY28_GPIO_PORT             GPIOD
#define KEY28_GPIO_CLK              RCU_GPIOD

#define KEY29_PIN                   GPIO_PIN_15
#define KEY29_GPIO_PORT             GPIOD
#define KEY29_GPIO_CLK              RCU_GPIOD

#define KEY30_PIN                   GPIO_PIN_0
#define KEY30_GPIO_PORT             GPIOE
#define KEY30_GPIO_CLK              RCU_GPIOE

#define KEY31_PIN                   GPIO_PIN_1
#define KEY31_GPIO_PORT             GPIOE
#define KEY31_GPIO_CLK              RCU_GPIOE

#define KEY32_PIN                   GPIO_PIN_2
#define KEY32_GPIO_PORT             GPIOE
#define KEY32_GPIO_CLK              RCU_GPIOE

#define KEY33_PIN                   GPIO_PIN_3
#define KEY33_GPIO_PORT             GPIOE
#define KEY33_GPIO_CLK              RCU_GPIOE


#define KEY34_PIN                   GPIO_PIN_4
#define KEY34_GPIO_PORT             GPIOE
#define KEY34_GPIO_CLK              RCU_GPIOE


#define KEY35_PIN                   GPIO_PIN_5
#define KEY35_GPIO_PORT             GPIOE
#define KEY35_GPIO_CLK              RCU_GPIOE



#define KEY36_PIN                   GPIO_PIN_6
#define KEY36_GPIO_PORT             GPIOE
#define KEY36_GPIO_CLK              RCU_GPIOE


static uint32_t KEY_PORT[KEYn] =
{
    KEY1_GPIO_PORT,  KEY2_GPIO_PORT, KEY3_GPIO_PORT, KEY4_GPIO_PORT, KEY5_GPIO_PORT,  KEY6_GPIO_PORT, KEY7_GPIO_PORT,  KEY8_GPIO_PORT,  KEY9_GPIO_PORT,  KEY10_GPIO_PORT,
    KEY11_GPIO_PORT, KEY12_GPIO_PORT, KEY13_GPIO_PORT, KEY14_GPIO_PORT, KEY15_GPIO_PORT, KEY16_GPIO_PORT, KEY17_GPIO_PORT, KEY18_GPIO_PORT, KEY19_GPIO_PORT, KEY20_GPIO_PORT,
    KEY21_GPIO_PORT, KEY22_GPIO_PORT, KEY23_GPIO_PORT, KEY24_GPIO_PORT, KEY25_GPIO_PORT, KEY26_GPIO_PORT, KEY27_GPIO_PORT, KEY28_GPIO_PORT, KEY29_GPIO_PORT, KEY30_GPIO_PORT,
    KEY31_GPIO_PORT, KEY32_GPIO_PORT, KEY33_GPIO_PORT, KEY34_GPIO_PORT, KEY35_GPIO_PORT, KEY36_GPIO_PORT
};
static uint32_t KEY_PIN[KEYn] =
{
    KEY1_PIN,  KEY2_PIN, KEY3_PIN, KEY4_PIN, KEY5_PIN,  KEY6_PIN, KEY7_PIN,  KEY8_PIN, KEY9_PIN,  KEY10_PIN,
    KEY11_PIN, KEY12_PIN, KEY13_PIN, KEY14_PIN, KEY15_PIN, KEY16_PIN, KEY17_PIN, KEY18_PIN, KEY19_PIN, KEY20_PIN,
    KEY21_PIN, KEY22_PIN, KEY23_PIN, KEY24_PIN, KEY25_PIN, KEY26_PIN, KEY27_PIN, KEY28_PIN, KEY29_PIN, KEY30_PIN,
    KEY31_PIN, KEY32_PIN, KEY33_PIN, KEY34_PIN, KEY35_PIN, KEY36_PIN
};



/*!
    \brief      return the selected button state
    \param[in]  button: specify the button to be checked
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the button GPIO pin value
*/
uint8_t gd_eval_key_state_get(uint8_t button_num)
{
    return gpio_input_bit_get(KEY_PORT[button_num], KEY_PIN[button_num]);
}
