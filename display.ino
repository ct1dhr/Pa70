#include <Arduino.h>

// Forçar configuração interna
#define LV_CONF_SKIP
#define LV_CONF_INCLUDE_SIMPLE
#define LV_COLOR_DEPTH 16
#define LV_MEM_SIZE (128 * 1024U)

#include <lvgl.h>
#include <Arduino_GFX_Library.h>

// Variáveis de Hardware
Arduino_RGB_Display *gfx = NULL;

// CALLBACK DE FLUSH (Versão à prova de erros de compilação)
// Usamos void* para que o compilador não precise saber o que é lv_display_t aqui
void my_disp_flush(void * disp_ptr, const void * area_ptr, uint8_t * px_map) {
    if (!gfx) return;

    // Convertemos os ponteiros genéricos de volta para os tipos do LVGL
    lv_display_t * d = (lv_display_t *)disp_ptr;
    lv_area_t * area = (lv_area_t *)area_ptr;

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    // Envia os dados para o display RGB
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);

    // Avisa o LVGL que terminou
    lv_display_flush_ready(d);
}

void setup_display() {
    // Painel RGB com os 34 argumentos
    Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
        39, 41, 42, 40, 45, 48, 47, 21, 14, 5, 6, 7, 15, 16, 4, 8, 3, 46, 9, 1,
        1, 8, 4, 8, 1, 8, 4, 8, 1, 16000000, 0, 0, 0, 0
    );

    gfx = new Arduino_RGB_Display(800, 480, rgbpanel);
    if (!gfx->begin()) return;

    gfx->fillScreen(0x0000); // Preto
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH); // Liga luz de fundo

    lv_init();

    // Alocação na PSRAM
    size_t buffer_size = 800 * 40 * sizeof(lv_color_t);
    void* draw_buf = heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM);
    if (!draw_buf) draw_buf = malloc(buffer_size);

    // Criar display no LVGL 9
    lv_display_t * display = lv_display_create(800, 480);
    lv_display_set_buffers(display, draw_buf, NULL, buffer_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    // O (lv_display_flush_cb_t) força o compilador a aceitar a nossa função com void*
    lv_display_set_flush_cb(display, (lv_display_flush_cb_t)my_disp_flush);
}