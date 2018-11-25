t ?= esp32

ifeq ($t, lm4f)
PROCESSOR_FAMILY := lm4f
BOARD := lplm4f120h5qr
CPPFLAGS = -DHARDWARE_H=\"hw/lm4f-utft.h\"
LIBRARIES = UTFT
endif

ifeq ($t, esp8266)
BOARD := d1_mini
UPLOAD_SPEED := 921600
SPIFFS_IMAGE :=
FLASH_SIZE := 4M1M
BUILD_FCPU := 80000000L

CPPFLAGS = -DUSER_SETUP_LOADED -DILI9341_DRIVER \
	-DTFT_WIDTH=240 -DTFT_HEIGHT=320 \
	-DTFT_CS=PIN_D6 -DTFT_DC=PIN_D8 \
	-DTFT_RST=-1 -DSPI_FREQUENCY=40000000 -DLOAD_GLCD
LIBRARIES = TFT_eSPI
endif

ifeq ($t, esp32)
BOARD := node32s
UPLOAD_SPEED := 921600
TERM_SPEED := 115200
SPIFFS_DIR :=

CPPFLAGS = -DUSER_SETUP_LOADED -DILI9341_DRIVER -DTFT_CS=5 -DTFT_DC=2 \
	-DTFT_RST=-1 -DTFT_WIDTH=240 -DTFT_HEIGHT=320 \
	-DSPI_FREQUENCY=40000000 -DLOAD_GLCD
LIBRARIES = TFT_eSPI FS SPIFFS
endif

include arduino-$t.mk
