#ifndef CONFIG_H
#define CONFIG_H

#define MAX_SERVICES 8

typedef struct {
    const char *name;
    const char *command;
} ServiceConfig;

const ServiceConfig *get_services(int *count);
const ServiceConfig *find_service(const char *name);

#endif
