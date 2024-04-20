#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_APPS 10
#define MAX_NAME_LENGTH 20

typedef struct {
    char name[MAX_NAME_LENGTH];
    int numWindows;
} Application;

void openApplications(Application apps[], int numApps);
void closeApplications();
void parseConfigFile(const char *filename);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s -o <app1> <num1> <app2> <num2> ... OR %s -f <filename> OR %s -k OR %s -k <filename>\n", argv[0], argv[0], argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-o") == 0) {
        if (argc % 2 != 0 || argc > (MAX_APPS * 2 + 2)) {
            printf("Invalid number of arguments\n");
            return 1;
        }

        Application apps[MAX_APPS];
        int numApps = (argc - 2) / 2;

        for (int i = 0; i < numApps; i++) {
            strcpy(apps[i].name, argv[i * 2 + 2]);
            apps[i].numWindows = atoi(argv[i * 2 + 3]);
        }

        openApplications(apps, numApps);
    } else if (strcmp(argv[1], "-f") == 0) {
        if (argc != 3) {
            printf("Invalid number of arguments\n");
            return 1;
        }

        parseConfigFile(argv[2]);
    } else if (strcmp(argv[1], "-k") == 0) {
        if (argc == 2) {
            closeApplications();
        } else if (argc == 3) {
            parseConfigFile(argv[2]);
            closeApplications();
        } else {
            printf("Invalid arguments\n");
            return 1;
        }
    } else {
        printf("Invalid option\n");
        return 1;
    }

    return 0;
}

void openApplications(Application apps[], int numApps) {
    for (int i = 0; i < numApps; i++) {
        for (int j = 0; j < apps[i].numWindows; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                execlp(apps[i].name, apps[i].name, NULL);
                exit(0);
            }
        }
    }
}

void closeApplications() {
    printf("Closing applications...\n");
}

void parseConfigFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }

    char line[100];
    Application apps[MAX_APPS];
    int numApps = 0;

    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME_LENGTH];
        int numWindows;
        if (sscanf(line, "%s %d", name, &numWindows) != 2) {
            printf("Invalid line in config file: %s", line);
            continue;
        }

        strcpy(apps[numApps].name, name);
        apps[numApps].numWindows = numWindows;
        numApps++;
    }

    fclose(file);

    openApplications(apps, numApps);
}
