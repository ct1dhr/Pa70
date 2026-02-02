// controlo do amplificador de potencia para o square SDR
// recebe via uart a frequencia do square sdr
// utiliza um esp32 s3
// para leituras analógica um ADS1115
#include <arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <arduino.h>
#include <Adafruit_ADS1X15.h>
#include <math.h>
#define LV_CONF_SKIP
#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>

Adafruit_ADS1115 ads;

// Variáveis globais para as leituras (em Volts ou valor bruto)
float v_ch0 = 0.0, v_ch1 = 0.0, v_ch2 = 0.0, v_ch3 = 0.0;
float ads_ch0 = 0.0;
float ads_ch1 = 0.0;
float ads_ch2 = 0.0;
float ads_ch3 = 0.0;
// Mutex para garantir que o LVGL e a Task não tentem ler/escrever ao mesmo tempo
SemaphoreHandle_t adcMutex;

SemaphoreHandle_t myMutex = NULL;
SemaphoreHandle_t xMutex_I2C = NULL;
SemaphoreHandle_t xMutex_Var = NULL;