#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Pomodoro {
    int session_minutes;
    int pause_time;
    int session_count;
} Pomodoro;

typedef struct Status {
    int current_sec;
    int current_min;
    int current_hour;
    int current_session;
    bool session_break;
    bool paused;
} Status;

Pomodoro pomodoro = {0, 0, 0};
Status status = {0, 0, 0, false, false};

void reset_timers() {
    status.current_hour = 0;
    status.current_min = 0;
    status.current_sec = 0;
}

void print_time() {
    if (!status.session_break)
        printf("\rSession: ");
    else
        printf("\rBreak: ");
    printf("%02d:%02d:%02d", status.current_hour, status.current_min,
           status.current_sec);
    fflush(stdout);
}

bool run_pomodoro() {
    while (!status.paused) {
        // Update time
        status.current_sec++;
        if (status.current_sec >= 60) {
            status.current_sec = 0;
            status.current_min++;
        }
        if (status.current_min >= 60) {
            status.current_min = 0;
            status.current_hour++;
        }

        // Check timers
        if (!status.session_break) {
            // Check if ended session
            if (status.current_min + status.current_hour * 60 >=
                pomodoro.session_minutes) {
                status.current_session++;
                if (status.current_session >= pomodoro.session_count)
                    return true;
                status.session_break = true;
                reset_timers();
            }
        } else {
            // Check if ended break
            if (status.current_min + status.current_hour * 60 >=
                pomodoro.pause_time) {
                status.session_break = false;
                reset_timers();
            }
        }

        // Display time
        print_time();

        // Sleep
        sleep(1);
    }
    return false;
}

void print_help() {
    char *help_message =
        "Usage: tuxdoro [options]\n"
        "Options:\n"
        "  -h      Display this help message\n"
        "  -m      Specify minutes of each session (Required)\n"
        "  -b      Specify minutes of break time between each session "
        "(Required)\n"
        "  -s      Specify the number of sessions to do\n";

    printf(help_message);
}

int main(int argc, char *argv[]) {
    int option;
    char *value;
    bool m_ok = false, b_ok = false;

    while ((option = getopt(argc, argv, "m:b:s:h")) != -1) {
        switch (option) {
        case 'm':
            m_ok = true;
            value = optarg;
            sscanf(value, "%d", &pomodoro.session_minutes);
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
            print_help();
            exit(0);
            break;
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
