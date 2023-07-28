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
pthread_t send_thread, receive_thread;

 

void* sender_function(void* arg) {
    struct msgbuf message;
    message.mtype = 2; // Receiver's message type is 2

 

    while (1) {
        printf("Type message: ");
        fgets(message.data, MAX_MESSAGE_SIZE, stdin);

 

        // Send the message to the sender
        if (msgsnd(msqid, &message, sizeof(struct msgbuf), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

 

    return NULL;
}

 

void* receiver_function(void* arg) {
    struct msgbuf message;

 

    while (1) {
        // Receive messages from the sender
        if (msgrcv(msqid, &message, sizeof(struct msgbuf), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

 

        printf("Received: %s\n", message.data);
    }

 

    return NULL;
}

 

int main() {
    key_t key;
    key = ftok(".", 'a'); // Create the same key as used in the sender

 

    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }

 

    // Create sender and receiver threads
    pthread_create(&send_thread, NULL, sender_function, NULL);
    pthread_create(&receive_thread, NULL, receiver_function, NULL);

 

    // Wait for both threads to finish (which will be never)
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

 

    return 0;
}
