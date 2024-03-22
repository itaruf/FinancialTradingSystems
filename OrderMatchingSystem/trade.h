#pragma once

// class that handles the trade between 2 orders and keep track of the data
class Trade
{
private:
	int m_id_aggressive_order;
	int m_id_passive_order;
	double m_price;
	int m_volume;
	long long m_timestamps;

public:
	// Constructor
	Trade(int id_aggressive_order, int id_passive_order, double price, int volume, long long timestamps);
	~Trade() = default;

	// Getters
	inline int get_id_aggressive_order() const { return m_id_aggressive_order; }
	inline int get_id_passive_order() const { return m_id_passive_order; }
	inline double get_price() const { return m_price; }
	inline int get_volume() const { return m_volume; }
	inline long long get_timestamps() const { return m_timestamps; }

	// Setters
	inline void set_id_aggressive_order(int new_id_aggressive_order) { m_id_aggressive_order = new_id_aggressive_order; }
	inline void set_id_passive_order(int new_id_passive_order) { m_id_aggressive_order = new_id_passive_order; }
	inline void set_price(double new_price) { m_price = new_price;  }
	inline void set_volume(int new_volume) { m_volume = new_volume; }
	inline void set_timestamps(long long new_timestamps) { m_timestamps = new_timestamps; }
};