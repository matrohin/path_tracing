#include <CLI/CLI.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  CLI::App app{"Path Tracing Toy Project"};

  std::string name;
  app.add_option("-n", name, "Name");

  CLI11_PARSE(app, argc, argv);

  std::cout << "Hello, " << name << '\n';
  return 0;
}
