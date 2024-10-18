inc =$(shell find Src -name *.c)
core =$(shell find Core -name *.c)
inc_obj =$(patsubst Src/%.c,Build/%.o,$(inc))
core_obj =$(patsubst Core/%.c,Build/%.o,$(core))
obj = Build/startup_stm32f10x_md.o $(inc_obj) $(core_obj)

Build/%.o :  Src/%.c
	@arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -c $^ -o $@
Build/%.o :  Core/%.c
	@arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -c $^ -o $@
Build/startup_stm32f10x_md.o : Config/startup_stm32f10x_md.s
	@arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -c Config/startup_stm32f10x_md.s -o Build/startup_stm32f10x_md.o 
Compile : $(obj)
	@arm-none-eabi-gcc -o Build/exec.elf $^ -mthumb -mcpu=cortex-m3 -T Config/stm32_flash.ld -specs=nosys.specs -static -Wl,-cref,-u,Reset_Handler -Wl,-Map=Build/test.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x80 -Wl,--start-group -lc -lm -Wl,--end-group
Bin : Compile
	@arm-none-eabi-objcopy Build/exec.elf -O binary Build/exec.bin
Run : Bin
	@st-flash write ./Build/exec.bin 0x08000000 
Clear : 
	rm -rf ./Build/