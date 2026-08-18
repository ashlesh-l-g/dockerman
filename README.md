# Dockerman

A lightweight process orchestration tool written in C, inspired by basic Docker concepts.

Dockerman is intentionally small and focuses on understanding the operating-system primitives behind a container-like CLI rather than trying to implement Docker itself.

## Features

- Start multiple configured services with one command
- Create service processes using `fork()`
- Execute service commands using `/bin/sh -c` and `exec`-style process replacement
- Track service PIDs using PID files
- Check whether a service is running
- Stop services using `SIGTERM`
- Display service status with PID information
- Capture service stdout/stderr into per-service log files
- Keep configuration, process management, monitoring, logging, and CLI handling in separate C modules

## Project Structure

```text
dockerman/
├── main.c          # CLI entry point and command handling
├── config.c        # Service definitions
├── config.h
├── process.c       # fork, PID tracking, signals and process lifecycle
├── process.h
├── monitor.c       # Service status display
├── monitor.h
├── logger.c        # Timestamped service lifecycle logs
├── logger.h
├── utils.c         # CLI usage/help
├── utils.h
├── Makefile
├── logs/           # Service output and lifecycle logs
└── run/            # Runtime PID files
```

## How It Works

### 1. Service configuration

Services are defined in `config.c`.

The current demo contains two services:

- `worker`
- `web`

Each service has a name and a shell command that continuously produces output.

### 2. Starting services

Running:

```bash
./dockman up
```

causes Dockerman to:

1. Read the configured services.
2. Call `fork()` for each service.
3. In the child process, redirect stdout/stderr to the service log file.
4. Execute the configured shell command.
5. In the parent process, store the child PID in `run/<service>.pid`.

Example:

```text
./dockman up
        |
        +-- worker -> fork() -> service process
        |
        +-- web    -> fork() -> service process
```

### 3. Process tracking

Dockerman stores each service PID in a small PID file:

```text
run/worker.pid
run/web.pid
```

For `ps`, the program reads the PID and uses `kill(pid, 0)` to check whether the process is still alive.

### 4. Stopping services

Running:

```bash
./dockman stop
```

reads the stored PIDs and sends:

```text
SIGTERM
```

to the corresponding service processes.

The PID files are then removed.

### 5. Logging

Each service has its own log file:

```text
logs/worker.log
logs/web.log
```

The child process redirects:

```text
stdout -> service log
stderr -> service log
```

This allows the service's output to be inspected later.

## Build

You need a Linux environment with GCC and Make.

Check that they are installed:

```bash
gcc --version
make --version
```

Build the project:

```bash
make
```

To rebuild from scratch:

```bash
make clean
make
```

## Usage

### Start all services

```bash
./dockman up
```

Example:

```text
Started worker (pid 1234)
Started web (pid 1235)
```

### Check service status

```bash
./dockman ps
```

Example:

```text
SERVICE      STATUS     PID
--------------------------------
worker       running    1234
web          running    1235
```

### View worker logs

```bash
./dockman logs worker
```

### View web logs

```bash
./dockman logs web
```

### Stop all services

```bash
./dockman stop
```

## Complete Demo

A quick demonstration can be run with:

```bash
make clean
make
./dockman up
./dockman ps
./dockman logs worker
./dockman logs web
./dockman stop
./dockman ps
```

## What This Project Is Not

Dockerman is **not a Docker replacement** and does not provide full container isolation.

It does not currently implement:

- Linux namespaces
- cgroups
- filesystem/container root isolation
- virtual networking
- container images
- layered filesystems
- resource limits
- container registries

The goal of this project is to demonstrate the basic process-management layer that can be used as a foundation for a more complete container runtime.

## Future Improvements

Possible next steps would be:

1. Add Linux namespaces for process, network, mount, and hostname isolation.
2. Add cgroups for CPU and memory limits.
3. Add filesystem isolation using a dedicated root filesystem.
4. Add a configuration file instead of hard-coded services.
5. Add service restart policies.
6. Add basic networking between services.

## Technologies

- C
- Linux/POSIX system calls
- `fork()`
- `exec`/`/bin/sh -c`
- `kill()` and Unix signals
- PID files
- GCC
- GNU Make

## Purpose

This project was built as a small systems-programming project to explore how process creation, execution, signals, PID management, and logging can be combined to build a simple service orchestration tool.
