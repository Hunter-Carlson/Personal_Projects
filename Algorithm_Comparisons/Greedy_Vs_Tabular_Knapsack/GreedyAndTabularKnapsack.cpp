#include <iostream>
#include <cmath> // floor()
#include <cstdlib>// rand(), srand()
#include <ctime>// clock(), CLOCKS_PER_SEC, time()
#include <assert.h> // assert()

using namespace std;

class GreedyCls
{
  private:
    int m_comparisonData[11][1001];
  public:
    int valPerWeightSort(int vals[], int weights[], int n); //sorts vals[] and weights[] by vals / weights using heapsort returning the number of comparisons made
    int MaxHeapify(int vals[], int weights[], int pos, int n); //changes vals[] and weights[] to have the max heap property based on val/weight for vals[pos...n] returning the number of comparisons made
    int MakeMaxHeap(int vals[], int weights[], int n); //changes vals[] and weights[] to have the max heap property based on val/weight for the entire array returning the number of comparisons made
    int knapGreedy(int vals[], int weights[], int n, int w); // calculates a somewhat optimal value for knapsack problem and returns it
    void printData(); // outputs m_comparisonData and the Greedy theoretical comparisons
};

class TabularCls
{
  private:
    int m_comparisonData[11][1001];
  public:
    int knapTabular(int vals[], int weights[], int n, int w); // calculates the optimal value for the knapsack problem and returns it
    void printData(); // outputs m_comparisonData and the Tabular theoretical comparisons
};

int max(int num1, int num2) // return the > of the two parameters
{
  if(num1 > num2)
    return num1;
  return num2;
}

int reverseArray(int arr[], int n) // reverses arr[]
{
  int temp;
  int start = 0;
  int end = 0;
  int num_comparisons = 0;

  if(n > 0)
  {
    end = n - 1;
    start = 0;
  }
  num_comparisons ++;

  while(start < end)
  {
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    start++;
    end--;
    num_comparisons++;
  }

  return num_comparisons + 1;
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

void printArray2D(double arr[][1001], int rowHeadMult, int h, int w) // prints 2D array of width [1001] // set the width = MAX_NUM_ELEMENTS
{
  for(int i = 0; i < h ; i++)
  {
    cout<<endl<<rowHeadMult * i<<" [ ";
    for(int j = 0; j < w; j++)
      cout<<arr[i][j]<<", ";
    cout<<" ]"<<endl;
  }
  return;
}

void printArray2D(int arr[][1001], int rowHeadMult, int h, int w) // prints 2D array of width [1001] // set the width = MAX_NUM_ELEMENTS
{
  for(int i = 0; i < h ; i++)
  {
    cout<<endl<<rowHeadMult * i<<" [ ";
    for(int j = 0; j < w; j++)
      cout<<arr[i][j]<<", ";
    cout<<" ]"<<endl;
  }
  return;
}

bool isSorted(int vals[], int weights[], int start, int stop) // returns false if arr[start...stop] is not sorted, returns true if arr[start...stop] is sorted
{
  for(int i = start; i < stop; i++)
  {
    if(i+1 < stop)
    {
      if(static_cast<float>(vals[i]) / weights[i] > static_cast<float>(vals[i+1]) / weights[i+1])
      {
        return false;
      }
    }
  }
  return true;
}

bool isMaxHeap(int vals[], int weights[], int start,int stop)
{
    for(int i = start; i <= stop; i++)
    {
        if(2*i + 1 < stop)
          if(static_cast<float>(vals[i]) / weights[i] < static_cast<float>(vals[2*i + 1]) / weights[2*i + 1] )
          {
            return false;
          }
        if(2*i + 2 < stop)
          if(static_cast<float>(vals[i]) / weights[i] < static_cast<float>(vals[2*i + 2]) / weights[2*i + 2])
          {
            return false;
          }
    }
    return true;
}

int sumTotal(int vals[], int stop) // returns the sum of the array elements from 0 to stop
{
  int sum = 0;
  for(int i = 0; i < stop; i++)
  {
    sum += vals[i];
  }
  return sum;
}

int finalIndex(int weights[], int n, int w) // returns the index after the last index that will be added in Greedy approach
{
  int i = 0;

  while(i < n && weights[i] <= w)
  {
    w -= weights[i];
    i++;
  }
  return i;
}

int main()
{
  const int MAX_NUM_ELEMENTS = 1000; // the number of items being tested up to 0 <= n <= MAX_NUM_ELEMENTS
  const int MAX_WEIGHT_TESTED = 1000;
  const int MAX_ITEM_VALUE = 5000;
  const int MAX_ITEM_WEIGHT = 100;
  const int NUM_OF_DIFF_WEIGHT_LOOPS = 10; // the amount of different max weights, w, being tested per n
  // the main computations will be done MAX_NUM_ELEMENTS * NUM_OF_DIFF_WEIGHT_LOOPS times

  GreedyCls Greed;
  TabularCls Tab;

  int * valsG;
  int * weightsG;
  int * valsT;
  int * weightsT;
  int temp,temp2;
  double time_taken;
  int numTimesGreedyNotOptimal = 0;

  double DataTabular[NUM_OF_DIFF_WEIGHT_LOOPS+1][MAX_NUM_ELEMENTS+1]; // [w][n]
  double DataGreedy[NUM_OF_DIFF_WEIGHT_LOOPS+1][MAX_NUM_ELEMENTS+1];

  srand(time(NULL));

  for(int i = 0; i <= MAX_NUM_ELEMENTS; i++) // main loop iterates for n
  {
    valsG = new int[i];
    weightsG = new int[i];
    valsT = new int[i];
    weightsT = new int[i];

    for(int j = 0; j < i; j++) // randomly sets the value arrays
    {
      temp = rand() % MAX_ITEM_VALUE + 1;
      valsG[j] = temp;
      valsT[j] = temp;
    }

    for(int j = 0; j < i; j++) // randomly sets the weight arrays
    {
      temp = rand() % MAX_ITEM_WEIGHT + 1;
      weightsG[j] = temp;
      weightsT[j] = temp;
    }

    for(int j = 0; j <= MAX_WEIGHT_TESTED; j += (MAX_WEIGHT_TESTED / NUM_OF_DIFF_WEIGHT_LOOPS)) // main computational loop iterates for W
    {
      time_taken = clock();
      temp = Greed.knapGreedy(valsG, weightsG, i,j);
      time_taken = ((clock() - time_taken) / CLOCKS_PER_SEC);
      DataGreedy[j/(MAX_WEIGHT_TESTED / NUM_OF_DIFF_WEIGHT_LOOPS)][i] = time_taken;
      cout<<endl<<endl<<"KnapSack # of items "<<i<<", Weight allowed "<<j;
      cout<<endl<<"Value of Greedy: "<<temp<<" time taken "<<time_taken<<" seconds";

      time_taken = clock();
      temp2 = Tab.knapTabular(valsT, weightsT, i,j);
      time_taken = ((clock() - time_taken) / CLOCKS_PER_SEC);
      DataTabular[j/(MAX_WEIGHT_TESTED / NUM_OF_DIFF_WEIGHT_LOOPS)][i] = time_taken;
      cout<<endl<<"Value of Tabular: "<<temp2<<" time taken "<<time_taken<<" seconds";
      if(temp2 > temp)
      {
        cout<<endl<<"Greedy approach did not produce the optimal value";
        numTimesGreedyNotOptimal ++;
      }
    }

    delete [] valsG;
    delete [] weightsG;
    delete [] valsT;
    delete [] weightsT;
  }

  cout<<endl<<"Greedy Data for time taken"<<endl;
  printArray2D(DataGreedy,(MAX_WEIGHT_TESTED / NUM_OF_DIFF_WEIGHT_LOOPS),NUM_OF_DIFF_WEIGHT_LOOPS+1, MAX_NUM_ELEMENTS + 1);
  cout<<endl<<"Tabular Data for time taken"<<endl;
  printArray2D(DataTabular,(MAX_WEIGHT_TESTED / NUM_OF_DIFF_WEIGHT_LOOPS),NUM_OF_DIFF_WEIGHT_LOOPS+1, MAX_NUM_ELEMENTS + 1);

  cout<<endl<<"Greedy Data for number of comparisons"<<endl;
  Greed.printData();
  cout<<endl<<"Tabular Data for time taken"<<endl;
  Tab.printData();

  cout<<endl<<endl<<"Greedy produced a non optimal value "<<numTimesGreedyNotOptimal<<" out of "<<((MAX_NUM_ELEMENTS + 1 )* NUM_OF_DIFF_WEIGHT_LOOPS)
  <<" uses"<<endl;
  cout<<endl<<"END PROGRAM"<<endl;
  return 0;
}

int GreedyCls::knapGreedy(int vals[], int weights[], int n, int w)
{
  int Value = 0;
  int num_comparisons = 0;
  int w_orig = w;

  num_comparisons += valPerWeightSort(vals, weights, n);
  num_comparisons += reverseArray(vals,n);
  num_comparisons += reverseArray(weights,n);
  for(int i = 0; i < n; i++)
  {
    assert(Value == sumTotal(vals,i) || w - sumTotal(weights,i) <= 0);
    if(weights[i] <= w)
    {
      Value += vals[i];
      w -= weights[i];
    }
    num_comparisons += 2;
    assert(Value == sumTotal(vals,i+1) || w - sumTotal(weights,i+1) <= 0);
  }
  num_comparisons ++;
  m_comparisonData[w/100][n] = num_comparisons; // 100 = MAX_WEIGHT_TESTED / NUM_OF_DIFF_WEIGHT_LOOPS

  assert(Value >= sumTotal(vals,finalIndex(weights,n,w_orig))); // postcondition
  return Value;
}

int GreedyCls::valPerWeightSort(int vals[], int weights[], int n)
{
  int num_comparisons = 0;
  int temp;

  num_comparisons += MakeMaxHeap(vals, weights, n);
  assert(isMaxHeap(vals,weights,0,n));

  for(int i = n-1; i>0; i--)
  {
    assert(isMaxHeap(vals,weights,0,i+1));
    assert(isSorted(vals,weights,i,n));
    temp = vals[0];
    vals[0] = vals[i];
    vals[i] = temp;
    temp = weights[0];
    weights[0] = weights[i];
    weights[i] = temp;
    num_comparisons ++;
    assert(isMaxHeap(vals,weights,1,i));
    num_comparisons += MaxHeapify(vals,weights,0,i);
    assert(isMaxHeap(vals,weights,0,i));
    assert(isSorted(vals,weights,i-1,n));
  }
  assert(isSorted(vals,weights,0,n)); // postcondition
  return num_comparisons + 1;
}

int GreedyCls::MaxHeapify(int vals[], int weights[], int pos, int n)
{
  int num_comparisons = 0;
  int temp;
  int index = 2 * pos + 1;

  assert(isMaxHeap(vals,weights,pos+1,n));

  if(index >= n)
    return 1;
  num_comparisons ++;

  if(index < n - 1)
  {
    if((static_cast<float>(vals[index]) / weights[index]) < (static_cast<float>(vals[index+1]) / weights[index+1]))
      index ++;
    num_comparisons ++;
  }
  num_comparisons ++;

  if((static_cast<float>(vals[index]) / weights[index]) > (static_cast<float>(vals[pos]) / weights[pos]))
  {
    temp = vals[index];
    vals[index] = vals[pos];
    vals[pos] = temp;
    temp = weights[index];
    weights[index] = weights[pos];
    weights[pos] = temp;
    num_comparisons += MaxHeapify(vals, weights, index, n);
  }
  assert(isMaxHeap(vals,weights,pos,n));

  return num_comparisons + 1;
}

int GreedyCls::MakeMaxHeap(int vals[], int weights[], int n)
{
  int num_comparisons = 0;
  for(int i = floor(n/2); i>=0; i--)
  {
    num_comparisons ++;
    assert(isMaxHeap(vals,weights,i+1,n));
    num_comparisons += MaxHeapify(vals, weights,i,n);
    assert(isMaxHeap(vals,weights,i,n));
  }
  num_comparisons ++;
  return num_comparisons;
}

int TabularCls::knapTabular(int vals[], int weights[], int n, int w)
{
  int Tablet[n+1][w+1];
  int num_comparisons = 0;

  for(int i = 0; i <= n; i++) // i = item, n
  {
    num_comparisons ++;
    for(int j = 0; j <= w; j++) // j = current weight limit, w
    {
      if(i > 0 && j > 0)
        assert(Tablet[i-1][j-1] == 0 || Tablet[i-1][j-1] == Tablet[i-2][j-1] || Tablet[i-1][j-1] == vals[i-2] + Tablet[i-2][j-1-weights[i-2]]);
      if(i==0 || j==0)
        Tablet[i][j] = 0;

      else if(weights[i-1] > j)
        Tablet[i][j] = Tablet[i-1][j];

      else
        Tablet[i][j] = max( vals[i-1] + Tablet[i-1][j-weights[i-1]], Tablet[i-1][j]);
      num_comparisons += 5; // 1 for max, 1 for the loop comparison, 3 for if conditions
      assert(Tablet[i][j] == 0 || Tablet[i][j] == Tablet[i-1][j] || Tablet[i][j] == vals[i-1] + Tablet[i-1][j-weights[i-1]]);
    }
    num_comparisons ++;
  }
  num_comparisons ++;
  m_comparisonData[w/100][n] = num_comparisons; // 100 = MAX_WEIGHT_TESTED / NUM_OF_DIFF_WEIGHT_LOOPS

  return Tablet[n][w];
}

void GreedyCls::printData()
{
  printArray2D(m_comparisonData,100,11,1001);
  cout<<endl<<endl<<"Theoretical Greedy Data O(nlg(n))"<<endl;
  for(int i = 0; i < 11 ; i++)
  {
    cout<<endl<<100 * i<<" [ ";
    for(int j = 0; j < 1001; j++)
    {
      if(j==0)
        cout<<0<<", ";
      cout<<(j*log2(j))<<", ";
    }
    cout<<" ]"<<endl;
  }
  return;
}

void TabularCls::printData()
{
  printArray2D(m_comparisonData,100,11,1001);
  cout<<endl<<endl<<"Theoretical Tabular Data O(n*w)"<<endl;
  for(int i = 0; i < 11 ; i++)
  {
    cout<<endl<<100 * i<<" [ ";
    for(int j = 0; j < 1001; j++)
    {
      cout<<(j*100*i)<<", ";
    }
    cout<<" ]"<<endl;
  }

  return;
}
