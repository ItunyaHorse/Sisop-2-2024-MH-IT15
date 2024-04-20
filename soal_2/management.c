#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>

#define MAX_FILENAME_LENGTH 256
#define HISTORY_LOG "history.log"
#define BACKUP_FOLDER "backup"
#define MAX_PATH_LENGTH 100

char *mode = "default";

void handle_signal(int signal);
void download_and_unzip();
void decrypt_filenames();
void rename_files();
void delete_file(const char *filename);
void move_to_backup(const char *filename);
void restore_from_backup(const char *filename);
void log_action(char *username, char *filename, char *action);

char *get_username() {
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        return pw->pw_name;
    }
    return "unknown";
}

int main(int argc, char *argv[]) {
    char *filename;
    if (argc > 1) {
        if (strcmp(argv[1], "-m") == 0 && argc > 2) {
            if (strcmp(argv[2], "default") == 0) {
                mode = "default";
            } else if (strcmp(argv[2], "backup") == 0) {
                mode = "backup";
            } else if (strcmp(argv[2], "restore") == 0) {
                mode = "restore";
            }
        }
    }

    signal(SIGRTMIN, handle_signal);
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);

    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if (sid < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        if (strcmp(mode, "default") == 0) {
            download_and_unzip();
            decrypt_filenames();
            rename_files();
        } else if (strcmp(mode, "backup") == 0) {
            move_to_backup(filename);
        } else if (strcmp(mode, "restore") == 0) {
            restore_from_backup(filename);
        }
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}

void handle_signal(int signal) {
    if (signal == SIGRTMIN) {
        mode = "default";
    } else if (signal == SIGUSR1) {
        mode = "backup";
    } else if (signal == SIGUSR2) {
        mode = "restore";
    }
}

void download_and_unzip() {
    pid_t child_pid;
    int status;
    child_pid = fork();

    if (child_pid == 0) {
        execlp("wget", "wget", "https://drive.google.com/file/d/1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup/view?usp=sharing", "-O", "library.zip", NULL);
        perror("exec wget");
        exit(EXIT_FAILURE);
    } else if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        execlp("unzip", "unzip", "library.zip", NULL);
        perror("exec unzip");
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_FAILURE);
    }
}

void decrypt_filenames() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *filename = entry->d_name;
            int i = 0;
            while (filename[i] != '\0') {
                if ((filename[i] >= 'a' && filename[i] <= 'z')) {
                    filename[i] = (((filename[i] - 'a') + 19) % 26) + 'a';
                } else if ((filename[i] >= 'A' && filename[i] <= 'Z')) {
                    filename[i] = (((filename[i] - 'A') + 19) % 26) + 'A';
                }
                i++;
            }
        }
    }

    closedir(dir);
}

void rename_files() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filename[MAX_FILENAME_LENGTH];
            strcpy(filename, entry->d_name);
            char *old_name = entry->d_name;
            char *new_name;
            if (strstr(old_name, "d3Let3") != NULL) {
                delete_file(old_name);
            } else if (strstr(old_name, "r3N4mE") != NULL) {
                if (strcmp(old_name + strlen(old_name) - 3, ".ts") == 0) {
                    new_name = "helper.ts";
                } else if (strcmp(old_name + strlen(old_name) - 3, ".py") == 0) {
                    new_name = "calculator.py";
                } else if (strcmp(old_name + strlen(old_name) - 3, ".go") == 0) {
                    new_name = "server.go";
                } else {
                    new_name = "renamed.file";
                }

                if (rename(old_name, new_name) != 0) {
                    perror("Error renaming file");
                } else {
                    log_action(get_username(), old_name, "Successfully renamed");
                }
            }
        }
    }

    closedir(dir);
}

void delete_file(const char *filename) {
    if (remove(filename) != 0) {
        perror("Delete failed");
        exit(EXIT_FAILURE);
    }
}

void move_to_backup(const char *filename) {
    char new_path[MAX_PATH_LENGTH];
    snprintf(new_path, sizeof(new_path), "%s/%s", BACKUP_FOLDER, filename);
    if (rename(filename, new_path) != 0) {
        perror("Move to backup failed");
        exit(EXIT_FAILURE);
    }
}

void restore_from_backup(const char *filename) {
    char new_path[MAX_PATH_LENGTH];
    snprintf(new_path, sizeof(new_path), "%s/%s", BACKUP_FOLDER, filename);
    if (rename(new_path, filename) != 0) {
        perror("Restore from backup failed");
        exit(EXIT_FAILURE);
    }
}

void log_action(char *username, char *filename, char *action) {
    FILE *log_file = fopen(HISTORY_LOG, "a");
    if (log_file == NULL) {
        perror("Failed to open history log file");
        exit(EXIT_FAILURE);
    }

    time_t now;
    struct tm *timestamp;
    time(&now);
    timestamp = localtime(&now);
    char timestamp_2[9];

    strftime(timestamp_2, sizeof(timestamp_2), "%H:%M:%S", timestamp);

    fprintf(log_file, "[%s][%s] - %s - %s\n", username, timestamp_2, filename, action);
    fclose(log_file);
}
