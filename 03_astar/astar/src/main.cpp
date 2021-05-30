#include <stdio.h>
#include "libmodule.h"
#include <vector>


int main(int argc, char *argv[]) {
  printf("Project A_Star\n");
  printf("Hell World\n");
  printf("libmodule() = %d\n", testmodule_Test());
  std::vector<int> vec = {0,1,2,3};

  

  return 0;
}
