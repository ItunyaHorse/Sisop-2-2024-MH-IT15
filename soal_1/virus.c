#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MAX_PATH_LENGTH 1024
#define MAX_FILENAME_LENGTH 256
#define MAX_LOG_LENGTH 512

void write_to_log(char *log_path, char *message, char *filename) {
    FILE *log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        printf("Error opening log file!\n");
        return;
    }

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char timestamp[MAX_LOG_LENGTH];
    strftime(timestamp, MAX_LOG_LENGTH, "[%d-%m-%Y][%H:%M:%S]", timeinfo);

    fprintf(log_file, "%s %s at %s successfully replaced!\n", timestamp, message, filename);
    fclose(log_file);
}

void replace_strings(char *file_path, char *log_path) {
    FILE *file = fopen(file_path, "r+");
    if (file == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }
    char line[MAX_LOG_LENGTH];
    char *filename = strrchr(file_path, '/');
if (filename != NULL) {
    filename++; // Move past the '/'
} else {
    filename = file_path; // If no '/' is found, use the entire path as filename
}

    long current_pos;
    long prev_pos = 0;
    while (fgets(line, sizeof(line), file)) {
        current_pos = ftell(file);

        for (int i = 0; line[i] != '\0'; ++i) {
            if (strncmp(&line[i], "m4LwAr3", 7) == 0) {
                fseek(file, prev_pos + i, SEEK_SET);
                fputs("[MALWARE]", file); // Replace the string
                printf("Suspicious string at %s successfully replaced!\n", file_path);
                write_to_log(log_path, "Suspicious string", filename);
            }
            if (strncmp(&line[i], "5pYw4R3", 7) == 0) {
                fseek(file, prev_pos + i, SEEK_SET);
                fputs("[SPYWARE]", file); // Replace the string
                printf("Suspicious string at %s successfully replaced!\n", file_path);
                write_to_log(log_path, "Suspicious string", filename);
            }
            if (strncmp(&line[i], "R4nS0mWaR3", 10) == 0) {
                fseek(file, prev_pos + i, SEEK_SET);
                fputs("[RANSOMWARE]", file); // Replace the string
                printf("Suspicious string at %s successfully replaced!\n", file_path);
                write_to_log(log_path, "Suspicious string", filename);
            }
        }
        prev_pos = current_pos;
    }

    fclose(file);
}

void run_daemon(char *folder_path, char *log_path) {
    while (1) {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(folder_path)) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                char file_path[MAX_PATH_LENGTH];
                snprintf(file_path, MAX_PATH_LENGTH, "%s/%s", folder_path, ent->d_name);
                if (ent->d_type == DT_DIR || strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                    continue;
                replace_strings(file_path, log_path);
            }
            closedir(dir);
        } else {
            printf("Error opening directory: %s\n", folder_path);
        }
        sleep(15);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s /path/to/folder\n", argv[0]);
        return 1;
    }

    char *folder_path = argv[1];
    char log_path[MAX_PATH_LENGTH];
    snprintf(log_path, MAX_PATH_LENGTH, "%s/virus.log", folder_path);

    run_daemon(folder_path, log_path);
    return 0;
}
