#include "trade.h"

Trade::Trade(int aggressive_order_id, int id_passive_order, double price, int volume, long long timestamps)
	: m_id_aggressive_order(aggressive_order_id), m_id_passive_order(id_passive_order), m_price(price), m_volume(volume), m_timestamps(timestamps)
{
}