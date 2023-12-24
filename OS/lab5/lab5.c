#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
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

}
