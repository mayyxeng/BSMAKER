#include <regex>
#include <fstream>
#include <iostream>
#include <vector>
#include "Overlay.h"
#include "Parser.h"
#include <memory>
int main(int argc, char **argv){

  ConnectionBox cb();
  ComputeUnit CU();
  SwitchBox CB();
  Block block();
  Overlay ov();
  std::vector <std::unique_ptr<int>> vec;
  for (int i = 0; i < 10; i ++) {
    vec.push_back(std::make_unique<int>(i));
  }
  for(int i = 0; i < 10; i++) {
    std::cout << *vec[i] << " ";
  }
  std::cout << std::endl;
  ParseFiles("../myblif");
}
