#pragma once

#include "order.h"
#include "trade.h"
#include "Helpers/string_helper.h"
#include "Helpers/time_helper.h"

#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_map>

class OrderBook
{
private:
    class Comparator
    {
    public:
        bool operator()(const Order& lhs, const Order& rhs) const
        {
            // 1. Pricing priority
            if (lhs.get_limit_price() != rhs.get_limit_price())
                return lhs.get_limit_price() > rhs.get_limit_price();

            // 2. If multiple orders have the same price, then time priority
            return lhs.get_timestamps() < rhs.get_timestamps(); // Orders which where inserted earlier are matched first
        }
    };

    // class BuyComparator : public Comparator
    // {
    // public:
    //     bool operator()(const Order& lhs, const Order& rhs) const 
    //     {
    //         // 1. pricing priority
    //         if (lhs.get_limit_price() != rhs.get_limit_price())
    //             return lhs.get_limit_price() > rhs.get_limit_price();

    //         // 2. if multiple orders have the same price, then time priority
    //         return lhs.get_timestamps() < rhs.get_timestamps(); // orders which where inserted earlier are matched first
    //     }
    // };

    // class SellComparator : public Comparator
    // {
    // public:
    //     bool operator()(const Order& lhs, const Order& rhs) const 
    //     {
    //         // 1. pricing priority
    //         if (lhs.get_limit_price() != rhs.get_limit_price())
    //             return lhs.get_limit_price() < rhs.get_limit_price();

    //         // 2. if multiple orders have the same price, then time priority
    //         return lhs.get_timestamps() < rhs.get_timestamps(); // orders which where inserted earlier are matched first
    //     }
    // };

    // All orders are managed and stored in either of the 2 sides (BUY or SELL)
    // Sets for :
    //          - sorting auto done using a custom function
    //          - unique objects
    std::set<Order, Comparator> buy_orders;
    std::set<Order, Comparator> sell_orders;

    // unord map for :
    //          - key : id of an order
    //          - value : iterator to have direct access to the corresponding order
    //          - faster operations time
    std::unordered_map<int, std::set<Order>::iterator> order_map;

    std::vector<Trade> m_trades; // Register the trades generated in this order book

    std::string m_symbol; // Symbol / name of this order book

public:
    // Constructors
    OrderBook() = default;
    OrderBook(std::string symbol);

    // Getters & Setters
    inline const std::vector<Trade>& get_trades() { return m_trades; }
    inline const std::string& get_symbol() const { return m_symbol; }

    // Member functions to manage those orders
    void insert_order(const Order& order); // inserting an order in either of the sides
    bool pull_order(int order_id); // removing an order
    bool amend_order(int order_id, double new_price, int new_volume); // make changes to an order
    void match_orders(Order& order);
    void generate_trade(Order& aggressive_order, const Order& passive_order);

    const Order& get_order(int order_id) const; // retrieve an order thanks to its unique ID

    // Debug functions
    template <typename T>
    void debug_display_order_side(T& orders) const;
    void debug_display_trades() const;
    void debug_display_sides() const;

    // Build results
    std::string build_trade_log(const Trade& trade) const;
    std::vector<std::string> build_ask_bid_levels();
};