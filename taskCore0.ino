//taskCore0
int var1 = 0; // Adicione esta linha no topo do ficheiro

void taskCore0(void *pvParameters) {
  while(true) {
    xSemaphoreTake(myMutex, portMAX_DELAY);
    var1 = 100;
    Serial.println(var1);
    xSemaphoreGive(myMutex);

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}