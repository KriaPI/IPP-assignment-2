#include <iostream>

#include "placeHolder.hpp"

/// This file can be used to test if Clang sanitizers work as intended (ASAN,
/// LSAN, UBSAN and TSAN).

namespace badPractices {
/// @brief Test to see if LSAN (leak sanitizer) reports an error.
auto leaky() {
    auto useless{new int{4}};
    std::cout << "Cool number: " << *useless << '\n';
}

/// @brief Test to see if ASAN (address sanitizer) reports an error.
auto useAfterFree() {
    auto freeMemory{new int{4}};
    delete freeMemory;

    if (*freeMemory == 4) {
        std::cout << "Infinite memory???" << '\n';
    }
}

/// @brief Test to see if UBSAN (undefined behavior sanitizer) reports an
/// runtime error.
auto undefinedBehavior() {
    int* coolAddress{nullptr};
    if (*coolAddress) {
        std::cout << "wow, that was awesome!" << '\n';
    }
}
}  // namespace badPractices

int main() {
    std::cout << "Nothing?" << '\n';

    badPractices::leaky();
    // badPractices::useAfterFree();
    // badPractices::undefinedBehavior();
    std::exit(EXIT_SUCCESS);
}