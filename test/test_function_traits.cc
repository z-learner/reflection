#include <iostream>
#include <string>

#include "reflection/traits/function_traits.hpp"

struct Person {
  Person(const std::string& name, float height, float age)
      : fullname(name), height(height), age(age) {}

  void Introduce() const {
    std::cout << "Hi, my name is " << fullname << " and I am " << age
              << " years old." << std::endl;
  }

  std::string fullname;
  float height;
  float age;
};

int main() { using namespace function_traits; }