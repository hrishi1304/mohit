#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#define MAX_MESSAGE_SIZE 512
struct msgbuf {
                long mtype;
                char data[MAX_MESSAGE_SIZE];
};
int main() {
                mqd_t server_queue, client_queue;
                struct msgbuf message, msg1;
                struct mq_attr attr;
                attr.mq_flags = 0;
                attr.mq_maxmsg = 10;
                attr.mq_msgsize = sizeof(struct msgbuf);
                attr.mq_curmsgs = 0;

 

                server_queue = mq_open("/server_queue", O_CREAT | O_RDONLY, 0644, &attr);
             if (server_queue == (mqd_t)-1) {
                perror("mq_open (server)");
                exit(1);
                }
                printf("Server started, waiting for clients...\n");
                while (1) {

 

               if (mq_receive(server_queue, (char *)&message, sizeof(struct msgbuf), NULL) == -1) {

 

                perror("mq_receive (server)");

 

 

                  continue;

 

                 }
  if (message.mtype==1)
                                {

 

                 client_queue = mq_open(message.data, O_WRONLY);

 

              if (client_queue == (mqd_t)-1)

 

                  {

 

                perror("mq_open (client)");

 

                continue;
                  }
        printf("Client connected: %s\n", message.data);

 

                      }

 

                else                                                                                                                                                                    {                                                                                       scanf("%s",msg1.data);
                  if (mq_send(client_queue, (const char *)&msg1, sizeof(struct msgbuf), 0) ==-1);{                                                                                                                                                                                                                                                              perror("mq_send (server)");
                      }

 

 

                   printf("%s\n",message.data);

 

                }

 

            }

 

                 return 0;
}
