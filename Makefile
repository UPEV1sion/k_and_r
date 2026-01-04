CC     := gcc
CFLAGS := -Wall -Wextra -Werror -pedantic
SRC    := src
OUT    := out

SRCS := $(wildcard $(SRC)/*/*.c)
BINS := $(patsubst $(SRC)/%.c,$(OUT)/%.bin,$(SRCS))
DIRS := $(sort $(dir $(BINS)))

.PHONY: all clean

all: $(BINS)

$(OUT)/%.bin: $(SRC)/%.c | $(DIRS)
	$(CC) $(CFLAGS) $< -o $@

$(DIRS):
	mkdir -p $@

clean:
	rm -rf $(OUT)
