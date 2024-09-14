#include <stdio.h>
#include "freertos/FreeRTOS.h"

#include "sdcard_interface.h"

#include "esp_log.h"
#include "camera_config.h"
#include "camera_interface.h"

const char cameraTag[7] = "camera";

static camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sccb_sda = SIOD_GPIO_NUM,
    .pin_sccb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,

    .xclk_freq_hz = 20000000,           // The clock frequency of the image sensor
    .fb_location = CAMERA_FB_IN_PSRAM,  // Set the frame buffer storage location
    .pixel_format = PIXFORMAT_JPEG,     // The pixel format of the image: PIXFORMAT_ + YUV422|GRAYSCALE|RGB565|JPEG
    .frame_size = FRAMESIZE_UXGA,       // The resolution size of the image: FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    .jpeg_quality = 15,                 // The quality of the JPEG image, ranging from 0 to 63.
    .fb_count = 2,                      // The number of frame buffers to use.
    .grab_mode = CAMERA_GRAB_LATEST //  The image capture mode.
};

void initialize_camera(void)
{
    esp_err_t ret = esp_camera_init(&camera_config);
    if (ret == ESP_OK)
    {
        ESP_LOGI(cameraTag, "Camera configured successful");
    }
    else
    {
        ESP_LOGI(cameraTag, "Camera configured unsuccessful");
        return;
    }

    sensor_t *s = esp_camera_sensor_get();

    s->set_brightness(s, 0);                 // -2 to 2
    s->set_contrast(s, 0);                   // -2 to 2
    s->set_saturation(s, 0);                 // -2 to 2
    s->set_special_effect(s, 0);             // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    s->set_whitebal(s, 1);                   // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1);                   // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0);                    // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_exposure_ctrl(s, 1);              // 0 = disable , 1 = enable
    s->set_aec2(s, 0);                       // 0 = disable , 1 = enable
    s->set_ae_level(s, 0);                   // -2 to 2
    s->set_aec_value(s, 300);                // 0 to 1200
    s->set_gain_ctrl(s, 1);                  // 0 = disable , 1 = enable
    s->set_agc_gain(s, 0);                   // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)0); // 0 to 6
    s->set_bpc(s, 0);                        // 0 = disable , 1 = enable
    s->set_wpc(s, 1);                        // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1);                    // 0 = disable , 1 = enable
    s->set_lenc(s, 1);                       // 0 = disable , 1 = enable
    s->set_hmirror(s, 0);                    // 0 = disable , 1 = enable
    s->set_vflip(s, 0);                      // 0 = disable , 1 = enable
    s->set_dcw(s, 1);                        // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);                   // 0 = disable , 1 = enable
}

void takePicture()
{
    ESP_LOGI(cameraTag, "Taking picture...");
    camera_fb_t *pic = esp_camera_fb_get();

    if (pic)
    {
        saveJpegToSdcard(pic); // Adjust quality as needed
    }

    // use pic->buf to access the image
    ESP_LOGI(cameraTag, "Picture taken! Its size was: %zu bytes", pic->len);

    esp_camera_fb_return(pic);
}

void cameraTakePicture_5_sec(void *pvParameters)
{
    for (;;)
    {
        takePicture();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void createCameraTask()
{
    TaskHandle_t task;
    xTaskCreate(
        cameraTakePicture_5_sec,      /* Function that implements the task. */
        "cameraTakePicture_5_sec",    /* Text name for the task. */
        configMINIMAL_STACK_SIZE * 4, /* Stack size in words, or bytes. */
        NULL,                         /* Parameter passed into the task. */
        tskIDLE_PRIORITY,             /* Priority at which the task is created. */
        &task                         /* Used to pass out the created task's handle. */
    );
}