#pragma once

#include "order_book.h"
#include "Helpers/string_helper.h"

#include <unordered_map>

// Class that manages multiple central limit order books
class MatchingEngine
{
public:
    // Handle commands
    enum class Operation
    {
        INSERT,
        PULL,
        AMEND
    };

private:
    // unord map for :
    //          - order books represented by differents and uniques symbols
    //          - managing and processing large number of symbols
    //          - managing them independently from one another
    std::unordered_map<std::string, OrderBook> m_order_books;

public:
    // Constructor
    MatchingEngine() = default;

    // Getters
    std::unordered_map<std::string, OrderBook>& get_order_books() { return m_order_books; }

    // Member functions
    void process_order(const std::string& order_str);
    Order build_order(const std::vector<std::string>& order_data) const;
};