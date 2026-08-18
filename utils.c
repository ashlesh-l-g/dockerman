#include "utils.h"
#include <stdio.h>

void print_usage(void) {
    printf("Usage: ./dockman <command> [service]\n\n");
    printf("Commands:\n");
    printf("  up             Start all services\n");
    printf("  stop           Stop all services\n");
    printf("  ps             Show service status\n");
    printf("  logs <service> Show service logs\n");
}
