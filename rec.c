#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

 

    // Generate the same key as the sender for the receiver's message queue
    if ((key_receive = ftok(".", 'r')) == -1) {
        perror("ftok receiver");
        exit(1);
    }

 

    // Connect to the existing receiver's message queue
    if ((msgid_receive = msgget(key_receive, 0666)) == -1) {
        perror("msgget receiver");
        exit(1);
    }

 

    printf("Receiver: Connected to the receiver's message queue with key %d\n", key_receive);

 

    // Generate the same key as the sender for the sender's message queue
    if ((key_send = ftok(".", 's')) == -1) {
        perror("ftok sender");
        exit(1);
    }

 

    // Connect to the existing sender's message queue
    if ((msgid_send = msgget(key_send, 0666)) == -1) {
        perror("msgget sender");
        exit(1);
    }

 

    printf("Receiver: Connected to the sender's message queue with key %d\n", key_send);

 

    // Receiver loop
    while (1) {
        // Receive the message from the sender's message queue
        if (msgrcv(msgid_receive, &msg, MAX_MESSAGE_SIZE, 1, 0) == -1) {
            perror("msgrcv receiver");
            exit(1);
        }

 

        // Print the received message
        printf("Received message from sender: %s", msg.mtext);

 

        // Check if the user wants to exit
        if (strcmp(msg.mtext, "exit\n") == 0)
            break;

 

        // Get input from the user
        printf("Enter a response to send back to the sender: ");
        fgets(msg.mtext, MAX_MESSAGE_SIZE, stdin);

 

        // Set the message type to 2 (you can use any value > 0)
        msg.mtype = 2;

 

        // Send the response to the sender's message queue
        if (msgsnd(msgid_send, &msg, strlen(msg.mtext) + 1, 0) == -1) {
            perror("msgsnd receiver");
            exit(1);
        }
    }

 

    return 0;
}
