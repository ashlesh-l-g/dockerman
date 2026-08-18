#include "process.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>

static void ensure_run_dir(void) {
    mkdir("run", 0755);
}

static void pid_path(const char *service, char *path, size_t size) {
    snprintf(path, size, "run/%s.pid", service);
}

int process_running(const char *service, int *pid_out) {
    char path[256];
    pid_path(service, path, sizeof(path));

    FILE *fp = fopen(path, "r");
    if (!fp) return 0;

    int pid;
    if (fscanf(fp, "%d", &pid) != 1) {
        fclose(fp);
        unlink(path);
        return 0;
    }
    fclose(fp);

    if (kill(pid, 0) == 0 || errno == EPERM) {
        if (pid_out) *pid_out = pid;
        return 1;
    }

    unlink(path);
    return 0;
}

int start_process(const char *service, const char *command) {
    ensure_run_dir();

    int existing_pid;
    if (process_running(service, &existing_pid)) {
        printf("%s is already running (pid %d)\n", service, existing_pid);
        return -1;
    }

    char log_path[256];
    snprintf(log_path, sizeof(log_path), "logs/%s.log", service);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        FILE *log = fopen(log_path, "a");
        if (!log) _exit(1);

        dup2(fileno(log), STDOUT_FILENO);
        dup2(fileno(log), STDERR_FILENO);
        fclose(log);

        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        _exit(127);
    }

    char pid_path_buf[256];
    pid_path(service, pid_path_buf, sizeof(pid_path_buf));

    FILE *fp = fopen(pid_path_buf, "w");
    if (!fp) {
        kill(pid, SIGTERM);
        return -1;
    }

    fprintf(fp, "%d\n", pid);
    fclose(fp);

    log_message(service, "service started");
    printf("Started %s (pid %d)\n", service, pid);
    return 0;
}

int stop_process(const char *service) {
    int pid;
    if (!process_running(service, &pid)) {
        printf("%s is not running\n", service);
        return -1;
    }

    if (kill(pid, SIGTERM) != 0) {
        perror("kill");
        return -1;
    }

    char path[256];
    pid_path(service, path, sizeof(path));
    unlink(path);

    log_message(service, "service stopped");
    printf("Stopped %s (pid %d)\n", service, pid);
    return 0;
}

void stop_all_processes(void) {
    const char *services[] = {"worker", "web"};
    for (size_t i = 0; i < 2; i++)
        stop_process(services[i]);
}
