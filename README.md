# Dockerman - Basic Container Orchestration Demo

A deliberately small C project inspired by core Docker concepts.

It demonstrates:
- service configuration
- starting background processes with `fork()` + `exec()`
- PID tracking
- stopping processes with signals
- basic service monitoring
- per-service logs

## Build

```bash
make
```

## Run

```bash
./dockman up
./dockman ps
./dockman logs worker
./dockman logs web
./dockman stop
```

The services are intentionally simple shell loops rather than real containers. The point is to demonstrate the process-management core of a Docker-like CLI without implementing namespaces, cgroups, images, networking, etc.
