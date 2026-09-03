# PS5 Account Activator
# Build with ps5-payload-dev/sdk.

PS5_HOST ?= ps5
PS5_PORT ?= 9021

ifndef PS5_PAYLOAD_SDK
$(error PS5_PAYLOAD_SDK is undefined. Set it to your ps5-payload-sdk installation.)
endif

include $(PS5_PAYLOAD_SDK)/toolchain/prospero.mk

ELF := account_activator.elf

CFLAGS := -std=gnu11 -Os -Wall -Wextra -Werror \
          -Wstrict-prototypes -Wmissing-prototypes \
          -ffunction-sections -fdata-sections -Iinclude

LDFLAGS := -Wl,--gc-sections

SRCS := src/main.c src/account_activator.c src/notification.c

.PHONY: all clean test

all: $(ELF)

$(ELF): $(SRCS) include/account_activator.h include/notification.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS) \
		-lkernel_sys -lSceRegMgr -lSceUserService
	$(PS5_PAYLOAD_SDK)/bin/prospero-strip --strip-all $@

clean:
	rm -f $(ELF) *.o src/*.o

test: $(ELF)
	$(PS5_DEPLOY) -h $(PS5_HOST) -p $(PS5_PORT) $^
