#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

int main(int argc, char* argv[]) {
  int n;
  std::string x;
  std::cout << "PLease input size of puzzle:";
  std::cin >> n;
  std::cout << "Input File name:";
  std::cin >> x;
  int i = 10;
  std::ofstream SaveFile(x);
  SaveFile << n << std::endl;
  for(int i = 0; i < n * n * n * n; i++){
  	SaveFile << ".";
  }
  SaveFile.close();
  return 0;
}
