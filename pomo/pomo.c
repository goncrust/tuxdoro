#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "~/tmp/tuxdoro.sock"
#define BUFFER_SIZE 256

void send_command(const char *command) {
  int sock_fd;
  struct sockaddr_un addr;
  char buffer[BUFFER_SIZE];

  sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("failed creating socket");
    exit(EXIT_FAILURE);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  // connect to server
  if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("connect failed: Is pomodaemon running?");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }
  printf("Connected to daemon.\n");

  // send command
  if (send(sock_fd, command, strlen(command), 0) == -1) {
    perror("send");
    close(sock_fd);
    exit(EXIT_FAILURE);
  }
  printf("Sent command: '%s'\n", command);

  // receive response
  ssize_t bytes_read = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    printf("Received response: '%s'\n", buffer);
  } else if (bytes_read == 0) {
    printf("Daemon closed connection.\n");
  } else {
    perror("failed recv");
  }

  close(sock_fd);
}

int main(int argc, char *argv[]) {
  // TODO
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <command>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  send_command(argv[1]);
  return 0;
}
