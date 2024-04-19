#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

void checkadmin(char *username) {
  strcat(username, ".log");
  char *argv[] = {"list", username, NULL};
  execv("/bin/cat", argv);
}

void mulai(char *nama){
  strcat(nama, ".log");
  char args[]= {"ps -eo pid,lstart,cmd | awk /mken/\'{print \"[\"$3\"-\"$4 \"-2024\"  \"]-[\" $5 \"]-\" $1 \"-\" $7}\' > %s", &*nama};
  execlp("sh", "sh", "-c", args, NULL);
}

void daemonstart(){
  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}

void stop(){
    char *argv[] = {"cp", "-9", NULL};
    execv("/bin/kill", argv);
}

void kill(){
    char *argv[] = {"killing", "-9", (int)getpid ,NULL};
    execv("/bin/kill", argv);
}


int main(int argc, char *argv[]) {

    char *username = argv[1];
    char *user = argv[2];

    if (strcmp(argv[1], "-m") == 0) {
        mulai(user);
    } 
    else if (strcmp(argv[1], "-c") == 0) {
        system("echo -c");
    } 
    else if (strcmp(argv[1], "-a") == 0) {
        system("echo -a");
    } 
    else if (strcmp(argv[1], "-s") == 0) {
        system("echo -s");
    } 
    else {
        checkadmin(username);
        return 0;
    }
}
