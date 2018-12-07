GCC_PATH = /usr/local/Cellar/arm-gcc-bin/7-2018-q2-update/bin

flash:
	 st-flash --format ihex write build/$(TARGET).hex