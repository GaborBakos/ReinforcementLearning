//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <numeric>
//#include <map>
//#include <deque>
#include <limits>

#include "limit_order_book.h"

using namespace lob;



int main() {
	LOB lob;
	/*std::map<unsigned int, std::deque<Order>> bid, ask;
	bid[99].assign({ Order{ 34, 1 }, Order{ 10, 5 }, Order{ 34, 5 } });
	bid[98].assign({ Order{ 34, 1 }, Order{ 10, 1 }, Order{ 6, 2 } });
	bid[96].assign({ Order{ 56, 2 }, Order{ 33, 4 }, Order{ 5, 5 } });
	bid[95].assign({ Order{ 32, 2 } });
	ask[101].push_back(Order{ 69, 4 });
	ask[102].assign({ Order{ 6, 1 }, Order{ 10, 2 }, Order{ 34, 3 } });
	ask[104].assign({ Order{ 98, 1 }, Order{ 10, 2 }, Order{ 6, 5 } });
	ask[105].assign({ Order{ 55, 2 }, Order{ 6, 1 }, Order{ 5, 2 } });
	lob.set_bid(bid);
	lob.set_ask(ask);*/

	//simulator(&lob);
	lob.simulator(1000);
	std::cout << "Simulation is finished, the resulting LOB is: " << std::endl;
	lob.print_lob();
	lob.buy_cancellation_order(0);
	lob.print_lob();































	//lob.print_map(lob.get_bid());
	//std::cout << lob.total_order_size(lob.get_bid()[100]) << std::endl;
	//std::cout << lob.top_of_book(bid, true) << std::endl;

	/*lob.print_lob();
	std::cout << lob.average_bid_price_price() << std::endl;
	std::cout << lob.weighted_average_bid_price() << std::endl;


	lob.buy_cancellation_order(LimitOrder({96, 56, 2}));
	lob.print_lob();

	lob.buy_cancel_all(321);
	lob.print_lob();

	lob.sell_cancel_all(321);
	lob.print_lob();

	lob.sell_cancellation_order(LimitOrder({ 101, 69, 32}));
	lob.print_lob();*/

	/*lob.buy_limit_order(LimitOrder({ 100, 43, 555 }));
	lob.print_lob();*/

	//std::cout << lob.top_of_book(lob.get_ask(), 0) << std::endl;

	//lob.buy_limit_order(LimitOrder({ 120, 69, 555 }));
	//lob.print_lob();


	//lob.buy_limit_order(LimitOrder({ 120, 7, 555 }));
	//lob.print_lob();

	//lob.buy_limit_order(LimitOrder({ 120, 50, 555 }));
	//lob.print_lob();

	//lob.buy_limit_order(LimitOrder({ 120, 250, 555 }));
	//lob.print_lob();


	//lob.sell_limit_order(LimitOrder({ 130, 250, 555 }));
	//lob.print_lob();


	//lob.buy_market_order(Order({ 10, 5 }));
	//lob.print_lob();

	//std::cout << lob.bid_ask_spread() << std::endl;
	//std::cout << lob.half_spread() << std::endl;


	/*lob[203].assign({ 3, 42, 5 });
	lob[100].push_back(2);
	lob[102].assign({5, 2, 7, 64});
	lob[104].assign({3, 3, 5, 6, 7});
	lob[105].push_back(53);
	lob[321321].assign({ 321, 421, 45231 });
	print_map(lob);
	std::cout << total_order_size(lob[1646]) << std::endl;*/
	return 0;
}