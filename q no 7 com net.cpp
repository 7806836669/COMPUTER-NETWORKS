#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NODES 10
#define INFINITY 9999


typedef struct {
    int cost[MAX_NODES];   
    int nextHop[MAX_NODES]; 
    bool isActive[MAX_NODES];
} Node;


void initializeNetwork(Node* network, int numNodes, int sourceNode) {
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i == sourceNode) {
                network[i].cost[j] = 0;  
                network[i].nextHop[j] = j; 
                network[i].isActive[j] = true;
            } else {
                network[i].cost[j] = INFINITY; 
                network[i].nextHop[j] = -1;   
                network[i].isActive[j] = false;
            }
        }
    }
}


void printRoutingTable(Node* network, int numNodes, int node) {
    printf("Routing table for Node %d:\n", node);
    printf("Destination\tNext Hop\tCost\n");
    for (int i = 0; i < numNodes; i++) {
        printf("%d\t\t%d\t\t%d\n", i, network[node].nextHop[i], network[node].cost[i]);
    }
    printf("\n");
}


void updateRoutingTable(Node* network, int numNodes, int node) {
    for (int i = 0; i < numNodes; i++) {
        if (i == node) {
            continue; 
        }
        int minCost = INFINITY;
        int nextHop = -1;
        for (int j = 0; j < numNodes; j++) {
            if (network[node].isActive[j] && network[node].cost[j] != INFINITY) {
                int totalCost = network[node].cost[j] + network[j].cost[i];
                if (totalCost < minCost) {
                    minCost = totalCost;
                    nextHop = j;
                }
            }
        }
        network[node].nextHop[i] = nextHop;
        network[node].cost[i] = minCost;
    }
}


void distanceVectorRouting(Node* network, int numNodes, int sourceNode) {
    initializeNetwork(network, numNodes, sourceNode);
    
    for (int i = 0; i < numNodes; i++) {
        network[i].isActive[i] = true;
    }
    
    for (int k = 0; k < numNodes; k++) {
        for (int i = 0; i < numNodes; i++) {
            updateRoutingTable(network, numNodes, i);
        }
    }
    
    for (int i = 0; i < numNodes; i++) {
        printRoutingTable(network, numNodes, i);
    }
}

int main
