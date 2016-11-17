/*Author: Aishwarya Ketkar; editor: vim; compiler: g++ std-c++11 */
/*This program simulates an orderbook. The input method can be specified by the user as either txt file input
or personal order input. Since, one cannot ammend a particular element of a vector (in case order is partially
filled, it still stays in it's time key on the map), an array needs to be of a definite length; the data structure
used here is a deque. The way to access the unique key-value pair from a map is to use the iterator pointing to the
first (key/price) or the second (value/time).
The function method "initialize" is for entering the data using a txt file. It will display the Bid Book and ask book
towards the end of the program. The function method names "MainOrderBook" is used to enter data personally by the user.*/
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include <utility>
#include <deque>
#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include "order.h"
#include "orderbook.h"
#include "message.h"
using namespace std;
Message::Message() {}
void Message::OrderReceived(Order* order) //Message to confirm that order was received and entered in the book
{
    cout<<"Order Received for: \n" <<endl;
    cout << setw(10) << order->getCounterPartyName();
    cout << setw(10) << order->getSide();
    cout << setw(10) << order->getTicker();
    cout << setw(10) << order->getType();
    cout << setw(10) << order->getQuantity();
    cout << setw(10) << order->getLimitPrice();
    cout << "\n" <<endl;
}
void Message::OrderFilled(double x, Order* order) //Message to confirm the filling of order and its price
{
    string Type= order->getType();
    if (Type == "Market"){
        cout<<"Market Order Filled: \n" <<endl;
        cout << setw(10) << order->getCounterPartyName();
        cout << setw(10) << order->getTicker();
        cout << setw(10) << order->getType();
        cout << setw(10) << order->getQuantity();
        cout << setw(10) << order->getSide();
        cout << setw(10) << x;
        cout << "\n" <<endl;
    }
    else {
        cout<<"Limit Order Filled: \n"<< endl;
        cout << setw(10) << order->getCounterPartyName();
        cout << setw(10) << order->getTicker();
        cout << setw(10) << order->getType();
        cout << setw(10) << order->getQuantity();
        cout << setw(10) << order->getSide();
        cout << setw(10) << order->getLimitPrice();
        cout << "\n" <<endl;
    }
}
void Message::OrderNotFilled(Order* order) //Intimation of order not being filled due to lack of liquidity
{
    if (order->getType()=="Limit") {
        cout<<"Order has been added to the Book: \n" <<endl;
        cout << setw(10) << order->getCounterPartyName();
        cout << setw(10) << order->getSide();
        cout << setw(10) << order->getTicker();
        cout << setw(10) << order->getType();
        cout << setw(10) << order->getQuantity();
        cout << setw(10) << order->getLimitPrice();
        cout << "\n" <<endl;
    }
    else {
        cout <<"\nNot enough liquidity in the Market."<<endl;
    }
}
void Message::OrderPartiallyFilled(double x, Order* order) //Intimation of partially filled order
{
    cout<<"Partially Filled Limit Order:"<<endl;
    cout << setw(10) << order->getCounterPartyName();
    cout << setw(10) << order->getTicker();
    cout << setw(10) << order->getType();
    cout << setw(10) << order->getQuantity();
    cout << setw(10) << order->getSide();
    cout << setw(10) << x;
    cout << "\n" <<endl;
}
void Message::MarketPartiallyFilled(double x, Order* order) //Intimation of partially filled market order
{
    cout<<"Partially filled Market Order: "<<endl;
    cout << setw(10) << order->getCounterPartyName();
    cout << setw(10) << order->getTicker();
    cout << setw(10) << order->getType();
    cout << setw(10) << order->getQuantity();
    cout << setw(10) << order->getSide();
    cout << setw(10) << x;
    cout << "\n" <<endl;
}
void OrderBook::initialize(string filename) //function to let user input his/her order via txt file
{ //Declaration of vectors to import data
    vector <string> CounterParty;
    vector<string> buyorsell;
    vector <double> Price;
    vector <int> Quantity;
    vector <string> Ticker;
    vector <string> OrderType;
    deque <Order *> Os; //deque for the aforementioned reasons
    ifstream inFile;
    inFile.open(filename);
    Order* MyOrder = new Order();
    vector <string> v;
    copy(istream_iterator<string>(inFile), istream_iterator<string>(), back_inserter(v)); //iterator
    int size= v.size();
    //Loops to get relevant data into relevant vectors
    for (int i=0; i<size; i+=6) {
        string temp= v.at(i);
        temp= temp.substr(0, temp.size()-1); //removing the "," from csv delimited file
        Ticker.push_back(temp);
    }
    for (int i=1; i<size; i+=6) {
        string temp= v.at(i);
        temp= temp.substr(0, temp.size()-1);
        buyorsell.push_back(temp);
    }
    for (int i=2; i<size; i+=6) {
        string temp= v.at(i);
        temp= temp.substr(0, temp.size()-1);
        OrderType.push_back(temp);
    }
    for (int i=3; i<size; i+=6) {
        string temp= v.at(i);
        temp= temp.substr(0, temp.size()-1);
        int newtemp= stoi(temp);
        Quantity.push_back(newtemp);
    }
    for (int i=4; i<size; i+=6) {
        string temp= v.at(i);
        temp= temp.substr(0, temp.size()-1);
        double newtemp= stod(temp);
        Price.push_back(newtemp);
    }
    for (int i=5; i<size; i+=6) {
        string temp= v.at(i);
        temp= temp.substr(0, temp.size()-1);
        CounterParty.push_back(temp);
    }
    //Entering the data into Deque of order for our order book
    int NewSize= OrderType.size();
    Message* message= new Message();
    for(int i = 0; i<NewSize; i++){
        MyOrder= new Order();
        MyOrder->setCounterPartyName(CounterParty.at(i)); //using accessor methods to get the relevant values
        MyOrder->setTicker(Ticker.at(i));
        MyOrder->setLimitPrice(Price.at(i));
        MyOrder->setQuantity(Quantity.at(i));
        MyOrder->setSide(buyorsell.at(i));
        MyOrder->setType(OrderType.at(i));
        message->OrderReceived(MyOrder); //intimation of order receipt
        Os.push_back(MyOrder); //adding to the order deque
    }
    Order* NewOrder = new Order();//creating new order object
    for(deque<Order*>::iterator iter = Os.begin(); iter!=Os.end(); iter++){
        NewOrder = *iter;
        EnterOrder(NewOrder);//passing each one as a new order
    }
    cout <<endl;
}
void OrderBook::MainOrderBook() //function to let user input his/her order via typing
{
    cout<<"Enter Security Name: ";//Getting order inputs
    string Ticker;
    cin>> Ticker;
    cout<<"\nEnter Side('Buy' for buy and 'Sell' for sell): ";
    string Side;
    cin>>Side;
    cout<<"\nEnter Type of your Order('Limit' for limit and 'Market' for Market): ";
    string Type;
    cin>>Type;
    cout<<"\nEnter Your Price: ";
    double Price;
    cin>>Price;
    cout<<"\nEnter the Quantity: ";
    int Quantity;
    cin>>Quantity;
    cout<<"\nEnter CounterParty Name: ";
    string CounterParty;
    cin>>CounterParty;
    Order* x = new Order(Ticker, Type, Side, Price, Quantity, CounterParty);
    Message *msg= new Message();
    msg->OrderReceived(x);
    if (Side== "Buy") {OrderBookBid(x);} //if buy order, add to Bid Book
    else if (Side== "Sell") {OrderBookAsk(x);} //if sell order, add to Ask Book
    else {cout <<"invalid entry of side"<<endl;} //else: Invalid entry
}
void OrderBook::EnterOrder(Order *x) //enter order to the Bid Book or the Ask Book
{
    if (x->getSide() == "Buy"){OrderBookBid(x);}
    if (x->getSide() == "Sell"){OrderBookAsk(x);}
}
void OrderBook::OrderBookBid(Order *x) //Enter order in the Bid Book
{
    map<double, deque<Order*> >::iterator iter;
    iter = OrderBid.begin();
    if(OrderBid.empty()&& x->getType()=="Limit") {
        UnfilledBid(x);
        return;
    }
    if(x->getType()=="Limit"){
        if (x->getLimitPrice()<iter->first && !iter->second.empty()) {UnfilledBid(x);}
        else if (x->getLimitPrice()>=iter->first){
            CompleteBid(x);
            return;
        }
    }
    else if(x->getType() == "Market"){
        CompleteBidMarket(x);
        return;
    }
}
void OrderBook::OrderBookAsk(Order *x) //Enter Order in the Ask Book
{
    map<double, deque<Order*> >::iterator iter;
    iter = OrderAsk.begin();
    if(OrderAsk.empty()&&x->getType()=="Limit") {
        UnfilledAsk(x);
        return;
    }
    if(x->getType()=="Limit"){ //if Order is limit order we see if lowest price is below limit or we leave it unfilled
        if (x->getLimitPrice()>iter->first && !iter->second.empty()){UnfilledAsk(x);}
        else {
            CompleteAsk(x);
            return;
        }
    }
    else if(x->getType() == "Market"){
        CompleteAskMarket(x);
        return;
    }
}
void OrderBook::CompleteBid(Order *x) //Complete the bid order
{
    map<double, deque<Order *> >::iterator iter; //iterate through the map
    iter = OrderAsk.begin(); //begin with the 0th index
    Message* message = new Message();
    if(OrderAsk.empty()) //if empty, then send to unfilled bid since it can't be satisfied
    {
        message->OrderNotFilled(x);
        UnfilledBid(x);
        return;
    }
    int TempQuantity, QuantityX; //temporary placeholders for changes in order and quantity; mechanism for partial fulfilment
    Order *TempOrder;
    deque<Order*> x1 = iter->second; //iter->second acts as an iterator through the "PRICE" element of our map
    TempOrder = x1.front();
    if(iter->second.empty()) //if no price, then order can't be fulfilled
    {
        message->OrderNotFilled(x);//message object as unfilled
        UnfilledBid(x);
        return;
    }
    else if(x1.empty() || x->getLimitPrice() < iter->first) //if can't satisfy at the given limit price
    {
        UnfilledBid(x);
        return;
    }
    if(x->getQuantity()<TempOrder->getQuantity()) //temporary for partial fulfilment
    {
        QuantityX = x->getQuantity();//store quantity
        TempQuantity = TempOrder->getQuantity();//store temp qty
        TempOrder->setQuantity(TempQuantity - QuantityX);
        message->OrderFilled(TempOrder->getLimitPrice(), x);//intimation of order being filled
        message->OrderPartiallyFilled(TempOrder->getLimitPrice(), TempOrder);
        x1.pop_front();//modify the deque
        x1.push_front(TempOrder);
        iter->second = x1;//place new deque at the particular price in the map
        return;
    }
    else if(x->getQuantity()==TempOrder->getQuantity())//if its equal then fill both orders
    {
        message->OrderFilled(TempOrder->getLimitPrice(), x);
        message->OrderFilled(TempOrder->getLimitPrice(), TempOrder);
        x1.pop_front(); //pop the order off
        if(!x1.empty()) {iter->second = x1;}
        else if(x1.empty() && !OrderAsk.empty()){OrderAsk.erase(iter);
        return;
        }
    }
    else if (x->getQuantity()>TempOrder->getQuantity() && x->getLimitPrice() >= iter->first)
    {
        QuantityX = x->getQuantity();
        TempQuantity = TempOrder->getQuantity();
        x->setQuantity(QuantityX - TempQuantity);//change x
        message->OrderFilled(TempOrder->getLimitPrice(), TempOrder);//fill temperory order
        message->OrderPartiallyFilled(TempOrder->getLimitPrice(), x);//partially fill x
        x1.pop_front();//delete the element if the deque is empty
        if(!x1.empty()){iter->second = x1;}
        else if(x1.empty() && !OrderAsk.empty())
        {OrderAsk.erase(iter);}
        CompleteBid(x);
        return;
    }
}
void OrderBook::UnfilledBid(Order *x)//record an unfilled bid order
{
    Message* message = new Message();
    message->OrderNotFilled(x);
    map<double, deque<Order*> >::iterator iter;
    deque<Order*> TempOrder; //modify queue at a position
    double FindOrder = x->getLimitPrice();
    iter = OrderBid.find(FindOrder); //Using iterator to find order with the same price
    if(iter!=OrderBid.end())//push it to the back of the current deque
    {iter->second.push_back(x);
        return;}
    else
    {
        TempOrder.push_back(x);//If empty then, push it into a new deque which is made the element at this price
        OrderBid[x->getLimitPrice()] = TempOrder;
        return;
    }
}
void OrderBook::UnfilledAsk(Order *x)//record an unfilled ask order
{
    Message* message = new Message();
    message->OrderNotFilled(x);
    map<double, deque<Order*> >::iterator iter;
    deque<Order*> TempOrder; //modify queue at a position
    iter = OrderAsk.find(x->getLimitPrice());
    if(iter!=OrderAsk.end())
    {iter->second.push_back(x);
        return;}
    else
    {
        TempOrder.push_back(x);
        OrderAsk[x->getLimitPrice()] = TempOrder;
        return;
    }
}
void OrderBook::CompleteAsk(Order *x)//complete an ask order
{
    Message* message = new Message();
    map<double, deque<Order*> >::iterator iter;
    map<double, deque<Order*> >::iterator iterTemp;  //temperory iterator
    iter = OrderBid.begin();
    int TempQuantity;//placeholder for modified quantities in case of partially filled
    int TempX;
    if(OrderBid.empty() || x->getLimitPrice() > iter->first)
    {UnfilledAsk(x);
        return;}
    Order* TempOrder;
    deque<Order*> x1 = iter->second;// store deque at a price
    TempOrder = x1.front();
    if(x->getQuantity()<TempOrder->getQuantity()){
        message->OrderFilled(TempOrder->getLimitPrice(), x);
        TempX = x->getQuantity();
        TempQuantity = TempOrder->getQuantity();
        TempOrder->setQuantity(TempQuantity - TempX);
        message->OrderPartiallyFilled(TempOrder->getLimitPrice(), TempOrder);
        x1.pop_front();
        x1.push_front(TempOrder);
        iter->second = x1;
        return;
    }
    else if(x->getQuantity()==TempOrder->getQuantity())
    {
        message->OrderFilled(TempOrder->getLimitPrice(),x);
        message->OrderFilled(TempOrder->getLimitPrice(), TempOrder);
        x1.pop_front();
        if(!x1.empty()){iter->second = x1;}
        else if(x1.empty()){OrderBid.erase(iter);
            return;}
    }
    else if (x->getQuantity()>TempOrder->getQuantity() && x->getLimitPrice() <= iter->first)
    {
        TempX = x->getQuantity();
        TempQuantity = TempOrder->getQuantity();
        x->setQuantity(TempX - TempQuantity);
        message->OrderFilled(TempOrder->getLimitPrice(), TempOrder);
        message->OrderPartiallyFilled(TempOrder->getLimitPrice(), x);
        x1.pop_front();
        if(!x1.empty()){iter->second = x1;
            return;}
        else if(x1.empty()){OrderBid.erase(iter);}
        CompleteAsk(x);//keep trying to fill partial orders
        return;
    }
}
void OrderBook::CompleteBidMarket(Order *x) //complete the market bid order; idea is similar to CompleteBid; except
//market orders don't get placed on the book if unfilled
{
    map<double, deque<Order*> >::iterator iter;
    Message* message = new Message();
    iter = OrderAsk.begin();
    if(OrderAsk.empty()) {message->OrderNotFilled(x);}
    if(iter->second.empty()){message->OrderNotFilled(x);}
    Order* TempOrder;
    deque<Order*> x1 = iter->second;
    int NewQuantity;
    while(x->getQuantity()!=0 && !OrderAsk.empty() && iter!=OrderAsk.end()){
        x1 = iter->second;
        TempOrder = x1.front();
        if (x->getQuantity()>TempOrder->getQuantity()){
            message->OrderFilled(TempOrder->getLimitPrice(), TempOrder);
            NewQuantity =x->getQuantity()-TempOrder->getQuantity();
            x->setQuantity(NewQuantity);
            message->MarketPartiallyFilled(TempOrder->getLimitPrice(), x);
            x1.pop_front();
            if(!x1.empty()){iter->second = x1;
                return;}
            else if(x1.empty() && !OrderAsk.empty()){
                cout<<"Um..."<<endl;
                OrderAsk.erase(iter);
                return;
            }
            CompleteBidMarket(x);
            return;
        }
        else if (x->getQuantity()==TempOrder->getQuantity()){
            message->OrderFilled(TempOrder->getLimitPrice(), x);
            message->OrderFilled(TempOrder->getLimitPrice(), TempOrder);
            x->setQuantity(0);
            x1.pop_front();
            iter->second = x1;
            return;
        }
        else if (x->getQuantity()<TempOrder->getQuantity()) {
            message->OrderFilled(TempOrder->getLimitPrice(), x);
            NewQuantity = TempOrder->getQuantity()-x->getQuantity();
            TempOrder->setQuantity(NewQuantity);
            message->OrderPartiallyFilled(TempOrder->getLimitPrice(), TempOrder);
            x->setQuantity(0);
            x1.pop_front();
            x1.push_front(TempOrder);
            iter->second = x1;
            return;
        }
    }
}
void OrderBook::CompleteAskMarket(Order *x) //complete the market ask order; idea is similar to CompleteAsk; except
//market orders won't get placed on the book if unfilled at that price
{
    Message* message = new Message();
    if(OrderBid.empty()){message->OrderNotFilled(x);
        return;}
    map<double, deque<Order*> >::iterator iter;
    iter = OrderBid.begin();
    if(iter->second.empty()) {message->OrderNotFilled(x);
        return;}
    Order* TempOrder;
    deque<Order*> x1;
    int NewQuantity;//temp variable to store new quantity of order if its partially filled
    while(x->getQuantity()!=0 && !OrderBid.empty() && iter!=OrderBid.end()){
        x1 = iter->second;
        TempOrder = x1.front();
        if (x->getQuantity()>TempOrder->getQuantity()) {
            message->OrderFilled(TempOrder->getLimitPrice(), TempOrder);
            NewQuantity =x->getQuantity()-TempOrder->getQuantity();
            x->setQuantity(NewQuantity);
            message->MarketPartiallyFilled(TempOrder->getLimitPrice(), x);
            x1.pop_front();
            if(!x1.empty()) {iter->second = x1;
                return;}
            else if(x1.empty() && !OrderBid.empty()){OrderBid.erase(iter);
                return;}
            CompleteAskMarket(x);
            return;
        }
        else if (x->getQuantity()==TempOrder->getQuantity()) {
            message->OrderFilled(TempOrder->getLimitPrice(), x);
            message->OrderFilled(TempOrder->getLimitPrice(), TempOrder);
            x->setQuantity(0);
            x1.pop_front();
            iter->second = x1;
            return;
        }
        else if (x->getQuantity()<TempOrder->getQuantity()) {
            message->OrderFilled(TempOrder->getLimitPrice(), x);
            NewQuantity = TempOrder->getQuantity()-x->getQuantity();
            TempOrder->setQuantity(NewQuantity);
            message->OrderPartiallyFilled(TempOrder->getLimitPrice(), TempOrder);
            x->setQuantity(0);
            x1.pop_front();
            x1.push_front(TempOrder);
            iter->second = x1;
            return;
        }
    }
}
void OrderBook::DisplayBids() //display the bid book
{
    deque<Order*> x1;
    Order* x = new Order();
    if(OrderBid.empty())
    {
        cout<<"There are no Bid Orders. The book is empty"<<endl<<endl<<endl;
        return;
    }
    for(map<double,deque<Order*> >::iterator iter=OrderBid.begin(); iter!=OrderBid.end(); iter++){
        x1 = iter->second;
        for (deque<Order*>::iterator iter1 = x1.begin(); iter1!=x1.end(); iter1++){
            x = *iter1;

            cout << setw(10) << x->getCounterPartyName();
            cout << setw(10) << x->getTicker();
            cout << setw(10) << x->getType();
            cout << setw(10) << x->getQuantity();
            cout << setw(10) << x->getLimitPrice();
            cout << endl;
        }
    }
    return;
}
void OrderBook::DisplayAsks() //display the ask book
{
    deque<Order*> x1;
    Order* x = new Order();
    map<double,deque<Order*> >::iterator iter1=OrderAsk.begin();
    if(OrderAsk.empty()){
        cout<<"There are no Ask Orders.\n"<<endl;
        return;
    }
    for(map<double,deque<Order*> >::iterator iter=OrderAsk.begin(); iter!=OrderAsk.end(); iter++)
    {
        x1 = iter->second;
        for (deque<Order*>::iterator iter2 = x1.begin(); iter2!=x1.end(); iter2++){
            x = *iter2;
            cout << setw(10) << x->getCounterPartyName();
            cout << setw(10) << x->getTicker();
            cout << setw(10) << x->getType();
            cout << setw(10) << x->getQuantity();
            cout << setw(10) << x->getLimitPrice();
            cout << endl;
        }
    }
    return;
}

int main() {
    OrderBook Test;
    int userinput=0;
    cout << "Welcome to Order Book simulation! Please Select your option: " <<endl;
    while (userinput!= 4) {
    cout << "\n1. Enter Order " <<"\n2. Display Bid Book" <<"\n3. Display Ask Book" <<"\n4. Exit" <<endl;
    cin >> userinput;
        if (userinput ==1) {
            cout <<"Please enter the mode of order entry: " <<"\n1: Via a txt file" <<"\n2: Via Keyboard" <<endl;
            int input;
            cin >> input;
            if (input == 1){
                string filename;
                cout << "Please enter the name of your file with .txt extension: " <<endl;
                cin >> (filename);
                Test.initialize(filename);
            }
            else if (input == 2){
                Test.MainOrderBook();
            }
            else {
                return 0;
            }
        }
        else if(userinput==2) {
            Test.DisplayBids();
        }
        else if(userinput==3) {
            Test.DisplayAsks();
        }
        else if (userinput==4) {
            cout <<"Thank You! " <<endl;
        }
    }
return 0;
}
