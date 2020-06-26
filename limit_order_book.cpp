#include "limit_order_book.h"


namespace lob {
	std::ostream& operator<< (std::ostream& out, const Order& l) {
		return out << "{Volume: " << l.volume << ", ID: " << l.id << ", Order_ID: " << l.order_id << "} ";
	}

	std::ostream& print_order_succint(std::ostream& out, const Order& l) {
		return out << "{" << l.volume << ", " << l.id << ", " << l.order_id << "} ";
	}

	
	std::ostream& operator<< (std::ostream& out, const LimitOrder& l) {
		return out << "{Price: " << l.price << ", Volume: " << l.volume << ", ID: " << l.id << ", Order_ID: " << l.order_id << "} ";
	}

	std::ostream& print_limit_order_succint(std::ostream& out, const LimitOrder& l) {
		return out << "{" << l.price << ", " <<l.volume << ", " << l.id << ", " << l.order_id << "} ";
	}


	LOB::LOB(){
		this->_total_ask_volume = 0; this->_total_bid_volume = 0; 
	}

	LOB::LOB(order_book bid, order_book ask) {
		this->bid = bid;
		this->ask = ask;
		this->_total_bid_volume = total_bid_volume();
		this->_total_ask_volume = total_ask_volume();
	}

	unsigned int LOB::current_order_id = 0;

	unsigned int LOB::get_total_bid_volume() {
		return _total_bid_volume;
	}

	unsigned int LOB::get_total_ask_volume() {
		return _total_ask_volume;
	}

	order_book LOB::get_bid() {
		return this->bid;
	}

	order_book LOB::get_ask() {
		return this->ask;
	}

	order_book LOB::set_bid(order_book bid) {
		return this->bid = bid;
	}

	order_book LOB::set_ask(order_book ask) {
		return this->ask = ask;
	}

	// Visualization functions
	//void LOB::print_queue(std::deque<Order> q) {
	//	for (auto v : q) {
	//		std::cout << '{' << v.volume << ", " << v.id << "}, ";
	//	}
	//}

	//void LOB::print_queue(std::deque<Order> q) {
	//	for (auto v : q) {
	//		std::cout << v;
	//	}
	//}

	void LOB::print_queue(std::deque<Order> q) {
		for (auto v : q) {
			print_order_succint(std::cout, v);
		}
	}

	void LOB::print_map(order_book const& m) {
		for (auto it = m.rbegin(); it != m.rend(); ++it) {
			std::cout << '{' << it->first << ": {";
			print_queue(it->second);
			std::cout << "}},\n";
		}
		std::cout << std::endl;
	}

	void LOB::print_lob() {
		std::cout << "Ask: " << std::endl;
		print_map(this->ask);
		std::cout << "Bid: " << std::endl;
		print_map(this->bid);
	}

	// Member functions
	double LOB::average_bid_price_price() {
		if (this->bid.empty()) {
			return -1;
		}
		double res(0.0);
		int num_vals(0);
		for (auto q : this->bid) {
			res += q.first;
			num_vals++;
		}
		return res / num_vals;
	}

	double LOB::average_ask_price_price() {
		if (this->ask.empty()) {
			return -1;
		}
		double res(0.0);
		int num_vals(0);
		for (auto q : this->ask) {
			res += q.first;
			num_vals++;
		}
		return res / num_vals;
	}

	double LOB::weighted_average_bid_price() {
		if (this->bid.empty()) {
			return -1;
		}
		double res(0.0), volume(0.0);
		double total_vol(0);
		for (auto q : this->bid) {
			volume = total_order_size(this->bid[q.first]);
			res += q.first * volume;
			total_vol += volume;
		}
		return res / total_vol;
	}

	double LOB::weighted_average_ask_price() {
		if (this->ask.empty()) {
			return -1;
		}
		double res(0.0), volume(0.0);
		double total_vol(0.0);
		for (auto q : this->ask) {
			volume = total_order_size(this->ask[q.first]);
			res += q.first * volume;
			total_vol += volume;
		}
		return res / total_vol;
	}

	int LOB::total_order_size(std::deque<Order> d) {
		int res(0);
		for (auto t : d) {
			res += t.volume;
		}
		return res;
		//return std::accumulate(d.begin(), d.end(), 0, myfunction);
	}

	int LOB::total_bid_volume() {
		if (this->bid.empty()) {
			return -1;
		}
		int total_vol(0);
		for (auto q : this->bid) {
			total_vol += total_order_size(this->bid[q.first]);;
		}
		return total_vol;
	}

	int LOB::total_ask_volume() {
		if (this->ask.empty()) {
			return -1;
		}
		int total_vol(0);
		for (auto q : this->ask) {
			total_vol += total_order_size(this->ask[q.first]);;
		}
		return total_vol;
	}

	unsigned int LOB::top_of_book(order_book book, bool bid) {
		if (!book.empty()) {
			if (bid) {
				return book.rbegin()->first;
			}
			else {
				return book.begin()->first;
			}
		}
		else if (this->ask.empty() && this->bid.empty()) {
			return 100;
		}
		else if (bid && book.empty()) {
			return LOB::top_of_book(this->ask, 0) - 2; // if one of the books is empty try and give a large spread and return a value
		}
		else if (!bid && book.empty()) {
			return LOB::top_of_book(this->bid, 0) + 2; // if one of the books is empty try and give a large spread and return a value
		}
		else {
			return 100; // return the initial value
		}
		return 0;
	}

	unsigned int LOB::best_bid() {
		return top_of_book(this->bid, 1);
	}

	unsigned int LOB::best_ask() {
		return top_of_book(this->ask, 0);
	}

	double LOB::mid_price() {
		return ((double) LOB::best_ask() + (double) LOB::best_bid()) / 2.0;
	}

	unsigned int LOB::bid_ask_spread() {
		if (bid.empty() || ask.empty()) {
			return UINT32_MAX;
		}
		return LOB::top_of_book(ask, 0) - LOB::top_of_book(bid, 1);
	}

	double LOB::half_spread() {
		return LOB::bid_ask_spread() / 2.0;
	}

	void LOB::buy_limit_order(LimitOrder l) {
		if (l.price < this->best_ask() || ask.empty()) {
			this->bid[l.price].push_back(Order({ l.volume, l.id }));
			_total_bid_volume += l.volume;
			//std::cout << "Buy limit order: " << l << " now sits in the bid book." << std::endl;
		}

		else {
			//TODO: When crossing the spread we need to return how much of the order is filled, and who got their order matched with the recent limit order
			double volume_weighted_avg_price(0.0);
			double total_volume_executed(0);

			while (!ask.empty() && l.price >= this->best_ask() && l.volume) {
				const int top_of_book_price = this->top_of_book(ask, 0);
				const int top_of_book_volume = ask[top_of_book_price].front().volume;

				if (top_of_book_volume > l.volume) {
					this->ask[top_of_book_price].front().volume -= l.volume;
					_total_ask_volume -= l.volume;
					//std::cout << "Buy limit order: " << l << " has been completly filled at price: " << top_of_book_price << '.' << std::endl;
					l.volume = 0;
					volume_weighted_avg_price += (double)top_of_book_price * top_of_book_volume;
					total_volume_executed += top_of_book_volume;
				}
				else if (top_of_book_volume == l.volume) {
					ask[top_of_book_price].pop_front();
					if (ask[top_of_book_price].empty())
						ask.erase(top_of_book_price);
					_total_ask_volume -= l.volume;
					//std::cout << "Buy limit order: " << l << " has been completly filled at price: " << top_of_book_price << '.' << std::endl;
					l.volume = 0;
					volume_weighted_avg_price += (double)top_of_book_price * top_of_book_volume;
					total_volume_executed += top_of_book_volume;
				}
				else {
					//std::cout << "Buy limit order: " << l << " has filled " << top_of_book_volume << " at price: " << top_of_book_price << '.' << std::endl;
					ask[top_of_book_price].pop_front();
					if (ask[top_of_book_price].empty())
						ask.erase(top_of_book_price);
					l.volume -= top_of_book_volume;
					_total_ask_volume -= top_of_book_volume;
					volume_weighted_avg_price += (double)top_of_book_volume * top_of_book_price;
					total_volume_executed += top_of_book_volume;
				}
			}
			volume_weighted_avg_price /= total_volume_executed;
			//std::cout << "Volume weighted average execution price is: " << volume_weighted_avg_price << std::endl;

			if (l.volume)
				buy_limit_order(l);
		}
	}

	void LOB::sell_limit_order(LimitOrder l) {
		if (l.price > this->best_bid() || bid.empty()) {
			this->ask[l.price].push_back(Order({ l.volume, l.id }));
			_total_ask_volume += l.volume;
			//std::cout << "Sell limit order: " << l << " now sits in the ask book." << std::endl;
		}

		else {
			//TODO: When crossing the spread we need to return how much of the order is filled, and who got their order matched with the recent limit order
			double volume_weighted_avg_price(0.0);
			double total_volume_executed(0);

			while (!bid.empty() && l.price <= this->best_bid() && l.volume) {
				const int top_of_book_price = this->top_of_book(bid, 1);
				const int top_of_book_volume = bid[top_of_book_price].front().volume;

				if (top_of_book_volume > l.volume) {
					this->bid[top_of_book_price].front().volume -= l.volume;
					_total_bid_volume -= l.volume;
					//std::cout << "Sell limit order: " << l << " has been completly filled at price: " << top_of_book_price << '.' << std::endl;
					l.volume = 0;
					volume_weighted_avg_price += (double)top_of_book_price * top_of_book_volume;
					total_volume_executed += top_of_book_volume;
				}
				else if (top_of_book_volume == l.volume) {
					bid[top_of_book_price].pop_front();
					if (bid[top_of_book_price].empty())
						bid.erase(top_of_book_price);
					_total_bid_volume -= l.volume;
					//std::cout << "Sell limit order: " << l << " has been completly filled at price: " << top_of_book_price << '.' << std::endl;
					l.volume = 0;
					volume_weighted_avg_price += (double)top_of_book_price * top_of_book_volume;
					total_volume_executed += top_of_book_volume;
				}
				else {
					//std::cout << "Sell limit order: " << l << " has filled " << top_of_book_volume << " at price: " << top_of_book_price << '.' << std::endl;
					bid[top_of_book_price].pop_front();
					if (bid[top_of_book_price].empty())
						bid.erase(top_of_book_price);
					l.volume -= top_of_book_volume;
					_total_bid_volume -= top_of_book_volume;
					volume_weighted_avg_price += (double)top_of_book_volume * top_of_book_price;
					total_volume_executed += top_of_book_volume;
				}
			}
			volume_weighted_avg_price /= total_volume_executed;
			//std::cout << "Volume weighted average execution price is: " << volume_weighted_avg_price << std::endl;

			if (l.volume)
				sell_limit_order(l);
		}
	}

	void LOB::buy_market_order(Order l) {
		if (!l.volume || ask.empty()) {
			//std::cout << "Buy market order: " << l << " could not be fully executed, not enough market volume." << std::endl;
		}
		else {
			//TODO: When crossing the spread we need to return how much of the order is filled, and who got their order matched with the recent limit order
			double volume_weighted_avg_price(0.0);
			double total_volume_executed(0);

			while (!ask.empty() && l.volume) {
				const int top_of_book_price = this->top_of_book(ask, 0);
				const int top_of_book_volume = ask[top_of_book_price].front().volume;

				if (top_of_book_volume > l.volume) {
					this->ask[top_of_book_price].front().volume -= l.volume;
					_total_ask_volume -= l.volume;
					//std::cout << "Buy market order: " << l << " has been completly filled at price: " << top_of_book_price << '.' << std::endl;
					l.volume = 0;
					volume_weighted_avg_price += (double)top_of_book_price * top_of_book_volume;
					total_volume_executed += top_of_book_volume;
				}
				else if (top_of_book_volume == l.volume) {
					ask[top_of_book_price].pop_front();
					if (ask[top_of_book_price].empty())
						ask.erase(top_of_book_price);
					_total_ask_volume -= l.volume;
					//std::cout << "Buy market order: " << l << " has been completly filled at price: " << top_of_book_price << '.' << std::endl;
					l.volume = 0;
					volume_weighted_avg_price += (double)top_of_book_price * top_of_book_volume;
					total_volume_executed += top_of_book_volume;
				}
				else {
					//std::cout << "Buy market order: " << l << " has filled " << top_of_book_volume << " at price: " << top_of_book_price << '.' << std::endl;
					ask[top_of_book_price].pop_front();
					if (ask[top_of_book_price].empty())
						ask.erase(top_of_book_price);
					l.volume -= top_of_book_volume;
					_total_ask_volume -= top_of_book_volume;
					volume_weighted_avg_price += (double)top_of_book_volume * top_of_book_price;
					total_volume_executed += top_of_book_volume;
				}
			}
			volume_weighted_avg_price /= total_volume_executed;
			//std::cout << "Volume weighted average execution price is: " << volume_weighted_avg_price << std::endl;

			if (l.volume)
				buy_market_order(l);
		}
	}

	void LOB::sell_market_order(Order l) {
		if (!l.volume || bid.empty()) {
			//std::cout << "Sell market order: " << l << " could not be fully executed, not enough market volume." << std::endl;
		}
		else {
			//TODO: When crossing the spread we need to return how much of the order is filled, and who got their order matched with the recent limit order
			double volume_weighted_avg_price(0.0);
			double total_volume_executed(0);

			while (!bid.empty() && l.volume) {
				const int top_of_book_price = this->top_of_book(bid, 0);
				const int top_of_book_volume = bid[top_of_book_price].front().volume;

				if (top_of_book_volume > l.volume) {
					this->bid[top_of_book_price].front().volume -= l.volume;
					_total_bid_volume -= l.volume;
					//std::cout << "Buy market order: " << l << " has been completly filled at price: " << top_of_book_price << '.' << std::endl;
					l.volume = 0;
					volume_weighted_avg_price += (double)top_of_book_price * top_of_book_volume;
					total_volume_executed += top_of_book_volume;
				}
				else if (top_of_book_volume == l.volume) {
					bid[top_of_book_price].pop_front();
					if (bid[top_of_book_price].empty())
						bid.erase(top_of_book_price);
					_total_bid_volume -= l.volume;
					//std::cout << "Buy market order: " << l << " has been completly filled at price: " << top_of_book_price << '.' << std::endl;
					l.volume = 0;
					volume_weighted_avg_price += (double)top_of_book_price * top_of_book_volume;
					total_volume_executed += top_of_book_volume;
				}
				else {
					//std::cout << "Buy market order: " << l << " has filled " << top_of_book_volume << " at price: " << top_of_book_price << '.' << std::endl;
					bid[top_of_book_price].pop_front();
					if (bid[top_of_book_price].empty())
						bid.erase(top_of_book_price);
					l.volume -= top_of_book_volume;
					_total_bid_volume -= top_of_book_volume;
					volume_weighted_avg_price += (double)top_of_book_volume * top_of_book_price;
					total_volume_executed += top_of_book_volume;
				}
			}
			volume_weighted_avg_price /= total_volume_executed;
			//std::cout << "Volume weighted average execution price is: " << volume_weighted_avg_price << std::endl;

			if (l.volume)
				sell_market_order(l);
		}
	}

	void LOB::buy_cancellation_order(unsigned int order_id) {
		if (bid.empty()) {
			//std::cout << "No such order exists: " << l << std::endl;
		}
		bool found(0);
		for (auto q : bid) {
			for (auto it = q.second.begin(); it != q.second.end(); ) {
				if (it->order_id == order_id) {
					_total_bid_volume -= it->volume;
					it = bid[q.first].erase(it);
					
					//std::cout << "Cancellation order has been executed: " << l << std::endl;
					found = 1;
					if (bid[q.first].empty())
						bid.erase(q.first);
					break; // Or should it remove all of the limit orders???
				}
				else {
					++it;
				}
			}
		}
		if (!found) {
			//std::cout << "No such order exists: " << l << std::endl;
		}

		
	}

	void LOB::buy_cancel_all(int id) {
		//TODO: this needs to be canged...
		if (bid.empty()) {
			//std::cout << "No orders for id: " << id << std::endl;
		}
		bool found(0);
		bool had_to_break(0);
		for (auto& [price, deque] : bid) {
			for (auto it = deque.begin(); it != deque.end(); ) {
				if (it->id == id) {
					//std::cout << "Deleting order id: " << it->id << ", volumne: " << it->volume << std::endl;
					_total_bid_volume -= it->volume;
					it = deque.erase(it);
					found = 1;
				}
				else {
					++it;
				}
			}
			if (deque.empty()) {
				bid.erase(price);
				had_to_break = 1;
				break;
			}
		}

		if (had_to_break) {
			LOB::buy_cancel_all(id);
		}

		if (!found) {
			//std::cout << "No orders for id: " << id << std::endl;
		}
	}
	
	void LOB::sell_cancellation_order(LimitOrder l) {
		if (ask.empty() || ask[l.price].empty() ) {
			//std::cout << "No such order exists: " << l << std::endl;
		}
		bool found(0);
		for (auto it = ask[l.price].begin(); it != ask[l.price].end(); ) {
			if (it->order_id == l.order_id) {
				it = ask[l.price].erase(it);
				_total_ask_volume -= l.volume;
				//std::cout << "Cancellation order has been executed: " << l << std::endl;
				found = 1;

				if (ask[l.price].empty())
					ask.erase(l.price);
				break; // Or should it remove all of the limit orders???
			}
			else {
				++it;
			}
		}

		if (!found) {
			//std::cout << "No such order exists: " << l << std::endl;
		}
	}

	void LOB::sell_cancel_all(int id) {
		//TODO: this needs to be canged...
		if (ask.empty()) {
			//std::cout << "No orders for id: " << id << std::endl;
		}
		bool found(0);
		bool had_to_break(0);
		for (auto& [price, deque] : ask) {
			for (auto it = deque.begin(); it != deque.end(); ) {
				if (it->id == id) {
					//std::cout << "Deleting order id: " << it->id << ", volumne: " << it->volume << std::endl;
					_total_ask_volume -= it->volume;
					it = deque.erase(it);
					found = 1;
				}
				else {
					++it;
				}
			}
			if (deque.empty()) {
				ask.erase(price);
				had_to_break = 1;
				break;
			}
		}

		if (had_to_break) {
			LOB::sell_cancel_all(id);
		}

		if (!found) {
			//std::cout << "No orders for id: " << id << std::endl;
		}
	}

	void LOB::simulator(unsigned int num_sims) {
		std::uniform_real_distribution<double> uniform_01(0.0, 1.0);
		std::uniform_int_distribution<unsigned int> agent_id(0, 9);
		std::uniform_int_distribution<unsigned int> volume(0, 10);
		std::uniform_int_distribution<unsigned int> volume_market(0, 2);

		std::binomial_distribution<unsigned int> buy_price(10, 0.25);
		std::binomial_distribution<unsigned int> sell_price(10, 0.25);


		double lambda_buy = 3.8;
		double lambda_sell = 1.1;
		double mu_buy = 0.8;
		double mu_sell = 0.15;
		double xi_buy = 0.0028;
		double xi_sell = 0.0028;

		for (unsigned int i = 0; i < num_sims; i++) {
			double r = lambda_buy + lambda_sell + mu_buy + mu_sell + xi_buy * _total_bid_volume + xi_sell * _total_ask_volume;
			double d1 = uniform_01(generator);
			double delta_t = -(std::log(d1) / r);
			double order_type = uniform_01(generator) * r;

			double c1 = lambda_buy;
			double c2 = lambda_buy + lambda_sell;
			double c3 = lambda_buy + lambda_sell + mu_buy;
			double c4 = lambda_buy + lambda_sell + mu_buy + mu_sell;
			double c5 = lambda_buy + lambda_sell + mu_buy + mu_sell + xi_buy * _total_bid_volume;
			double c6 = lambda_buy + lambda_sell + mu_buy + mu_sell + xi_buy * _total_bid_volume + xi_sell * _total_ask_volume;
			


			if (order_type >= 0 && order_type < c1) {
				buy_limit_order(LimitOrder({ (unsigned int)(mid_price() - buy_price(generator)), volume(generator), agent_id(generator) }));
			}
			else if (order_type >= c1 && order_type < c2) {
				sell_limit_order(LimitOrder({ (unsigned int)(sell_price(generator) + mid_price()), volume(generator), agent_id(generator) }));
			}
			else if (order_type >= c2 && order_type < c3) {
				buy_market_order(Order({ volume_market(generator), agent_id(generator) }));
			}
			else if (order_type >= c3 && order_type < c4) {
				sell_market_order(Order({ volume_market(generator), agent_id(generator) }));
			}
			else if (order_type >= c4 && order_type < c5) {
				buy_cancel_all(agent_id(generator));
			}
			else if (order_type >= c5 && order_type <= c6) {
				sell_cancel_all(agent_id(generator));
			}
			else {

			}

			if (i % (num_sims / 10) == 0) {
				print_lob();
				std::cout << "==========================================" << std::endl;
			}

		}

	}

}