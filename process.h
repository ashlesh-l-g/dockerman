#ifndef PROCESS_H
#define PROCESS_H

int start_process(const char *service, const char *command);
int stop_process(const char *service);
int process_running(const char *service, int *pid_out);
void stop_all_processes(void);

#endif
