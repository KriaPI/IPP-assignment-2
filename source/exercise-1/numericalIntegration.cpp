#include "integralApproximator.hpp"
#include <format>
#include <iostream>
#include <span>
#include <chrono>

void usage() {
    std::cout 
    << "Usage: program T S\n"
    << "\tT: total number of threads working on the task.\n"
    << "\tS: number of trapezes used to approximate the integral.\n\n";
}

int parseArgument(char* argument, int argument_index) {
    try {
        return std::stoi(argument);
    } catch (const std::exception&) {
        std::cout << std::format("Could not parse argument {} assigned input '{}'.\n", argument_index, argument);
        usage();
        std::exit(0);
    }
}

int main (int argc, char** argv) {
    // TODO: implement command line argument parsing.

    int threadCount {1};
    int trapezes {1};
    
    std::span<char*, std::dynamic_extent> arguments (argv + 1, argc - 1);
    if (arguments.size() != 2) {
        usage();
        return 0;
    }
    threadCount = {parseArgument(arguments[0], 0)};
    trapezes = parseArgument(arguments[1], 1);
    auto integrand = [] (numerical x) {return 4 / (1 + x * x);};

    auto start {std::chrono::system_clock::now()};
    [[maybe_unused]] auto result = approximateIntegralThreaded(integrand, IntegralBounds{.lower = 0, .upper = 1}, trapezes, threadCount);
    std::chrono::duration<double> duration {std::chrono::system_clock::now() - start};
    std::cout << std::format("Duration: {} seconds\n", duration.count());
    //std::cout << std::format("Result: {}\n", result);

    return 0;
}