#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

 

#define MAX_MESSAGE_SIZE 512

 

struct msgbuf {
    long mtype;
    char data[MAX_MESSAGE_SIZE];
};

 

int msqid;
pthread_t send_thread;

 

void* sender_function(void* arg) {
    struct msgbuf message;
    message.mtype = 1; // Sender's message type is 1

 

    while (1) {
        printf("You: ");
        fgets(message.data, MAX_MESSAGE_SIZE, stdin);

 

        // Send the message to the receiver
        if (msgsnd(msqid, &message, sizeof(struct msgbuf), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

 

    return NULL;
}

 

int main() {
    key_t key;
    key = ftok(".", 'a'); // Create a unique key for the message queue

 

    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }

 

    // Create sender thread
    pthread_create(&send_thread, NULL, sender_function, NULL);

 

    struct msgbuf message;
    message.mtype = 2; // Receiver's message type is 2

 

    while (1) {
        // Receive messages from the sender
        if (msgrcv(msqid, &message, sizeof(struct msgbuf), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

 

        printf("Received: %s\n", message.data);
    }

 

    pthread_join(send_thread, NULL);

 

    return 0;
}
