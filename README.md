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

revisi:

- kodenya belum run secara daemon
- kodenya belum mengeset jeda 15 detik setiap string diganti di per 1 file

## Soal no 2
Dikerjakan oleh **Veri Rahman (5027231088)**
Pada soal nomor 2 ini, kita diminta untuk membuat sebuah program management.c yang program tersebut berjalan secara daemon kemudian dapat mengunduh file pada link yang ada untuk di unzip, setelah itu isi file tersebut di decrypt agar menampilkan nama file yang sebenarnya kemudian di ubah sesuai perintah yang ada. program ini juga dapat mebackup dan merestore file dalam bentuk mode dan bisa mengganti mode tanpa mengehentikannya terlebih dahulu, lalu program ini juga memiliki fungsi agar dapat mematikan program dengan aman. semua ini tercatat dalam history.log agar memudahkan pengguna.

```bash
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
#define BACKUP_FOLDER "library/backup"
#define MAX_PATH_LENGTH 100

char *mode = "default";
char *filename = NULL;

void handle_signal(int signal);
void decrypt();
void Rename();
void delete(char *filename);
void backup(char *filename);
void restore(char *filename);
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
    int status;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        execlp("wget", "wget", "https://drive.google.com/file/d/1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup/view?usp=sharing", "-O", "library.zip", NULL);
        perror("exec wget");
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
    while(1){
        if (strcmp(mode, "default") == 0) {
            decrypt();
            Rename();
        } else if (strcmp(mode, "backup") == 0) {
            backup(filename);
        } else if (strcmp(mode, "restore") == 0) {
            restore(filename);
        }
        sleep(1);
    exit(EXIT_SUCCESS);
    }
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


void decrypt() {
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
void Rename() {
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
                delete(old_name);
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
void delete(char *filename) {
    if (remove(filename) != 0) {
        perror("Delete failed");
        exit(EXIT_FAILURE);
    }
}

void backup(char *filename) {
    char new_path[MAX_PATH_LENGTH];
    snprintf(new_path, sizeof(new_path), "%s/%s", BACKUP_FOLDER, filename);
    if (rename(filename, new_path) != 0) {
        perror("Move to backup failed");
        exit(EXIT_FAILURE);
    }
}

void restore(char *filename) {
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

//@Hsy
```
**Penjelasan**
```bash
pid_t pid, sid;
    int status;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        execlp("wget", "wget", "https://drive.google.com/file/d/1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup/view?usp=sharing", "-O", "library.zip", NULL);
        perror("exec wget");
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
```
**A** Berfungsi untuk mendownload dan men unzip file dari link yang disedikan. kita menggunakan "execlp()" untuk menjalankan perintah wget untuk mengunduh file dari URL yang ditentukan. Jika execlp() gagal, pesan kesalahan akan dicetak dan child process akan keluar dengan kode kesalahan EXIT_FAILURE. Lalu, "wait(&status)" adalah Proses parent menunggu child process selesai. Fungsi wait() akan menghentikan eksekusi parent process sampai child process selesai dieksekusi. "if (WIFEXITED(status) && WEXITSTATUS(status) == 0)" Setelah child process selesai dieksekusi, parent process memeriksa status keluaran child process. Jika child process berakhir secara normal dan keluar dengan status 0, maka parent process akan menjalankan execlp() untuk mengekstraksi file zip menggunakan perintah unzip. Jika tidak, parent process akan keluar dengan kode kesalahan EXIT_FAILURE.

```bash
void decrypt() {
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
```
**B** Fungsi decrypt() memiliki tujuan untuk mendekripsi nama file yang ada dalam sebuah direktori. Proses dekripsi dilakukan pada setiap karakter dalam nama file menggunakan algoritma ROT19. Karakter huruf kecil (a-z) dan huruf besar (A-Z) akan digeser ke posisi 19 karakter ke kanan dalam alfabet. Proses ini dilakukan dengan memodifikasi langsung nilai karakter dalam filename. Setelah proses dekripsi selesai, pointer entry dipindahkan ke entri file berikutnya dalam direktori. Setelah seluruh entri file dalam direktori diproses, direktori ditutup dengan memanggil fungsi closedir(dir) untuk membebaskan sumber daya yang digunakan.

```bash
void Rename() {
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
                delete(old_name);
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
```
**C** Fungsi Rename() bertujuan untuk melakukan operasi renaming terhadap file-file yang ada dalam sebuah direktori. Proses pemilihan nama baru dilakukan berdasarkan kondisi yang ditentukan. Jika nama file mengandung substring "d3Let3", maka file tersebut akan dihapus dengan memanggil fungsi delete(old_name). Jika nama file mengandung substring "r3N4mE", maka nama baru akan ditentukan berdasarkan ekstensi file. Jika ekstensinya adalah ".ts", ".py", atau ".go", nama baru akan ditetapkan sesuai dengan jenis file tersebut. Jika ekstensinya tidak dikenali, maka nama baru akan diset menjadi "renamed.file". Setelah nama baru ditetapkan, fungsi rename() akan dipanggil untuk melakukan operasi renaming dari old_name menjadi new_name. Jika operasi renaming gagal, pesan kesalahan akan dicetak menggunakan perror(). Jika berhasil, fungsi log_action() akan dipanggil untuk mencatat kegiatan renaming tersebut. Setelah seluruh entri file dalam direktori diproses, direktori ditutup dengan memanggil fungsi closedir(dir) untuk membebaskan sumber daya yang digunakan.

```bash
void backup(char *filename) {
    char new_path[MAX_PATH_LENGTH];
    snprintf(new_path, sizeof(new_path), "%s/%s", BACKUP_FOLDER, filename);
    if (rename(filename, new_path) != 0) {
        perror("Move to backup failed");
        exit(EXIT_FAILURE);
    }
}

void restore(char *filename) {
    char new_path[MAX_PATH_LENGTH];
    snprintf(new_path, sizeof(new_path), "%s/%s", BACKUP_FOLDER, filename);
    if (rename(new_path, filename) != 0) {
        perror("Restore from backup failed");
        exit(EXIT_FAILURE);
    }
}
```
**D** Fungsi backup() dan restore() berfungsi untuk melakukan operasi backup dan restore terhadap file pada direktori yang ada. Variabel "new_path" digunakan untuk menyimpan path baru untuk file yang akan dibackup. Fungsi "snprintf()" digunakan untuk mengonversi BACKUP_FOLDER dan filename menjadi string yang merupakan path lengkap menuju file yang akan dibackup. snprintf() digunakan untuk memastikan bahwa buffer new_path tidak melebihi ukuran maksimum yang telah ditentukan (MAX_PATH_LENGTH).
"if (rename(filename, new_path) != 0)" dipanggil untuk memindahkan (rename) file yang diberikan dari path semula ke new_path. Jika operasi rename gagal (nilai kembalian tidak sama dengan 0), maka pesan kesalahan akan dicetak menggunakan perror() dan program akan keluar dengan kode kesalahan EXIT_FAILURE.
Variabel "new_path" digunakan untuk menyimpan path baru untuk file yang akan direstore dari backup. Fungsi "snprintf()" digunakan untuk mengonversi BACKUP_FOLDER dan filename menjadi string yang merupakan path lengkap menuju file yang akan direstore dari backup. "if (rename(new_path, filename) != 0)" dipanggil untuk memindahkan (rename) file dari path backup (new_path) kembali ke path semula (filename). Jika operasi rename gagal (nilai kembalian tidak sama dengan 0), maka pesan kesalahan akan dicetak menggunakan perror() dan program akan keluar dengan kode kesalahan EXIT_FAILURE.

```bash
void handle_signal(int signal) {
    if (signal == SIGRTMIN) {
        mode = "default";
    } else if (signal == SIGUSR1) {
        mode = "backup";
    } else if (signal == SIGUSR2) {
        mode = "restore";
    }
}
```
**E** Fungsi handle_signal adalah penangan sinyal yang bertanggung jawab untuk mengubah mode kerja program. "SIGRTMIN" digunakan untuk mengubah mode program menjadi mode default. Ketika sinyal ini diterima, variabel mode akan diubah menjadi string "default". "SIGUSR1" digunakan untuk mengubah mode program menjadi mode backup. Ketika sinyal ini diterima, variabel mode akan diubah menjadi string "backup". "SIGUSR2" digunakan untuk mengubah mode program menjadi mode restore. Ketika sinyal ini diterima, variabel mode akan diubah menjadi string "restore".

```bash
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
```
**F** Fungsi log_action bertanggung jawab untuk mencatat suatu tindakan (action) yang dilakukan pada file ke dalam sebuah file log. "FILE *log_file = fopen(HISTORY_LOG, "a")" Fungsi fopen() digunakan untuk membuka file log dengan mode "a" (append), yang berarti data akan ditambahkan ke akhir file tanpa menghapus konten yang sudah ada sebelumnya. Jika file tidak dapat dibuka, maka akan dicetak pesan kesalahan menggunakan perror() dan program akan keluar dengan kode kesalahan EXIT_FAILURE.
"time_t now; struct tm *timestamp; time(&now); timestamp = localtime(&now)" Variabel now digunakan untuk menyimpan waktu saat ini dalam bentuk Unix timestamp. Fungsi time() digunakan untuk mendapatkan waktu saat ini. Selanjutnya, waktu saat ini diubah menjadi waktu lokal menggunakan fungsi localtime() dan disimpan dalam variabel timestamp.
"char timestamp_2[9]; strftime(timestamp_2, sizeof(timestamp_2), "%H:%M:%S", timestamp)" Waktu yang telah diubah menjadi waktu lokal kemudian diformat sesuai dengan format yang diinginkan (HH:MM:SS) menggunakan fungsi strftime(). Hasil format disimpan dalam variabel timestamp_2. "fprintf(log_file, "[%s][%s] - %s - %s\n", username, timestamp_2, filename, action)" Menggunakan fprintf(), tindakan (action) yang terjadi pada file direkam ke dalam file log. Format yang digunakan adalah "[username][timestamp] - filename - action\n". Data yang direkam mencakup username, timestamp (dalam format HH:MM:SS), nama file, dan tindakan yang dilakukan. "fclose(log_file)" Setelah selesai menulis ke file log, file ditutup menggunakan fclose() untuk memastikan data tertulis ke dalam file.

**HASIL**

![image](https://drive.google.com/uc?export=view&id=1bNatp_pGeDNyxbeofX3tcxC5sqpoexLy)

![image](https://drive.google.com/uc?export=view&id=1Rqln1iame4Aq6vDDinue7X3RprjRisgK)

![image](https://drive.google.com/uc?export=view&id=1YnoEQSLrElbGiNxoLt13MFIniU3M1dyK)

**KENDALA**
link yang tidak bisa di download sehingga menyebabkan step selanjutnya berhenti atau tidak jalan. lalu program tidak berjalan secara daemon

## Soal no 3
Dikerjakan oleh **Michael Kenneth Salim (5027231008)**

Pada soal nomer 3 ini, kita diminta untuk membuat sebuah program admin.c, yang mana memiliki fungsi untuk memantau proses yang dilakukan oleh pengguna sistem. Dengan menggunakan argumen baris perintah, program ini dapat menampilkan seluruh proses yang dijalankan oleh seorang pengguna, beroperasi dalam mode daemon untuk memantau proses secara terus menerus, memblokir proses yang dijalankan oleh pengguna, dan mencatat aktivitas proses dalam file log khusus pengguna. Log tersebut memiliki format [dd:mm:yyyy]-[hh:mm:ss]_pid-process_nama-process_GAGAL/JALAN.

Secara garis besar, command yang diinginkan adalah sebagai berikut :
./admin <user>: Menampilkan semua proses yang dijalankan oleh pengguna yang ditentukan.
./admin -m <user>: Menjalankan program sebagai daemon untuk terus memantau proses user.
./admin -s <user>: Menghentikan mode daemon.
./admin -c <user>: Menggagalkan proses user.
./admin -a <user>: Menghentikan penggagalan proses user.

```bash
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
```
```bash
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
```
Fungsi daemoner di atas ini digunakan mengubah program menjadi proses daemon (proses latar belakang). Didalam fungsi ini juga, disimpan pid dari daemon dalam sebuah pid file agar nantinya bisa dikill melalui command admin -s <user>. Lalu, fungsi checkadmin dibawahnya berfungsi untuk fungsi checkadmin mengubah ps -aux | grep <user> dalam bentuk C menggunakan pipe dan fork untuk menjalankan perintah ps dan grep. Informasi tentang semua proses diambil dan difilter berdasarkan nama pengguna yang ditentukan. Informasi proses yang difilter tersebut akan diperlihatkan dalam terminal. 

```bash
void mulai(char *nama, int status){
char logfile[100];
FILE *file_ptr;
sprintf(logfile, "%s.log", nama);
file_ptr = fopen(logfile, "w");

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
```

Fungsi mulai di atas menggunakan 2 pipe dan 3 fork untuk mengubah ps -eo | tail -n 1 | awk /<user>/'{print "[" $3 "-" $4 "-2024" "]-[" $5 "]-" $1 "-" $7 "_GAGAL/BERHASIL"}' dalam bentuk c. Awalnya, sebuah string logfile dibuat dengan menggunakan fungsi sprintf untuk menambahkan ekstensi ".log" pada parameter nama. File log ini akan digunakan untuk menyimpan hasil dari proses yang dijalankan. Kemudian, file log tersebut dibuka dengan mode "w" menggunakan fungsi fopen. Jika pembukaan file gagal, maka tidak ada proses selanjutnya yang akan dilakukan. Selanjutnya, terdapat sebuah kondisional if yang memeriksa nilai dari parameter status. Jika status sama dengan 0, maka akan dilakukan pencatatan untuk proses yang berhasil. Jika tidak, artinya proses yang dilakukan oleh user telah gagal. 

Terdapat 2 kemungkinan dari kondisi tersebut :
1. Apabila status = 0, maka akan digunakan fungsi fork dan pipe untuk mengisi sebuah array dengan nama normal yang berisi dua file descriptor untuk untuk mengalirkan output dari perintah ps ke input dari perintah awk. Di dalam fork tersebut, digunakan fungsi dup2 untuk mengalirkan output dari command ps di child process menjadi input dari command awk pada parent prosesnya. Di proses parent, file descriptor yang tidak diperlukan ditutup (close). Selanjutnya, input dari pipe akan dialirkan ke dalam file log yang sudah dibuka dengan dup2, kemudian file log tersebut ditutup.
2. Apabila status = 1, maka akan dilakukan beberapa fork.  Pertama, dilakukan fork untuk menjalankan perintah ps dan outputnya akan dialirkan ke sebuah pipe. Selanjutnya, dilakukan fork kedua untuk menjalankan perintah tail yang mengambil satu baris terakhir dari output ps, dan outputnya akan dialirkan ke sebuah pipe yang ketiga, dimana outputnya akan dibentuk menjadi format yang diinginkan. Terakhir, dilakukan fork ketiga untuk menjalankan perintah awk yang akan mengubah format dari input yang diperoleh dari perintah tail, dan menulisnya ke dalam file log. Setelah semua proses child selesai, file descriptor yang tidak diperlukan ditutup untuk masing-masing pipe yang digunakan.

```bash
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
```
Kedua fungsi diatas berguna untuk mengubah file yang akan menjadi penanda apakah suatu proses akan digagalkan atau tidaknya. Untuk melakukan hal tersebut, digunakan fungsi fprint untuk mengubah isi dari file txt yang nanti akan dibuat. Fungsi penggagal disini akan mengubah isi dari file txt menjadi 1, yang mana menandakan bahwa proses yang dilakukan oleh user akan digagalkan. Sebaliknya, fungsi hilanggagal disini akan mengubah isi dari file txt menjadi 0 yang menandakan bahwa penggagalan proses telah di stop

```bash
void pembunuh(){
    FILE *gudang = fopen("daemon.pid", "r");
    pid_t pid;
    fscanf(gudang, "%d", &pid);
    fclose(gudang);
    kill(pid, 9);
}
```
Fungsi pembunuh di atas ini digunakan untuk membunuh proses daemon yang dijalankan sebelumnya. Fungsi ini dilakukan dengan membuka daemon.pid nya dengan fopen. Lalu, kita mengunakan fscanf untuk memasukkan pid daemon tersebut dalam variabel pid. Terakhir, kita menggunakan fungsi kill dari library signal untuk menghentikan proses monitoring user.

```bash
int main(int argc, char *argv[]) {

    char *username = argv[1];
    char *user = argv[2];
    int status;
    
    struct stat buffer;
    if (stat("state.txt", &buffer) == 0) {
        ;
    } else {
        FILE *state_file = fopen("state.txt", "w");
    }

    FILE *state_file = fopen("state.txt", "r");
    fscanf(state_file, "%d", &status);

    if (strcmp(argv[1], "-c") == 0) {
      penggagal("state.txt");
    } 
    else if (strcmp(argv[1], "-m") == 0) {
      daemoner();
      while(1){
        mulai(user, status);
        sleep(5);
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
    return 0;
}
```
Di dalam main, program akan memeriksa keberadaan file "state.txt" terlebih dahulu dengan menggunakan if function. Jika file tersebut sudah ada, status akan dibaca dari file tersebut. Jika tidak, file "state.txt" akan dibuat dengan status kosong. Setelah status dibaca, program akan memeriksa argumen pertama yang diberikan. Jika argumen pertama yang diberikan adalah "-c", maka program akan menjalankan fungsi penggagal dengan menggunakan file "state.txt" sebagai acuan gagal tidaknya suatu proses. Jika argumen adalah "-m", program akan menjalankan fungsi daemoner, yang kemudian akan menjalankan fungsi mulai secara berulang dengan interval waktu tertentu, dan status yang telah dibaca sebelumnya. Jika argumen adalah "-a", program akan menjalankan fungsi hilanggagal dengan menggunakan file "state.txt". Jika argumen adalah "-s", program akan menjalankan fungsi pembunuh. Jika argumen tidak cocok dengan yang disebutkan di atas, program akan memeriksa apakah argumen tersebut adalah nama pengguna dengan menggunakan fungsi checkadmin dan menampilkan seluruh proses yang telah dilakukan user tersebut.

Masalah yang ditemukan :
1. Daemon tidak dapat dijalankan
2. Tidak bisa menunjukkan proses dari user spesifik

Foto daemon tidak berjalan :


Revisi dari asisten penguji :
1. Daemon perlu diperbaiki
2. Fungsi mulai perlu diperbaiki agar daemon bisa berjalan

## Soal no 4
Dikerjakan oleh **Nicholas Emanuel Fade (5027231070)**

pertamanya programnya mengambil input:

```bash
int main(int argc, char *argv[]) {

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
```

kode untuk membuka aplikasi:

```bash
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
```

kode untuk membaca config filenya:

```bash
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
```

- kode untuk menutup aplikasinya belum ada
- kode untuk membuka aplikasi lewat configfile masih belum bisa
- kodenya belum run secara daemon

output command manual:

![image](https://drive.google.com/uc?export=view&id=15LU-k5fE_q4Nit3_9P0HtJ3C5Ri7BN11)
