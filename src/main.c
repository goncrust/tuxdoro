#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Pomodoro {
    int minutes;
    int pause_time;
    int session_count;
} Pomodoro;

typedef struct Status {
    int current_min;
    int current_session;
    bool pause;
} Status;

Pomodoro pomodoro = {0, 0, 0};
Status status = {0, 0, false};

bool run_pomodoro() { return false; }

void print_help() {
    char *help_message =
        "Usage: tuxdoro [options]\n"
        "Options:\n"
        "  -h      Display this help message\n"
        "  -m      Specify minutes of each session (Required)\n"
        "  -b      Specify minutes of break time between each session "
        "(Required)\n"
        "  -s      Specify the number of sessions to do\n";
}

int main(int argc, char *argv[]) {
    int option;
    char *value;
    bool m_ok = false, b_ok = false;

    while ((option = getopt(argc, argv, "m:b:s:h"))) {
        switch (option) {
        case 'm':
            m_ok = true;
            value = optarg;
            sscanf(value, "%d", &pomodoro.minutes);
            break;
        case 'b':
            b_ok = true;
            value = optarg;
            sscanf(value, "%d", &pomodoro.pause_time);
            break;
        case 's':
            value = optarg;
            sscanf(value, "%d", &pomodoro.session_count);
            break;
        case 'h':
        case '?':
            print_help();
            exit(0);
            break;
        default:
            break;
        }
    }

    if (!m_ok || !b_ok) {
        print_help();
        exit(0);
    }

    run_pomodoro();
}
