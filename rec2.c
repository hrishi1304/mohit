#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<mqueue.h>
#define MAX_MESSAGE_SIZE 512
struct msgbuf{

        long mtype;

        char data[MAX_MESSAGE_SIZE];

};

int main(){

        mqd_t server_queue,client_queue;

        struct msgbuf message,msg1;

        struct mq_attr attr;

        attr.mq_flags=0;

        attr.mq_maxmsg =10;

        attr.mq_msgsize =sizeof(struct msgbuf);

        attr.mq_curmsgs =0;

 

        char queue_name[20];

        sprintf(queue_name,"/client_queue_%d",1);

        client_queue = mq_open(queue_name,O_CREAT|O_RDONLY,0644,&attr);

        if(client_queue ==(mqd_t)-1){

                perror("mq_open(client)");

                exit(1);

        }

 

        server_queue=mq_open("/server_queue",O_CREAT | O_RDONLY, 0644, &attr);

      if(server_queue==(mqd_t)-1){

                perror("mq_open(server)");

                mq_close(client_queue);

                mq_unlink(queue_name);

                exit(1);

        }

        strcpy(message.data,queue_name);

        message.mtype =1;

if(mq_send(server_queue,(const char *)&message,sizeof(struct msgbuf),0)== -1){

        perror("mq_send(client)");

        mq_close(client_queue);

        mq_unlink(queue_name);

        mq_close(server_queue);

        exit(1);

        }

        printf("connected to server \n");

        while(1)

        {

                fgets(message.data,MAX_MESSAGE_SIZE,stdin);

        //      message.mtype=getpid();

                if(mq_send(server_queue,(const char *)&message,sizeof(struct msgbuf),0)==-1){

                perror("mq_send(client)");

 

                }

                if(mq_receive(client_queue,(char*)&msg1,sizeof(struct msgbuf),NULL)==-1)

                {

                        perror("mq_receive(client)");

                }

                else{

                        printf("received data from server %s",msg1.data);

                }

        }

        mq_close(client_queue);

        mq_unlink(queue_name);

        return 0;

}


