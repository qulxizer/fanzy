compile:
	make -C build/
flash: 
	st-flash write build/fanzy.bin 0x08000000 
