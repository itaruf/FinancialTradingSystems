#pragma once

#include "Helpers/time_helper.h"

#include <iostream>

// Class which represents an order to be stored and managed in an order book
class Order
{
public:
    enum class Side // May need to be accessible outside this class
    {
        BUY,
        SELL
    };

private:
    int m_id;
    Side m_side; // either SELL or BUY
    double m_limit_price;
    int m_volume;
    long long m_timestamps; // time priority check

public:
    // Constructors
    Order() = default;
    Order(int id, Side side, double limit_price, int volume);

    // Getters & Setters
    inline int get_id() const { return m_id; }
    inline Side get_side() const { return m_side; }
    inline double get_limit_price() const { return m_limit_price; }
    inline int get_volume() const { return m_volume; }
    inline long long get_timestamps() const { return m_timestamps; }

    inline void set_limit_price(double new_price) { m_limit_price = new_price; }
    inline void set_volume(int new_volume) { m_volume = new_volume; }
    inline void set_timestamps(long long new_timestamps) { m_timestamps = new_timestamps; }

    // Debug functions
    void debug_display_data() const;
};