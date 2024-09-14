idf_component_register(SRCS "sdcard_interface.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES fatfs sdmmc esp32-camera )
