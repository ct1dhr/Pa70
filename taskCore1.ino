#include <lvgl.h>

// Variáveis que vêm de outros arquivos
extern float ads_ch0, ads_ch1, ads_ch2, ads_ch3;
extern SemaphoreHandle_t xMutex_Var;

// Objetos temporários para o teste
lv_obj_t * label_teste[4];

void taskCore1(void *pvParameters) {
    // --- CRIAÇÃO DOS OBJETOS DE SIMULAÇÃO ---
    // Criamos 4 labels manualmente para não depender do EEZ Studio agora
    for(int i = 0; i < 4; i++) {
        label_teste[i] = lv_label_create(lv_screen_active());
        lv_obj_set_style_text_color(label_teste[i], lv_color_white(), 0);
        lv_obj_align(label_teste[i], LV_ALIGN_TOP_LEFT, 20, 30 + (i * 40));
        lv_label_set_text(label_teste[i], "Aguardando ADS...");
    }

    Serial.println("Task UI (Simulada) Iniciada.");

    while (true) {
        lv_timer_handler(); // Mantém o LVGL vivo

        static uint32_t last_update = 0;
        if (millis() - last_update > 100) {
            last_update = millis();

            if (xSemaphoreTake(xMutex_Var, pdMS_TO_TICKS(5)) == pdTRUE) {
                float v[4] = {ads_ch0, ads_ch1, ads_ch2, ads_ch3};
                xSemaphoreGive(xMutex_Var);

                // Atualiza os labels manuais
                for(int i = 0; i < 4; i++) {
                    lv_label_set_text_fmt(label_teste[i], "CANAL %d: %.3f V", i, v[i]);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}