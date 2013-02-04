/*
 * Onur Yilmaz
 * CENG 315 - HW #3
 *
 */

// Libraries
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>


// Maximum of 10000 nodes
#define MAX 10000

// Big M for infinity values
#define INF 9999

using namespace std;

// Amount of current gold
int CurrentGold;

// Current time
int CurrentTime;

// Total distance traveled
int totalDistance = 0;

// Number of nodes
int numberOfNodes;

// Special rooms
int ChamberRoom;
int KeyRoom;
int LuigiRoom;

// Original adjacency matrix read from file
int OriginalGraph[MAX][MAX];

// Adjacency matrix used for comparisons
int OnlineGraph[MAX][MAX];

// Closed rooms
int ClosedOnEven[MAX] = {0};
int ClosedOnOdd[MAX] = {0};

// Gold rooms
int GoldNodeOriginal[MAX] = {0};
int GoldNodeOnline[MAX] = {0};

// Distance array
int distance[MAX] = {0};

// Predecessor array
int predecessor[MAX] = {0};

// Visit time array
int visitedAt[MAX] = {0};

// Vector for saving final traverse order
vector<int> VisitedNodesInOrder;

// Function declarations
void BellmanFordIterate(int);
void TraverseNode(int);
void clearAll(int);
void clearAll(int);
void goldNodeAdjust();
void returnToOriginal();

/*
 * Function for closing / opening nodes at even times
 *
 */
void updateEvenClosed() {

    extern int OnlineGraph[MAX][MAX];
    extern int ClosedOnEven[MAX];
    extern int numberOfNodes;

    returnToOriginal();

    // Close
    for (int i = 1; i <= numberOfNodes; i++) {
        if (ClosedOnEven[i] == 1) {
            for (int j = 1; j <= numberOfNodes; j++)
                OnlineGraph[j][i] = INF;
        }
    }

}

/*
 * Function for closing / opening nodes at odd times
 *
 */
void updateOddClosed() {

    extern int OnlineGraph[MAX][MAX];
    extern int numberOfNodes;

    returnToOriginal();

    // Close
    for (int i = 1; i <= numberOfNodes; i++) {
        if (ClosedOnOdd[i] == 1) {
            for (int j = 1; j <= numberOfNodes; j++)
                OnlineGraph[j][i] = INF;
        }
    }


}

/*
 * Function for complete iteration of Bellman-Ford Algorithm
 * Implementation of pseudo-code: http://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm#Algorithm
 */
void BellmanFordIterate(int start) {

    extern int OnlineGraph[MAX][MAX];
    extern int numberOfNodes;
    extern int distance[MAX];
    extern int predecessor[MAX];
    extern int CurrentTime;


    // Save current time as time for start node
    visitedAt[start] = CurrentTime;

    for (int i = 1; i <= numberOfNodes - 1; i++) {
        for (int u = 1; u <= numberOfNodes; u++) {
            for (int v = 1; v <= numberOfNodes; v++) {

                // If they are connected
                if (OnlineGraph[u][v] != 0) {

                    // Even / Odd time adjustments
                    if ((visitedAt[u] % 2) == 0) {
                        updateOddClosed();
                    }
                    if ((visitedAt[u] % 2) == 1) {
                        updateEvenClosed();
                    }
                    // Test cout for tracking
                    //cout << "Where: " << u << " - When: " << visitedAt[u] << endl;
                   
                    goldNodeAdjust();

                    // If there is a shorter way

                     for(int r=1; r <= numberOfNodes;r++)
                             if(predecessor[r] == v){
                                cout << "bu daha once var!"<<endl;
                                 
                                GoldNodeOriginal[v] = 0;
                        goldNodeAdjust();
                      }
                    
                    if (distance[v] > distance[u] + OnlineGraph[u][v]) {

                        distance[v] = distance[u] + OnlineGraph[u][v];

                       
                        visitedAt[v] = visitedAt[u] + 1;
         
                        predecessor[v] = u;

                        //GoldNodeOnline[u] = 0;

                        //goldNodeAdjust();
                    
                        //cout << "From " << u << " at " << visitedAt[u] << " to " << v << " at " << visitedAt[v] << endl;
                    }
                }
            }
        }
    }
}

/*
 * Function for constructing the paths using predecessors
 */

void TraverseNode(int GoTo) {

    extern int CurrentTime;
    extern int distance[MAX];
    extern int predecessor[MAX];

    int arr[MAX];
    int k = 1;

    int j = GoTo;

    // Track predecessors
    while (predecessor[j] != 0) {
        arr[k] = predecessor[j];
        k++;
        j = predecessor[j];
    }

    // Construct the path
    for (--k; k > 0; k--) {

        cout << "Time " << CurrentTime << ": " << arr[k] << endl;
        VisitedNodesInOrder.push_back(arr[k]);
        visitedAt[k] = CurrentTime;
        CurrentTime++;

        GoldNodeOriginal[arr[k]] = 0;
        goldNodeAdjust();

    }
    cout << "Time " << visitedAt[GoTo] << ": " << GoTo << endl;
    cout << "Distance: " << distance[GoTo] << endl;

    totalDistance += distance[GoTo];

}

/*
 * Function for clearing all the information related to Bellman-Ford Iteration
 *
 */

void clearAll(int startNode) {
    extern int numberOfNodes;
    extern int distance[MAX];
    extern int predecessor[MAX];
    extern int visitedAt[MAX];

    visitedAt[0] = 0;
    // Clear all distance, predecessor and visiting time information
    for (int i = 1; i <= numberOfNodes; i++) {
        distance[i] = INF;
        predecessor[i] = 0;
        visitedAt[i] = 0;
    }
    distance[startNode] = 0;


}

/*
 * Function decreasing distance if it is a gold node
 *
 */
void goldNodeAdjust() {

    extern int OriginalGraph[MAX][MAX];
    extern int OnlineGraph[MAX][MAX];
    extern int ClosedOnEven[MAX];
    extern int numberOfNodes;
    extern int GoldNodeOriginal[MAX];
    extern int GoldNodeOnline[MAX];

    for (int i = 1; i <= numberOfNodes; i++) {
        for (int j = 1; j <= numberOfNodes; j++){
            if (GoldNodeOriginal[j] != 0 && GoldNodeOnline[j] != 0 && OnlineGraph[i][j] != 0 && OnlineGraph[i][j] != INF && OnlineGraph[i][j] == OriginalGraph[i][j]) {
               
                //cout << "Gold Node! " << " From " << i << " to " << j <<  " Amount: " << GoldNode[j] << endl;
                //cout << "Original distance " << OnlineGraph[i][j] << endl;
                OnlineGraph[i][j] = OriginalGraph[i][j] - GoldNodeOnline[j];
                //cout << "New distance " << OnlineGraph[i][j] << endl;
            
        }
     
                
        
    }
}
}

/*
 * Function changing adjacency matrix to original
 *
 */
void returnToOriginal() {

    extern int OriginalGraph[MAX][MAX];
    extern int OnlineGraph[MAX][MAX];

    for (int i = 1; i <= numberOfNodes; i++)
        for (int j = 1; j <= numberOfNodes; j++)
            OnlineGraph[i][j] = OriginalGraph[i][j];

    for (int j = 1; j <= numberOfNodes; j++)
        GoldNodeOnline[j] = GoldNodeOriginal[j];

}

/*
 * Main function
 */
int main() {

    extern int CurrentGold;
    extern int numberOfNodes;
    extern int OriginalGraph[MAX][MAX];

    extern int ChamberRoom;
    extern int KeyRoom;
    extern int LuigiRoom;

    extern int OnlineGraph[MAX][MAX];

    extern int ClosedOnEven[MAX];
    extern int ClosedOnOdd[MAX];

    int startNode;

    // Open input file
    ifstream myReadFile;
    myReadFile.open("hw3.inp");

    // Read current gold
    myReadFile >> CurrentGold;

    // Read number of nodes
    myReadFile >> numberOfNodes;

    // Clear adjacency matrix
    for (int i = 1; i <= numberOfNodes; i++)
        for (int j = 1; j <= numberOfNodes; j++) {
            OriginalGraph[i][j] = 0;
        }

    // Read special rooms
    myReadFile >> ChamberRoom;
    myReadFile >> KeyRoom;
    myReadFile >> LuigiRoom;


    int temp;
    // Read closed on odd
    myReadFile >> temp;
    for (int i = 1; i <= temp; i++) {
        int temp2;
        myReadFile >> temp2;
        // cout << "Closed on odd " << temp2 << endl;
        ClosedOnOdd[temp2] = 1;
    }

    // Read closed on even
    myReadFile >> temp;
    for (int i = 1; i <= temp; i++) {
        int temp2;
        myReadFile >> temp2;
        //cout << "Closed on even " << temp2 << endl;
        ClosedOnEven[temp2] = 1;
    }

    // Read distances
    myReadFile >> temp;
    //cout << "Number of arcs " << temp << endl;
    for (int i = 1; i <= temp; i++) {
        int temp1, temp2, temp3;
        myReadFile >> temp1 >> temp2 >> temp3;
        OriginalGraph[temp1][temp2] = temp3;
        OriginalGraph[temp2][temp1] = temp3;
    }

    // Read gold nodes
    myReadFile >> temp;
    for (int i = 1; i <= temp; i++) {
        int temp1, temp2;
        myReadFile >> temp1 >> temp2;
        GoldNodeOriginal[temp1] = temp2;
    }

    for (int i = 1; i <= numberOfNodes; i++)
        GoldNodeOnline[i] = GoldNodeOriginal[i];

    // Close input files
    myReadFile.close();

    // Create a copy of adjacency matrix for comparison
    for (int i = 1; i <= numberOfNodes; i++)
        for (int j = 1; j <= numberOfNodes; j++)
            OnlineGraph[i][j] = OriginalGraph[i][j];

    // Gold node adjustments
    goldNodeAdjust();

    // Current time
    extern int CurrentTime;
    CurrentTime = 1;

    // Start at node 1, go to Key Room
    startNode = 1;
    clearAll(startNode);
    BellmanFordIterate(startNode);
    TraverseNode(KeyRoom);

    // Start at Key Room, go to Luigi's Room
    startNode = KeyRoom;
    clearAll(startNode);
    BellmanFordIterate(KeyRoom);
    TraverseNode(LuigiRoom);

    // Start at Luigi's Room, go to Chamber Room
    startNode = LuigiRoom;
    clearAll(startNode);
    BellmanFordIterate(LuigiRoom);
    TraverseNode(ChamberRoom);


    // Write time to output file
    ofstream outputFile("hw3.out");

    //cout << "Total distance: " << totalDistance << endl;
    //cout << "Ending gold: " << CurrentGold - totalDistance << endl;
    //cout << "End time: " << CurrentTime << endl;

    outputFile << CurrentGold - totalDistance << endl;
    outputFile << CurrentTime << endl;

    VisitedNodesInOrder.push_back(ChamberRoom);

    for (unsigned int i = 0; i < VisitedNodesInOrder.size(); i++) {
        outputFile << VisitedNodesInOrder[i];
        if ((VisitedNodesInOrder.size() - i) != 1)
            outputFile << " ";
    }
    outputFile << endl;

    // Close the output file
    outputFile.close();

    return 0;
}

// End of code
