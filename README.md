This project implements a realistic limit order book matching engine in C++. It supports:
Limit Orders
Market Orders
Modify Orders
Cancel Orders
Areas currently working on the multithreading, logging efficiently.
Matching is done using price-time priority. The system uses STL containers for clean and efficient management of orders across price levels.

---

 Features

Handles BUY/SELL orders with:
  - Price-time priority
  - Quantity tracking
  - Order cancellation & modification
Uses `std::map<Price, list<Order>>` for O(log n) access to price levels
Uses `std::unordered_map<OrderId, {Price, iterator}>` for fast lookups

---

to run g++ main.cpp Orderbook.cpp Order.cpp -o a.out
./a.out

#SAMPLE OUTPUT
[ BUY ]  100 x 10
[ TRADE ] BUY @95 Qty: 5 | 1001 is Matched with 2002
[ TRADE ] BUY @95 Qty: 5 | 1001 is Matched with 2003
[ MODIFY ] OrderId 1002 -> NEW PRICE 102
[ CANCEL ] OrderId 1003 Price: 101 Qty: 5
Buy Orders:
Price: 100 Order: 10
Price: 99 Order: 5 5

Sell Orders:
Price: 101 Order: 10

