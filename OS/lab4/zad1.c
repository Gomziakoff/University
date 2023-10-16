#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid1, pid2, pid3, pid4, pid5, pid6, pid7;
    printf("Порождение процесса 1 PID=%d PPID=%d\n", getpid(), getppid());

    // Порождение процесса 2
    if ((pid1 = fork()) < 0) {
        printf("Ошибка при порождении процесса 2!\n");
        exit(1);
    } else if (pid1 == 0) {
        printf("Порождение процесса 2 (от процесса 1) PID=%d PPID=%d\n", getpid(), getppid());
        if ((pid5 = fork()) == 0){
        char *args[] = {"whoami", NULL};
        execvp("whoami", args);
        }
        // Порождение процесса 4
        if ((pid4 = fork()) < 0) {
            printf("Ошибка при порождении процесса 4!\n");
            exit(1);
        } else if (pid4 == 0) {
            printf("Порождение процесса 4 (от процесса 2) PID=%d PPID=%d\n", getpid(), getppid());
            printf("Завершился процесс 4 PID=%d PPID=%d\n", getpid(), getppid());
            exit(0);
        } else {
            sleep(1);
        }

        // Порождение процесса 5
        if ((pid5 = fork()) < 0) {
            printf("Ошибка при порождении процесса 5!\n");
            exit(1);
        } else if (pid5 == 0) {
            printf("Порождение процесса 5 (от процесса 2) PID=%d PPID=%d\n", getpid(), getppid());
            
            // Порождение процесса 6
            if ((pid6 = fork()) < 0) {
                printf("Ошибка при порождении процесса 6!\n");
                exit(1);
            } else if (pid6 == 0) {
                printf("Порождение процесса 6 (от процесса 5) PID=%d PPID=%d\n", getpid(), getppid());

                // Порождение процесса 7 от процесса 6
                if ((pid7 = fork()) < 0) {
                    printf("Ошибка при порождении процесса 7!\n");
                    exit(1);
                } else if (pid7 == 0) {
                    printf("Порождение процесса 7 (от процесса 6) PID=%d PPID=%d\n", getpid(), getppid());
                    printf("Завершился процесс 7 PID=%d PPID=%d\n", getpid(), getppid());
                    exit(0);
                } else {
                    sleep(1);
                }

                printf("Завершился процесс 6 PID=%d PPID=%d\n", getpid(), getppid());
                exit(0);
            } else {
                sleep(1);
            }

            printf("Завершился процесс 5 PID=%d PPID=%d\n", getpid(), getppid());
            exit(0);
        } else {
            sleep(1);
        }

        printf("Завершился процесс 2 PID=%d PPID=%d\n", getpid(), getppid());
        exit(0);
    } else {
        sleep(1);
    }

    // Порождение процесса 3
    if ((pid2 = fork()) < 0) {
        printf("Ошибка при порождении процесса 3!\n");
        exit(1);
    } else if (pid2 == 0) {
        printf("Порождение процесса 3 (от процесса 1) PID=%d PPID=%d\n", getpid(), getppid());

        printf("Завершился процесс 3 PID=%d PPID=%d\n", getpid(), getppid());
        exit(0);
    } else {
        sleep(1);
    }

    return 0;
}

