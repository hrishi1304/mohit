#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

 

#define MAX_MESSAGE_SIZE 100

 

// Structure for the message
struct message {
    long mtype; // Message type (must be greater than 0)
    char mtext[MAX_MESSAGE_SIZE]; // Message data
};

 

int main() {
    key_t key_send, key_receive;
    int msgid_send, msgid_receive;
    struct message msg;

 

    // Generate a unique key for the sender's message queue
    if ((key_send = ftok(".", 's')) == -1) {
        perror("ftok sender");
        exit(1);
    }

 

    // Create the sender's message queue (0666 permission allows read/write for all)
    if ((msgid_send = msgget(key_send, 0666 | IPC_CREAT)) == -1) {
        perror("msgget sender");
        exit(1);
    }

 

    printf("Sender: Sender's message queue created with key %d\n", key_send);

 

    // Generate a unique key for the receiver's message queue
    if ((key_receive = ftok(".", 'r')) == -1) {
        perror("ftok receiver");
        exit(1);
    }

 

    // Create the receiver's message queue (0666 permission allows read/write for all)
    if ((msgid_receive = msgget(key_receive, 0666 | IPC_CREAT)) == -1) {
        perror("msgget receiver");
        exit(1);
    }

 

    printf("Sender: Receiver's message queue created with key %d\n", key_receive);

 

    // Sender loop
    while (1) {
        // Get input from the user
        printf("Enter a message to send (type 'exit' to quit): ");
        fgets(msg.mtext, MAX_MESSAGE_SIZE, stdin);

 

        // Check if the user wants to exit
        if (strcmp(msg.mtext, "exit\n") == 0)
            break;

 

        // Set the message type to 1 (you can use any value > 0)
        msg.mtype = 1;

 

        // Send the message to the receiver's message queue
        if (msgsnd(msgid_receive, &msg, strlen(msg.mtext) + 1, 0) == -1) {
            perror("msgsnd receiver");
            exit(1);
        }

 

        // Receive the response from the receiver
        if (msgrcv(msgid_send, &msg, MAX_MESSAGE_SIZE, 2, 0) == -1) {
            perror("msgrcv sender");
            exit(1);
        }

 

        // Print the received response
        printf("Received response from receiver: %s", msg.mtext);
    }

 

    // Remove the message queues
    if (msgctl(msgid_send, IPC_RMID, NULL) == -1) {
        perror("msgctl sender");
        exit(1);
    }

 

    if (msgctl(msgid_receive, IPC_RMID, NULL) == -1) {
        perror("msgctl receiver");
        exit(1);
    }

 

    printf("Sender: Message queues removed\n");

 

    return 0;
}
