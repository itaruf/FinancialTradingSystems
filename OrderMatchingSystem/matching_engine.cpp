#include "matching_engine.h"

void MatchingEngine::process_order(const std::string& order_str)
{
    // /!\ sensitive to changes in data format
    std::vector<std::string> order_data = StringHelper::parse_string(order_str, ','); // Process the string input
    std::string order_operation = order_data[0]; // Command is the first element

    // Dirty way ?
    if (order_operation == "INSERT")
    {
        std::string order_symbol = order_data[2]; // Get the symbol

        if (!m_order_books.contains(order_symbol)) // We create a new entry if the symbol doesn't exist yet
            m_order_books[order_symbol] = OrderBook(order_symbol);

        Order order = build_order(order_data); // Create the new order with the data provided and insert it into the corresponding symbol
        m_order_books[order_symbol].insert_order(order);

        m_order_books[order_symbol].match_orders(order); // Search for matches once a new order is received
    }

    else if (order_operation == "PULL")
    {
        // iterate through all CLOBs (both buy and sell sides) to find the order to pull
        for (auto& [symbol, order_book] : m_order_books)
        {
            if (order_book.pull_order(std::stoi(order_data[1]))) // If found, since it's unique, stop searching
                break;
        }
    }

    else if (order_operation == "AMEND")
    {
        // iterate through all CLOBs (both buy and sell sides) to find the order to amend
        for (auto& [symbol, order_book] : m_order_books)
        {
            if (order_book.amend_order
            (
                std::stoi(order_data[1]),
                std::stod(order_data[2]),
                std::stoi(order_data[3]))
                )
                break;
        }
    }
}

Order MatchingEngine::build_order(const std::vector<std::string>& order_data) const
{
    Order order
    {
        std::stoi(order_data[1]),
        StringHelper::string_to_enum(order_data[3]),
        std::stod(order_data[4]),
        std::stoi(order_data[5])
    };

    return order;
}