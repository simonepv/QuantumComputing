#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<unistd.h>
#include<vector>

//this algorithm searchs an element in a database
// to use this algorithm with different number of elements: change dim and n

using namespace std;

int main()
  {
  //first of all let's create an array of 100 elemnts randomely filled
  
  const int dim = 8;
  float storage[dim];
  vector<float> pCorrect_storage;
  vector<float> pWrong_storage; 

  cout << "table with the numbers in the storage" << endl;
  
  for(int i=0; i<dim; i++)
    {
    storage[i]=random()%10000;
    cout << storage[i] << " ";
    }
  cout << endl;
  
  //to solve this problem we need a number n of qubits: dim < 2^n
  //for dim = 8, n=3
  
  const int n = 3;
  
  cout << "number of qubits n=" << n << endl;
  cout << endl;

  //choose one of the random numbers to create the matrix O
  cout << "choose one of the numbers in the table" << endl;
  float chosen; 
  cin >> chosen;

  int i_chosen;
  
  for (int i=0; i<dim; i++)
    {
    if (storage[i]==chosen)
      {
      i_chosen=i;
      cout << "i_chosen:" << i_chosen << " | it is the " << i_chosen+1 << "th in the storag vector"<<endl;
      }
    }
  
  //prepare psi
  //float psi0[dim] = {1, 1, 1, 1, 1, 1, 1, 1};
  float psi0[dim];
  for (int p=0; p<dim; p++)
  {
    psi0[p]=1;
  }
  
  cout << "psi0: ";
  //divide psi by sqrt(2^n)
  for (int i=0; i<dim; i++)
  {
  psi0[i]=psi0[i]/sqrt(pow(2, n));
  cout << psi0[i] << " ";
  }
  cout << endl;

  
  //create now the matrix |psi_0><psi_0|
  
  float psi0_psi0[dim][dim];

  cout << "Matrix psi0_psi0" << endl;
  
  for (int i=0; i<dim; i++)
    {
    for (int j=0; j<dim; j++)
      {
      psi0_psi0[i][j] = 1./pow(2, n);
      cout << psi0_psi0[i][j] << " ";
      }
    cout << endl;
    }
  cout << endl;
  
  //create now the identity matrix
  //create now the matrix |psi_0><psi_0|
  
  float identity[dim][dim];

  cout << "Matrix identity" << endl;
  
  for (int i=0; i<dim; i++)
    {
    for (int j=0; j<dim; j++)
      {
      if (j==i)
        {
        identity[i][j] = 1.;
        }
      else identity[i][j] = 0.;
      
      cout << identity[i][j] << " ";
      }
    cout << endl;
    }
  cout << endl;
  
  //create the first operator M in the G operator G = M*O
  float M[dim][dim];
  
  cout << "Matrix M=2*psi0_psi0-1 in the G operator G=M*O" << endl;
  
  for (int i=0; i<dim; i++)
    {
    for (int j=0; j<dim; j++)
      {
      M[i][j] = 2.*psi0_psi0[i][j] - identity[i][j];
      
      cout << M[i][j] << " ";
      }
    cout << endl;
    }
  cout << endl;

  //create the oracle operator O
  float O[dim][dim];
  
  cout << "Matrix O in the G operator G=M*O" << endl;
  
  for (int i=0; i<dim; i++)
    {
    for (int j=0; j<dim; j++)
      {
      if (i==j)
        {
        O[i][j] = 1.;      
        }
      else
        {
        O[i][j] = 0;
        }  
      }
    }
  
  O[i_chosen][i_chosen] = -1.;

  for (int i=0; i<dim; i++)
    {
    for (int j=0; j<dim; j++)
      {
      cout << O[i][j] << " ";
      }
    cout << endl;
    }
  cout << endl;
  
  //product G=M*O
  cout << "Matrix G=M*O" << endl;
  
  float G[dim][dim];

  for (int i=0; i<dim; i++)
  {
    for(int j=0; j<dim; j++)
    {
    G[i][j]=0;
    }
  }
  
  for (int i=0; i<dim; i++)
  {
    for(int j=0; j<dim; j++)
    {
      for(int k=0; k<dim; k++)
      {
      G[i][j]= G[i][j] + M[i][k]*O[k][j];
      } 
      cout << G[i][j] << " ";  
    }
    cout << endl;
  }
  cout << endl;
  
  //do G*psi0
  
  cout << "psi1 = G*psi0" << endl;
  float psi1[dim];
  
  for (int i=0; i<dim; i++)
  {
    float a=0;
    for(int k=0; k<dim; k++)
    {
      a = a + G[i][k]*psi0[k];
    } 
    psi1[i] = a;
    
    cout << psi1[i] << " ";  
  }
  cout << endl;
  
  //repeat the product
  cout << "How many times do you want to make the product? "; 
  int cont_loop; 
  cin >> cont_loop;
  cout << endl;
  
  for (int loops=1; loops<cont_loop; loops++)
  { 
    cout  << "(G^"<<loops+1<<"):" << endl;
    
    float psiLoops[dim];
    float GLoops[dim][dim];
    //the first GLoops is G
    for (int i=0; i<dim; i++)
    {
      for (int j=0; j<dim; j++)
      {
      GLoops[i][j]=G[i][j];
      //cout << "GLoops["<<i<<"]["<<j<<"]="<<GLoops[i][j];
      }
      //cout << endl;
    }
    //cout << endl; 

    
    //calculate the matrix G^loops
    for (int cont=1; cont<loops; cont++)
    {
      float temp[dim];
      for (int tr=0; tr<dim; tr++)
      {
      temp[tr]=0;
      }
      
      for (int i=0; i<dim; i++)
      {
        float a=0;
        for(int j=0; j<dim; j++)
        {
          a = 0;
          for(int k=0; k<dim; k++)
          {
          
          a = a + GLoops[i][k]*G[k][j];
          //cout << "a:" << a << " i:" << i << " j:" << j << " k:" << k << endl;
          } 
          
          temp[j]=a;
          
        }
      //cout << endl;
        for (int r=0; r<dim; r++)
        {
        GLoops[i][r] = temp[r];
        }
      
      //cout << "GLoops["<<i<<"]["<<j<<"]="<<GLoops[i][j] << endl;
      }
    }
    
    //cout << endl;
    
    for (int i=0; i<dim; i++)
    {
      for (int j=0; j<dim; j++)
      {
      cout << GLoops[i][j] << " ";
      }
    cout << endl;
    }
    cout << endl;
    
    //I calculate now the final vector
    cout << "psi" << loops+1 << "=(G^"<<loops+1<<")*psi0:" << endl;
    
    float psiloops[dim];
    
    for (int i=0; i<dim; i++)
    {
      double a=0;
      for(int k=0; k<dim; k++)
      {
        //cout << "a:" << a << " GLoopsik:" << GLoops[i][k] << " psi0[k]" << psi0[k] << endl;
        a = a + GLoops[i][k]*psi0[k];
        //cout << "a:" << a << " GLoopsik:" << GLoops[i][k] << " psi0[k]" << psi0[k] << " dopo" << endl;
      } 
      psiloops[i] = a;
      
      cout << psiloops[i] << " ";  
    }
    cout << endl;
    
    //correct and wrong probability
    double PWrong = 0;
    double PCorrect = 0;

    for (int i=0; i<dim; i++)
    { 
      if (i==i_chosen) continue;
      PWrong = PWrong + psiloops[i]*psiloops[i];
    }
    
    PCorrect = psiloops[i_chosen]*psiloops[i_chosen];
    cout << "wrong probability:" << PWrong << " correct probability:" << PCorrect << endl;

    pCorrect_storage.push_back(PCorrect); 
    pWrong_storage.push_back(PWrong); 
  }

  //show the optimal times of computations
  int optimal_count = 1;

  for (int m=0; m < pCorrect_storage.size(); m++)
  {
    cout << "pcorrect[" << m+2 << "]:" << pCorrect_storage[m] <<" pwrong[" << m+2 << "]:" << pWrong_storage[m] << endl; 
  }

  int m=1;
  while (pCorrect_storage[m-1] < pCorrect_storage[m])
  {
    int temp = m+2;
    //cout << pCorrect_storage[m-1] << " " << pCorrect_storage[m] << endl;
    cout << "Optimal number of computations:" << temp << endl;
    m++;
  }

  return 0;
  }
