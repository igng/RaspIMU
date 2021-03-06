BIN_DIR = bin
SRC_DIR = source
INC_DIR = include
OBJ_DIR = object

TARGET = $(BIN_DIR)/imu

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc
CFLAGS += -Wall -Werror -Wextra -O2 -I$(INC_DIR)
LDFLAGS += -Llib
LDLIBS += -lm
DEFINES += -DDEBUG

.PHONY: all clean

all: clean $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(DEFINES) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
