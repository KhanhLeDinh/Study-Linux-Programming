# Tên của chương trình sau khi biên dịch
TARGET = log_program

# Các tệp nguồn (source files)
SRC = main.c

# Các tệp đối tượng (object files)
OBJ = $(SRC:.c=.o)

# Cờ biên dịch (compiler flags)
CFLAGS = -Wall -std=c11

# Trình biên dịch
CC = gcc

# Mục tiêu mặc định
all: $(TARGET)

# Cách xây dựng chương trình
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Quy tắc để biên dịch tệp .c thành .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Mục tiêu làm sạch (clean)
clean:
	rm -f $(OBJ) $(TARGET)

# Mục tiêu kiểm tra
check: $(TARGET)
	./$(TARGET)

# Mục tiêu để tạo lại FIFO (nếu cần)
fifo:
	mkfifo logFifo

