CC     := gcc
CFLAGS := -std=c99 -Wall -Wextra -Werror -pedantic -ggdb
SRC    := src
OUT    := out

SRCS := $(wildcard $(SRC)/*/*.c)
BINS := $(patsubst $(SRC)/%.c,$(OUT)/%.bin,$(SRCS))
DIRS := $(sort $(dir $(BINS)))

.PHONY: all clean

all: $(BINS)

$(OUT)/%.bin: $(SRC)/%.c | $(DIRS)
	$(CC) $(CFLAGS) $< -o $@ -lm

$(DIRS):
	mkdir -p $@

clean:
	rm -rf $(OUT)
