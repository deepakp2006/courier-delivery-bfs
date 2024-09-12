#include <stdio.h>
#include <stdbool.h>

#define MAX_LOCATIONS 100

void createDotFile(int totalLocations, char locationNames[][50], int deliverySuccess[], bool hasCycle) {
    FILE *file = fopen("graph.dot", "w");
    if (!file) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    fprintf(file, "digraph G {\n");
    fprintf(file, "    graph [bgcolor=pink];\n");
    fprintf(file, "    node [shape=circle style=filled fontname=\"Arial\" fontsize=14];\n");
    fprintf(file, "    edge [fontname=\"Arial\" fontsize=12];\n");

    fprintf(file, "    \"DeliveryOffice\" [label=\"Delivery Office\" shape=box color=lightblue style=filled fontcolor=black];\n");

    for (int i = 0; i < totalLocations; i++) {
        if (deliverySuccess[i] == 0) {
            fprintf(file, "    \"%s\" [label=\"%s\" shape=circle color=lightcoral style=filled fontcolor=black];\n", locationNames[i], locationNames[i]);
        } else {
            fprintf(file, "    \"%s\" [label=\"%s\" shape=circle color=lightgreen style=filled fontcolor=black];\n", locationNames[i], locationNames[i]);
        }
    }

    for (int i = 0; i < totalLocations; i++) {
        if (i == 0) {
            fprintf(file, "    \"DeliveryOffice\" -> \"%s\" [color=black];\n", locationNames[i]);
        } else {
            fprintf(file, "    \"%s\" -> \"%s\" [color=black];\n", locationNames[i - 1], locationNames[i]);
        }
    }

    if (hasCycle) {
        fprintf(file, "    \"%s\" -> \"DeliveryOffice\" [color=red];\n", locationNames[totalLocations - 1]);
    }

    fprintf(file, "}\n");
    fclose(file);

    printf("DOT file generated successfully.\n");
}

bool performBFS(int totalLocations, int startNode, int deliverySuccess[]) {
    bool visited[MAX_LOCATIONS] = {false};
    int queue[MAX_LOCATIONS];
    int front = 0, rear = 0;
    
    visited[startNode] = true;
    queue[rear++] = startNode;
    
    bool hasCycle = false;

    while (front < rear) {
        int currentNode = queue[front++];

        if (deliverySuccess[currentNode] == 0) {
            printf("Delivery at location %d failed. Returning to office.\n", currentNode + 1);
            hasCycle = true;
        } else {
            printf("Delivery at location %d was successful.\n", currentNode + 1);
        }

        if (currentNode - 1 >= 0 && !visited[currentNode - 1]) {
            visited[currentNode - 1] = true;
            queue[rear++] = currentNode - 1;
        }
        if (currentNode + 1 < totalLocations && !visited[currentNode + 1]) {
            visited[currentNode + 1] = true;
            queue[rear++] = currentNode + 1;
        }
    }

    return hasCycle;
}

int main() {
    int totalLocations;
    printf("Enter number of locations: ");
    scanf("%d", &totalLocations);

    char locationNames[totalLocations][50];
    int deliverySuccess[totalLocations];

    for (int i = 0; i < totalLocations; i++) {
        printf("Enter name for location %d: ", i + 1);
        scanf("%s", locationNames[i]);

        char deliveryStatus;
        printf("Is the delivery at %s successful (y/n)? ", locationNames[i]);
        scanf(" %c", &deliveryStatus);

        deliverySuccess[i] = (deliveryStatus == 'y' || deliveryStatus == 'Y') ? 1 : 0;
    }

    bool hasCycle = performBFS(totalLocations, 0, deliverySuccess);

    createDotFile(totalLocations, locationNames, deliverySuccess, hasCycle);

    if (hasCycle) {
        printf("A cycle was detected as some deliveries were not successful.\n");
    } else {
        printf("No cycle detected. All deliveries were successful.\n");
    }

    return 0;
}
