#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
FILE *file = fopen("/etc/passwd","r");

if (file == NULL)
{
perror("Error file doesnt exist");
return 1;
}

char tmp[256];
int line_number = 1;

while (fgets(tmp, sizeof(tmp),file) != NULL){
if (line_number % 2 == 0){
printf("%s", tmp);
} else {
fprintf(stderr,"%s", tmp);
}
line_number++;
}

fclose(file);

 uid_t uid = getuid(); // Получаем UID пользователя
 gid_t gid = getgid(); // Получаем GID пользователя
char *username = getenv("USER");
    if (username == NULL) {
        perror("Ошибка при получении имени пользователя");
        return 1;
    }
    FILE *uid_username_file = fopen("uid_username.txt", "w");
    if (uid_username_file == NULL) {
        perror("Ошибка при создании файла uid_username.txt");
     return 1;
    }
    fprintf(uid_username_file, "UID: %d\nUsername: %s\n", uid, username);
    fclose(uid_username_file);
    FILE *gid_username_file = fopen("gid_username.txt", "w");
    if (gid_username_file == NULL) {
        perror("Ошибка при создании файла gid_username.txt");
        return 1;
    }
    fprintf(gid_username_file, "GID: %d\nUsername: %s\n", gid, username);
    fclose(gid_username_file);
    printf("Файлы uid_username.txt и gid_username.txt успешно созданы.\n");
}
