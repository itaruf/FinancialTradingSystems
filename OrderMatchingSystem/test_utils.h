#pragma once

#include "matching_engine.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> run(std::vector<std::string> const& input)
{
    std::vector<std::string> result;

    MatchingEngine matching_engine;
    for (int i = 0; i < input.size(); ++i)
        matching_engine.process_order(input[i]);

    auto& order_books = matching_engine.get_order_books();
    std::vector<std::string> order_books_symbols; // We'll store the symbols to order

    // Extract the unordered keys
    for (auto it = order_books.begin(); it != order_books.end(); ++it)
        order_books_symbols.emplace_back(it->first);

    std::sort(order_books_symbols.begin(), order_books_symbols.end()); // Sort them alphabetically

    // List trades chronologically
    for (const std::string& symbol : order_books_symbols) // Iterate through the sorted order books
    {
        for (const Trade& trade : order_books[symbol].get_trades()) // Iterate through the trades of each order books
        {
            std::string trade_log = order_books[symbol].build_trade_log(trade);
            // std::cout << trade_log << std::endl;
            result.emplace_back(trade_log);
        }
    }

    // List bid and ask price levels for each symbol
    for (const std::string& symbol : order_books_symbols) // Iterate through the sorted order books
    {
        result.emplace_back("===" + symbol + "===");
        // std::cout << "===" + symbol + "===" << std::endl;
        std::vector<std::string> ask_bid_levels = order_books[symbol].build_ask_bid_levels();
        for (const std::string& ask_bid_level : ask_bid_levels)
            result.emplace_back(ask_bid_level);
    }

    return result;
}

// Simple assert functions
void requireEqual(size_t actual, size_t expected, const std::string& message) {
    if (actual != expected) {
        std::cerr << "Require failed: " << message << std::endl;
    }
}

void checkEqual(const std::string& actual, const std::string& expected, const std::string& message) {
    if (actual != expected) {
        std::cerr << "Check failed: " << message << std::endl;
    }
}

// Test scenario macro
#define TEST_SCENARIO(description, inputs, expectedResults) \
    void test_##description() { \
        std::cout << "Running " << #description << std::endl; \
        auto input = std::vector<std::string> inputs; \
        auto result = run(input); \
        requireEqual(result.size(), expectedResults.size(), "Size mismatch in " #description); \
        for (size_t i = 0; i < result.size(); ++i) { \
            checkEqual(result[i], expectedResults[i], "Value mismatch in " #description " at index " + std::to_string(i)); \
        } \
    }