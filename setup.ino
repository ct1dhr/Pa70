//
#include <Arduino.h>

// 1. Protótipos das funções que estão em outros arquivos
void taskCore0(void *pvParameters);
void taskCore1(void *pvParameters);
void taskADC(void *pvParameters);
void setup_display();

// 2. Variáveis globais que vêm de outros arquivos
extern SemaphoreHandle_t xMutex_I2C;
extern SemaphoreHandle_t xMutex_Var;

void setup() {
  Serial.begin(115200);

  // Inicializa os Mutexes
  xMutex_I2C = xSemaphoreCreateMutex();
  xMutex_Var = xSemaphoreCreateMutex();

  // Inicializa o Display antes de começar as tarefas
  setup_display();

  if (xMutex_Var != NULL && xMutex_I2C != NULL) {
    // Cria tarefas no Núcleo 0
    xTaskCreatePinnedToCore(taskCore0, "Task_Core0", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(taskADC,   "TaskADC",   4096, NULL, 2, NULL, 0);

    // Cria tarefa no Núcleo 1
    xTaskCreatePinnedToCore(taskCore1, "Task_Core1", 4096, NULL, 1, NULL, 1);
    
    Serial.println("Sistema iniciado com sucesso!");
  }
}