#ifndef ORDER_H
#define ORDER_H
#include <iostream>
#include <string.h>
using namespace std;
class Order
{
public:
	Order() {}
	Order(string Ticker, string Type,  string Side, double LimitPrice, int Quantity,  string CounterPartyName)
		: Ticker(Ticker), Type(Type),Side(Side), LimitPrice(LimitPrice), Quantity(Quantity), CounterPartyName(CounterPartyName) {}
	//encapsulation-accessor methods
	void setTicker(const string Ticker) {this->Ticker= Ticker;}
	string getTicker() const {return Ticker;}
	void setType(const string Type) {this->Type= Type;}
	string getType() const {return Type;}
	void setSide(const string Side) {this->Side= Side;}
	string getSide() const {return Side;}
	void setLimitPrice(const double LimitPrice) {this->LimitPrice= LimitPrice;}
	double getLimitPrice() const {return LimitPrice;}
	void setQuantity(const int Quantity) {this->Quantity= Quantity;}
	int getQuantity()const {return Quantity;}
	void setCounterPartyName(const string CounterPartyName) {this->CounterPartyName= CounterPartyName;}
	string getCounterPartyName() const {return CounterPartyName;}
private:
	string Ticker;
	string Type;
	string Side;
	double LimitPrice;
	int Quantity;
	string CounterPartyName;
};
#endif // ORDER_H
