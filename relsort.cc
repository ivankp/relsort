#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <regex>

using std::cout, std::cerr, std::endl;

int main(int argc, char* argv[]) {
  if (argc<3 || 5<argc) {
    cout << "usage: " << argv[0]
         << " file1 file2 [regex1 [regex2]]\n";
    return 1;
  }

  const std::regex  re1(argc>3 ? argv[3] : "^.*");
  const std::regex& re2 = argc>4 ? std::regex(argv[4]) : re1;

  std::unordered_map<std::string,size_t> order;
  { std::ifstream f(argv[1]);
    size_t i = 0;
    std::smatch m;
    for (std::string line; std::getline(f,line); ) {
      if (std::regex_search(line, m, re1)) {
        auto str = m.str(0);
        if (!order.try_emplace( std::move(str), ++i ).second) {
          cerr << "repeated \"" << str << "\"\n";
        }
      }
    }
  }

  std::vector<std::tuple<size_t,std::string>> lines;
  { std::ifstream f(argv[2]);
    std::smatch m;
    for (std::string line; std::getline(f,line); ) {
      if (std::regex_search(line, m, re2)) {
        auto it = order.find(m.str(0));
        const size_t i = it!=order.end() ? it->second : 0;
        lines.emplace_back( i, std::move(line) );
      }
    }
  }

  std::stable_sort( lines.begin(), lines.end(),
    [](const auto& a, const auto& b) -> bool {
      return std::get<0>(a) < std::get<0>(b);
    }
  );

  for (const auto& line : lines)
    cout << std::get<1>(line) << '\n';
}
