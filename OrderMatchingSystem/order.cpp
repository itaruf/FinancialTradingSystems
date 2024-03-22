#include "order.h"

Order::Order(int id, Side side, double limit_price, int volume) : m_id(id), m_side(side), m_limit_price(limit_price), m_volume(volume), m_timestamps(TimeHelper::get_timestamps_in_microseconds())
{
}

void Order::debug_display_data() const
{
    std::cout << "order: " << get_id();
    std::cout << " - side: " << (get_side() == Side::BUY ? "BUY" : "SELL");
    std::cout << " - price: " << get_limit_price();
    std::cout << " - volume: " << get_volume();
    std::cout << " - timestamps: " << get_timestamps();
    std::cout << std::endl;
}