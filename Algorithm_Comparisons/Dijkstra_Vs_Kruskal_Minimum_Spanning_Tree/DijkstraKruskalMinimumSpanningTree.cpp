#include <iostream>
#include <cmath> // floor()
#include <cstdlib>// rand(), srand()
#include <ctime>// clock(), CLOCKS_PER_SEC, time()
#include <assert.h> // assert()
#include <climits> // INT_MAX

using namespace std;

class KruskalCls
{
  private:
    int m_Data_Edge_Cost[(20*(20-1)/2)-(20-1)+1];
    float m_Data_Average_Routing[(20*(20-1)/2)-(20-1)+1];
  public:
    void setUnion(int arr[], int a, int b);// used with find to eliminate loops
    int find(int arr[], int x); // used with setUnion to eliminate loops
    void HeapSort(int arr[], int n); // sorts arr[]
    void MaxHeapify(int arr[], int pos, int n); //changes arr[] to have the max heap property for arr[pos...n]
    void MakeMaxHeap(int arr[], int n); // makes arr[] a maxheap
    void KruskalMST(int matrix[][20], int Vertices); // finds a MST for matrix
    bool isSorted(int arr[], int start, int stop); // returns false if arr[start...stop] is not sorted, returns true if arr[start...stop] is sorted
    bool isMaxHeap(int arr[], int start, int stop); // returns false if arr[start..stop] does not have the max heap property, returns true if arr[start...stop] has the max heap property
    void printData(); // outputs m_Data_Edge_Cost and m_Data_Average_Routing
};

class DijkstraCls
{
  private:
    int m_Data_Edge_Cost[(20*(20-1)/2)-(20-1)+1];
    float m_Data_Average_Routing[(20*(20-1)/2)-(20-1)+1];
  public:
    void DijkstraMST(int matrix[][20], int Vertices); // finds a MST for matrix
    void printData(); // outputs m_Data_Edge_Cost and m_Data_Average_Routing
};

void clearMatrix(int matrix[20][20]) // sets all the values of a [20][20] int matrix to 0
{
  for(int i = 0; i < 20; i++)
  {
    for(int j = 0; j < 20; j++)
    {
      matrix[i][j] = 0;
    }
  }
  return;
}

void printArray(int arr[], int n) // outputs arr[]
{
    cout<<endl<<"[ ";
    for(int i = 0; i < n; i++)
    {
      cout<<arr[i]<<", ";
    }
    cout<<"]"<<endl;

    return;
}

void printArray2D(int arr[][20], int h, int w) // prints 2D array of width [1001] // set the width = MAX_NUM_ELEMENTS
{
  for(int i = 0; i < h ; i++)
  {
    cout<<endl<<" [ ";
    for(int j = 0; j < w; j++)
      cout<<arr[i][j]<<", ";
    cout<<" ]"<<endl;
  }
  return;
}

int totalWeight(int matrix[][20], int V) // returns the sum of the weights of all edges in a undirected matrix
{
  int CostofEdges = 0;
  for(int i = 0; i < V; i++)
  {
    for(int j = i + 1; j < V; j++)
    {
      if (matrix[i][j] > 0)
        CostofEdges += matrix[i][j];
    }
  }
  return CostofEdges;
}

float averageRoutingDistance(int matrix[][20], int start, int V)//calculates the average routing distance from start
{
  float TotalRoutingDistance = 0;
  int totalVerticesReached = 0;
  int startNode = start; // whatever startNode is set to is the node that the average routing distance from a single source to all other nodes will be calculated from
  int nodeLvls[V-1][20];
  int visited[V] = {0};
  for(int i = 0; i < V-1;i++)
  {
    for(int j = 0; j < V; j++)
      nodeLvls[i][j] = startNode;
  }
  for(int i = 0; i < V; i++)
  {
    if(matrix[startNode][i] > 0)
    {
      nodeLvls[0][totalVerticesReached] = i;
      totalVerticesReached ++;
    }
  }
  visited[startNode] = 1;
  for(int i = 0; i < V-1; i++)
  {
    totalVerticesReached = 0;
    for(int j = 0; j < V; j++)
    {
      if(nodeLvls[i][j] != startNode)
      {
        for(int z = 0; z < V; z++)
        {
          if (matrix[nodeLvls[i][j]][z] > 0 && !(visited[z]))
          {
            nodeLvls[i+1][totalVerticesReached] = z;
            totalVerticesReached ++;
          }
        }
        visited[nodeLvls[i][j]] = 1;
      }
    }
  }
  for(int i =0; i < V-1; i++)
  {
    for(int j = 0; j < V; j++)
    {
      if(nodeLvls[i][j] != startNode)
        TotalRoutingDistance += i + 1;
    }
  }
  return (TotalRoutingDistance/(V-1));
}

bool edgesMatch(int matrix[][20], int V, int edges) // outputs whether a undirected matrix contains edges amount of edges
{
  int edgeCount = 0;
  for(int i = 0; i < V; i++)
    for(int j = i+1; j < V; j++)
      if(matrix[i][j] > 0)
        edgeCount ++;
  return (edgeCount==edges);
}


int main()
{
  const int NUM_OF_VERTICES = 20; //number of vertices in the generated graphs, the entire program is designed around 20 Vertices
  const int MAX_EDGES = NUM_OF_VERTICES * (NUM_OF_VERTICES - 1) / 2; // maximum edges for a 20 vertice graph
  const int MAX_WEIGHT = 35; // the maximum weight for a given edge

  KruskalCls Krsk;//used to access the methods from the KruskalCls
  DijkstraCls Djk;//used to access the methods from the DijkstraCls

  float p; // edge density p = 2 * Edges / (NUM_OF_VERTICES * (NUM_OF_VERTICES - 1)), p = 0 gives NUM_OF_VERTICES - 1 edges since that is the minimum for a connected graph
  int edges; // p * NUM_OF_VERTICES * (NUM_OF_VERTICES - 1) / 2 = edges // the edges left to add to the graph after NUM_OF_VERTICES - 1 edges are added

  int GraphKruskal[NUM_OF_VERTICES][NUM_OF_VERTICES]; //edge matrix; if value = 0 there is no edge, if value >= 0 then the edge has the weight of that value
  int GraphDijkstra[NUM_OF_VERTICES][NUM_OF_VERTICES]; // these matrices are indirect and have no self edges, and always connected and therefor have a minimum of NUM_OF_VERTICES - 1
  int numOfEdgesLeft;//used to generate the graphs
  int tempRandom;//used to generate the graphs

  srand(time(NULL));

  for(edges = 0; edges <= MAX_EDGES - NUM_OF_VERTICES + 1; edges++)
  {
    clearMatrix(GraphKruskal);
    clearMatrix(GraphDijkstra);
    numOfEdgesLeft = edges;
    p = 2*(static_cast<float>(edges) + NUM_OF_VERTICES - 1) / (NUM_OF_VERTICES*(NUM_OF_VERTICES-1));

    do // generating random indirect simple connected graph
    {

      for(int i = 0; i < NUM_OF_VERTICES; i++)
      {
        for(int j = i; j < NUM_OF_VERTICES; j++)
        {
          if(j!=i)
          {
            if(i == j - 1)
            {
              tempRandom = rand() % MAX_WEIGHT + 1;
              GraphKruskal[i][j] = tempRandom;
              GraphKruskal[j][i] = tempRandom;
              GraphDijkstra[i][j] = tempRandom;
              GraphDijkstra[j][i] = tempRandom;
            }
            else if(numOfEdgesLeft > 0)
            {
              tempRandom = rand() % 20;
              if(tempRandom <= 5 && GraphKruskal[i][j] == 0)
              {
                numOfEdgesLeft --;
                tempRandom = rand() % MAX_WEIGHT + 1;
                GraphKruskal[i][j] = tempRandom;
                GraphKruskal[j][i] = tempRandom;
                GraphDijkstra[i][j] = tempRandom;
                GraphDijkstra[j][i] = tempRandom;
              }
            }
          }
        }
      }
    } while (numOfEdgesLeft > 0);// end while
    //Outputting
    cout<<endl<<"Running MST algorithms for Graphs of edge density p: "<<p<<endl;
    cout<<"Edges: "<<edges + 19<<" / "<<MAX_EDGES<<endl;
    cout<<"Kruskal: "<<endl;
    Krsk.KruskalMST(GraphKruskal, NUM_OF_VERTICES);
    cout<<endl<<"Dijkstra: ";
    Djk.DijkstraMST(GraphDijkstra, NUM_OF_VERTICES);
  }
  Krsk.printData();
  Djk.printData();

  return 0;
}

void KruskalCls::KruskalMST(int matrix[][20], int Vertices)
{
  static int func_calls = 0;
  int edges = 0;
  int edgesInGraph = 0;
  int edgeRank[Vertices*(Vertices-1)/2];
  int parent[Vertices];
  int MST[Vertices][20] = {0};

  for(int i = 0; i < (Vertices*(Vertices-1)/2); i++) // setting edgeRank values above the weights of the graph
  {
    edgeRank[i] = 150;
  }

  for(int i = 0; i < Vertices; i++) // gathers all undirected edges in matrix and puts it into edgeRank
  {
    parent[i] = i;
    for(int j = i+1; j < Vertices; j++)
    {
      if((i <20 && j <20) && matrix[i][j] > 0)
      {
        edgeRank[edgesInGraph] = matrix[i][j];
        edgesInGraph ++;
      }
    }
  }
  HeapSort(edgeRank, (Vertices*(Vertices-1)/2)); // sorts edgeRank
  assert(isSorted(edgeRank,0,(Vertices*(Vertices-1)/2)));
  edgesInGraph = 0;

  while(edges < Vertices - 1) // based on the lowest values in edgeRank, adds edges to MST if it does not create a loop
  {
    assert(edges < Vertices - 1);
    for(int i = 0; i < Vertices; i++)
    {
      for(int j = i+1; j < Vertices; j++)
      {
        if(matrix[i][j] == edgeRank[edgesInGraph])
        {
          edgesInGraph++;
          if(find(parent,i) != find(parent,j) && edges < Vertices - 1)
          {
            edges++;
            setUnion(parent,i,j);
            MST[i][j] = matrix[i][j]; // Making the MST
            MST[j][i] = MST[i][j];
          }
        }
      }
    }
    assert(edges <= Vertices - 1);
  }
  m_Data_Edge_Cost[func_calls] = totalWeight(MST,Vertices);
  m_Data_Average_Routing[func_calls] = averageRoutingDistance(MST,0,Vertices);

  //Outputing Data
  cout<<endl<<"Kruskal's MST resulted in a Total edge cost: "<<m_Data_Edge_Cost[func_calls]
  <<", and average routing distance from vertex 0: "<<m_Data_Average_Routing[func_calls]<<endl;

  //PostCondition
  assert(edgesMatch(MST,Vertices,Vertices-1));

  func_calls ++;
  return;
}

void DijkstraCls::DijkstraMST(int matrix[][20], int Vertices)
{
  static int func_calls = 0;
  int edges = 0;
  int W[Vertices];
  int parent[Vertices];
  int visited[Vertices];
  int MST[Vertices][20] = {0};
  int sortHelp;
  int target;

  for(int i = 0; i < Vertices; i++)
  {
    W[i] = INT_MAX;
    visited[i] = 0;
    parent[i] = i;
  }
  W[0] = 0;
  while(edges != Vertices-1)
  {
    assert(edges < Vertices - 1);
    sortHelp = INT_MAX;
    for(int i = 0; i < Vertices; i++) // find the lowest weighted node
    {
      if(W[i] < sortHelp && !(visited[i]))
      {
        sortHelp = W[i];
        target = i;
      }
    }
    visited[target] = 1;

    for(int i = 0; i < Vertices; i++) // lowers any nodes possible and sets target as the parent if this happens
    {
      if(!(visited[i]))
        if(matrix[target][i] < W[i] && (matrix[target][i] != 0))
        {
          W[i] = matrix[target][i];
          parent[i] = target;
        }
    }
    edges ++;
    assert(edges <= Vertices - 1);
  }

  for(int i = 0; i < Vertices; i++) // based on parent[] generated in the above loop, generates the MST
  {
    if(i != parent[i])
    {
      MST[parent[i]][i] = W[i];
      MST[i][parent[i]] = W[i];
    }
  }
  m_Data_Edge_Cost[func_calls] = totalWeight(MST,Vertices);
  m_Data_Average_Routing[func_calls] = averageRoutingDistance(MST,0,Vertices);

  cout<<endl<<endl<<"Dijkstra's MST resulted in a Total edge cost: "<<m_Data_Edge_Cost[func_calls]
  <<", and average routing distance from vertex 0: "<<m_Data_Average_Routing[func_calls]<<endl;

  //PostCondition
  assert(edgesMatch(MST,Vertices,Vertices-1));

  func_calls ++;
  return;
}

int KruskalCls::find(int arr[], int x)
{
  while(arr[x] != x)
    x = arr[x];
  return x;
}

void KruskalCls::setUnion(int arr[], int a, int b)
{
  arr[find(arr,a)] = find(arr,b);
  return;
}

bool KruskalCls::isMaxHeap(int arr[],int start,int stop)
{
    for(int i = start; i <= stop; i++)
    {
        if(2*i + 1 < stop)
          if(arr[i] < arr[2*i + 1])
          {
            return false;
          }
        if(2*i + 2 < stop)
          if(arr[i] < arr[2*i + 2])
          {
            return false;
          }
    }
    return true;
}

bool KruskalCls::isSorted(int arr[], int start, int stop) // returns false if arr[start...stop] is not sorted, returns true if arr[start...stop] is sorted
{
  for(int i = start; i < stop; i++)
  {
    if(i+1 < stop)
    {
      if(arr[i] > arr[i+1])
      {
        return false;
      }
    }
  }
  return true;
}

void KruskalCls::HeapSort(int arr[], int n)
{
  int temp;

  MakeMaxHeap(arr,n);
  assert(isMaxHeap(arr,0,n));

  for(int i = n-1; i>0; i--)
  {
    assert(isMaxHeap(arr,0,i+1));
    assert(isSorted(arr,i,n));
    temp = arr[0];
    arr[0] = arr[i];
    arr[i] = temp;
    assert(isMaxHeap(arr,1,i));
    MaxHeapify(arr,0,i);
    assert(isMaxHeap(arr,0,i));
    assert(isSorted(arr,i-1,n));
  }
  return;
}

void KruskalCls::MaxHeapify(int arr[], int pos, int n)
{
  int temp;
  int index = 2 * pos + 1;

  assert(isMaxHeap(arr,pos+1,n));

  if(index >= n)
    return;

  if(index < n - 1)
  {
    if(arr[index] < arr[index+1])
      index ++;
  }

  if(arr[index] > arr[pos])
  {
    temp = arr[index];
    arr[index] = arr[pos];
    arr[pos] = temp;
    MaxHeapify(arr, index, n);
  }
  assert(isMaxHeap(arr,pos,n));
  return;
}

void KruskalCls::MakeMaxHeap(int arr[], int n)
{
  for(int i = floor(n/2); i>=0; i--)
  {
    assert(isMaxHeap(arr,i+1,n));
    MaxHeapify(arr,i,n);
    assert(isMaxHeap(arr,i,n));
  }
  return;
}

void KruskalCls::printData()
{
  cout<<endl<<"Kruskal Data"<<endl<<"Edges cost"<<endl;
  cout<<endl<<"[ ";
  for(int i = 0; i < (20*(20-1)/2)-(20-1)+1; i++)
  {
    cout<<m_Data_Edge_Cost[i]<<", ";
  }
  cout<<"]"<<endl;

  cout<<endl<<"Average routing distance"<<endl;
  cout<<endl<<"[ ";
  for(int i = 0; i < (20*(20-1)/2)-(20-1)+1; i++)
  {
    cout<<m_Data_Average_Routing[i]<<", ";
  }
  cout<<"]"<<endl;

  return;
}

void DijkstraCls::printData()
{
  cout<<endl<<"Dijkstra Data"<<endl<<"Edges cost"<<endl;
  cout<<endl<<"[ ";
  for(int i = 0; i < (20*(20-1)/2)-(20-1)+1; i++)
  {
    cout<<m_Data_Edge_Cost[i]<<", ";
  }
  cout<<"]"<<endl;

  cout<<endl<<"Average routing distance"<<endl;
  cout<<endl<<"[ ";
  for(int i = 0; i < (20*(20-1)/2)-(20-1)+1; i++)
  {
    cout<<m_Data_Average_Routing[i]<<", ";
  }
  cout<<"]"<<endl;

  return;
}
