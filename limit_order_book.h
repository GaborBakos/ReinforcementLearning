#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <deque>
#include <random>

#define ll long long
#define order_book std::map<unsigned int, std::deque<Order>>
//typedef std::map<unsigned int, std::deque<lob::Order>> order_book;

namespace lob {

	struct Order {
		unsigned int volume;
		unsigned int id;
		unsigned int order_id;
	};

	struct LimitOrder {
		unsigned int price;
		unsigned int volume;
		unsigned int id;
		unsigned int order_id;
	};

	class LOB {
	private:
		order_book bid;
		order_book ask;
		unsigned int _total_bid_volume;
		unsigned int _total_ask_volume;
		std::default_random_engine generator;
		static unsigned int current_order_id;
	public:
		
		LOB();

		LOB(order_book bid, order_book ask);

		order_book get_bid();

		order_book get_ask();

		order_book set_bid(order_book bid);

		order_book set_ask(order_book ask);

		unsigned int get_total_bid_volume();

		unsigned int get_total_ask_volume();

		// Visualization functions
		void print_queue(std::deque<Order> q);

		void print_map(order_book const& m);

		void print_lob();

		// Member functions
		double average_bid_price_price();

		double average_ask_price_price();

		double weighted_average_bid_price();

		double weighted_average_ask_price();

		int total_order_size(std::deque<Order> d);

		int total_bid_volume();

		int total_ask_volume();

		unsigned int top_of_book(order_book book, bool bid);

		unsigned int best_bid();

		unsigned int best_ask();

		double mid_price();

		unsigned int bid_ask_spread();

		double half_spread();
		
		// Market Order Types

		void buy_limit_order(LimitOrder l);

		void sell_limit_order(LimitOrder l);

		void buy_market_order(Order l);

		void sell_market_order(Order l);

		void buy_cancellation_order(unsigned int order_id);

		void buy_cancel_all(int id);
		
		void sell_cancellation_order(LimitOrder l);

		void sell_cancel_all(int id);
		
		void simulator(unsigned int num_sims);

	};

	
}