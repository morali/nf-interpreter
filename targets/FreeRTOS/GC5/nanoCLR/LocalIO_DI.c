/*
 * Created on Mon Sep 02 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "localIO_DI.h"
#include "fsl_gpio.h"

#define DIGITAL_INPUTS_AMOUNT 4

static volatile uint32_t digitalInputState;
static volatile uint32_t digitalInputCounter[DIGITAL_INPUTS_AMOUNT];

typedef struct {
  GPIO_Type *port;
  uint32_t pin;
} DIPin_t;

static DIPin_t DIPins[DIGITAL_INPUTS_AMOUNT] = {
    {.port = GPIO2, .pin = 28},
    {.port = GPIO2, .pin = 29},
    {.port = GPIO2, .pin = 30},
    {.port = GPIO2, .pin = 31}};

void InitDI(void) {
  const gpio_pin_config_t gpioConfig =
      {
          .direction = kGPIO_DigitalInput,
          .outputLogic = 0,
          .interruptMode = kGPIO_NoIntmode};

  for (int i = 0; i < DIGITAL_INPUTS_AMOUNT; i++) {
    GPIO_PinInit(DIPins[i].port, DIPins[i].pin, &gpioConfig);
  }
}

void DIReadPinsState(void) {
  for (uint8_t i = 0; i < DIGITAL_INPUTS_AMOUNT; i++) {
    if (GPIO_PinRead(DIPins[i].port, DIPins[i].pin)) {
      digitalInputState |= (1 << i);
    } else {
      digitalInputState &= ~(1 << i);
    }
  }
}

void DICountersHandler(void) {
  static uint32_t inputLastState = 0;
  uint32_t inputChange = 0;

  inputChange = digitalInputState ^ inputLastState;
  inputLastState = digitalInputState;

  for (uint8_t i = 0; i < DIGITAL_INPUTS_AMOUNT; i++) {
    if ((inputChange & (1 << i)) && (digitalInputState & (1 << i))) {
      digitalInputCounter[i]++;
    }
  }
}

uint32_t GetDINumber() {
  return DIGITAL_INPUTS_AMOUNT;
}

bool GetDIState(uint32_t id) {
  if (id >= GetDINumber()) {
    return false;
  }

  bool state = (digitalInputState & (1 << id)) == (uint32_t)(1 << id);
  return state;
}

uint32_t GetDICounter(uint32_t id) {

  if (id >= GetDINumber()) {
    return 0;
  }

  return digitalInputCounter[id];
}

void SetDICounter(uint32_t id, uint32_t value) {

  if (id >= GetDINumber()) {
    return;
  }

  digitalInputCounter[id] = value;
}