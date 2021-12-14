#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include<fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    int shmd,fd[2];
    pid_t id;
    unsigned char buff[16];

    shmd = shm_open("nameshm",O_RDWR | O_CREAT,S_IRUSR|S_IWUSR);
    ftruncate (shmd ,sizeof(int));
    pipe(fd);
    id = fork ();
    if (id == 0){
        close (fd[1]);
        printf("child reading data \n");
        read (fd[0],buff,16);
        printf ("child read : %s \n",buff);


    }
    else
    {
        close (fd[0]);
        printf ("parent writing data \n");
        write (fd[1],"malpoise\n",sizeof("malpoise\n"));
    }

    return 0;
}
