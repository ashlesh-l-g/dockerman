#include "monitor.h"
#include "config.h"
#include "process.h"
#include <stdio.h>

void show_status(void) {
    int count;
    const ServiceConfig *services = get_services(&count);

    printf("%-12s %-10s %s\n", "SERVICE", "STATUS", "PID");
    printf("--------------------------------\n");

    for (int i = 0; i < count; i++) {
        int pid;
        if (process_running(services[i].name, &pid))
            printf("%-12s %-10s %d\n", services[i].name, "running", pid);
        else
            printf("%-12s %-10s -\n", services[i].name, "stopped");
    }
}
