#ifndef MESSAGE_H
#define MESSAGE_H
#include <iostream>
#include <string>
#include "order.h"

using namespace std;

class Message
{
public:
	Message();
    void OrderReceived(Order* order);
    void OrderFilled(double x, Order* order);
    void OrderNotFilled(Order* order);
    void OrderPartiallyFilled(double x,Order* order);
    void MarketOrderFilled(Order* order);
    void MarketPartiallyFilled(double x,Order* order);
private:
    Order* MyOrder;
};

#endif // MESSAGE_H
