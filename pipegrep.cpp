#include <iostream>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <thread>
using namespace std;

char* bufferOne[100];
char* bufferTwo[100];
char bufferThree[100];
char bufferFour[100];
sem_t mtex;
sem_t vacant;
sem_t filled;




void fileNameAcquisition();
void fileFilter(int filesize, int uid, int gid);


void errormsg(char* msg){
    perror(msg);
    exit(1);
}

int main() {
    /*
    int argc, char **argv
    int buffsize = atoi(argv[1]);
    int filesize= atoi(argv[2]);
    int uid = atoi(argv[3]);
    int gid = atoi(argv[4]);
    string string = argv[5];
*/


    int buffsize;
    int filesize;
    int uid;
    int gid;
    string string;
    sem_init(&mtex, 0,1);
    sem_init(&vacant, 0, 3);
    sem_init(&filled,0, 0);

    thread threadOne(&fileNameAcquisition);
    thread threadTwo(&fileFilter,filesize,uid,gid);
    threadOne.join();
    threadTwo.join();

   for(int i = 0; i< 3; i++){
       printf("%s\n", bufferTwo[i]);
   }
    sem_destroy(&vacant);
    sem_destroy(&filled);
    sem_destroy(&mtex);

    return 0;
}

void fileNameAcquisition(){
     DIR *directory;
     dirent *entry;

     if( (directory = opendir("/Users/martinarriaga/Desktop/TestDirectory")) != NULL){
         int x =0;
         while( (entry= readdir(directory)) != NULL){
             if( !(strcmp(entry->d_name, ".")) || !(strcmp(entry->d_name,"..")) ){

             }else {
                 sem_wait(&vacant);
                 sem_wait(&mtex);
                 bufferOne[x] = entry->d_name;
                 x++;
                 sem_post(&mtex);
                 sem_post(&filled);
             }
         }
         closedir(directory);

     }else{
         errormsg("Could not access Directory");
     }

}
void fileFilter(int filesize, int uid, int gid){
    int x = 0;
    while(true){

        sem_wait(&filled);
        sem_wait(&mtex);
        char x = *bufferOne[x];
        char y = *bufferTwo[x];
        strcpy(&y,&x);
        sem_post(&mtex);
        sem_post(&vacant);

        x++;

    }

}
