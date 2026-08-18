#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

static void ensure_log_dir(void) {
    mkdir("logs", 0755);
}

void log_message(const char *service, const char *message) {
    ensure_log_dir();

    char path[256];
    snprintf(path, sizeof(path), "logs/%s.log", service);

    FILE *fp = fopen(path, "a");
    if (!fp) return;

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(fp, "[%s] %s\n", timestamp, message);
    fclose(fp);
}

void log_service_output(const char *service, const char *path) {
    (void)service;
    (void)path;
}
