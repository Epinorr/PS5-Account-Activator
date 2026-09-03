# EPINOR PS5 Account Activator
# Built against ps5-payload-dev/sdk.

PS5_HOST ?= ps5
PS5_PORT ?= 9021

ifdef PS5_PAYLOAD_SDK
include $(PS5_PAYLOAD_SDK)/toolchain/prospero.mk
else
$(error PS5_PAYLOAD_SDK is undefined)
endif

ELF := EPINOR-Account-Activator.elf

CFLAGS := -std=gnu11 -O2 -Wall -Wextra -Werror -Iinclude
SRCS := src/main.c src/account_activator.c src/notification.c
LDADD := -lkernel_sys -lSceRegMgr -lSceUserService

.PHONY: all clean test

all: $(ELF)

$(ELF): $(SRCS) include/account_activator.h include/notification.h
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDADD)

clean:
	rm -f $(ELF) *.o src/*.o

test: $(ELF)
	$(PS5_DEPLOY) -h $(PS5_HOST) -p $(PS5_PORT) $^
