#include "config.h"
#include "logger.h"
#include "monitor.h"
#include "process.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static void start_all(void) {
    int count;
    const ServiceConfig *services = get_services(&count);

    for (int i = 0; i < count; i++)
        start_process(services[i].name, services[i].command);
}

static void show_logs(const char *service) {
    const ServiceConfig *config = find_service(service);
    if (!config) {
        printf("Unknown service: %s\n", service);
        return;
    }

    char path[256];
    snprintf(path, sizeof(path), "logs/%s.log", service);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        printf("No logs for %s\n", service);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp))
        fputs(line, stdout);

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    if (strcmp(argv[1], "up") == 0) {
        start_all();
    } else if (strcmp(argv[1], "stop") == 0) {
        stop_all_processes();
    } else if (strcmp(argv[1], "ps") == 0) {
        show_status();
    } else if (strcmp(argv[1], "logs") == 0) {
        if (argc < 3) {
            printf("Usage: ./dockman logs <service>\n");
            return 1;
        }
        show_logs(argv[2]);
    } else {
        printf("Unknown command: %s\n", argv[1]);
        print_usage();
        return 1;
    }

    return 0;
}
