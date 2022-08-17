#include <iostream>
#include <cmath> // log2(),
#include <cstdlib> // rand(), srand()
#include <ctime> // clock(), CLOCKS_PER_SEC,time()
#include <assert.h>// assert()

using namespace std;


class HeapSrtCls
{
  private:
    int m_Average_Case_Data[1001];
    int m_Best_Case_Data[1001];
    int m_Worst_Case_Data[1001];
    double m_Average_Case_Time_Data[1001];
    double m_Best_Case_Time_Data[1001];
    double m_Worst_Case_Time_Data[1001];


  public:
    int HeapSort(int arr[], int n); //sorts arr[] using heapsort returning the number of comparisons made
    int MaxHeapify(int arr[], int pos, int n); //changes arr[] to have the max heap property for arr[pos...n] returning the number of comparisons made
    int MakeMaxHeap(int arr[], int n); //changes arr[] to have the max heap property for the entire array returning the number of comparisons made
    bool isMaxHeap(int arr[],int start,int stop); //returns false if any element in arr[start...stop] does not have the max heap property, returns true if every element in arr[start...stop] has the max heap property
    void set_Average_Case(int index, int comparisons);//puts data into the corresponding array at index
    void set_Best_Case(int index, int comparisons);//puts data into the corresponding array at index
    void set_Worst_Case(int index, int comparisons);//puts data into the corresponding array at index
    void set_Average_Case_Time(int index, double time);//puts data into the corresponding array at index
    void set_Best_Case_Time(int index, double time);//puts data into the corresponding array at index
    void set_Worst_Case_Time(int index, double time);//puts data into the corresponding array at index
    void print_data(); //outputs the six data arrays for the object and outputs the theoretical data aswell

};

class InsertionSrtCls
{
  private:
    int m_Average_Case_Data[1001];
    int m_Best_Case_Data[1001];
    int m_Worst_Case_Data[1001];
    double m_Average_Case_Time_Data[1001];
    double m_Best_Case_Time_Data[1001];
    double m_Worst_Case_Time_Data[1001];

  public:
    int InsertionSort(int arr[], int n); //sorts arr[] using insertion sort returning the number of comparisons made
    void set_Average_Case(int index, int comparisons);//puts data into the corresponding array at index
    void set_Best_Case(int index, int comparisons);//puts data into the corresponding array at index
    void set_Worst_Case(int index, int comparisons);//puts data into the corresponding array at index
    void set_Average_Case_Time(int index, double time);//puts data into the corresponding array at index
    void set_Best_Case_Time(int index, double time);//puts data into the corresponding array at index
    void set_Worst_Case_Time(int index, double time);//puts data into the corresponding array at index
    void print_data(); //outputs the six data arrays for the object and outputs the theoretical data aswell
};

void reverseArray(int arr[], int n) // reverses arr[]
{
  int temp;
  int start = 0;
  int end = 0;

  if(n > 0)
  {
    end = n - 1;
    start = 0;
  }
  while(start < end)
  {
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    start++;
    end--;
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

void printArray(double arr[], int n) // outputs arr[]
{
    cout<<endl<<"[ ";
    for(int i = 0; i < n; i++)
    {
        cout<<arr[i]<<", ";
    }
    cout<<"]"<<endl;

    return;
}

bool isSorted(int arr[], int start, int stop) // returns false if arr[start...stop] is not sorted, returns true if arr[start...stop] is sorted
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

int main()
{
  const int MAX_NUM_ELEMENTS = 1000;

  HeapSrtCls Heap;
  InsertionSrtCls Insert;

  int * arrayH;
  int * arrayI;
  int temp;
  double time_taken;

  srand(time(NULL));
  for(int i = 0; i <= MAX_NUM_ELEMENTS; i++)
  {
    arrayH = new int[i];
    arrayI = new int[i];

    for(int j = 0; j < i; j++) // this loop randomly generates an int for each index, but each array gets the same int for the same index
    {
      temp = rand() % MAX_NUM_ELEMENTS + 1;
      arrayH[j] = temp;
      arrayI[j] = temp;
    }

    cout<<endl<<endl<<"Average Case: "<<endl;
    cout<<endl<<"Heap Sort: size "<<i<<endl;
    printArray(arrayH,i);

    time_taken = clock();
    Heap.set_Average_Case(i,Heap.HeapSort(arrayH, i));
    time_taken = ((clock() - time_taken) / CLOCKS_PER_SEC);
    Heap.set_Average_Case_Time(i,time_taken);
    assert(isSorted(arrayH,0,i));//postcondition
    printArray(arrayH, i);
    cout<<endl<<"sorting took "<<time_taken<<"seconds"<<endl;

    cout<<endl<<"InsertionSort: size "<<i<<endl;
    printArray(arrayI, i);

    assert(isSorted(arrayI,0,0)); //precondition
    time_taken = clock();
    Insert.set_Average_Case(i, Insert.InsertionSort(arrayI, i));
    time_taken = ((clock() - time_taken) / CLOCKS_PER_SEC);
    Insert.set_Average_Case_Time(i,time_taken);
    assert(isSorted(arrayI,0,i)); //postcondition
    printArray(arrayI, i);
    cout<<endl<<"sorting took "<<time_taken<<"seconds"<<endl;

    reverseArray(arrayH, i);
    cout<<endl<<endl<<"Best Case: "<<endl;
    cout<<endl<<"Heap Sort: size "<<i<<endl;
    printArray(arrayH,i);

    time_taken = clock();
    Heap.set_Best_Case(i,Heap.HeapSort(arrayH, i));
    time_taken = ((clock() - time_taken) / CLOCKS_PER_SEC);
    Heap.set_Best_Case_Time(i,time_taken);
    assert(isSorted(arrayH,0,i));//postcondition
    printArray(arrayH, i);
    cout<<endl<<"sorting took "<<time_taken<<"seconds"<<endl;

    cout<<endl<<"InsertionSort: size "<<i<<endl;
    printArray(arrayI, i);

    assert(isSorted(arrayI,0,0));//precondition
    time_taken = clock();
    Insert.set_Best_Case(i, Insert.InsertionSort(arrayI, i));
    time_taken = ((clock() - time_taken) / CLOCKS_PER_SEC);
    Insert.set_Best_Case_Time(i,time_taken);
    assert(isSorted(arrayI,0,i));//postcondition
    printArray(arrayI, i);
    cout<<endl<<"sorting took "<<time_taken<<"seconds"<<endl;

    reverseArray(arrayI, i);
    cout<<endl<<endl<<"Worst Case: "<<endl;
    cout<<endl<<"Heap Sort: size "<<i<<endl;
    printArray(arrayH,i);

    time_taken = clock();
    Heap.set_Worst_Case(i,Heap.HeapSort(arrayH, i));
    time_taken = ((clock() - time_taken) / CLOCKS_PER_SEC);
    Heap.set_Worst_Case_Time(i,time_taken);
    assert(isSorted(arrayH,0,i));//postcondition
    printArray(arrayH, i);
    cout<<endl<<"sorting took "<<time_taken<<"seconds"<<endl;

    cout<<endl<<"InsertionSort: size "<<i<<endl;
    printArray(arrayI, i);

    assert(isSorted(arrayI,0,0));//precondition
    time_taken = clock();
    Insert.set_Worst_Case(i, Insert.InsertionSort(arrayI, i));
    time_taken = ((clock() - time_taken) / CLOCKS_PER_SEC);
    Insert.set_Worst_Case_Time(i,time_taken);
    assert(isSorted(arrayI,0,i));//postcondition
    printArray(arrayI, i);
    cout<<endl<<"sorting took "<<time_taken<<"seconds"<<endl;

    cout<<endl<<endl;
    delete [] arrayH;
    delete [] arrayI;
  }
  Heap.print_data();
  Insert.print_data();
  cout<<"ending program"<<endl;
  return 0;
}

int HeapSrtCls::HeapSort(int arr[], int n)
{
  int num_comparisons = 0;
  int temp;

  num_comparisons += MakeMaxHeap(arr,n);
  assert(isMaxHeap(arr,0,n));

  for(int i = n-1; i>0; i--)
  {
    assert(isMaxHeap(arr,0,i+1));
    assert(isSorted(arr,i,n));
    temp = arr[0];
    arr[0] = arr[i];
    arr[i] = temp;
    num_comparisons ++;
    assert(isMaxHeap(arr,1,i));
    num_comparisons += MaxHeapify(arr,0,i);
    assert(isMaxHeap(arr,0,i));
    assert(isSorted(arr,i-1,n));
  }
  num_comparisons ++;
  return num_comparisons;
}

int HeapSrtCls::MaxHeapify(int arr[], int pos, int n)
{
  int num_comparisons = 0;
  int temp;
  int index = 2 * pos + 1;

  assert(isMaxHeap(arr,pos+1,n));

  if(index >= n)
    return 1;
  num_comparisons ++;

  if(index < n - 1)
  {
    if(arr[index] < arr[index+1])
      index ++;
    num_comparisons ++;
  }
  num_comparisons ++;

  if(arr[index] > arr[pos])
  {
    temp = arr[index];
    arr[index] = arr[pos];
    arr[pos] = temp;
    num_comparisons += MaxHeapify(arr, index, n);
  }
  assert(isMaxHeap(arr,pos,n));
  num_comparisons ++;
  return num_comparisons;
}

int HeapSrtCls::MakeMaxHeap(int arr[], int n)
{
  int num_comparisons = 0;
  for(int i = floor(n/2); i>=0; i--)
  {
    num_comparisons ++;
    assert(isMaxHeap(arr,i+1,n));
    num_comparisons += MaxHeapify(arr,i,n);
    assert(isMaxHeap(arr,i,n));
  }
  num_comparisons ++;
  return num_comparisons;
}

int InsertionSrtCls::InsertionSort(int arr[], int n)
{
  int num_comparisons = 0;
  int temp,j;
  for(int i=1; i < n; i++)
  {
    assert(isSorted(arr,0,i-1));
    j = i;
    num_comparisons ++;
    while(j > 0 and arr[j] < arr[j-1])
    {
      num_comparisons += 2;
      temp = arr[j];
      arr[j] = arr[j-1];
      arr[j-1] = temp;
      j -= 1;
    }
    if(j-1 < 0)
      num_comparisons ++;
    else
      num_comparisons += 2;
    assert(isSorted(arr,0,i));
  }
  num_comparisons ++;

  return num_comparisons;
}

bool HeapSrtCls::isMaxHeap(int arr[],int start,int stop)
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

void HeapSrtCls::set_Average_Case(int index, int comparisons)
{
  m_Average_Case_Data[index] = comparisons;
  return;
}

void HeapSrtCls::set_Best_Case(int index, int comparisons)
{
  m_Best_Case_Data[index] = comparisons;
  return;
}

void HeapSrtCls::set_Worst_Case(int index, int comparisons)
{
  m_Worst_Case_Data[index] = comparisons;
  return;
}

void InsertionSrtCls::set_Average_Case(int index, int comparisons)
{
  m_Average_Case_Data[index] = comparisons;
  return;
}

void InsertionSrtCls::set_Best_Case(int index, int comparisons)
{
  m_Best_Case_Data[index] = comparisons;
  return;
}

void InsertionSrtCls::set_Worst_Case(int index, int comparisons)
{
  m_Worst_Case_Data[index] = comparisons;
  return;
}

void HeapSrtCls::set_Average_Case_Time(int index, double time)
{
  m_Average_Case_Time_Data[index] = time;
  return;
}

void HeapSrtCls::set_Best_Case_Time(int index, double time)
{
  m_Best_Case_Time_Data[index] = time;
  return;
}

void HeapSrtCls::set_Worst_Case_Time(int index, double time)
{
  m_Worst_Case_Time_Data[index] = time;
  return;
}

void InsertionSrtCls::set_Average_Case_Time(int index, double time)
{
  m_Average_Case_Time_Data[index] = time;
  return;
}

void InsertionSrtCls::set_Best_Case_Time(int index, double time)
{
  m_Best_Case_Time_Data[index] = time;
  return;
}

void InsertionSrtCls::set_Worst_Case_Time(int index, double time)
{
  m_Worst_Case_Time_Data[index] = time;
  return;
}

void HeapSrtCls::print_data()
{
  cout<<endl<<"Heap Sort: Average Case Data "<<endl;
  printArray(m_Average_Case_Data, 1001);

  cout<<endl<<"Heap Sort: Best Case Data "<<endl;
  printArray(m_Best_Case_Data, 1001);

  cout<<endl<<"Heap Sort: Worst Case Data "<<endl;
  printArray(m_Worst_Case_Data, 1001);

  cout<<endl<<"vs. Heap Sort: theoretical complexity"<<endl<<"[";
  for(int i=0; i <= 1000; i++)
  {
    if(i==0)
      cout<<0<<", ";
    else
      cout<<(i*log2(i))<<", ";
  }
  cout<<"]"<<endl;

  cout<<endl<<"Heap Sort: Average Case Time Data "<<endl;
  printArray(m_Average_Case_Time_Data, 1001);

  cout<<endl<<"Heap Sort: Best Case Time Data "<<endl;
  printArray(m_Best_Case_Time_Data, 1001);

  cout<<endl<<"Heap Sort: Worst Case Time Data "<<endl;
  printArray(m_Worst_Case_Time_Data, 1001);
}

void InsertionSrtCls::print_data()
{
  cout<<endl<<"Insertion Sort: Average Case Data "<<endl;
  printArray(m_Average_Case_Data, 1001);

  cout<<endl<<"vs. Insertion Sort: Theoretical Average Case Complexity"<<endl<<"[";
  for(int i=0; i <= 1000; i++)
  {
    cout<<(i*i)<<", ";
  }
  cout<<"]"<<endl;

  cout<<endl<<"Insertion Sort: Best Case Data "<<endl;
  printArray(m_Best_Case_Data, 1001);

  cout<<endl<<"vs. Insertion Sort: Theoretical Best Case Complexity"<<endl<<"[";
  for(int i=0; i <= 1000; i++)
  {
    cout<<i<<", ";
  }
  cout<<"]"<<endl;

  cout<<endl<<"Insertion Sort: Worst Case Data "<<endl;
  printArray(m_Worst_Case_Data, 1001);

  cout<<endl<<"vs. Insertion Sort: Theoretical Worst Case Complexity"<<endl<<"[";
  for(int i=0; i <= 1000; i++)
  {
    cout<<(i*i)<<", ";
  }
  cout<<"]"<<endl;

  cout<<endl<<"Insertion Sort: Average Case Time Data "<<endl;
  printArray(m_Average_Case_Time_Data, 1001);

  cout<<endl<<"Insertion Sort: Best Case Time Data "<<endl;
  printArray(m_Best_Case_Time_Data, 1001);

  cout<<endl<<"Insertion Sort: Worst Case Time Data "<<endl;
  printArray(m_Worst_Case_Time_Data, 1001);
}
