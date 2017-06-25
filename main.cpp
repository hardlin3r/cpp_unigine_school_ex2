#include <iostream>
#include <regex>
#include <string>
#include <stdexcept>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;
using std::regex;
using std::string;
using std::vector;

struct args {
  int N;
  string in;
  string out;
};

regex r(R"(\b(http:\/\/|https:\/\/)([a-zA-Z0-9.-]+)(\/[a-zA-Z0-9.,\/+_]*)?)");

args parseCmdLineParams(int argc, char** argv) {
  args a;
  a.N = 0;
  if(!(argc == 3 || argc == 5)) {
    throw invalid_argument("Invalid arguments\nUsage: main [-n NNN] in.txt out.txt");
  }
  if(argc == 5 && strcmp(argv[1], "-n") != 0) {
    throw invalid_argument("Invalid arguments\nUsage: main [-n NNN] in.txt out.txt");
  }
  if(argc == 3) {
    a.in = string(argv[1]);
    a.out = string(argv[2]);
  } else if(argc == 5) {
    a.N = atoi(argv[2]);
    a.in = string(argv[3]);
    a.out = string(argv[4]);
  }
  return a;
}

void parseInputFileAndOutputResults(args &a) {
  
}

int main(int argc, char** argv) {
  try {
    args a = parseCmdLineParams(argc, argv);
    parseInputFileAndOutputResults(a);
    cout << a.N << a.in << a.out << endl;
  } catch(exception& e) {
    cerr << e.what() << endl;
    return -1;
  } catch(...) {
    cerr << "Unknown error" << endl;
    return -1;
  }
  return 0;
}
