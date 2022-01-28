#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#define BUF_SIZE 4096

char rand_char(int no){
    return 'A' + no;
}

int write_page(const char * fname, int blkcount, float ratio){
    int i, j;
    if(fname == NULL){
        printf("arg error\n");
        return -1;
    }
    srand((unsigned int)time(0));
    int fd = open(fname, O_RDWR | O_CREAT, 0666);
    for(i = 0; i < blkcount; i++){

        char cbuf[BUF_SIZE];

        if(ratio > 0) {
            char lastch = 'a';
            for (j = 0; j < BUF_SIZE; j++) {
                float ra = (float) rand() / RAND_MAX;
                if (ra > ratio) {
                    int rdd = rand() % 60;
                    cbuf[j] = rand_char(rdd);
                    lastch = cbuf[j];
                } else {
                    cbuf[j] = lastch;
                }
            }
        }else{
            memset(cbuf, 'a', BUF_SIZE);
        }

        if(write(fd, cbuf, BUF_SIZE) == -1){
            printf("write error\n");
            return -1;
        }
    }
	fsync(fd);
    close(fd);
}

int read_page(const char * fname, int blkcount){
    char buffer[BUF_SIZE];
    int i;
    if(fname == NULL){
        printf("arg error\n");
        return -1;
    }
    int fd = open(fname, O_RDWR);
    for(i = 0; i < blkcount; i++){
        if(read(fd, buffer, BUF_SIZE) == -1){
            printf("error error\n");
            return -1;
        }
    }
    close(fd);
}
int main(int argc, char* argv[]){ 
    int nfile = 1000; // nfile = 1000
    int blkcount = 4; // size = 4*4096
    int max_files = 1024*1024; // 本次测试产生的最大的文件数量，大于这个数量则停止测试。
    float ratio = -1;
    long long delta_time;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for(int i = 0; i < max_files; i++){
        char fname[20] = {'\0'};
	    sprintf(fname,"%d",i);
        if(write_page(fname,blkcount,ratio) == -1)
            return 0;
	    remove(fname);
    }
    gettimeofday(&end, NULL);
    delta_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("[benchmark] write cost = %lld\n", delta_time);

    return 0;
}

