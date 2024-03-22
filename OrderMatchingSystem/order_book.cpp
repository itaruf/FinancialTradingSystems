#include "order_book.h"

OrderBook::OrderBook(std::string symbol) : m_symbol(symbol) {}

void OrderBook::insert_order(const Order& order)
{
    // Check the side in which we'll insert the order
    auto& order_set = (order.get_side() == Order::Side::BUY) ? buy_orders : sell_orders;

    auto pair = order_set.insert(order); // The new, updated order is inserted
    order_map[order.get_id()] = pair.first; // Store, update the iterator for this order

    // Debugs
    // order.debug_display_data();
    // debug_display_sides();
}

bool OrderBook::pull_order(int order_id)
{
    // std::cout << "pulled: " << order_id << std::endl;
    // Find in which side the order is
    for (auto it = buy_orders.begin(); it != buy_orders.end(); ++it)
    {
        if (it->get_id() == order_id)
        {
            buy_orders.erase(it); // Remove the order from the buy set
            order_map.erase(order_id); // Remove the entry from the general order map

            // debug_display_sides();
            return true;
        }
    }

    for (auto it = sell_orders.begin(); it != sell_orders.end(); ++it)
    {
        if (it->get_id() == order_id)
        {
            sell_orders.erase(it); // Remove the order from the sell set
            order_map.erase(order_id); // Remove the entry from the general order map

            // debug_display_sides();
            return true;
        }
    }

    return false;
}

//An amend causes the order to lose time priority in the order book, unless the only change to the order is that the volume is decreased. If the price of the order is amended, it needs to be re-evaluated for potential matches.
bool OrderBook::amend_order(int order_id, double new_price, int new_volume)
{
    auto it = order_map.find(order_id);
    if (it != order_map.end()) // The order exists
    {
        auto& it_order = it->second;
        Order order = *it_order; // We are building a new order based on the old one

        bool b_lose_time_priority = false; // true = changes are detected, false = only the volume was decreased in those changes
        bool b_check_match = false; // true = only if the price is amended

        if (new_volume == 0) // No volume left -> should be deleted
        {
            pull_order(order_id);
            return true;
        }

        if (new_price != order.get_limit_price()) // Changes detected to price
        {
            b_lose_time_priority = true;
            b_check_match = true;
            order.set_limit_price(new_price);
        }

        if (new_volume != order.get_volume()) // Changes detected to volume
        {
            if (new_volume > order.get_volume()) // Volume has been increased
                b_lose_time_priority = true;

            order.set_volume(new_volume);
        }

        if (b_lose_time_priority) // Update the timestamps if the order is marked to lose priority
            order.set_timestamps(TimeHelper::get_timestamps_in_microseconds());

        // Update the order
        pull_order(order_id);
        insert_order(order);

        if (b_check_match)
            match_orders(order);

        return true;
    }

    return false; // the order doesn't exist
}

// Order& order => aggressive order looking for a match
void OrderBook::match_orders(Order& order)
{
    // Check the opposite side of the current order
    auto& opposite_orders = (order.get_side() == Order::Side::BUY) ? sell_orders : buy_orders;

    // Separation Buy / Sell ?
    if (order.get_side() == Order::Side::BUY) // Order is a buy ..
    {
        // So we look for a sell match
        for (auto it_opposite = opposite_orders.rbegin(); it_opposite != opposite_orders.rend(); ++it_opposite)
        {
            if (order.get_limit_price() >= it_opposite->get_limit_price()) // We have a match
            {
                // std::cout << "b: " << order.get_id() << " -> " << "s: " << it_opposite->get_id() << " match" << std::endl;
                generate_trade(order, *it_opposite);
                return;
            }
        }
    }
    else // Order is a sell ..
    {
        // So we look for a buy match
        for (auto it_opposite = opposite_orders.begin(); it_opposite != opposite_orders.end(); ++it_opposite)
        {
            if (order.get_limit_price() <= it_opposite->get_limit_price()) // We have a match
            {
                // std::cout << "s: " << order.get_id() << " -> " << "b: " << it_opposite->get_id() << " - match" << std::endl;
                generate_trade(order, *it_opposite);
                return;
            }
        }
    }
}

void OrderBook::generate_trade(Order& aggressive_order, const Order& passive_order)
{
    const int trade_volume = std::min(aggressive_order.get_volume(), passive_order.get_volume()); // The volume traded is the min amount either order can buy/sell
    Trade& trade = m_trades.emplace_back(aggressive_order.get_id(), passive_order.get_id(), passive_order.get_limit_price(), trade_volume, TimeHelper::get_timestamps_in_microseconds()); // Passive_order id = matchedID

    amend_order(passive_order.get_id(), passive_order.get_limit_price(), passive_order.get_volume() - trade_volume); // Update the passive order's volume

    const int new_aggr_volume = aggressive_order.get_volume() - trade_volume; // Update the aggressive order's volume
    amend_order(aggressive_order.get_id(), aggressive_order.get_limit_price(), new_aggr_volume);

    if (new_aggr_volume > 0) // As long as the aggr has volume, it keeps looking for a match
    {
        aggressive_order = get_order(aggressive_order.get_id()); // Retrieve the updated aggr order
        match_orders(aggressive_order);
    }
}

const Order& OrderBook::get_order(int order_id) const
{
    auto it = order_map.find(order_id);
    if (it != order_map.end())
    {
        auto& it_order = it->second;
        return *it_order;
    }
    throw std::out_of_range("Order not found"); // Some protection
}

template <typename T>
void OrderBook::debug_display_order_side(T& orders) const
{
    for (auto it = orders.begin(); it != orders.end(); ++it)
        it->debug_display_data();
}

void OrderBook::debug_display_trades() const
{
    size_t size = m_trades.size();
    for (int i = 0; i < size; ++i)
        std::cout << get_symbol() << " " << m_trades[i].get_price() << " "
        << m_trades[i].get_volume() << " "
        << m_trades[i].get_id_aggressive_order() << " "
        << m_trades[i].get_id_passive_order() << " "
        << m_trades[i].get_timestamps() << std::endl;
}

void OrderBook::debug_display_sides() const
{
    std::cout << std::endl;
    std::cout << "buy orders :" << std::endl;
    debug_display_order_side(buy_orders);
    std::cout << "sell orders :" << std::endl;
    debug_display_order_side(sell_orders);
    std::cout << std::endl;
}

std::string OrderBook::build_trade_log(const Trade& trade) const
{
    // <symbol>,<price>,<volume>,<aggressive_order_id>,<id_passive_order>
    return get_symbol() + ',' + StringHelper::double_to_string(trade.get_price(), 4) + ',' +
        std::to_string(trade.get_volume()) + ',' +
        std::to_string(trade.get_id_aggressive_order()) + ',' +
        std::to_string(trade.get_id_passive_order());
}

std::vector<std::string> OrderBook::build_ask_bid_levels()
{
    //   - bid and ask price levels (sorted best to worst) for that symbol in the format:
    //     <bid_price>,<bid_volume>,<ask_price>,<ask_volume>

    std::vector<std::string> result;

    auto it_bid = buy_orders.begin();
    auto it_ask = sell_orders.rbegin();

    double current_bid_price = 0;
    int current_bid_volume = 0;

    double current_ask_price = 0;
    int current_ask_volume = 0;

    while (it_bid != buy_orders.end() || it_ask != sell_orders.rend())
    {
        std::string bid_str = ",,"; // Default format for bid
        std::string ask_str = ","; // Default format for ask

        if (it_bid != buy_orders.end()) // Security
        {
            current_bid_price = it_bid->get_limit_price();
            current_bid_volume = it_bid->get_volume();
            ++it_bid;

            for (; it_bid != buy_orders.end();)
            {
                if (current_bid_price == it_bid->get_limit_price())
                {
                    current_bid_volume += it_bid->get_volume(); // We sum all the volumes which have the same price
                    ++it_bid;
                }
                else
                    break; // Price changed, we go for the next price
            }

            // std::cout << "before format: " << current_bid_price << std::endl;
            // std::cout << "formatted: " << StringHelper::double_to_string(current_bid_price, 4) << std::endl;
            bid_str = StringHelper::double_to_string(current_bid_price, 4) + "," + std::to_string(current_bid_volume) + ","; // Update the format
        }

        if (it_ask != sell_orders.rend()) // Security
        {
            current_ask_price = it_ask->get_limit_price();
            current_ask_volume = it_ask->get_volume();
            ++it_ask;

            for (; it_ask != sell_orders.rend();)
            {
                if (current_ask_price == it_ask->get_limit_price())
                {
                    current_ask_volume += it_ask->get_volume();
                    ++it_ask;
                }
                else
                    break;
            }

            // std::cout << "before format: " << current_ask_price << std::endl;
            // std::cout << "formatted: " << StringHelper::double_to_string(current_ask_price, 4) << std::endl;
            ask_str = StringHelper::double_to_string(current_ask_price, 4) + "," + std::to_string(current_ask_volume);
        }

        // if (bid_str != ",," || ask_str != ",")
        result.emplace_back(bid_str + ask_str);
    }

    // std::cout << "result" << std::endl;
    // for (const auto& str : result)
    //     std::cout << str << std::endl;

    return result;
}