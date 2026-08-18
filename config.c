#include "config.h"
#include <string.h>

static const ServiceConfig services[] = {
    {"worker", "while true; do echo '[worker] running'; sleep 3; done"},
    {"web",    "while true; do echo '[web] serving'; sleep 5; done"}
};

const ServiceConfig *get_services(int *count) {
    if (count) *count = (int)(sizeof(services) / sizeof(services[0]));
    return services;
}

const ServiceConfig *find_service(const char *name) {
    int count;
    const ServiceConfig *list = get_services(&count);

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].name, name) == 0)
            return &list[i];
    }
    return NULL;
}
