#ifndef ORDERBOOK_H
#define ORDERBOOK_H
#include "order.h"
#include "message.h"
#include <utility>
#include <deque>
#include <map>
#include <functional>
#include <iostream>
#include <string.h>
using namespace std;
class OrderBook
{
public:
	OrderBook(){};
	void initialize(string);
	void MainOrderBook();
	void DataImport();
	void EnterOrder(Order *x);
	void OrderBookBid(Order *x);
	void OrderBookAsk(Order *x);
	void UnfilledBid(Order *x);
	void UnfilledAsk(Order *x);
	void CompleteBid(Order *x);
	void CompleteAsk(Order *x);
	void CompleteBidMarket(Order *x);
	void CompleteAskMarket(Order *x);
	void DisplayBids();
	void DisplayAsks();
	void UserInterface();
private:
	map<double, deque<Order *>, greater<double> > OrderBid;
	map<double, deque<Order *> > OrderAsk;
};
#endif // ORDERBOOK_H
