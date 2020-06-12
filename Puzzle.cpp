#include "Puzzle.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

void Puzzle::extract(string filename){
  string line; 
  ifstream file(filename.c_str());
  if(!file){
    cout << "File not exits!" <<endl;
    exit(0);
  }
  
  getline(file, line); 
  stringstream ss(line);
  ss >> n;
  k = n * n;

  getline(file, line);
  int value, i; 
  bool getNum = false;
  for(i = 0; i <= line.length(); i++){
    char X = line[i];
      switch(X){
        case '.':
  	  if (getNum){
	    puzzle.push_back(value);
	    getNum = false;
	  }
	  puzzle.push_back(0);
	  break;
	case ',':
	  if (getNum){
	    puzzle.push_back(value);
	    getNum = false;
	  }
      	  break;
      	default:
          if (getNum){
	    value *= 10;
	    value += X - '0';
	  } else{
	    getNum = true;
	    value = X - '0';
	  }
      }    
  }
  cout<< "Question is " << n * n << " X " << n * n <<endl;
}

void Puzzle::solve(){
  ijv vars[k * k * k + 1];
  int i = 0, j = 0, z = 1, m = 1, x = 0, y = 0, l, t;
  while (i < k){
    while (j < k){
      while (z <= k){
	ijv var;
	var.i = i;
	var.j = j;
	var.z = z;
	vars[i * k * k + j * k + z] = var;
	z++;
      }
      z = 1;
      j++;
    }
    j = 0;
    i++;
  }
  
  if (puzzle.empty()){
    cout << "empty puzzle." << endl;
    return;
  }
  
  ofstream cnf; 
  cnf.open("CNF");

#define reve "-"<<
#define O <<" "<<
#define A <<" 0"<<endl
  i = 0, j = 0, z = 1, m = 1;
  while(i < k){
    while(j < k){
      while(z <= k){
	while(m <= k){
	  if (m != z) cnf << reve a(i, j, z) O reve a(i, j, m) A;
	  m++;
	}
	m = 1;
	z++;
      }
      z = 1;
      j++;
    }
    j = 0;
    i++;
  }
 
  i = 0, j = 0, z = 1, x = 0, y = 0; 
  while (i < k){
    while (j < k){
      while (z <= k){
	while (x < k){
	  while(y < k){
	    if ((x == i || y == j || ( (x/n) == (i/n) && (y/n) == (j/n) )) && (x != i || y != j))
	      cnf << reve a(i, j, z) O reve a(x, y, z) A;
	    y++;
	  }
	  y = 0;
	  x++;
	}
	x = 0;
	z++;
      }
      z = 1;
      j++;
    }
    j = 0;
    i++;
  }

  i = 0, j = 0, z = 1;
  while (i < k){
    while (j < k){
      while (z <= k){
	cnf << a(i, j, z) << " ";
	z++;
      }
      cnf A;
      z = 1;
      j++;
    }
    j = 0;
    i++;
  }

  i = 0, j = 0; 
  while (i < k){
    while (j < k){
      if(puzzle[i + j * k] > 0)
	cnf << a(i, j, puzzle[i + j * k]) A;
      j++;
    }
    j = 0;
    i++;
  }
   
   cnf.close();
   system("minisat CNF TEMP >/dev/null 2>&1");
   cout << system("minisat -rcheck CNF TEMP") << endl;
   
   ifstream solfile; 
   solfile.open("TEMP");
   string sol;
   
   getline(solfile, sol);
   if(sol == "UNSAT"){
     cout << "No solution!" << endl;
     return;
   }

   getline(solfile, sol);
   stringstream ss(sol);
   t = 0;
   while(t < k * k * k){
     ss >> l;
     ijv var = vars[abs(l)];
     i = var.i;
     j = var.j;
     z = var.z;
     if (l > 0) {
       puzzle[i + j * k] = z;
     }
     t++;
   }
#undef reve
#undef O
#undef A   
}

void Puzzle::show(){
  
  k = n * n;
  int i, y, x, Total = 0;
  i = 0;
  while (i < puzzle.size()){
    if (to_string(puzzle[i]).length() > Total)
      Total = to_string(puzzle[i]).length();
    i++;
  }
  

  string side(n * (Total + 1) - 1, '-');
  string peak = "+";
  i = 0;
  while (i < n){
    peak += side + '+';
    i++;
  }
  
  
  y = 0, x = 0;    
  while (y < k){
    if (y % n == 0) cout << peak << endl;
    while (x < k){
      if (x % n == 0) cout<<"|";
      string spaces(Total - to_string(puzzle[y * k + x]).length(), ' ');
      cout<<(puzzle[y * k + x] == 0 ? " " : to_string(puzzle[y * k + x])) + spaces;
      if (x % n != n - 1) cout<<" ";
      x++;
    }
    cout << "|" << endl;
    x = 0;
    y++;
  }
  cout<<peak<<endl;
}

int main(int argc, char* argv[]){
  Puzzle p;
  p.extract(argv[1]);
  cout<<"Puzzle:"<<endl;
  p.show();
  p.solve();
  cout<<"Solution:"<<endl;
  p.show();
  remove("TEMP");
  remove("CNF");
  return 0;
}
