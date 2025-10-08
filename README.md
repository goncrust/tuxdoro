# 🐧 tuxdoro: A Minimalist CLI Pomodoro Timer for Linux

**WIP**

**tuxdoro** is a light-weight Pomodoro timer for Linux written in C. It seperates the timer logic (daemon) from the user interface (CLI client) using IPC.

## Architecture

tuxdoro is split into two components:

### 1. `pomodaemon` (The Service)

`pomodaemon` is the background service that manages the core state of the Pomodoro timer. It runs continuously, tracking time, sessions completed, and storing all user-defined configuration settings (work time, rest times, and session count).

### 2. `pomo` (The Client)

`pomo` is the command-line interface (CLI) that communicates with `pomodaemon`. It sends commands to start, stop, configure, and query the current status of the running timer.

## Usage and Commands

The primary way to interact with the timer is through the `pomo` client utility.

### Timer Management

| Command           | Description                                                                                  |
| ----------------- | -------------------------------------------------------------------------------------------- |
| **`pomo start`**  | Starts the current timer (Work or Rest).                                                     |
| **`pomo pause`**  | Pauses the currently running timer.                                                          |
| **`pomo status`** | Requests the current timer state, remaining time, and number of Pomodoro sessions completed. |
| **`pomo skip`**   | Skips the current phase (e.g., skips a short rest to start the next session).                |
| **`pomo reset`**  | Resets the timer state and the count of completed Pomodoro sessions.                         |

### Configuration

To customize the Pomodoro cycles you can use the following commands:

| Command                          | Description                                              | Default    |
| -------------------------------- | -------------------------------------------------------- | ---------- |
| **`pomo set-session [minutes]`** | Sets the length of a Pomodoro work session.              | 25 min     |
| **`pomo set-srest [minutes]`**   | Sets the length of a short rest period.                  | 5 min      |
| **`pomo set-lrest [minutes]`**   | Sets the length of a long rest period.                   | 15 min     |
| **`pomo set-irest [sessions]`**  | Sets the number of Pomodoro sessions before a long rest. | 4 sessions |
