#include "esp_camera.h"

void initialize_sdcard(void);
void deinitialise_sdcard();
void saveJpegToSdcard(camera_fb_t *);