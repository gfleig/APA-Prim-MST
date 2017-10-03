#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static void printMatrix(const int size, int matrix[size][size])
{
    printf("\n");
    for (int i = 0; i < size; ++i)
    {
        for(int j = 0; j < size; ++j)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

static int getMatrixSize(FILE* file)
{
    int size;
    fscanf(file, "%d", &size);
    return size;
}

static void buildMatrix(FILE* file, const int size, int matrix[size][size])
{
    int temp_value;    

    for(int i = 0; i < size - 1; ++i)
    {
        for(int j = i + 1; j < size; ++j)
        {
            fscanf(file, "%d", &temp_value);
            matrix[j][i] = temp_value;
            matrix[i][j] = temp_value;                      
        }
    }

    for(int i = 0; i < size; ++i)
    {
        matrix[i][i] = 0;
    }    
}

static void minHeapify(int array[], int arraySize, const int index, int key[])
{
    int lowest      = index;
    int leftChild   = 2*index + 1;
    int rightChild  = 2*index + 2;

    if(leftChild < arraySize && key[array[leftChild]] < key[array[lowest]])
    {
        lowest = leftChild;        
    }
    if(rightChild < arraySize && key[array[rightChild]] < key[array[lowest]])
    {
        lowest = rightChild;
    }
    
    if(lowest != index)
    {
        int aux = array[lowest];
        array[lowest] = array[index];
        array[index] = aux;

        minHeapify(array, arraySize, lowest, key);
    }
}

static void buildMinHeap(int heap[], const int heapSize, int key[])
{
    for(int i = heapSize / 2 - 1; i >= 0; --i)
    {
        minHeapify(heap, heapSize, i, key);
    }
}

static int extractMinHeap(int heap[], int* heapSize, int key[])
{
     const int minimum = heap[0];

     heap[0] = heap[*heapSize - 1];
     (*heapSize)--;

     minHeapify(heap, *heapSize, 0, key);

     return minimum;
}

static void decreaseHeap(int heap[], int a, int dec, int heapSize, int key[])
{
    int i;
    for(i = 0; i < heapSize; ++i)
    {
        if(heap[i] == a)
        {
            break;
        }
    }

    if(key[a] < dec)
    {
        ;
    }else
    {
        key[a] = dec;
        int parent = (i - 1) / 2;

        while(i > 0 && key[heap[parent]] > key[heap[i]])
        {
            int aux = heap[i];
            heap[i] = heap[parent];
            heap[parent] = aux;
            i = parent;
            parent = (i - 1) / 2;
        }
    }
}

static int getMinKey(const int graphSize, const int key[], const int setMST[])
{
    int minimum = INT_MAX;
    int minimum_index;

    for(int v = 0; v < graphSize; ++v)
    {
        if(setMST[v] == 0 && key[v] < minimum)
        {
            minimum = key[v];
            minimum_index = v;
        }
    }    
    return minimum_index;    
}

static void printMST(const int parent[], const int graphSize, int graph[graphSize][graphSize])
{
    printf("\nParent Vertice: Key:");
    int primTotal = 0;         
    for(int i = 1; i < graphSize; ++i)
    {
        printf("\n%d\t%d\t%d", parent[i], i, graph[i][parent[i]]);
        primTotal += graph[i][parent[i]];
    }
    printf("\nPrim Total Lenght = %d\n", primTotal);
}

static void primMST(const int graphSize, int graph[graphSize][graphSize])
{
    int parent[graphSize];      
    int key[graphSize];
    int mstSet[graphSize];      //vertices in MST - boolean, 0 or 1;

    for(int i = 1; i < graphSize; ++i)
    {
        key[i] = INT_MAX; 
        mstSet[i] = 0;
    }    
    
    key[0] = 0;
    parent[0] = -1;    

    int queueSize = graphSize;
    int queue[graphSize];
    for(int i = 0; i < graphSize; ++i)
    {
        queue[i] = i;
    }

    buildMinHeap(queue, graphSize, key);

    while(queueSize > 0)
    //for(int count =  0; count < graphSize - 1; ++count)
    {       
        //int minimum_index = getMinKey(graphSize, key, mstSet);

        int minimum_index = extractMinHeap(queue, &queueSize, key);

        //mstSet[minimum_index] = 1;

        mstSet[minimum_index] = 1;

        for(int v = 0; v < graphSize; ++v)
        {            
            if( graph[minimum_index][v]
                && mstSet[v] == 0
                && graph[minimum_index][v] < key[v])
            {
                parent[v] = minimum_index;
                key[v] = graph[minimum_index][v];
                decreaseHeap(queue, v, graph[minimum_index][v], graphSize, key);
            }
        }
    }

    printMST(parent, graphSize, graph);
}

int main(int argc, char **argv)
{
    FILE* file; 
    file = fopen(argv[1], "r");

    const int size = getMatrixSize(file);  

    int graph[size][size];

    buildMatrix(file, size, graph);       

    primMST(size, graph);

    return 0;
}