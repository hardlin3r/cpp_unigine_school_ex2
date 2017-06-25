#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::ifstream;
using std::invalid_argument;
using std::make_pair;
using std::map;
using std::ofstream;
using std::pair;
using std::regex;
using std::regex_search;
using std::smatch;
using std::sort;
using std::sregex_iterator;
using std::string;
using std::stringstream;
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

string make_output(int urls, int N, vector<pair<int, string>>& d, vector<pair<int, string>>& p) {
  stringstream ss;
  sort(begin(d), end(d), [](pair<int, string>& x, pair<int, string> &y) {
      if(x.first > y.first) {
        return true;
      } else if(x.first == y.first) {
        return x.second < y.second;
      } else {
        return false;
      }
    });
  sort(begin(p), end(p), [](pair<int, string>& x, pair<int, string> &y) {
      if(x.first > y.first) {
        return true;
      } else if(x.first == y.first) {
        return x.second < y.second;
      } else {
        return false;
      }
    });

  ss << "total urls " << urls << ", domains " << d.size()
     << ", paths " << p.size() << endl;
  ss << endl;
  ss << "top domains" << endl;
  if(N == 0) {
    for(auto& i: d) {
      ss << i.first << " " << i.second << endl;
    }
    ss << endl << "top paths" << endl;
    for(auto& i: p) {
      ss << i.first << " " << i.second << endl;
    }
  } else {
    int ND = std::min((unsigned long)N, d.size());
    int NP = std::min((unsigned long)N, p.size());
    for(auto i = 0; i < ND; i++) {
      ss << d[i].first << " " << d[i].second << endl;
    }
    ss << endl << "top paths" << endl;
    for(auto i = 0; i < NP; i++) {
      ss << p[i].first << " " << p[i].second << endl;
    }
  }
  return ss.str();
}
void parseInputFileAndOutputResults(args &a) {
  map<string, int> domains;
  map<string, int> paths;
  int urls = 0;
  vector<pair<int, string>> domains_v;
  vector<pair<int, string>> paths_v;
  ifstream in(a.in);
  ofstream o(a.out);
  stringstream buffer;
  buffer << in.rdbuf();
  string s = buffer.str();
  smatch sm;
  for(sregex_iterator it(s.begin(), s.end(), r), it_end; it != it_end; it++) {
    sm = *it;
    domains[sm[2]]++;
    paths[sm[3]]++;
    urls++;
  }
  paths["/"] += paths["\0"];
  paths.erase("\0");
  if(paths["/"] == 0) {
    paths.erase("/");
  }
  for(auto sss: domains) {
    domains_v.push_back(make_pair(sss.second, sss.first));
  }
  for(auto sss: paths) {
    paths_v.push_back(make_pair(sss.second, sss.first));
  }
  o << make_output(urls, a.N, domains_v, paths_v) << endl;
}

int main(int argc, char** argv) {
  try {
    args a = parseCmdLineParams(argc, argv);
    parseInputFileAndOutputResults(a);
  } catch(exception& e) {
    cerr << e.what() << endl;
    return -1;
  } catch(...) {
    cerr << "Unknown error" << endl;
    return -1;
  }
  return 0;
}
