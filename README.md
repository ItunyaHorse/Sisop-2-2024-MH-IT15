# Soal-Shift-Sisop-Modul-2-IT15-2024

Anggota:

1. Michael Kenneth Salim (5027231008)
2. Nicholas Emanuel Fade (5027231070)
3. Veri Rahman (5027231088)

## Soal no 1
Dikerjakan oleh **Nicholas Emanuel Fade (5027231070)**

Pertama-tama kita disuruh untuk membuat Programnya dapat menerima input path berupa ‘argv’ untuk mengatur folder dimana file akan dieksekusi, jadi ini kita tinggal buat seperti ini

```bash
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
```

Setelah itu kita buat fungsi pertama kodenya yaitu untuk mereplace string dengan ketentuan sebagai berikut:
- String 'm4LwAr3' direplace dengan string '[MALWARE]'
- String '5pYw4R3' direplace dengan string '[SPYWARE]'
- String 'R4nS0mWaR3' direplace dengan string '[RANSOMWARE]'

```bash
void replace_strings(char *file_path, char *log_path) {
    FILE *file = fopen(file_path, "r+");
    if (file == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }
    char line[MAX_LOG_LENGTH];
    char *filename = strrchr(file_path, '/');
if (filename != NULL) {
    filename++;
} else {
    filename = file_path;
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
```

Kemudian kita buat fungsi kedua kodenya yaitu untuk mencatat setiap penghapusan string yang dilakukan oleh program pada sebuah file bernama virus.log

```bash
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
```
Terakhir programnya harus berjalan setiap 15 detik

```bash
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
```
Setelah membuat kode C nya kita perlu untuk mengcompile filenya dengan command: gcc virus.c -o virus

Jadi cara kerja programnya adalah:

1. program pertama menerima input di fungsi main: ./virus /path/to/directory
2. kemudian ia membuat log filenya di direktori akhir file pathnya
3. setelah itu fungsi 'run_daemon' dipanggil dan loop dengan jeda 15 detik
4. fungsi 'replace_strings' membuka dan membaca semua file yang didalam direktori yang di input
5. jika di dalam file ia menemukan string 'm4LwAr3', '5pYw4R3' atau 'R4nS0mWaR3' funsginya akan menggantinya
6. Setiap kali sebuah string diganti dalam sebuah berkas, program mencetak pesan yang menunjukkan penggantian yang berhasil dan juag di catat di log filenya
7. Setelah memproses semua berkas dalam direktori, program menunggu selama 15 detik

virus.log setelah virus.c di run:

![image](https://drive.google.com/uc?export=view&id=1W2K5N0m3Bau6qetRQhFruaG_C4QOL5d_)

contoh file setelah virus.c di run:

![image](https://drive.google.com/uc?export=view&id=1on1D2mBwxS9pE8qPcgjy0RSxfWcSV_-o)

## Soal no 2
Dikerjakan oleh **Veri Rahman (5027231088)**

## Soal no 3
Dikerjakan oleh **Michael Kenneth Salim (5027231008)**

## Soal no 4
Dikerjakan oleh **Michael Kenneth Salim (5027231008)**
