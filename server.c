#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"pthread.h"
#include"ctype.h"

#define PORT 4480
#define BUF_SIZE 2000
#define CLADDR_LEN 100
int sockfd, len, ret, newsockfd;
struct sockaddr_in addr, cl_addr;


void * receiveMessage(void * socket) {
 int sockfd, ret;
 char buffer[BUF_SIZE];
 sockfd = (int) socket;
 memset(buffer, 0, BUF_SIZE);
 for (;;) {
    ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
    if (ret < 0) {
        printf("Error receiving data!\n");
        }
    else{
    //printf("BUffer rec %s\n\n\n",buffer);
        char x[500],method[10];int u=0,i;
        for(i=0;i<strlen(buffer);i++){
            if(buffer[i]==' '){
                break;
            }
            if(isalnum(buffer[i])){
                method[u++]=buffer[i];
                }
        }

        printf("Method to implement %s\n",method);
        if(!strcmp(method,"GET")){

        u=0;i++;
        //printf("\n");
        for(i;i<strlen(buffer);i++){
        if(buffer[i]==' '){break;}

                x[u++]=buffer[i];


            }
    //printf("\ni%d\n\n",i);
        printf("client requested for: \n");
        printf("%s",x);
        FILE *ptr;
        if((ptr = fopen(x,"r"))==NULL){
            ret = sendto(newsockfd, "error 404", sizeof("error 404"), 0, (struct sockaddr *) &cl_addr, len);
        printf("Response:\n\nHTTP/1.1 404\n");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year+1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

            if (ret < 0) {
                printf("Error sending data! er404\n");
                exit(1);
            }
        }
        else{
            memset(&buffer, 0, sizeof(buffer));
        printf("Response:\n\nHTTP/1.1 200 OK\n");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year+1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        int s=0;
        fseek(ptr, 0L, SEEK_END);
            s = ftell(ptr);
        rewind(ptr);
        printf("content-Length: %d\n",s);
        printf("Content-Type: text\n---------------------\n");

            while (!feof(ptr)) { //sends the file
                fread(&buffer, sizeof(buffer), 1, ptr);
                sendto(newsockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cl_addr, len);
        s=s+strlen(buffer);
        printf("%s",buffer);
                memset(&buffer, 0, sizeof(buffer));
            }

            strcpy(buffer,"finish");
            sendto(newsockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cl_addr, len);

            printf("\n\nFile sent...\n");
            }
        }   //get ends

        if(!strcmp(method,"PUT")){
            char nfile[100];u=0;
        for(i;i<strlen(buffer);i++){
        if(buffer[i]==' '){break;}
            nfile[u++]=buffer[i];
            } strcpy(nfile,"google/lmn");
        printf("file name : %s\n",nfile);
        for (;;) {
      ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
      if (ret < 0) {
       printf("Error receiving data!\n");
      } else {
    int ll=strlen(buffer);char file[BUF_SIZE];
    if(!strncmp("finish",buffer,ll-2)){
    FILE *f  = fopen(nfile,"w");
    fprintf(f,"%s",file);
    fclose(f);
    char command[100];
    sprintf(command, "gedit %s", "temp");
    system(command);
    //printf("%s",file);
        }
        else{strcat(file,buffer);}
      }
      }
        }
        if(!strcmp(method,"DELETE")){
            printf("enter delete code here\n");

        }
        if(!strcmp(method,"POST")){

            printf("enter post code here\n");
        }
        if(!strcmp(method,"OPTIONS")){
            printf("enter options code here\n");

        }
        if(!strcmp(method,"...")){

            printf("enter ... code here\n");
        }


        }
    }
}




void main() {


 char buffer[BUF_SIZE];
 pid_t childpid;
 char clientAddr[CLADDR_LEN];
 pthread_t rThread;

 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) {
  printf("Error creating socket!\n");
  exit(1);
 }
 printf("Socket created...\n");

 memset(&addr, 0, sizeof(addr));
 addr.sin_family = AF_INET;
 addr.sin_addr.s_addr = INADDR_ANY;
 addr.sin_port = PORT;

 ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
 if (ret < 0) {
  printf("Error binding!\n");
  exit(1);
 }
 printf("Binding done...\n");

 printf("Waiting for a connection...\n");
 listen(sockfd, 5);


 len = sizeof(cl_addr);
 newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
 if (newsockfd < 0) {
  printf("Error accepting connection!\n");
  exit(1);
 }

 inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
 printf("Connection accepted from %s...\n", clientAddr);

 memset(buffer, 0, BUF_SIZE);
 printf("Enter your messages one by one and press return key!\n");

 //creating a new thread for receiving messages from the client
 ret = pthread_create(&rThread, NULL, receiveMessage, (void *) newsockfd);
 if (ret) {
  printf("ERROR: Return Code from pthread_create() is %d\n", ret);
  exit(1);
 }

 while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
  ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);
  if (ret < 0) {
   printf("Error sending data!\n");
   exit(1);
  }
 }

 close(newsockfd);
 close(sockfd);

 pthread_exit(NULL);
 return;
}
