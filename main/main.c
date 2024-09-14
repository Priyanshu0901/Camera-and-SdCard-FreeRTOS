#include <stdio.h>
#include "camera_interface.h"
#include "sdcard_interface.h"

void initialize_drivers()
{
    initialize_sdcard();
    initialize_camera();
}

void start_tasks()
{
    createCameraTask();
}

void app_main(void)
{
    initialize_drivers();
    start_tasks();
}