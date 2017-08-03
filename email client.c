#include <stdio.h> 
#include<string.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h>
/* Our server port as in the previous program */ 
#define SERVER_PORT 5432 
void *mailclient();
int sn();
int enter();
int date(int m,int d,int y); 
int len, rc,x,ln,attach; 
int sockfd; 
char send_buf[10000],buffer0[101],buffer[1000],bufferAtt[1000],buffer8[20],buffer9[100000],read_buffer[100000],buffer11[20]; 
char recv_buf[10],s,z,buffer4[50],search_choice,s_user[10],tem[100000]; 
struct sockaddr_in addr;
pthread_t td;
struct
{
  pthread_mutex_t mu;
  pthread_cond_t sig_con;
  int result;
//pthread_mutex_t mu= PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t sig_con= PTHREAD_COND_INITIALIZER;
}s1={PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER,0};
//pthread_cond_t sig_pro= PTHREAD_COND_INITIALIZER;
int result=0;
int choice;

FILE *f,*ftpr;
int i,len_user,h,mm,dd,yy;
char a[10],b[4],c[10],d[10],ch,to[10],from[10],subject[100],message[256],sent,fname[10];
char user[10],pass[10],temp[100],first[10],last[10],name[10],passwd[4],uname[10],dat[100];

int main (int argc, char *argv[]) 
{ 


/* Create an AF_INET stream socket */ 
 sockfd = socket(AF_INET, SOCK_STREAM, 0); 
if(sockfd < 0) 
{ 
printf("Client ERROR!!!"); 
exit(-1); 
} 
else 
printf("Client Socket done.\n"); 
// Initialize the socket address structure 
memset(&addr, 0, sizeof(addr)); 
addr.sin_family = AF_INET; 
addr.sin_addr.s_addr = htonl(INADDR_ANY); 
addr.sin_port = htons(SERVER_PORT); 
// Connect to the server 
rc = connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)); 
if(rc < 0) 
{ 
printf("Client connection ERROR!!!"); 
close(sockfd); 
exit(-1); 
} 
else 
{ 
printf("connect() completed successfully.\n"); 
printf("Connection with %s using port %d established!\n", argv[1], SERVER_PORT); 
} 


printf("****WELCOME!!!!****\n\n A. Signin	B.Create new account	C.QUIT \n");
printf("Enter the choice:");
scanf("%c",&s);



send_buf[0]=s;
send(sockfd,send_buf,strlen(send_buf)+1,0);


if(s=='A')
{
printf("\n Enter Username = ");
scanf("%s",user);
printf("\n Password = ");
scanf("%s",pass);
strcpy(temp,user);
strcat(temp,pass);
strcpy(send_buf,temp);
send(sockfd,send_buf,strlen(send_buf)+1,0);
recv(sockfd,recv_buf,sizeof(recv_buf),0);

if(recv_buf[0]=='y')
{
printf("Successfully authenticated !!!\n");
printf("A.Read	\nB.Compose \nC.QUIT\n");
printf("\n Select an Option :");
scanf("%s",&z);
send_buf[0]=z;
send(sockfd,send_buf,strlen(send_buf)+1,0);

if(z== 'A')
{
    pthread_create(&td, NULL,&mailclient, NULL);
    pthread_mutex_lock(&s1.mu);
    while(s1.result==0)
    {
       pthread_cond_wait(&s1.sig_con,&s1.mu);
    }
    s1.result--;
    printf("\n The contents of the mail are:");
    printf("%s",read_buffer);

printf("\n Do you want Filter or not (if yes press Y else N):");
scanf("%s",&search_choice);
send_buf[0]=search_choice;
send(sockfd,send_buf,strlen(send_buf)+1,0);
if(search_choice=='Y')
{
printf("\n Enter Username to be searched : ");
scanf("%s",s_user);
send(sockfd,s_user,strlen(s_user)+1,0);
recv(sockfd,tem,sizeof(tem),0);
printf("%s",tem);
}


    pthread_mutex_unlock(&s1.mu);
}
else if(z=='B')
{
printf("\n From - ");
scanf("%s",from);
printf("\n To - ");
scanf("%s",to);
enter();
printf("\n Subject - ");
scanf("%s",subject);
printf("\n Enter Message - ");
scanf("%s",message);
printf("\n Want to attach any file?");
printf("\n1. Yes \n2.No");
printf("\n Select An option - ");
scanf("%d",&attach);
if(attach==1)
{
printf("\n Enter File Name =");
scanf("%s",fname);
FILE *att;
att=fopen(fname,"r");
if (!att)
{
exit(0);
}
while (fgets(bufferAtt, sizeof bufferAtt, att))
{
sprintf(bufferAtt,"%s",bufferAtt);
}
strcat(message," Attachment - ");
strcat(message, bufferAtt);
sn();

}
else if(attach==2)
{
sn();
}
}
else
{
printf("You have Quit, Thank you!!\n");
exit(0);

}
}
else
{
printf(" Authentication Failed !!!\n");

}
}




else if(s=='B')
{
printf("\n Enter Username = ");
scanf("%s",name);
printf("\n Enter Password = ");
scanf("%s",passwd);
printf("\n Enter Firstname = ");
scanf("%s",first);
printf("\n Enter Lastname = ");
scanf("%s",last);
strcpy(temp,name);
strcat(temp,passwd);
strcat(temp," ");
strcat(temp,first);
strcat(temp," ");
strcat(temp,last);
strcpy(send_buf,temp);
send(sockfd,send_buf,strlen(send_buf)+1,0);
recv(sockfd,buffer4,sizeof(buffer4),0);
printf("\n%s\n",buffer4);
strcat(name,".txt");
strcpy(send_buf,name);
send(sockfd,send_buf,strlen(send_buf)+1,0);
}


else 
{
 printf("You have quit now, Thank you!!\n");
exit(0);
}

}
int sn()
{
printf("\n Want to Send");
printf("\n s-send \n n-no \n");
scanf("%s",&sent);
if(sent=='s')
{
sprintf(buffer,"From - ");
strcat(buffer,from);
strcpy(send_buf,to);
send(sockfd,send_buf,strlen(send_buf)+1,0);
recv(sockfd,buffer4,sizeof(buffer4),0);
printf("\n%s\n",buffer4);
strcat(buffer," Subject - ");
strcat(buffer,subject);
strcat(buffer," Message - ");
strcat(buffer,message);
strcat(buffer," Date - ");
strcat(buffer,dat);
strcpy(send_buf,buffer);
send(sockfd,send_buf,strlen(send_buf)+1,0);
}
return 0;
}

int enter()
{
printf("\n Enter date -");
scanf("%d/%d/%d",&mm,&dd,&yy);
date(mm,dd,yy);
return 0;
}

int date(int m,int d,int y)
{
if(m>=1 && m<=12)
{
if(d>=1 && d<=31)
{
sprintf(dat,"%d/%d/%d ",m,d,y);
}
else
{
printf("\n Enter valid date");
enter();
}
}
else
{
printf("\n Enter valid month");
enter();
}
return 0;
}

void *mailclient()
{
  pthread_mutex_lock(&s1.mu);
  strcat(user,".txt");
  strcpy(buffer8,user);
  send(sockfd,buffer8,strlen(buffer8)+1,0);
  sleep(1);
  printf("\n-----read------");
  recv(sockfd,buffer9,sizeof(buffer9),0);
  strcpy(read_buffer,buffer9);
  pthread_cond_signal(&s1.sig_con);
  s1.result++;
  pthread_mutex_unlock(&s1.mu);
  

}



  
