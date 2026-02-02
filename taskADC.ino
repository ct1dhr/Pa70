#include <Wire.h>

#define I2C_SDA 17
#define I2C_SCL 18

void taskADC(void *pvParameters) {
    // Inicializa o I2C com os pinos específicos da placa JC8048W550
    Wire.begin(I2C_SDA, I2C_SCL);
    
    // Opcional: Aumentar a velocidade para 400kHz para leituras mais rápidas
    Wire.setClock(400000); 

    if (!ads.begin()) {
        Serial.println("Erro: ADS1115 nao encontrado nos pinos 17/18!");
        vTaskDelete(NULL);
    }

    // ... restante da sua configuração (DataRate, Gain, etc) ...
    ads.setDataRate(RATE_ADS1115_860SPS);



  while (true) {
    // Leitura rápida dos 4 canais
    float c0 = ads.computeVolts(ads.readADC_SingleEnded(0));
    float c1 = ads.computeVolts(ads.readADC_SingleEnded(1));
    float c2 = ads.computeVolts(ads.readADC_SingleEnded(2));
    float c3 = ads.computeVolts(ads.readADC_SingleEnded(3));

    // Protege a escrita nas variáveis globais
    if (xSemaphoreTake(adcMutex, pdMS_TO_TICKS(5)) == pdTRUE) {
      v_ch0 = c0;
      v_ch1 = c1;
      v_ch2 = c2;
      v_ch3 = c3;
      xSemaphoreGive(adcMutex);
    }
    
    
    // Delay mínimo para não travar o Watchdog do ESP32
    vTaskDelay(pdMS_TO_TICKS(1)); 
  }
}