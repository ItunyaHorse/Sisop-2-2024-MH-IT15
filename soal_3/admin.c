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
    int sementara[2];
    pid_t pid;
    pipe(sementara);
    pid = fork();
    if (pid == 0) { 
        close(sementara[0]);
        dup2(sementara[1], STDOUT_FILENO);
        close(sementara[1]);
        execlp("ps", "ps", "-aux", NULL);
    } else {
        close(sementara[1]);
        dup2(sementara[0], STDIN_FILENO);
        close(sementara[0]);
        execlp("grep", "grep", username, NULL);
    }
  
}

void mulai(char *nama){
  char logfile[100];
  FILE *file_ptr;
  sprintf(logfile, "%s.log", nama);
  file_ptr = fopen(logfile, "w");
 //ps -eo pid,lstart,cmd | awk /mken/\'{print \"[\"$3\"-\"$4 \"-2024\"  \"]-[\" $5 \"]-\" $1 \"-\" $7}\'"
  while (1) {
    int bentar[2];
    pid_t pid;
    pipe(bentar);
    pid = fork();
    if (pid == 0) { 
        close(bentar[0]);
        dup2(bentar[1], STDOUT_FILENO);
        close(bentar[1]);
        execlp("ps", "ps", "-eo", "pid,lstart,cmd", NULL);
    } else {
        close(bentar[1]);
        dup2(bentar[0], STDIN_FILENO);
        close(bentar[0]);
        execlp("awk", "awk", "{print \"[\"$3\"-\"$4 \"-2024\"  \"]-[\" $5 \"]-\" $1 \"-\" $7}", NULL);
    }
  sleep(30);
  }
}

int main(int argc, char *argv[]) {

    char *username = argv[1];
    char *user = argv[2];

    if (strcmp(argv[1], "-m") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            mulai(user);
            exit(0);
        }
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
