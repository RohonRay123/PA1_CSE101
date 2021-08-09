#include <iostream>
#include <vector>
#include <tuple>
#include <climits>
#include <cmath>
using namespace std;

// Name:Rohon Ray
// PID: A15380394
bool containsValue(vector<int> array, int n)
{
  for(int index1=0;index1<array.size();index1++)
  {
    if(array[index1]==n)
    {
      return true;
    }
  }
  return false;
}
vector<int> makeNumberList(int k)
{
  vector<int> numbers;
  for(int x=0;x<k;x++)
  {
    numbers.push_back(x);
  }
  return numbers;
} 
vector<vector<int> > addVectors(vector<int> a, vector<int> numberList)
{
   vector<int> numberList1=numberList;
  vector<vector<int> > vectors;
 // int count=0;
 
  for(int count=0;count<numberList.size();count++)
  {
    if(!containsValue(a,numberList[count]))
    {
       vector<int> b=a;
      b.push_back(numberList1[count]);
      vectors.push_back(b);
      //count++;
    }
  }
  return vectors;
}
vector<vector<int> > calcMatrixMinor(vector<vector<int> > A, int i, int j)
{
   vector<vector <int> > B=A;
   for(int index=0;index<A.size();index++)
   {
     // B[index].erase(B[index].begin()+j);
     B[index].at(j)=0;
   }
   vector<int> zeros(A[0].size(),0);
   //B.erase(B.begin()+i);
   B[i]=zeros;
   return B;
}
vector<int> minElement(vector<vector<int> > A)
{
  vector<int> list(3,-1);
  {
    return list;
  }
  if(A[0].size()<=0)
  {
    return list;
  }
  int min1=A[0][0];
  for(int row=0;row<A.size();row++)
  {
    for(int col=0;col<A[0].size();col++)
    {
      if(min1==0)
      {
        min1=A[row][col];
      }
      else if(A[row][col]<min1 && A[row][col]>0)
      {
        min1=A[row][col];
        list.at(0)=row;
        list.at(1)=col;
        list.at(2)=min1;
      }
    }
  }
  return list;
}
int minVectorValue(vector<int> K)
{
  int min=K[0];
  for(int x=1;x<K.size();x++)
  {
    if(min==0)
    {
      min=K[x];
    }
    else if(K[x]<min && K[x]>0)
    {
      min=K[x];
    }
  }
  return min;
}
int rowMin(vector<vector<int> > A)
{
   int sum=0;
   vector<int> zeros(A[0].size(),0);
   for(int x=0;x<A.size();x++)
   {
     if(A[x]!=zeros)
     {
       sum=sum+minVectorValue(A[x]);
     }
   }
   return sum;
}
int SNH(vector<vector<int> > A)
{
  int sum=0;
  vector<vector<int> > B=A;
  vector<vector<int> > zeroMatrix(B.size(),vector<int>(B[0].size(),0));
  while(B!=zeroMatrix)
  { 
    sum=sum+minElement(B).at(2);
    B=calcMatrixMinor(B,minElement(B).at(0),minElement(B).at(1));
  }

  return sum;
}
vector<int > SNHPathway(vector<vector<int> > A, vector<int> X)
{
   vector<vector<int> > B=A;
   for(int x=0;x<X.size();x++)
   {
     B=calcMatrixMinor(B,x,X[x]);
   }
   
  vector<vector<int> > zeroMatrix(B.size(),vector<int>(B[0].size(),0));
  while(B!=zeroMatrix)
  {
    X.push_back(minElement(B).at(1));
    B=calcMatrixMinor(B,minElement(B).at(0),minElement(B).at(1));
  }
   return X;

}
int upper_bound(vector<vector<int> > A, vector<int> X)
{
    vector<vector<int> > B=A;
    int sum1=0;
   for(int x=0;x<X.size();x++)
   {
     sum1=sum1+A[x][X[x]];
     B=calcMatrixMinor(B,x,X[x]);
   }
   //vector<vector<int> > zeroMatrix(B.size(),vector<int>(B[0].size(),0));
   sum1=sum1+SNH(B);
   return sum1;   
}
int lower_bound(vector<vector<int> > A, vector<int> X)
{
    vector<vector<int> > B=A;
    int sum1=0;
   for(int x=0;x<X.size();x++)
   {
     sum1=sum1+A[x][X[x]];
     B=calcMatrixMinor(B,x,X[x]);
   }
   //vector<vector<int> > zeroMatrix(B.size(),vector<int>(B[0].size(),0));
   sum1=sum1+rowMin(B);
   return sum1;  

}


std::tuple< vector<vector<int> >, vector<int>, int> myBranchBound(vector<vector<int> > C)
{
  tuple< vector<vector<int> >, vector<int>, int> maker;
  vector<vector<int> > matrixZeros(C.size(),vector<int>(C.size(),0));
  vector<int> zeros(C.size(),0);
  maker=make_tuple(matrixZeros, zeros, -1);
  if(C.size()<2 || C.size()>10)
  {
    cout<<"Invalid Input Size" << endl;
    return maker;
  }
  if(C[0].size()!=C.size())
  {
    cout<<"Invalid Input" << endl;
    return maker;
  }
  class nodeRecord
  {
     public:
     vector<int> tasks;
     int count;
     bool isCrossed;
     int upperbound;
     int lowerbound;
    
     nodeRecord(vector<int> tasks1,int count1)
     {
       this->tasks=tasks1;
       this->count=count1;
       isCrossed=false;
       upperbound=-1;
       lowerbound=-1;
       
     }
     void setBounds(int a,int b)
     {
       this->lowerbound=a;
       this->upperbound=b;
     }
     void setisCrossed(int lowestUpperBound)
     {
       if(lowestUpperBound<lowerbound)
       {
         isCrossed=true;
       }
     }
  };
  vector<nodeRecord* > record1;
  vector<nodeRecord* > record2;
  vector<int> listUpperBound;
  vector<int> numberList=makeNumberList(C.size());
  nodeRecord* root=new nodeRecord({},0);
  int upperbound=upper_bound(C,root->tasks);
  int lowerbound=lower_bound(C,root->tasks);
  root->setBounds(upperbound,lowerbound);
  listUpperBound.push_back(upperbound);
  nodeRecord* isOptimal;
  isOptimal=root;
  record1.push_back(root);
  record2.push_back(root);
  vector<vector<int> > listAdd=addVectors(root->tasks,numberList);
  for(int x=0;x<listAdd.size();x++)
  {
    nodeRecord* a=new nodeRecord(listAdd[x],record1.size());
    int upperbound=upper_bound(C,a->tasks);
    int lowerbound=lower_bound(C,a->tasks);
    a->setBounds(lowerbound,upperbound);
    if(upperbound < listUpperBound.at(listUpperBound.size()-1))
    {
      listUpperBound.push_back(upperbound);
      isOptimal=a;
    }
    if(lowerbound>=listUpperBound.at(listUpperBound.size()-1))
    {
      a->isCrossed=true;
    }
    record1.push_back(a);
    record2.push_back(a);
  }
  record2.erase(record2.begin());
  for(int x=0;x<record2.size();x++)
  {
    if(record2[x]->lowerbound >= listUpperBound[listUpperBound.size()-1])
    {
      record2[x]->isCrossed=true;
    }
  }
  int count=0;
  while(record2.size()!=0)
  {
     vector<nodeRecord*> record4;
     for(int x=0;x<record2.size();x++)
     {
      
      vector<int> check=record2[x]->tasks;
      if(check.size()==0)
      {
        continue;
      }
      if(record2[x]->isCrossed==false)
      {
        listAdd=addVectors(record2[x]->tasks,numberList);
        for(int i=0;i<listAdd.size();i++)
        {
          nodeRecord* p=new nodeRecord(listAdd[i],record1.size());
          int upperbound=upper_bound(C,p->tasks);
          int lowerbound=lower_bound(C,p->tasks);
          p->setBounds(lowerbound,upperbound);
          record1.push_back(p);
          record4.push_back(p);
        }
      }
    }

    record2=record4;
    int min2=listUpperBound[listUpperBound.size()-1];
    for(int x=0;x<record2.size();x++)
    {
      nodeRecord* rec=record2[x];
      if(rec->upperbound<min2)
      {
        min2=rec->upperbound;
        listUpperBound.push_back(min2);
        isOptimal=rec;
      }
    }
    for(int y=0;y<record2.size();y++)
    {
       nodeRecord* rec1=record2[y];
       if(rec1->lowerbound >= min2)
       {
         rec1->isCrossed=true;
       }
    }





  
    //count++;
    //record2.erase(record2.begin());

  }
  vector<int> tasking=SNHPathway(C,isOptimal->tasks);
  vector<vector<int> > taskInformation(C.size(),vector<int>(C[0].size(),0));
  for(int index=0;index<tasking.size();index++)
  {
     taskInformation[index][tasking[index]]=1;
  }

  tuple< vector<vector<int> > , vector<int> , int> hello;
  hello=make_tuple(taskInformation,listUpperBound,record1.size());
  return hello;

 

  
  /*
    TODO:
    
    Implement Assignment Branch and Bound function under here.
    Some Helper functions that might help you modularize the code:
        - int upper_bound(vector<vector<int>> A, vector<int> X) : calculates upper bound at node X
        - int lower_bound(vector<vector<int>> A, vector<int> X) : calculates lower bound at node X
        - int SNH(vector<vector<int>> A) : calculates Smallest Number Hueristic of a Matrix
        - int rowMin(vector<vector<int>> A) : calculates Row-Min strategy Lower bound of a Matrix
        - vector<int> minElement(vector<vector<int>> A) : calculates minimum element in a matrix with its position
        - vector<vector<int>> calcMatrixMinor(vector<vector<int>> A, int i, int j) : calculates minor of a Matrix at a given location
    Note: These functions are recommended however we won't be grading your implementations of the
          above stated functions

    Input:
    vector<vector<int>> C: (N x N) with c_ij representing the time taken by agent i to complete task j - vector<vector<int>>

    Input constraints: 2<N<10

    return:
    tuple <vector<vector<int>>, vector<int>, int> mySol: A tuple with the following elements in the exact order:
        0:  vector<vector<int>> X:  Optimal Assignment of Jobs -  vector<vector<int>>
        1:  vector<int> ub_list: List of upper bounds when they were updated(0th index should be the first upper bound calculated by SNH) - vector<int>
        2:  int node_count: Number of nodes evaluated by your branch and bound algorithm - int
    
  */
}

/////////////////////////////////////////////////////////
//////////////       QUESTION 2 HERE   //////////////////
/////////////////////////////////////////////////////////
vector<vector<int> > myDynamicProgramming(int n, int c, int V[], int W[])
{
  vector<vector<int> > matrix(n+1,vector<int> (c+1,0) );
  for(int row=0;row<matrix.size();row++)
  {
     for(int col=0;col<matrix[0].size();col++)
     {
         if(row==0)
         {
           matrix[row][col]=0;
           
         }
         else if(col>=W[row-1])
         {
           matrix[row][col]=max(matrix[row-1][col-W[row-1]]+V[row-1], matrix[row-1][col]);
         }
         else if(W[row-1]>col)
         {
           matrix[row][col]=matrix[row-1][col];
         }
     }

  }
  return matrix;

  /*
    TODO:

    Implement Knapsack Dynamic Programming function under here and return the 2-D matrix generated to solve this question.

    Input:
    int n: Number of items - int
    int c: Capacity of the Knapsack - int
    int V[]: Array of Values of each item - int[] (1-D int array)
    int W[]: Array of Weights of each item - int[] (1-D int array)

    return:
    vector<vector<int>> DP: Dynamic Programming two-dimensional matrix generated while calculating - vector<vector<int>>
  */
}

vector<int> DecToBin(int n, int number)
{
  vector<int> binary(n,0);
  int quotient=number;
  for(int x=0;x<n;x++)
  {
    int mod=quotient%2;
    binary.at(x)=mod;
    quotient=quotient/2;
    if(quotient==0)
    {
      break;
    }

  }
  return binary;
}

vector<int> myBitmask(int n, int c, int V[], int W[])
{
  
   vector<vector<int> > dynamicProgram= myDynamicProgramming(n,c,V,W);
   vector<int> bin(n,0);
   vector<int> zeros(n,0);
   int highestValue=dynamicProgram[n][c];
   int numerations=pow(2,n);
   int TotalWeight=0;
   int TotalValue=0;
  for(int x=0;x<numerations;x++)
  {
    bin=DecToBin(n,x);
    
    for(int y=0;y<n;y++)
    {
      TotalWeight=TotalWeight+bin[y]*W[y];
      TotalValue=TotalValue+bin[y]*V[y];
    }
    if(TotalValue==highestValue && TotalWeight<=c)
      {
        break;
      }
      else
      {
        TotalWeight=0;
        TotalValue=0;
        bin=zeros;
      }
  }
  return bin;
   


  /*
    TODO:

    Return the bitmask for the Knapsack Dynamic Programming problem given the 2-D matrix which you can generate 
    by calling myDynamicProgramming(n, c, V, W).

    Input:
    int n: Number of items - int
    int c: Capacity of the Knapsack - int
    int V[]: Array of Values of each item - int[] (1-D int array)
    int W[]: Array of Weights of each item - int[] (1-D int array)

    return:
    vector<int> Z: Optimal choice of items for the given constraints - vector<int>
  */
}
/*int main()
{
  //cout<<"Hello word studfgfd"<<endl;
  vector<int> record;
  int n=3;
  int c=11;
  int V[]={5,8,12};
  int W[]={4,5,10};
  record=myBitmask(n,c,V,W);
  for(int x=0;x<record.size();x++)
  {
    cout<<record[x]<<"," << endl;
  }
   // PA1 hello=new PA1; 
  return 0;
}*/
