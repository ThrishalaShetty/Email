#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<features.h>
#define KEY ((key_t)45678L)
union semun{
int val;
struct semid_ds *buff;
ushort *array;
}arg;
int semid,oflag,semaphoreKey;
//key_t key = 1234;
struct sembuf setlock,rellock;
#define SERVER_PORT 5432 
time_t tloc; 

struct list {
    char *string;
    struct list *next;
};

typedef struct list LIST;


char line[128];
LIST *current, *head;


/* Run with a number of incoming connection as argument */ 
int main(int argc, char *argv[]) 
{
head = current = NULL; 
int  len,num, rc,m,n,x,k,ec; 
int listen_sd, accept_sd,newsockfd; 
/* Buffer for data */ 
char buffer1[100], buffer[10],buffer2[100],buffer0[100]="No text in file ",buffer4[100],filename[100],buffer3[100],buffer5[10000],buffer99[10],buffer9[100000],buffer8[20],end[100],buff[10],teme[100000]; 
struct sockaddr_in addr; 
int pid;


FILE *f,*ftpr,*re,*se;
int i,len_user,h,s,z;
char a[10],b[4],c[10],d[10],ch[20],buffer11[20];
char user[10],pass[10],temp[100000],first[10],last[10],name[10],passwd[4],s_user[10];
char from[10],ru[10],sub[10],sb[50],msg[10],in[100],da[10],tem[100000];
int mo,day,ye;

// Create an AF_INET stream socket to receive 
// incoming connections on  
listen_sd = socket(AF_INET, SOCK_STREAM, 0); 
if(listen_sd < 0) 
{ 
printf("ERROR!!!!\n"); 
exit(-1); 
} 
else 
printf("Socket is created \n"); 
  
printf(" Now Binding the socket...\n"); 
// Bind the socket 
memset(&addr, 0, sizeof(addr)); 
addr.sin_family = AF_INET; 
addr.sin_addr.s_addr =htonl(INADDR_ANY); 
addr.sin_port = htons(SERVER_PORT); 
rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr)); 
if(rc < 0) 
{ 
printf("ERROR!!!"); 
close(listen_sd); 
exit(-1); 
} 
else 
printf("Bind was successful \n"); 
  
// Set the listen backlog  
rc = listen(listen_sd, 5); 
if(rc < 0) 
{ 
printf("ERROR!!!!"); 
close(listen_sd); 
exit(-1); 
} 
else 
printf("Listen was successful\n"); 

  
// Inform the user that the server is ready 
printf("The Server is ready to serve!!\n"); 
setlock.sem_num=0;
setlock.sem_op=-1;
setlock.sem_flg=SEM_UNDO;
rellock.sem_num=0;
rellock.sem_op=1;
rellock.sem_flg=SEM_UNDO;
//semid=semget(key,nsems,IPC_CREAT|0666);
semid=semget(KEY,1,0666);
arg.val=1;
semctl(semid,1,SETVAL,arg);
/*semaphoreKey = ftok(".",4);
oflag=IPC_CREAT|0666;
if ((semid = semget(semaphoreKey, 1, oflag)) == -1) {
	perror("semget: semget failed");
	exit(1);
      }
 else 
	
 (void) fprintf(stderr, "semget: semget succeeded: semid =\%d\n", semid);

arg.val=1;*/
semctl(semid,1,SETVAL,arg);
while(1)

 {

ftpr=fopen("user.config","a");
if(ftpr==NULL)
{
printf("\n File doesnot exist");
}
if((f=fopen("user.config","r"))== NULL)
{
send(accept_sd,buffer0,sizeof(buffer0)+1,0);
exit(1);
}

 newsockfd = accept(listen_sd, NULL, NULL); 

  //child process is created for serving each new clients

  pid=fork();

  printf("Child process created!!");

  if(pid==0)//child process rec and send

  {
recv(newsockfd, buffer1, sizeof(buffer1), 0);
recv(newsockfd, buffer2, sizeof(buffer2), 0);

if(buffer1[0]=='A')
{
while(!feof(f))
{
fscanf(f,"%s %s %s",a,c,d);
if(strcmp(buffer2,a)==0)
{
ch[0]='y';
strcpy(buffer,ch);
break;
}
else
{
ch[0]='n';
strcpy(buffer,ch);
}
}
send(newsockfd,buffer,sizeof(buffer),0);
recv(newsockfd, buffer99, sizeof(buffer99), 0);
if(ch[0]=='y')
{

if(buffer99[0]=='A')
{

recv(newsockfd, buffer8, sizeof(buffer8), 0);
semop(semid,&setlock,1);
re=fopen(buffer8,"rb");

int x=1;
while(fgets(line, sizeof(line), re))
{
LIST *node = malloc(sizeof(LIST));
node->string = strdup(line);
sprintf(end,"\n%d",x);
strcat(buffer9,end);
strcat(buffer9,".");
strcat(buffer9,node->string);
x++;
node->next =NULL;
if(head == NULL)
{
current = head = node;
} 
else
{
current = current->next = node;
}
}
send(newsockfd,buffer9,sizeof(buffer9),0);
fclose(re);

semop(semid,&rellock,1);

recv(newsockfd, buff, sizeof(buff), 0);
if(buff[0]=='Y')
{
recv(newsockfd, s_user, sizeof(s_user), 0);
printf("%s",s_user);
se=fopen(buffer8,"rb");
while(!feof(se))
{
fscanf(se,"%s - %s %s -%s %s - %s %s - %d/%d/%d",from,ru,sub,sb,msg,in,da,&mo,&day,&ye);
temp[0]='\0';

if(strcmp(s_user,ru)==0)
{
sprintf(temp,"\n%s - %s %s - %s %s - %s %s - %d/%d/%d\n",from,ru,sub,sb,msg,in,da,mo,day,ye);
strcpy(teme,temp);
strcat(tem,temp);
temp[0]='\0';
//strcpy(temp,"");
//strcat(tem,teme);
//strcpy(teme,temp);
//strcat(tem,teme);
//printf("%s",tem);
}
else
{
temp[0]='\0';
printf("%s ----",temp);
}
}
send(newsockfd,tem,sizeof(tem)+1,0);
}
fclose(se);


semop(semid,&rellock,1);
}
else if(buffer99[0]=='B')
{
recv(newsockfd, buffer3, sizeof(buffer3), 0);
printf("\n %s",buffer3);
sprintf(buffer4,"Account Created");
send(newsockfd,buffer4,sizeof(buffer4)+1,0);
FILE *q;
strcat(buffer3,".txt");
printf("\n%s",buffer3);
q=fopen(buffer3,"a+");
recv(newsockfd, buffer5, sizeof(buffer5),0);
printf("\n%s",buffer5);
fprintf(q,"%s\n",buffer5);
fclose(q);
}
}

}


else if(buffer1[0]=='B')
{
fprintf(ftpr,"%s\n",buffer2);
sprintf(buffer4,"Account Created");
send(newsockfd,buffer4,sizeof(buffer4)+1,0);
recv(newsockfd, buffer3, sizeof(buffer3), 0);
FILE *t;
t=fopen(buffer3,"a+");
fclose(t);
}
fclose(f);
fclose(ftpr);
return 0;

}
}
}
   











