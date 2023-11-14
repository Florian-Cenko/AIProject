#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>



#define INCREMENT_COST 2
#define DECREMENT_COST 2
#define DOUBLE_COST(x) (ceil((double)x / 2) + 1)
#define HALVE_COST(x) (ceil((double)x / 4) + 1)
#define SQUARE_COST(x) (((x * x) - x) / 4 + 1)
#define SQRT_COST(x) (((x - sqrt(x))) / 4 + 1)

// ������� ��� �������
typedef enum {
    INCREASE_BY_ONE,
    DECREASE_BY_ONE,
    MULTIPLY,
    DIVIDE,
    SQUARE,
    SQUARE_ROOT
} Operation;

// ������� ��� ����� ��� ���� �������
typedef struct Node {
    int value;
    Operation operation;
    struct Node* parent;
    int cost; // ���������� ��� ����� ��� �� ������
    int prevValue;
    int currCost;
    int plithos;
} Node;

typedef struct QueueNode {
    Node* data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;


// ������� ��� ����� ��� ��� ����� ��� �������
typedef struct StackNode {
    Node* data;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

// ���������� ���� ���� ������
Node* createNode(int value, Operation operation, Node* parent, int currCost,int cost,int plithos) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->operation = operation;
    newNode->parent = parent;
    newNode->cost = cost;
    newNode->currCost = currCost;
    newNode->plithos = plithos;
    return newNode;
}

// ���������� ��� �������
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

// ������� �� � ������ ����� ����
bool isEmpty(Stack* stack) {
    return (stack->top == NULL);
}

// �������� ������ ���� ������
void push(Stack* stack, Node* data) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

// �������� ������ ��� ��� ������
Node* pop(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    StackNode* topNode = stack->top;
    Node* data = topNode->data;
    stack->top = stack->top->next;
    free(topNode);
    return data;
}

// ���������� ��� �����
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

// ������� �� � ���� ����� ����
bool isEmptyQ(Queue* q) {
    return (q->front == NULL);
}

// �������� ������ ���� ����
void enqueue(Queue* q, Node* data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = data;
    newNode->next = NULL;
    if (isEmptyQ(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

// �������� ������ ��� ��� ����
Node* dequeue(Queue* q) {
    if (isEmptyQ(q)) {
        return NULL;
    }
    QueueNode* frontNode = q->front;
    Node* data = frontNode->data;
    q->front = q->front->next;
    free(frontNode);
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return data;
}


void printCost_Plithos(Node* node, FILE* outputFile, int tarValue) {
    if (node->value == tarValue) {
        fprintf(outputFile, "%d, %d       // N: πλήθος εντολών, C: συνολικό κόστος\n",
                node->plithos, node->cost);
    }
}


// �������� ��� ���������� �������
void printOperations(Node* node, FILE* outputFile) {
    if (node == NULL) {
        return;
    }
    Stack* stepsStack = createStack(); // Stack to store all the steps
    while (node != NULL) {
        push(stepsStack, node);
        node = node->parent;
    }

    while (!isEmpty(stepsStack)) {
        Node* currentNode = pop(stepsStack);
        char* operationString = NULL;
        switch (currentNode->operation) {
            case INCREASE_BY_ONE:
                operationString = "Increase";
                break;
            case DECREASE_BY_ONE:
                operationString = "Decrease";
                break;
            case MULTIPLY:
                operationString = "Multiply";
                break;
            case DIVIDE:
                operationString = "Divide";
                break;
            case SQUARE:
                operationString = "Square";
                break;
            case SQUARE_ROOT:
                operationString = "Square Root";
                break;
        }




        if(operationString!=NULL){

           fprintf(outputFile, "%s %d %d\n", operationString, currentNode->prevValue, currentNode->currCost);

        }
    }

    free(stepsStack);
}

// �������� ��� ���������� BFS
void bfs(int startValue, int targetValue) {

    Queue* queue = createQueue();
    Node* startNode = createNode(startValue, -1, NULL, 0,0,0); // �� ������ ������ ����� 0
    enqueue(queue, startNode);
    int previousValue;
    FILE* outputFile = fopen("solution.txt", "w");
    if (outputFile == NULL) {
        perror("Unable to create or open the output file");
        return;
    }

    while (!isEmptyQ(queue)) {
        Node* currentNode = dequeue(queue);
        if (currentNode->value == targetValue) {
            printf("Found a solution with cost %d:\n", currentNode->cost);
            printCost_Plithos(currentNode,outputFile,targetValue);
            printOperations(currentNode, outputFile);
            return;
        }


        int newValue;
        int currentCost;
        int cost;
        int plithos=0;

        int pValue = currentNode->value;
        newValue = currentNode->value + 1;
        if (newValue <= targetValue) {
            currentCost = INCREMENT_COST;
            plithos = currentNode->plithos + 1;
            cost = currentNode->cost + INCREMENT_COST;
            Node* newNode = createNode(newValue, INCREASE_BY_ONE, currentNode, currentCost,cost,plithos);
            newNode->prevValue = pValue;
            enqueue(queue, newNode);
        }

        previousValue = currentNode->value;
        newValue = currentNode->value - 1;
        if (newValue >= 0) {
            currentCost = DECREMENT_COST;
            plithos = currentNode->plithos + 1;
            cost = currentNode->cost + DECREMENT_COST;
            Node* newNode = createNode(newValue, DECREASE_BY_ONE, currentNode, currentCost,cost,plithos);
            newNode->prevValue = previousValue;
            enqueue(queue, newNode);
        }
        // �������� ������ �� �����������
        previousValue = currentNode->value;
        newValue = currentNode->value * 2;
        if (newValue <= targetValue) {
            currentCost = DOUBLE_COST(previousValue);
            plithos = currentNode->plithos + 1;
            cost = currentNode->cost + DOUBLE_COST(previousValue);
            Node* newNode = createNode(newValue, MULTIPLY, currentNode, currentCost,cost,plithos);
            newNode->prevValue = previousValue;
            enqueue(queue, newNode);
        }
        // �������� ������ �� ��������������
        if (currentNode->value % 2 == 0) {
            previousValue = currentNode->value;
            newValue = currentNode->value / 2;
            currentCost = HALVE_COST(previousValue);
            plithos = currentNode->plithos + 1;
            cost = currentNode->cost + HALVE_COST(previousValue);
            Node* newNode = createNode(newValue, DIVIDE, currentNode,currentCost, cost,plithos);
            newNode->prevValue = previousValue;
            enqueue(queue, newNode);
        }
        // �������� ������ �� ���������
        previousValue = currentNode->value;
        newValue = currentNode->value * currentNode->value;
        if (newValue <= targetValue) {
            currentCost = SQUARE_COST(previousValue);
            plithos = currentNode->plithos + 1;
            cost = currentNode->cost + SQUARE_COST(previousValue);
            Node* newNode = createNode(newValue, SQUARE, currentNode,currentCost, cost,plithos);
            newNode->prevValue = previousValue;
            enqueue(queue, newNode);
        }
        // �������� ������ �� ����������� ����
        int sqrtValue = (int)sqrt(currentNode->value);
        if (sqrtValue * sqrtValue == currentNode->value && currentNode->value > 1) {
            previousValue = currentNode->value;
            newValue = sqrtValue;
            currentCost = SQRT_COST(previousValue);
            plithos = currentNode->plithos + 1;
            cost = currentNode->cost + SQRT_COST(previousValue);
            Node* newNode = createNode(newValue, SQUARE_ROOT, currentNode,currentCost, cost,plithos);
            newNode->prevValue = previousValue;
            enqueue(queue, newNode);
        }

    }

    printf("No solution found.\n");
    fclose(outputFile);
}


// ���������� ������ ��� ��� ����
void destroyQueue(Queue* q) {
    while (!isEmptyQ(q)) {
        dequeue(q); // Remove all nodes from the queue
    }
    free(q); // Free the queue structure
}

int main() {
    int startValue, targetValue;

    // �������� ��� ������� ��� ������� ����� ��� �� ������
    printf("Enter the start integer value: ");
    scanf("%d", &startValue);
    printf("Enter the final integer value: ");
    scanf("%d", &targetValue);

    // ������� ��� ��������� BFS ��� ��� ��������� ��� ��������� ���������� �������
    bfs(startValue, targetValue);



    return 0;
}
