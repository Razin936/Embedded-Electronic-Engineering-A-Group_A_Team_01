.RECIPEPREFIX := >

APP = rsu_freertos_demo

FREERTOS_KERNEL = FreeRTOS-Kernel

CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -g -O0
CFLAGS += -I.
CFLAGS += -I$(FREERTOS_KERNEL)/include
CFLAGS += -I$(FREERTOS_KERNEL)/portable/ThirdParty/GCC/Posix
CFLAGS += -I$(FREERTOS_KERNEL)/portable/ThirdParty/GCC/Posix/utils

SRCS = main.c \
       vehicle_tasks.c \
       rsu_controller.c \
       hw_interface.c \
       $(FREERTOS_KERNEL)/tasks.c \
       $(FREERTOS_KERNEL)/queue.c \
       $(FREERTOS_KERNEL)/list.c \
       $(FREERTOS_KERNEL)/portable/MemMang/heap_3.c \
       $(FREERTOS_KERNEL)/portable/ThirdParty/GCC/Posix/port.c \
       $(FREERTOS_KERNEL)/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c

LDLIBS = -pthread -lrt

all: $(APP)

$(APP): $(SRCS) FreeRTOSConfig.h rsu_common.h
>$(CC) $(CFLAGS) $(SRCS) -o $(APP) $(LDLIBS)

run: $(APP)
>timeout 18s stdbuf -oL ./$(APP) | tee output_mm2.txt || true

clean:
>rm -f $(APP) output_mm1.txt output_mm2.txt
