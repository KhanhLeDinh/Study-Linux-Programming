#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define LOG_FIFO "logFifo"

void log_process() {
    // Mở FIFO để đọc log
    int fifo_fd = open(LOG_FIFO, O_RDONLY);
    if (fifo_fd < 0) {
        perror("Failed to open FIFO");
        exit(1);
    }

    // Mở file log
    FILE *log_file = fopen("gateway.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
        exit(1);
    }

    char log_event[256];
    while (read(fifo_fd, log_event, sizeof(log_event)) > 0) {
        // Thêm timestamp cho mỗi log
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[20];
        strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(log_file, "%s %s\n", time_str, log_event);
        fflush(log_file);  // Đảm bảo dữ liệu được ghi ra file ngay lập tức
    }

    close(fifo_fd);
    fclose(log_file);
}

int main() {
    if (mkfifo(LOG_FIFO, 0666) < 0) {  // Tạo FIFO với quyền đọc/ghi
        perror("Failed to create FIFO");
        exit(1);
    }

    pid_t log_pid = fork();  // 
    if (log_pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (log_pid == 0) {
        log_process();  // Chạy tiến trình ghi log
        exit(0);
    }

    // Tiến trình chính sẽ tiếp tục quản lý các luồng xử lý
    printf("Main process running...\n");

    // Gửi log mẫu để kiểm tra
    int fifo_fd = open(LOG_FIFO, O_WRONLY);
    write(fifo_fd, "Log Process Started", strlen("Log Process Started"));
    close(fifo_fd);

    return 0;
}
