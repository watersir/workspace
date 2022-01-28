#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

/* for min heap*/
/* Sudhanshu Patel sudhanshuptl13@gmail.com */
/*
Min Heap implementation in c
*/
#include<stdio.h>
#include<stdlib.h>
/*
 Array Implementation of MinHeap data Structure
*/

HEAP_SIZE = 20;

struct Heap{
    unsigned int *arr;
    int count;
    int capacity;
    int heap_type; // for min heap , 1 for max heap
};
typedef struct Heap Heap;

Heap *CreateHeap(int capacity,int heap_type);
void insert(Heap *h, unsigned int key);
void print(Heap *h);
void heapify_bottom_top(Heap *h,int index);
void heapify_top_bottom(Heap *h, int parent_node);
int PopMin(Heap *h);

Heap *CreateHeap(int capacity,int heap_type){
    Heap *h = (Heap * ) malloc(sizeof(Heap)); //one is number of heap

    //check if memory allocation is fails
    if(h == NULL){
        printf("Memory Error!");
        return;
    }
    h->heap_type = heap_type;
    h->count=0;
    h->capacity = capacity;
    h->arr = (unsigned int *) malloc(capacity*sizeof(unsigned int)); //size in bytes

    //check if allocation succeed
    if ( h->arr == NULL){
        printf("Memory Error!");
        return;
    }
    return h;
}

void insert(Heap *h, unsigned int key){
    if( h->count < h->capacity){
        h->arr[h->count] = key;
        heapify_bottom_top(h, h->count);
        h->count++;
    }
}

void heapify_bottom_top(Heap *h,int index){
    int temp;
    int parent_node = (index-1)/2;

    if(h->arr[parent_node] > h->arr[index]){
        //swap and recursive call
        temp = h->arr[parent_node];
        h->arr[parent_node] = h->arr[index];
        h->arr[index] = temp;
        heapify_bottom_top(h,parent_node);
    }
}

void heapify_top_bottom(Heap *h, int parent_node){
    int left = parent_node*2+1;
    int right = parent_node*2+2;
    int min;
    int temp;

    if(left >= h->count || left <0)
        left = -1;
    if(right >= h->count || right <0)
        right = -1;

    if(left != -1 && h->arr[left] < h->arr[parent_node])
        min=left;
    else
        min =parent_node;
    if(right != -1 && h->arr[right] < h->arr[min])
        min = right;

    if(min != parent_node){
        temp = h->arr[min];
        h->arr[min] = h->arr[parent_node];
        h->arr[parent_node] = temp;

        // recursive  call
        heapify_top_bottom(h, min);
    }
}

int PopMin(Heap *h){
    int pop;
    if(h->count==0){
        printf("\n__Heap is Empty__\n");
        return -1;
    }
    // replace first node by last and delete last
    pop = h->arr[0];
    h->arr[0] = h->arr[h->count-1];
    h->count--;
    heapify_top_bottom(h, 0);
    return pop;
}
void print(Heap *h){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0;i< h->count;i++){
        printf("-> %d ",h->arr[i]);
    }
    printf("->__/\\__\n");
}

/**
 * @brief for file read and write
 * 
 */
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
int initial_files(int nfile, int blkcount, Heap *h){

    unsigned int file_id;
    for(int i = 0; i < nfile; i++){ 
        file_id = random();
        char name[30] = {'\0'}; 
        sprintf(name, "%d", file_id);
        if(write_page(name, blkcount, -1)==-1)
            return -1;
        insert(h, file_id);
    }
    return 0;
}
int write_append(const char * fname, int blkcount, float ratio){
    int i, j;
    if(fname == NULL){
        printf("arg error\n");
        return -1;
    }

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
        lseek(fd,-1,SEEK_END);
        if(write(fd, cbuf, BUF_SIZE) == -1){
            printf("write error\n");
            return -1;
        }
    }
	fsync(fd);
    close(fd);
}

int execute(int blkcount, int max_files, Heap *h) {

    float ratio = -1;
    unsigned int file_id;
    long long delta_time;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for(int i = 0; i < max_files; i++){

        // write a file.
        file_id = random();
        char fname[30] = {'\0'};
        sprintf(fname,"%d",file_id);
        if(write_page(fname,blkcount,ratio) == -1)
            return 0;
        insert(h, file_id);

        // rewrite a file.
        file_id = random() % 100;
        char fname_append[30] = {'\0'};
        sprintf(fname_append,"%d.append",file_id);
        if(write_append(fname_append,1,ratio) == -1)
            return 0;

        // delete a file.
        file_id = PopMin(h);
        char fname_read[30] = {'\0'};
        sprintf(fname_read,"%d",file_id);
        if(remove(fname_read)==-1)
            printf("remove erro.\n");

    }
    gettimeofday(&end, NULL);
    delta_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("[benchmark] write cost = %lld\n", delta_time);

    return 0;
}

int main(){

    /* create a heap */
    int heap_size = 11000;
    Heap *heap = CreateHeap(heap_size, 0); //Min Heap
    if( heap == NULL ){
        printf("__Memory Issue____\n");
        return -1;
    }

    srand ( time(NULL) );

    /* create fileset */
    int nfile = 10000;
    int blkcount = 4;
    initial_files(nfile, blkcount, heap);
    
    /* create appendwrite file */
    printf("-- create append write file. --\n");
    for(int i = 0; i < 100; i++){ 
        char name[30] = {'\0'}; 
        sprintf(name, "%d.append", i);
        if(write_page(name, 1, -1)==-1)
            return -1;
    }
    printf("-- create append write file down. --\n");

    int testblkcount = 4; // size = 4*4096
    int max_files = 1024*256;
    execute(testblkcount,max_files,heap);

    return 0;
}
