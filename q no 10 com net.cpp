#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define WINDOW_SIZE 4
#define TIMEOUT 2

typedef struct {
    int sequenceNumber;
    int ack;
    char data[20];
} Packet;

void simulateTransmission(Packet packets[], int numPackets) {
    srand(time(0));

    int currentIndex = 0;
    int nextSequenceNumber = 0;

    while (currentIndex < numPackets) {
        if (nextSequenceNumber < currentIndex + WINDOW_SIZE) {
            printf("Sending packet with sequence number %d\n", nextSequenceNumber);
            packets[nextSequenceNumber].sequenceNumber = nextSequenceNumber;

            if (nextSequenceNumber == numPackets - 1) {
                packets[nextSequenceNumber].ack = 1; // Set ack flag for last packet
            } else {
                packets[nextSequenceNumber].ack = 0;
            }

            // Simulating packet loss
            if (rand() % 4 != 0) {
                // Simulating packet corruption
                if (rand() % 3 != 0) {
                    strcpy(packets[nextSequenceNumber].data, "DATA");
                } else {
                    strcpy(packets[nextSequenceNumber].data, "CORRUPT");
                }
            } else {
                strcpy(packets[nextSequenceNumber].data, "LOST");
            }

            nextSequenceNumber++;
        }

        int ackPacketNumber = -1;
        printf("Acknowledgment received for packets: ");

        for (int i = currentIndex; i < nextSequenceNumber; i++) {
            if (packets[i].ack == 1) {
                ackPacketNumber = i;
                printf("%d ", ackPacketNumber);
            }
        }

        printf("\n");

        if (ackPacketNumber != -1) {
            currentIndex = ackPacketNumber + 1;
        }
    }
}

int main() {
    int numPackets;
    printf("Enter the number of packets to be transmitted: ");
    scanf("%d", &numPackets);

    Packet packets[numPackets];

    printf("Simulating transmission...\n");

    simulateTransmission(packets, numPackets);

    printf("Transmission complete.\n");

    return 0;
}
