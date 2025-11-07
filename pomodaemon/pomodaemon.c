#include "pomoconfig.h"
#include "pomostate.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define SOCKET_PATH "/home/goncrust/tmp/tuxdoro.sock"
#define BACKLOG 5
#define BUFFER_SIZE 256

int setup_socket() {
  int sfd;
  struct sockaddr_un addr;

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1) {
    perror("failed to create socket");
    exit(EXIT_FAILURE);
  }

  unlink(SOCKET_PATH);

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(SOCKET_PATH) - 1);
  addr.sun_path[sizeof(SOCKET_PATH)] = '\0';

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("failed to bind");
    close(sfd);
    exit(EXIT_FAILURE);
  }

  if (listen(sfd, BACKLOG) == -1) {
    perror("failed to listen");
    close(sfd);
    exit(EXIT_FAILURE);
  }
  printf("Daemon is listening on %s\n", SOCKET_PATH);

  return sfd;
}

void handle_command(char *buffer, char *response, struct PomoState *pstate,
                    struct PomoConfig *pconfig) {
  // TODO
  if (strncmp(buffer, "test", 4) == 0) {
    strncpy(response, "testresponse", 12);
    response[13] = '\0';
  }
}

void next_phase(struct PomoState *pstate, struct PomoConfig *pconfig) {
  // TODO
}

void run_server(int sfd) {
  int cfd;
  fd_set read_fds;
  struct timeval timeout;
  char buffer[BUFFER_SIZE];

  struct PomoConfig pconfig = {25 * 60, 5 * 60, 15 * 60, 4};
  struct PomoState pstate = {WORK, 0, pconfig.work_time, 0, 0};

  time_t start_time = time(NULL);

  // main loop
  while (1) {
    // calculate remaining time
    if (!pstate.stopped) {
      long elapsed_time = (long)(time(NULL) - start_time);
      pstate.remaining_seconds -= elapsed_time;
    }

    if (pstate.remaining_seconds <= 0) {
      next_phase(&pstate, &pconfig);
      start_time = time(NULL);
      continue;
    }

    FD_ZERO(&read_fds);
    FD_SET(sfd, &read_fds);
    timeout.tv_sec = pstate.remaining_seconds;
    timeout.tv_usec = 0;

    // wait for client
    printf("Waiting for client (or %ld seconds until phase end)...\n",
           pstate.remaining_seconds);
    int ready_count = select(sfd + 1, &read_fds, NULL, NULL, &timeout);

    if (ready_count < 0) {
      if (errno == EINTR)
        continue;
      perror("failed select");
      break;
    }

    if (ready_count == 0) {
      // timeout
      continue;
    }

    if (FD_ISSET(sfd, &read_fds)) {
      cfd = accept(sfd, NULL, NULL);
      if (cfd == -1) {
        perror("failed accepting connection");
        continue;
      }
      printf("Client connected.\n");

      // receive msg and reply
      ssize_t bytes_read = recv(cfd, buffer, BUFFER_SIZE - 1, 0);
      if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Received command: '%s'\n", buffer);

        char response[BUFFER_SIZE];
        handle_command(buffer, response, &pstate, &pconfig);

        if (send(cfd, response, strlen(response), 0) == -1) {
          perror("failed to send response");
        }
        printf("Sent response: '%s'\n", response);
      }

      close(cfd);
    }
  }
}

int main() {
  int sfd = setup_socket();
  run_server(sfd);

  close(sfd);
  unlink(SOCKET_PATH);

  return 0;
}
