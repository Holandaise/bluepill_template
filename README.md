Easy "clone and go" repository for a libopencm3 based project.

# Instructions
 1. git clone --recurse-submodules https://github.com/libopencm3/libopencm3-template.git your-project
 2. cd your-project
 3. make -C libopencm3 # (Only needed once)
 4. make -C my-project

If you have an older git, or got ahead of yourself and skipped the ```--recurse-submodules```
you can fix things by running ```git submodule update --init``` (This is only needed once)

# Directories
* my-project contains your application
* my-common-code contains something shared.

# As a template
You should replace this with your _own_ README if you are using this
as a template.

# Debugging
First open an openocd context with the stlink. It will give you a port to connect to with GDB.
```sh
openocd -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/board/stm32f103c8_blue_pill.cfg
```
Then open GDB.
``` sh
arm-none-eabi-gdb firmware.elf
```
Need to connect to the remote target from within GDB console.