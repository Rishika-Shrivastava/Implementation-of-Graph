#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#define MAXSIZE 12

typedef enum {u, d} graphTypeD;          // whether directed or undirected
typedef enum {w, un} graphTypeW;         // whether weighted or unweighted

typedef enum {False, True} tf;          // works like a boolean

tf visited[MAXSIZE];
tf countStack[MAXSIZE];
int costMatrix[MAXSIZE][MAXSIZE];



//STRUCTURE FOR NODES IN ADJACENCY LIST
typedef struct adjEdgeNode {
    int vertexNo;
    int gVal;
    struct adjEdgeNode *nextEdge;
} edgeNode;


//STRUCTURE FOR ADJACENCY LIST
typedef struct adjHeadNode {
    int vertexNo;
    int value;
    edgeNode *connectEdge;
} headNode;


//STRUCTURE FOR GRAPH
typedef struct graphStruct {
    graphTypeD gTypeD;
    graphTypeW gTypeW;
    int noVertices;
    headNode vertexArray[MAXSIZE];
} graphNode;

//------------------------------------ QUEUE FUNCTIONS --------------------------------------

typedef struct Queue_tag {
    int vertexVal[MAXSIZE];
    int front;
    int rear;
    int count;
} Queue;

//INITIALIZE QUEUE
Queue *InitializeQ() {
    Queue *newQueue;
    newQueue = (Queue*)malloc(sizeof(Queue));

    newQueue->front = 0;
    newQueue->rear = -1;
    newQueue->count = 0;

    return newQueue;
}


//INSERT VALUE IN QUEUE
void insertQ(int vertexNo, Queue *myQueue) {

    if(myQueue->count == MAXSIZE) {
        printf("Queue full\n");
    }
    else {
        myQueue->rear = (myQueue->rear + 1)%MAXSIZE;
        myQueue->vertexVal[myQueue->rear] = vertexNo;
        (myQueue->count)++;
    }
}

//REMOVE VALUE FROM QUEUE
void deleteQ(Queue *myQueue) {

    if(myQueue->count == 0) {
        printf("Queue empty\n");
    }
    else {
        (myQueue->count)--;
         myQueue->vertexVal[myQueue->front] = -1;
         (myQueue->front)++;
    }

}

//------------------------------------------------------------------------------------------

//----------------------------------- STACK FUNCTIONS --------------------------------------

typedef struct Stack_tag {
    int vertexVal[MAXSIZE];
    int top;
} stk;

stk *initializeStk() {
    stk *newStack;
    newStack = (stk*)malloc(sizeof(stk));

    newStack->top = 0;

    return newStack;
}

void pushStk(int x, stk *myStack) {
    if(myStack->top == MAXSIZE) {
        printf("Stack full\n");
    }
    else {
        myStack->vertexVal[myStack->top] = x;
        //printf("%d ", myStack->vertexVal[myStack->top]);
        (myStack->top)++;
    }

}

void popStk(stk *myStack) {

    if(myStack->top == 0) {
        printf("Stack empty\n");
    }
    else {
        (myStack->top)--;
        printf("%d ", myStack->vertexVal[myStack->top]);
        myStack->vertexVal[myStack->top] = -1;
    }

}

//------------------------------------------------------------------------------------------


//FUNCTION TO CREATE AN EDGE NODE
edgeNode *createEdgeNode(int v, int weight) {

    edgeNode *newNode;

    newNode = (edgeNode*)malloc(sizeof(edgeNode));

    newNode->vertexNo = v;
    newNode->nextEdge= NULL;
    newNode->gVal = weight;

    return newNode;
}


//FUNCTION TO CREATE A GRAPH
graphNode *createGraph(graphTypeD g, graphTypeW t) {

    graphNode *newGraph;

    newGraph = (graphNode*)malloc(sizeof(graphNode));

    newGraph->noVertices = 0;
    newGraph->gTypeD = g;
    newGraph->gTypeW = t;

    //printf("Entering conditions\n");

    if(!newGraph->vertexArray) {
        printf("Not enough space\n");
    }
    else {
        int i =0;

        for(i=0; i<MAXSIZE; i++) {
            newGraph->vertexArray[i].connectEdge = NULL;
            newGraph->vertexArray[i].vertexNo= -1;
        }
    }

//    printf("Graph created\n\n");

    return newGraph;
}


//FUNCTION TO ADD NODE
void addNode(int index, int val, graphNode *myGraph) {

    if(index >= MAXSIZE){
        printf("Max limit reached\n\n");
    }
    else if(myGraph->vertexArray[index].vertexNo != -1){
        printf("This vertex already exists.\n\n");
    }
    else {
        myGraph->vertexArray[index].connectEdge = NULL;
        myGraph->vertexArray[index].vertexNo = index;
        myGraph ->vertexArray[index].value = val;
        myGraph->noVertices++;
//        printf("Node inserted\n\n");
    }

}


//FUNCTION TO ADD EDGE NODE
void addEdge(int u, int v, int wt, graphNode *myGraph) {

    if(myGraph->vertexArray[u].vertexNo == -1 || myGraph->vertexArray[v].vertexNo == -1) {
        printf("Check your vertices\n\n");
    }
    else {

        if(myGraph->gTypeD == d) {

            edgeNode *newEdge;
            newEdge = createEdgeNode(v, wt);

            if(myGraph->vertexArray[u].connectEdge == NULL) {
                myGraph->vertexArray[u].connectEdge = newEdge;
            }
            else {
                edgeNode *ptr = myGraph->vertexArray[u].connectEdge;

                while(ptr->nextEdge != NULL) {
                    ptr = ptr->nextEdge;
                }

                ptr->nextEdge = newEdge;
            }

        }
        else {

            edgeNode *newEdge1, *newEdge2;

            newEdge1 = createEdgeNode(v, wt);
            newEdge2 = createEdgeNode(u, wt);

            if(myGraph->vertexArray[u].connectEdge == NULL) {
                myGraph->vertexArray[u].connectEdge = newEdge1;
            }
            else {
                edgeNode *ptr = myGraph->vertexArray[u].connectEdge;

                while(ptr->nextEdge != NULL) {
                    ptr = ptr->nextEdge;
                }

                ptr->nextEdge = newEdge1;
            }

            if(myGraph->vertexArray[v].connectEdge == NULL) {
                myGraph->vertexArray[v].connectEdge = newEdge2;
            }
            else {
                edgeNode *ptr = myGraph->vertexArray[v].connectEdge;

                while(ptr->nextEdge != NULL) {
                    ptr = ptr->nextEdge;
                }

                ptr->nextEdge = newEdge2;
            }


        }

    }

}


//FUNCTION TO DELETE AN EDGE
void deleteEdge(int u, int v, graphNode *myGraph) {

    if( myGraph->gTypeD == d) {
        edgeNode *temp;

        temp = myGraph->vertexArray[u].connectEdge;

        if(temp->vertexNo == v) {
            myGraph->vertexArray[u].connectEdge = temp ->nextEdge;
            free(temp);
        }
        else {
            edgeNode *ptr = NULL;
            int flag = 0;

            while(temp->vertexNo != v && temp->nextEdge != NULL) {
                ptr = temp;
                temp = temp->nextEdge;

                if(temp->vertexNo == v) {
                    flag = 1;
                }
            }

            if(flag == 1) {
                ptr->nextEdge = temp->nextEdge;
                free(temp);
                printf("Freed\n");
            }
            else {
              //  printf("Node does not exist\n\n");
            }

        }
    }
    else {
        edgeNode *temp1, *temp2;
        edgeNode *ptr = NULL;
        int flag = 0;

        temp1 = myGraph->vertexArray[u].connectEdge;
        temp2 = myGraph->vertexArray[v].connectEdge;

        if(temp1->vertexNo == v) {
            myGraph->vertexArray[u].connectEdge = temp1->nextEdge;
            free(temp1);
        }
        else {
            edgeNode *ptr = NULL;
            int flag = 0;

            while(temp1->vertexNo != v && temp1->nextEdge != NULL) {
                ptr = temp1;
                temp1 = temp1->nextEdge;

                if(temp1->vertexNo == v) {
                    flag = 1;
                }
            }

            if(flag == 1) {
                ptr->nextEdge = temp1->nextEdge;
                free(temp1);
                printf("Freed\n");
            }
            else {
               // printf("Node does not exist\n\n");
            }

        }


        if( temp2->vertexNo == u ) {
            myGraph->vertexArray[v].connectEdge = temp2->nextEdge;
            free(temp2);
        }
        else {
             ptr = NULL;
             flag = 0;

            while(temp2->vertexNo != u && temp2->nextEdge != NULL) {

                ptr = temp2;
                temp2 = temp2->nextEdge;

                if(temp2->vertexNo == u) {
                    flag = 1;
                }
            }

            if( flag == 1 ) {
                ptr->nextEdge = temp2->nextEdge;
                free(temp2);
                printf("Freed\n");
            }
            else {
             //   printf("Node does not exist\n\n");
            }

        }

    }

}


//FUNCTION TO DELETE A NODE
void deleteNode(int v, graphNode *myGraph) {

    headNode inNode;
    edgeNode *adjNode, *temp = NULL;
    int i =0;

    inNode = myGraph->vertexArray[v];

    while( adjNode != NULL ) {

        temp = inNode.connectEdge;

        adjNode = temp->nextEdge;

        inNode.connectEdge = adjNode;

       // printf("%d  ", temp->vertexNo);

        free(temp);
       // printf("Free edge\n");

        temp = adjNode;
        adjNode = adjNode->nextEdge;

    }

    free(temp);

    myGraph->vertexArray[v].vertexNo = -1;
    myGraph->vertexArray[v].value = -1;

    for(i=0; i<MAXSIZE; i++) {

        if(myGraph->vertexArray[i].vertexNo != -1) {
            deleteEdge(i, v, myGraph);
        }

    }


}


//INITIALISE VISIT ARRAY
void initVisit() {
    int i=0;

    for(i=0; i<MAXSIZE ; i++) {
        visited[i] = False;
    }

}


//PRINT VALUE AT INDEX
void visit(int index, graphNode *myGraph) {

    if(myGraph->vertexArray[index].vertexNo != -1) {
        printf("%d \n", myGraph->vertexArray[index].vertexNo);
    }

}


//FUNCTION TO TRAVERSE
void Traverse(int vertex, graphNode *myGraph) {

//    printf("Enter traverse function\n");

    headNode inNode;
    edgeNode *adjNode;

    inNode = myGraph->vertexArray[vertex];
    adjNode = inNode.connectEdge;

 //   printf("inNode vertex value %d \n", inNode.vertexNo);
 //   printf("adjNode vertex value &d \n", adjNode->vertexNo);

    visited[vertex]  = True;

    visit(vertex, myGraph);

    while(adjNode != NULL) {

 //       printf("Enter traversal loop\n");

        if(visited[adjNode->vertexNo] == False) {
            Traverse(adjNode->vertexNo, myGraph);
        }

        adjNode = adjNode->nextEdge;

    }

}


//FUNCTION FOR DEPTH FIRST TRAVERSAL
void depthFirstTraversal(graphNode *myGraph) {

//    printf("Enter depth first\n");

    int i =0, vertex =0;

    initVisit();

    for(i=0 ; i<MAXSIZE; i++) {

        if(myGraph->vertexArray[i].vertexNo != -1) {
            if(visited[i] == False) {
                vertex = i;
                Traverse(vertex, myGraph);
            }
        }
    }
}


//FUNCTION FOR BREADTH FIRST TRAVERSAL
void breadthFirstTraversal(graphNode *myGraph) {

    int i = 0;
    Queue *Q;
    edgeNode *adjNode = NULL;

    for(i=0; i<MAXSIZE; i++) {
        visited[i] = False;
    }

    Q = InitializeQ();

    for(i=0 ; i<MAXSIZE ; i++) {

        if(myGraph->vertexArray[i].vertexNo != -1) {
            if(visited[i] == False) {
                insertQ(i, Q);

                do {
                    deleteQ(Q);

                    if(visited[i] == False) {
                        visited[i] = True;
                        visit(i, myGraph);
                    }

                    while(adjNode != NULL) {
                        if(visited[adjNode->vertexNo] == False) {
                            insertQ(adjNode->vertexNo, Q);
                        }
                    }
                } while(Q->count != 0);
            }
        }
    }

}


//FUNCTIONS FOR TOPOLOGICAL SORT
void topTraverse(int vertex, stk *S, graphNode *myGraph) {

    visited[vertex] = True;

    headNode inNode;
    edgeNode *adjNode;

    inNode = myGraph->vertexArray[vertex];
    adjNode = inNode.connectEdge;

    while(adjNode != NULL) {

        if(visited[adjNode->vertexNo] == False) {
            topTraverse(adjNode->vertexNo, S, myGraph);
        }

        adjNode = adjNode->nextEdge;
    }

    pushStk(vertex, S);

}


void topSort(graphNode *myGraph) {

    stk *S;
    S = initializeStk();
    int i=0;

    initVisit();

    for(i=0 ; i<MAXSIZE ; i++) {

        if(myGraph->vertexArray[i].vertexNo != -1) {
            if(visited[i] == False) {
                topTraverse(i, S, myGraph);
            }
        }
    }
    while(S->top != 0) {
        popStk(S);
    }

}


//REVERSE GRAPH
graphNode *getReverse(graphNode *myGraph) {

    graphNode *G = createGraph(myGraph->gTypeD, myGraph->gTypeW);

    int i=0, j=0;

    for(i=0; i<MAXSIZE; i++) {

        if(myGraph->vertexArray[i].vertexNo != -1) {
            addNode(myGraph->vertexArray[i].vertexNo,myGraph->vertexArray[i].value, G);
//            printf("adding nodes %d \n", G->vertexArray[i].vertexNo);
        }

    }

    edgeNode *adjNode = NULL;

    for(i=0; i<MAXSIZE; i++) {

        if(myGraph->vertexArray[i].vertexNo != -1) {
            adjNode = myGraph->vertexArray[i].connectEdge;

            while(adjNode != NULL) {

             //   printf("add edge %d %d \n", i, adjNode->vertexNo);
                addEdge(adjNode->vertexNo, i, adjNode->gVal, G);
                adjNode = adjNode->nextEdge;

            }

          //  printf("\n loop ends once \n ");
        }

    }

    return G;

}


//FUNCTION FOR CONNECTED OR DISCONNECTED
void connectOrDisconnect(graphNode *myGraph) {


    if(myGraph->gTypeD == u) {

        int i = 0, count = 0;

        initVisit();

         Traverse(0, myGraph);

        for(i=0 ; i<MAXSIZE; i++) {
            if(visited[i] == True) {
                count++;
            }
        }

        if(myGraph->noVertices == count) {
            printf("Graph is connected \n");
        }
        else {
            printf("Graph is not connected\n");
        }

    }
    else {

        int i = 0, count = 0;

        initVisit();

         Traverse(0, myGraph);

         printf("\n\n");

        for(i=0 ; i<MAXSIZE; i++) {
            if(visited[i] == True) {
                count++;
            }
        }

        if(myGraph->noVertices == count) {

            initVisit();

            graphNode *G;
            G = getReverse(myGraph);

//            printf("\nAfter reverse \n");

            count = 0, i=0;

            Traverse(0, G);

//            printf("Second traverse\n");

            for(i=0 ; i<MAXSIZE; i++) {
                if(visited[i] == True) {
                    count++;
                }
            }

            if(G->noVertices == count) {
                printf("Graph is connected \n");
            }

        }
        else {
            printf("Graph is not connected\n");
        }


    }


}


//DEPTH FIRST SEARCH
void searchVal(int vertex, int k, graphNode *myGraph) {

    headNode inNode;
    edgeNode *adjNode;

    inNode = myGraph->vertexArray[vertex];
    adjNode = inNode.connectEdge;

    visited[vertex]  = True;

    if(myGraph->vertexArray[vertex].value == k) {
         visit(vertex, myGraph);
    }


    while(adjNode != NULL) {

        if(visited[adjNode->vertexNo] == False) {
            searchVal(adjNode->vertexNo, k, myGraph);
        }

        adjNode = adjNode->nextEdge;

    }
}


void depthFirstSearch(graphNode *myGraph, int key) {

//    printf("Enter depth first\n");

    int i =0, vertex =0;

    for(i=0 ; i<MAXSIZE ; i++) {
        visited[i] = False;
    }

    for(i=0 ; i<MAXSIZE; i++) {

        if(myGraph->vertexArray[i].vertexNo != -1) {
            if(visited[i] == False) {
                vertex = i;
                searchVal(vertex, key, myGraph);
            }
        }
    }
}


//BREADTH FIRST SEARCH
void breadthFirstSearch(graphNode *myGraph, int key) {

    int i = 0;
    Queue *Q;
    edgeNode *adjNode = NULL;

    for(i=0; i<MAXSIZE; i++) {
        visited[i] = False;
    }

    Q = InitializeQ();

    for(i=0 ; i<MAXSIZE ; i++) {

        if(myGraph->vertexArray[i].vertexNo != -1) {
            if(visited[i] == False) {
                insertQ(i, Q);

                do {
                    deleteQ(Q);

                    if(visited[i] == False) {
                        visited[i] = True;

                        if(myGraph->vertexArray[i].value == key) {
                            visit(i, myGraph);

                        }

                    }

                    while(adjNode != NULL) {
                        if(visited[adjNode->vertexNo] == False) {
                            insertQ(adjNode->vertexNo, Q);
                        }
                    }
                } while(Q->count != 0);
            }
        }
    }

}

//INITIALISE  COUNT STACK
void initCount() {
    int i=0;

    for(i=0; i<MAXSIZE; i++) {
        countStack[i] = False;
    }
}


//DETECT CYCLE
tf checkCycle(int v, graphNode *myGraph, int parent) {

    tf retVal = False;

    int flag = 0;

    if(myGraph->gTypeD == d) {

        if(visited[v] == False) {

            visited[v] = True;
            countStack[v] = True;

            headNode inNode;
            edgeNode *adjNode;

            inNode = myGraph->vertexArray[v];
            adjNode = inNode.connectEdge;

            while(adjNode != NULL && flag == 0) {

                if(visited[adjNode->vertexNo] == False && (checkCycle(adjNode->vertexNo, myGraph, 0) == True)) {
                    retVal = True;
                    flag = 1;
                }
                else if(countStack[adjNode->vertexNo] == True) {
                    retVal = True;
                    flag = 1;
                }
                adjNode = adjNode->nextEdge;
            }
        }

        countStack[v] = False;
    }
    else {
            printf("Enter check cycle undirected\n");

            visited[v] = True;

            headNode inNode;
            edgeNode *adjNode;

            inNode = myGraph->vertexArray[v];
            adjNode = inNode.connectEdge;

            while(adjNode != NULL && flag == 0) {

                if(visited[adjNode->vertexNo] == False) {

                        if(checkCycle(adjNode->vertexNo, myGraph, v) == True) {
                            retVal = True;
                            flag = 1;
                        }
                        else if(adjNode->vertexNo != parent) {
                            retVal = True;
                            flag = 1;
                        }

                }

                adjNode = adjNode->nextEdge;
            }


    }
    return retVal;
}


tf hasCycle(graphNode *myGraph) {

    tf retVal = False;

    int i =0, flag = 0;

    if(myGraph->gTypeD == d) {

        initCount();
        initVisit();

        for(i=0; i<MAXSIZE && flag == 0; i++) {

            if(checkCycle(i, myGraph, 0) == True) {
                retVal = True;
                flag = 1;
            }
        }
    }
    else {

        initVisit();

        for(i=0; i< MAXSIZE && flag == 0; i++) {

            if(visited[i] == False) {
                if(checkCycle(i, myGraph, -1) == True) {
                    retVal = True;
                    flag = 1;
                }
            }

        }

    }

    return retVal;
}

//PRINT TF VALUE
void printTF(tf boolVal) {

    if(boolVal == True) {
        printf("True\n");
    }
    else {
        printf("False\n");
    }

}


//CREATE COST MATRIX FROM LIST
void createCostMatrix(graphNode *myGraph) {

    int i=0, j=0;

    for(i=0; i<MAXSIZE ;i++) {
        for(j=0; j<MAXSIZE; j++) {
            costMatrix[i][j] = INT_MAX;
        }
    }

    headNode inNode;
    edgeNode *adjNode;

    for(i=0; i<MAXSIZE; i++) {

        inNode = myGraph->vertexArray[i];
        adjNode = inNode.connectEdge;

        while(adjNode != NULL) {
            costMatrix[i][adjNode->vertexNo] = adjNode->gVal;
            adjNode = adjNode->nextEdge;
        }

    }

}


//CALCULATE SHORTEST PATH : DIJKSTRAS SHORTEST PATH
void printPath(int prev[], int j) {

    if(prev[j] == -1) {
        return;
    }

    printPath(prev, prev[j]);

    printf("%d ", j);

}


void printSol(int prev[], int dist[], int u) {

    int i=0;

    printf("SHORTEST DISTANCE FROM VERTEX\n\n");

    for(i=0; i<MAXSIZE; i++) {
        if(dist[i] != INT_MAX) {
            printf("from: %d -> %d  value: %d path: %d ", u, i, dist[i], u);
            printPath(prev, i);
            printf("\n");
        }

    }

}


void shortDist(int dist[], int u, graphNode *myGraph) {

    initVisit();
    createCostMatrix(myGraph);

    int prev[MAXSIZE];

    int i =0, w= 0, minVal, v;

    for(i=0; i<MAXSIZE; i++) {
        dist[i] = costMatrix[u][i];
        prev[i] = u;
    }

    prev[u] = -1;

    visited[u] = True;
    dist[u] = 0;

    for(i=0; i<MAXSIZE-2; i++) {
        minVal = INT_MAX;

        for(w=0; w<MAXSIZE; w++) {

            if(visited[w] == False) {

                if(dist[w] < minVal) {
                    minVal = dist[w];
                    v = w;
                }
            }
        }

        visited[v] = True;

        for(w=0; w<MAXSIZE; w++) {

            if(visited[w] == False && myGraph->vertexArray[w].vertexNo != -1) {

                if(dist[v] + costMatrix[v][w] < dist[w] && costMatrix[v][w] != INT_MAX) {
                    dist[w] = dist[v] + costMatrix[v][w];
                    prev[w] = v;
                }

            }
        }
    }

    printSol(prev, dist, u);

}


//SHORTEST DISTANCE ALL PAIRS SHORTEST PATH
void printAPSPPath(int prev[][MAXSIZE], int i, int j) {

    if(prev[i][j] == i) {
        return;
    }

    printAPSPPath(prev, i, prev[i][j]);

    printf("%d ", prev[i][j]);

}


void printAPSP(int dist[][MAXSIZE], int prev[][MAXSIZE]) {
    int i=0, j=0;

    for(i=0; i<MAXSIZE; i++) {
        for(j=0; j<MAXSIZE; j++) {

            if(i!=j && prev[i][j] != -1) {
                printf("Shortest path from %d -> %d is %d. Path : %d ", i, j, dist[i][j], i);
                printAPSPPath(prev, i, j);
                printf("%d\n", j);
            }
        }

    }
}


void apsp(int dist[][MAXSIZE], graphNode *myGraph) {
    int i=0 , j=0, k=0;

//    printf("Enter asps\n");

    int prev[MAXSIZE][MAXSIZE];

    createCostMatrix(myGraph);

    for(i=0; i<MAXSIZE; i++) {
        for(j=0; j<MAXSIZE; j++) {

            dist[i][j] = costMatrix[i][j];
            if(i == j) {
                prev[i][j] = 0;
            }
            else if(costMatrix[i][j] != INT_MAX) {
                prev[i][j] = i;
            }
            else {
                prev[i][j] = -1;
            }

        }
    }

    for(k=0; k<MAXSIZE; k++) {
        for(i=0; i<MAXSIZE; i++) {
            for(j=0; j<MAXSIZE; j++) {

                if(dist[i][j] > dist[i][k] + dist[k][j] && (dist[i][k] != INT_MAX) && (dist[k][j] != INT_MAX)) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    prev[i][j] = prev[k][j];
                }

            }
        }
    }

    printAPSP(dist, prev);

}


//MINIMUM SPANNING TREE
int minKey(int key[], graphNode *myGraph) {

    int minVal = INT_MAX;
    int minLoc = -1;

    int i =0;

    for(i=0; i<MAXSIZE; i++) {

        if(visited[i] == False && key[i] < minVal && myGraph->vertexArray[i].vertexNo != -1) {
            minVal = key[i];
            minLoc = i;
        }

    }

    return minLoc;
}


void printMST(int prev[]) {

    int i=0;

    for(i=0; i<MAXSIZE; i++) {
        if(costMatrix[i][prev[i]] != INT_MAX && prev[i] >= 0) {
            printf("Edges: %d - %d Weight: %d\n", prev[i], i, costMatrix[i][prev[i]]);
        }
    }

}


void prims(graphNode *myGraph) {

    createCostMatrix(myGraph);
    initVisit();

    int prev[MAXSIZE], key[MAXSIZE], i=0, u=0, v=0;

    for(i=0; i<MAXSIZE; i++) {
        key[i] = INT_MAX;
    }

    key[0] = 0;
    prev[0] = -1;

    for(i=0; i<MAXSIZE-1; i++) {
        u = minKey(key, myGraph);
        visited[u] = True;

         for(v=0; v<MAXSIZE; v++) {

            if(costMatrix[u][v] != INT_MAX && visited[v] == False && costMatrix[u][v] < key[v]) {
                prev[v] = u;
                key[v] = costMatrix[u][v];
            }

        }
    }


    printMST(prev);

}


//DISPLAY ALL PATHS BETWEEN A PAIR OF NODES
void printAllPaths(int v1, int v2, int prev[], int *prevLoc, graphNode *myGraph) {

    visited[v1] = True;
    prev[*prevLoc] = v1;
//    printf("%d prevloc\n", prevLoc);
    (*prevLoc) = (*prevLoc) + 1;

    int i =0;

    if(v1 == v2) {

        for(i=0; i<*prevLoc; i++) {
            printf("%d ", prev[i]);
        }

        printf("\n");
    }
    else {
        headNode inNode;
        edgeNode *adjNode;

        inNode = myGraph->vertexArray[v1];
        adjNode = inNode.connectEdge;

        while(adjNode != NULL) {

            if(visited[adjNode->vertexNo] == False) {
               // printf("vertex no %d \n", adjNode->vertexNo);
                printAllPaths(adjNode->vertexNo, v2, prev, prevLoc, myGraph);
            }
            adjNode = adjNode->nextEdge;
        }
    }

    (*prevLoc) = (*prevLoc) - 1;
    visited[v1] = False;

}


void printPathPair(int start, int end, graphNode *myGraph) {

    initVisit();
    int prev[MAXSIZE];
    int prevLoc=0, i=0;

    printAllPaths(start, end, prev, &prevLoc, myGraph);

}




int main()
{
    graphNode *g1, *g2, *g3, *g4;
    int select = 0;

    int sw;
    int ch = 1;

	while(ch != 0)
	{
	    printf("Enter option number :\n 1.Create Graph \n 2.Add a node \n 3.Delete a node \n 4.Add edge \n 5.Delete edge \n 6.DFT \n 7.BFT \n 8.Topological Sort \n 9.Connected or Disconnected \n 10.Shortest Path \n 11.Shortest Path all pairs \n 12.BFS \n 13.DFS \n 14.detect cycle \n 15.Minimum spanning tree \n 16.Display all possible paths \n 17.Exit \n");
	    printf("---------------------------------------------- \n");

        scanf("%d",&sw);

        switch(sw)
        {
            case 1: printf("Enter the type of graph: \n 1. Undirected and Unweighted \n 2. Directed and Unweighted \n 3. Undirected and Weighted \n 4. Directed and Weighted\n");

                    scanf("%d", &select);

                    if(select == 1) {
                        g1 = createGraph(u, un);
                    }
                    else if(select == 2) {
                        g2 = createGraph(d, un);
                    }
                    else if(select == 3) {
                        g3 = createGraph(u, w);
                    }
                    else if(select == 4) {
                        g4 = createGraph(d, w);
                    }
                    else {
                        printf("Check option number again\n");
                    }
                    break;

            case 2: int nodeNo, value;

                    printf("Enter node number \n");
                    scanf("%d", &nodeNo);

                    printf("Enter node value \n");
                    scanf("%d", &value);

                    if(nodeNo > MAXSIZE || nodeNo < 0) {
                        printf("Node range between 0 and %d \n", MAXSIZE);
                    }
                    else {

                        if(select < 1 || select > 4) {
                            printf("Create graph first\n");
                        }
                        else if(select == 1) {
                            addNode(nodeNo, value, g1);
                        }
                        else if(select == 2) {
                            addNode(nodeNo, value, g2);
                        }
                        else if(select == 3) {
                            addNode(nodeNo, value, g3);
                        }
                        else if(select == 4) {
                            addNode(nodeNo, value, g4);
                        }

                    }

                    break;

            case 3: int del;

                    printf("Enter node number to be deleted \n");
                    scanf("%d", &del);

                    if(select < 1 || select > 4) {
                        printf("Create graph first\n");
                    }
                    else if(select == 1) {
                        deleteNode(del, g1);
                    }
                    else if(select == 2) {
                        deleteNode(del, g2);
                    }
                    else if(select == 3) {
                        deleteNode(del, g3);
                    }
                    else if(select == 4) {
                        deleteNode(del, g4);
                    }


                    break;

            case 4: int src, dest;
                    printf("Edge from source to destination? \n");
                    scanf("%d", src);
                    scanf("%d", dest);

                    if(select < 1 || select > 4) {
                        printf("Create graph first\n");
                    }
                    else if(select == 1) {
                        addEdge(src, dest, 0, g1);
                    }
                    else if(select == 2) {
                        addEdge(src, dest, 0, g2);
                    }
                    else if(select == 3) {
                        int wt;

                        printf("Enter weight of edge\n");
                        scanf("%d", wt);

                        addEdge(src, dest, wt, g3);
                    }
                    else if(select == 4) {
                        int wt;

                        printf("Enter weight of edge\n");
                        scanf("%d", wt);

                        addEdge(src, dest, wt, g4);
                    }

                    break;

            default: printf("You entered a wrong option. Try again \n");
                    break;
        }

	}



    return 0;
}
