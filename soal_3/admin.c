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

void daemoner(){
    pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    FILE *pid_file = fopen("daemon.pid", "w");
    fprintf(pid_file, "%d", pid);
    fclose(pid_file);
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

void mulai(char *nama, int status){

char logfile[100];
FILE *file_ptr;
sprintf(logfile, "%s.log", nama);
file_ptr = fopen(logfile, "a");
  if (status == 0){
    int normal[2];
    pid_t pid;
    pipe(normal);
    pid = fork();
    if (pid == 0) {
        dup2(normal[1], STDOUT_FILENO);
        close(normal[0]);
        execlp("ps", "ps", "-eo", "pid,lstart,cmd", NULL);
    }
    else{
      dup2(normal[0], STDIN_FILENO);
      close(normal[1]);
      dup2(fileno(file_ptr), STDOUT_FILENO);
      fclose(file_ptr);
      execlp("awk", "awk", "{print \"[\"$3\"-\"$4 \"-2024\"  \"]-[\" $5 \"]-\" $1 \"-\" $7 \"_BERHASIL\"}", NULL);
    }
  }

  else {

 int ps_tail[2];
    int tail_awk[2];
    pid_t pid_ps, pid_tail, pid_awk;

    pipe(ps_tail);

    // FFork ke 1 buat dasar
    pid_ps = fork();
    if (pid_ps == 0) {  
        dup2(ps_tail[1], STDOUT_FILENO);
        close(ps_tail[0]);  
        execlp("ps", "ps", "-eo", "pid,lstart,cmd", NULL);
    }

    pipe(tail_awk);

    // Fork ke 2 buat pipe 2
    pid_tail = fork();
    if (pid_tail == 0) {  
        dup2(ps_tail[0], STDIN_FILENO);
        close(ps_tail[1]);  
        close(tail_awk[0]); 
        dup2(tail_awk[1], STDOUT_FILENO);
        close(tail_awk[1]); 
        execlp("tail", "tail", "-n", "1", NULL);
    }

    // Fork ke 3 buat pipe 3
    pid_awk = fork();
    if (pid_awk == 0) {  
        close(ps_tail[0]); 
        close(ps_tail[1]);  
        close(tail_awk[1]); 
        dup2(tail_awk[0], STDIN_FILENO);
        dup2(fileno(file_ptr), STDOUT_FILENO);
        fclose(file_ptr);
        execlp("awk", "awk", "{print \"[\"$3\"-\"$4 \"-2024\"  \"]-[\" $5 \"]-\" $1 \"-\" $7 \"_GAGAL\"}", NULL);
    }

    close(ps_tail[0]);
    close(ps_tail[1]);
    close(tail_awk[0]);
    close(tail_awk[1]);
  }
}

void penggagal(char *teks){
  FILE *file_ptr;
  file_ptr = fopen(teks, "w");
  fprintf(file_ptr, "1");
  fclose(file_ptr);
}

void hilanggagal(char *file){
  FILE *file_ptr;
  file_ptr = fopen(file, "w");
  fprintf(file_ptr, "0");
  fclose(file_ptr);
}

void pembunuh(){
    FILE *gudang = fopen("daemon.pid", "r");
    pid_t pid;
    fscanf(gudang, "%d", &pid);
    fclose(gudang);
    kill(pid, 9);
}

int main(int argc, char *argv[]) {

    char *username = argv[1];
    char *user = argv[2];
    int status;
    FILE *state_file = fopen("state.txt", "r");
    fscanf(state_file, "%d", &status);

    if (strcmp(argv[1], "-c") == 0) {
      penggagal("state.txt");
    } 
    else if (strcmp(argv[1], "-m") == 0) {
      daemoner();
      while(1){
        mulai(user, status);
        sleep(60);
      }
    } 
    else if (strcmp(argv[1], "-a") == 0) {
      hilanggagal("state.txt");

    } 
    else if (strcmp(argv[1], "-s") == 0) {
      pembunuh();
    } 
    else {
        checkadmin(username);
    }
}
