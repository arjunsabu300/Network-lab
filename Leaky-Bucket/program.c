leaky_bucket.c
3 of 3
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep function

#define BUCKET_SIZE 10  // Maximum bucket size
#define OUTPUT_RATE 3    // Rate at which packets are sent out

void leaky_bucket(int packets[], int n) {
    int bucket = 0; // Current bucket size
    
    printf("\nTime Step | Incoming | Bucket Size | Sent | Remaining\n");
    printf("------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-9d | %-8d | ", i + 1, packets[i]);
        
        // Add incoming packets if there is space
        if (packets[i] + bucket > BUCKET_SIZE) {
            printf("Overflow!   | ");
            bucket = BUCKET_SIZE;  // Bucket is full, overflow occurs
        } else {
            bucket += packets[i];
            printf("%-11d | ", bucket);
        }
        
        // Send packets at output rate
        int sent = (bucket < OUTPUT_RATE) ? bucket : OUTPUT_RATE;
        bucket -= sent;
        printf("%-4d | %-8d\n", sent, bucket);
        
        sleep(1); // Simulating delay for each time step
    }
    
    // Process remaining packets in the bucket
    while (bucket > 0) {
        int sent = (bucket < OUTPUT_RATE) ? bucket : OUTPUT_RATE;
        bucket -= sent;
        printf("%-9s | %-8s | %-11d | %-4d | %-8d\n", "Drain", "--", bucket + sent, sent, bucket);
        sleep(1);
    }
}

int main() {
    int n;
    printf("Enter the number of time steps: ");
    scanf("%d", &n);
    
    int packets[n];
    printf("Enter the incoming packets at each time step:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &packets[i]);
    }
    
    leaky_bucket(packets, n);
    return 0;
}