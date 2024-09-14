idf_component_register(SRCS "camera_interface.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp32-camera sdcard_interface freertos
                    )
