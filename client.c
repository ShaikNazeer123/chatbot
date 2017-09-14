#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"netdb.h"
#include"pthread.h"

#define PORT 4480
#define BUF_SIZE 2000
int type,in=0,index1=0,index2=0,flag1=1;


void * receiveMessage(void * socket) {
 int sockfd, ret;
 char buffer[BUF_SIZE];
 char file[BUF_SIZE];
 sockfd = (int) socket;
 memset(buffer, 0, BUF_SIZE);
 for (;;) {
  ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
  if (ret < 0) {
   printf("Error receiving data!\n");
  } else {
   printf("server: ");
int ll=strlen(buffer);
if(!strncmp("finish",buffer,ll-2)){
    FILE *f  = fopen("temp","w");
    fprintf(f,"%s",file);
    fclose(f);
    char command[100];
    sprintf(command, "gedit %s", "temp");
    system(command);
    //printf("%s",file);
}
else{strcat(file,buffer);}
//fputs(buffer, stdout);
   //printf("\n");
  }
 }
}

int main(int argc, char**argv) {
 struct sockaddr_in addr, cl_addr;
 int sockfd, ret;
 char buffer[BUF_SIZE];
 char * serverAddr;
 pthread_t rThread;

 if (argc < 2) {
  printf("usage: client < ip address >\n");
  exit(1);
 }

 serverAddr = argv[1];

 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) {
  printf("Error creating socket!\n");
  exit(1);
 }
 printf("Socket created...\n");

 memset(&addr, 0, sizeof(addr));
 addr.sin_family = AF_INET;
 addr.sin_addr.s_addr = inet_addr(serverAddr);
 addr.sin_port = PORT;

 ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
 if (ret < 0) {
  printf("Error connecting to the server!\n");
  exit(1);
 }
 printf("Connected to the server...\n");

 memset(buffer, 0, BUF_SIZE);
 
 //creating a new thread for receiving messages from the server
 ret = pthread_create(&rThread, NULL, receiveMessage, (void *) sockfd);
 if (ret) {
  printf("ERROR: Return Code from pthread_create() is %d\n", ret);
  exit(1);
 }

 while (flag1==1) { 
  printf("Enter the method u want to implement! \n1.GET\n2.PUT\n3.DELETE\n4.POST\n5.OPTIONS\n6....\n");
  scanf("%d",&type);
  char req[1000];
  if(type==1){
    //get
    printf("enter url : ");
    scanf("%s",buffer);
   
    char host[100]={"o"},url[100]={"p"};
    index1=0;index2=0;
    for(in=0;in<strlen(buffer);in++){
        if(buffer[in]=='/'){
            break;
        }
        host[index1++]=buffer[in];
    }
    in++;
    for(in;in<strlen(buffer);in++){
        url[index2++]=buffer[in];
    }
    index1=0;index2=4;
    strcpy(req,"GET ");
    for(index1=0;index1<strlen(buffer);index1++){
       req[index2++]=buffer[index1];
    }
    index1=0;index2=0;
    printf("Your request structure : \n\n");
    printf("GET Url: %s",url);
    printf(" HTTP/1.1\n");
    printf(" User-Agent: CS\n");
    printf(" Host : %s",host);
    printf("\n Accept-Language:en-us\n Accept-Encoding: gzip\n Connection:Keep alive");
   
   
    memset(buffer, 0, BUF_SIZE);
   
    strcpy(buffer,req);
      //printf("\n\nactual  %s\n\n",req);
      ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));
      if (ret < 0) {
       printf("Error sending data!\n\t-%s", buffer);
      }
    flag1=0;
      printf("If you want to close connection press 0 else 1\n");
    scanf("%d",&flag1);
    if(flag1==0){
        close(sockfd);
            pthread_exit(NULL);   
    }
   


  }
  if(type==2){
    //put
    char host[1000],url[1000],pat[1000];
    printf("enter the path of the file : ");
    scanf("%s",pat);
    printf("Enter in what name the file to be saved : ");
    scanf("%s",url);
    printf("enter the host name : ");
    scanf("%s",host);
    strcpy(req,"PUT ");
    printf("Your request structure : \n\n");
    printf("PUT Url: %s",url);
    printf(" HTTP/1.1\n");
    printf(" User-Agent: CS\n");
    printf(" Host : %s",host);
    printf("\n Accept-Language:en-us\n Accept-Encoding: gzip\n Connection:Keep alive\n-----------------\n");
    FILE *ptr;strcat(host,url);index2=4;index1=0;
        if((ptr = fopen(pat,"r"))==NULL){
        printf("\nfile not found");
    }
    else{
            for(index1=0;index1<strlen(host);index1++){
               req[index2++]=host[index1];
        }
        printf("req %s \n",req);
    sendto(sockfd, req, sizeof(req), 0, (struct sockaddr *) &addr, sizeof(addr));
     while (!feof(ptr)) { //sends the file
                fread(&buffer, sizeof(buffer), 1, ptr);
                sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr, sizeof(addr));
        printf("%s",buffer);
                memset(&buffer, 0, sizeof(buffer));
            }
       
            strcpy(buffer,"finish");
        sendto(sockfd, host, sizeof(host), 0, (struct sockaddr *) &addr, sizeof(addr));
    } 

   strcpy(req,"");
   
  }
  if(type==3){
    //delete
  }
  if(type==4){
    //post
  }
  if(type==5){
    //options
  }
  if(type==6){
    //...
  }
 
 }

 close(sockfd);
 pthread_exit(NULL);

 return 0;
}


