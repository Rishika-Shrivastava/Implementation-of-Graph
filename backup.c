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

    visited[vertex]  = True;

    visit(vertex, myGraph);

    while(adjNode != NULL) {

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

    for(i=0 ; i<MAXSIZE ; i++) {
        visited[i] = False;
    }

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


//FUNCTION FOR CONNECTED OR DISCONNECTED
void connectOrDisconnect(graphNode *myGraph) {

    int i = 0, count = 0;

    //initVisit();

    for(i = 0; i<MAXSIZE ; i++) {
        visited[i] = False;
    }

    Traverse(0, myGraph);

    for(i=0 ; i<MAXSIZE; i++) {
        if(visited[i] == True) {
            count++;
        }
    }

    if(myGraph->noVertices == count) {
        printf("Graph is connected\n");
    }
    else {
        printf("Graph is not connected\n");
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

void shortDist(int dist[], int u, graphNode *myGraph) {

    initVisit();
    createCostMatrix(myGraph);

    int i =0, w= 0, minVal, v=0;

    for(i=0; i<MAXSIZE; i++) {
        dist[i] = costMatrix[u][i];
    }

    visited[u] = True;
    dist[u] = 0;

    for(i=0; i<MAXSIZE-2; i++) {
        minVal = INT_MAX;

        for(w=0; w<MAXSIZE; w++) {
            if(visited[w] == False) {

                if(dist[w] < minVal) {
                    v = w;
                }
            }
        }

        visited[v] = True;

        for(w=0; w<MAXSIZE; w++) {
            if(visited[w] == False) {

                if(dist[v] + costMatrix[v][w] < dist[w]) {
                    dist[w] = dist[v] + costMatrix[v][w];
                }

            }
        }
    }

}



int main()
{
    graphNode *g1;

    //tf results;

    g1 = createGraph(u, w);

/*    addNode(1, 4, g1);
    addNode(2, 3, g1);
    addNode(3, 5, g1);
    addNode(4, 7, g1);
    addNode(5, 2, g1);

    addEdge(1, 2, 4, g1);
    addEdge(1, 3, 4, g1);
    addEdge(1, 5, 5, g1);
    addEdge(2, 3, 6, g1);
    addEdge(2, 5, 3, g1);
    addEdge(3, 5, 8, g1);
    addEdge(3, 4, 2, g1);
    addEdge(4, 5, 7, g1);

    int i =0;

    headNode inNode;
    edgeNode *adjNode;

    for(i=0; i<MAXSIZE; i++) {

        inNode = g1->vertexArray[i];

        if(inNode.vertexNo != -1) {
            adjNode = inNode.connectEdge;

            printf("Vertex %d : ", inNode.vertexNo);

            while(adjNode != NULL) {
                printf("%d ", adjNode->vertexNo);
                adjNode = adjNode->nextEdge;
            }

            printf("\n");
        }


    }

   // printf("\n\n");
    deleteNode(1, g1);

    for(i=0; i<MAXSIZE; i++) {

        inNode = g1->vertexArray[i];

        if(inNode.vertexNo != -1) {
            adjNode = inNode.connectEdge;

            printf("Vertex %d : ", inNode.vertexNo);

            while(adjNode != NULL) {
                printf("%d ", adjNode->vertexNo);
                adjNode = adjNode->nextEdge;
            }
        }

        printf("\n");

    }


    results = hasCycle(g1);
    printTF(results);

//    addNode(0, 4, g1);
    addNode(1, 4, g1);
    addNode(2, 3, g1);
    addNode(3, 5, g1);
    addNode(4, 2, g1);
    addNode(5, 4, g1);
    addNode(6, 3, g1);
    addNode(7, 5, g1);
    addNode(8, 7, g1);
    addNode(9, 4, g1);

    addEdge(1, 3, 0, g1);
    addEdge(1, 2, 0, g1);
    addEdge(1, 4, 0, g1);
    addEdge(2, 5, 0, g1);
    addEdge(3, 5, 0, g1);
    addEdge(3, 6, 0, g1);
    addEdge(4, 6, 0, g1);
    addEdge(4, 8, 0, g1);
    addEdge(5, 7, 0, g1);
    addEdge(6, 7, 0, g1);
    addEdge(6, 9, 0, g1);

    breadthFirstSearch(g1, 4);


    //topSort(g1);

    addNode(0, 7, g1);
    addNode(1, 4, g1);
    addNode(2, 3, g1);
    addNode(3, 5, g1);
    addNode(4, 4, g1);

    addEdge(0, 1, 0, g1);
    addEdge(1, 3, 0, g1);
    addEdge(3, 2, 0, g1);

    printf("\n\n");

    connectOrDisconnect(g1);

    printf("\n\n");

    addEdge(3, 4, 0, g1);

    connectOrDisconnect(g1);

    addNode(1, g1);
    addNode(2, g1);
    addNode(3, g1);
    addNode(4, g1);
    addNode(5, g1);
    addNode(6, g1);
    addNode(7, g1);
    addNode(8, g1);

    addEdge(1, 2, 0, g1);
    addEdge(2, 5, 0, g1);
    addEdge(4, 8, 0, g1);
    addEdge(3, 1, 0, g1);
    addEdge(7, 3, 0, g1);
    addEdge(8, 7, 0, g1);
    addEdge(5, 8, 0, g1);
    addEdge(8, 6, 0, g1);
    addEdge(2, 4, 0, g1);
    addEdge(6, 3, 0, g1);

*/

 /*   int i =0;

    for(i=0; i<MAXSIZE ; i++) {
        if(g1->vertexArray[i].vertexNo != -1) {
            printf("%d\n", g1->vertexArray[i].vertexNo);
        }
    }

    printf("Total number of vertex is %d \n\n", g1->noVertices);

    edgeNode *e = g1->vertexArray[4].connectEdge;

    while(e != NULL) {
        printf("vertex connection %d\n" , e->vertexNo);
        e = e->nextEdge;
    }

    printf("\n");

    deleteEdge(4,3, g1);
    deleteEdge(4,5, g1);

    e = g1->vertexArray[4].connectEdge;

    while(e != NULL) {
        printf("vertex connection %d\n" , e->vertexNo);
        e = e->nextEdge;
    } */

 //   depthFirstTraversal(g1);
 //   printf("\n\n");
 //   breadthFirstTraversal(g1);


    return 0;
}
