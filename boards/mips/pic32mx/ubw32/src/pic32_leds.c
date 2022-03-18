/****************************************************************************
 * boards/mips/pic32mx/ubw32/src/pic32_leds.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

#include <nuttx/board.h>
#include <arch/board/board.h>

#include "chip.h"
#include "mips_internal.h"
#include "pic32mx.h"
#include "ubw32.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* LED Configuration ********************************************************/

/* ----- ----- -------------------------------------------------------------
 * LABEL COLOR CONTROL
 * ----- ----- -------------------------------------------------------------
 * USB   Green   RF3. This could be used by software if USB is not used.
 *                    Otherwise, RF3 is used as the USBID signal.
 * LED1  While   RE2, Pulled up.  Low value illuminates
 * LED2  Red     RE1, Pulled up.  Low value illuminates
 * LED3  Yellow  RE0, Pulled up.  Low value illuminates
 * PWR   Blue    Illuminated when 5V is present, not controlled by software
 *
 * If CONFIG_ARCH_LEDS is defined,
 * then NuttX will control these LEDs as follows:
 *
 *                           ON                  OFF
 * ------------------------- ---- ---- ---- ---- ---- ----
 *                           LED1 LED2 LED3 LED1 LED2 LED3
 * ------------------------- ---- ---- ---- ---- ---- ----
 * LED_STARTED            0  OFF  OFF  OFF  ---  ---  ---
 * LED_HEAPALLOCATE       1  ON   OFF  N/C  ---  ---  ---
 * LED_IRQSENABLED        2  OFF  ON   N/C  ---  ---  ---
 * LED_STACKCREATED       3  ON   ON   N/C  ---  ---  ---
 * LED_INIRQ              4  N/C  N/C  ON   N/C  N/C  OFF
 * LED_SIGNAL             4  N/C  N/C  ON   N/C  N/C  OFF
 * LED_ASSERTION          4  N/C  N/C  ON   N/C  N/C  OFF
 * LED_PANIC              5  ON   N/C  N/C  OFF  N/C  N/C
 */

#define GPIO_LED_1   (GPIO_OUTPUT|GPIO_VALUE_ZERO|GPIO_PORTE|GPIO_PIN2)
#define GPIO_LED_2   (GPIO_OUTPUT|GPIO_VALUE_ZERO|GPIO_PORTE|GPIO_PIN1)
#define GPIO_LED_3   (GPIO_OUTPUT|GPIO_VALUE_ZERO|GPIO_PORTE|GPIO_PIN0)

/* LED Management Definitions ***********************************************/

#ifdef CONFIG_ARCH_LEDS
#  define LED_OFF 0
#  define LED_ON  1
#  define LED_NC  2
#endif

/****************************************************************************
 * Private types
 ****************************************************************************/

#ifdef CONFIG_ARCH_LEDS
struct led_setting_s
{
  uint8_t led1   : 2;
  uint8_t led2   : 2;
  uint8_t led3   : 2;
  uint8_t unused : 2;
};
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* If CONFIG_ARCH_LEDS is defined then NuttX will control the LEDs.  The
 * following structures identified the LED settings for each NuttX LED state.
 */

#ifdef CONFIG_ARCH_LEDS
static const struct led_setting_s g_ledonvalues[LED_NVALUES] =
{
  {LED_OFF, LED_OFF, LED_OFF, LED_OFF},
  {LED_ON,  LED_OFF, LED_NC,  LED_OFF},
  {LED_OFF, LED_ON,  LED_NC,  LED_OFF},
  {LED_ON,  LED_ON,  LED_NC,  LED_OFF},
  {LED_NC,  LED_NC,  LED_ON,  LED_OFF},
  {LED_ON,  LED_NC,  LED_NC,  LED_OFF},
};

static const struct led_setting_s g_ledoffvalues[LED_NVALUES] =
{
  {LED_NC,  LED_NC,  LED_NC,  LED_OFF},
  {LED_NC,  LED_NC,  LED_NC,  LED_OFF},
  {LED_NC,  LED_NC,  LED_NC,  LED_OFF},
  {LED_NC,  LED_NC,  LED_NC,  LED_OFF},
  {LED_NC,  LED_NC,  LED_OFF, LED_OFF},
  {LED_OFF, LED_NC,  LED_NC,  LED_OFF},
};

/* If CONFIG_ARCH_LEDS is not defined, then the user can control the LEDs in
 * any way.  The following array simply maps the PIC32MX_UBW32_LEDn
 * index values to the correct LED pin configuration.
 */

#else
static const uint16_t g_ledpincfg[PIC32MX_UBW32_NLEDS] =
{
  GPIO_LED_1, GPIO_LED_2, GPIO_LED_3
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pic32mx_setleds
 ****************************************************************************/

#ifdef CONFIG_ARCH_LEDS
static void pic32mx_setleds(FAR const struct led_setting_s *setting)
{
  if (setting->led1 != LED_NC)
    {
      pic32mx_gpiowrite(GPIO_LED_1, setting->led1 == LED_ON);
    }

  if (setting->led2 != LED_NC)
    {
      pic32mx_gpiowrite(GPIO_LED_2, setting->led2 == LED_ON);
    }

  if (setting->led3 != LED_NC)
    {
      pic32mx_gpiowrite(GPIO_LED_3, setting->led3 == LED_ON);
    }
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_userled_initialize
 ****************************************************************************/

#ifndef CONFIG_ARCH_LEDS
uint32_t board_userled_initialize(void)
{
  /* Configure output pins */

  pic32mx_configgpio(GPIO_LED_1);
  pic32mx_configgpio(GPIO_LED_2);
  pic32mx_configgpio(GPIO_LED_3);
  return 3;
}
#endif

/****************************************************************************
 * Name: board_userled
 ****************************************************************************/

#ifndef CONFIG_ARCH_LEDS
void board_userled(int led, bool ledon)
{
  if ((unsigned)led < PIC32MX_UBW32_NLEDS)
    {
      pic32mx_gpiowrite(g_ledpincfg[led], ledon);
    }
}
#endif

/****************************************************************************
 * Name: board_userled_all
 ****************************************************************************/

#ifndef CONFIG_ARCH_LEDS
void board_userled_all(uint32_t ledset)
{
  board_userled(PIC32MX_UBW32_LED1, (ledset & PIC32MX_UBW32_LED1_BIT) != 0);
  board_userled(PIC32MX_UBW32_LED2, (ledset & PIC32MX_UBW32_LED2_BIT) != 0);
  board_userled(PIC32MX_UBW32_LED3, (ledset & PIC32MX_UBW32_LED3_BIT) != 0);
}
#endif

/****************************************************************************
 * Name: pic32mx_led_initialize
 ****************************************************************************/

#ifdef CONFIG_ARCH_LEDS
void pic32mx_led_initialize(void)
{
  /* Configure output pins */

  pic32mx_configgpio(GPIO_LED_1);
  pic32mx_configgpio(GPIO_LED_2);
  pic32mx_configgpio(GPIO_LED_3);
}
#endif

/****************************************************************************
 * Name: board_autoled_on
 ****************************************************************************/

#ifdef CONFIG_ARCH_LEDS
void board_autoled_on(int led)
{
  if ((unsigned)led < LED_NVALUES)
    {
      pic32mx_setleds(&g_ledonvalues[led]);
    }
}
#endif

/****************************************************************************
 * Name: board_autoled_off
 ****************************************************************************/

#ifdef CONFIG_ARCH_LEDS
void board_autoled_off(int led)
{
  if ((unsigned)led < LED_NVALUES)
    {
      pic32mx_setleds(&g_ledoffvalues[led]);
    }
}
#endif
