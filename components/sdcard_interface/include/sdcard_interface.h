#ifndef SDCARD_INTERFACE_H
#define SDCARD_INTERFACE_H
#include "esp_camera.h"

void initialize_sdcard(void);
void deinitialise_sdcard();
void saveJpegToSdcard(camera_fb_t *);

#endif //SDCARD_INTERFACE_H