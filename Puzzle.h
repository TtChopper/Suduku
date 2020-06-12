#include <vector>
#include <string>

class Puzzle
{
 public:
  void extract(std::string filename);
  void solve();   
  void show();

 protected:
 private:
  int n; 
  int k;
  std::vector<int> puzzle;

  struct ijv{int i; int j; int z;};
  int a(int i, int j, int z){ return i * k * k + j * k + z; }
};
