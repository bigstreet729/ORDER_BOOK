#C++ Order Book Matching Engine

A high-performance console-based order book matching engine written in C++.

###Features

- Supports Limit, Market, Modify, and Cancel orders
- Implements **price-time priority**
- Matches buy/sell orders with proper logging
- Stores orders in efficient `std::map` + `std::list` structures
- Clean modular C++ design

[ BUY ] 100 x 5
[ BUY ] 101 x 5
[ SELL ] 100 x 5
[ TRADE ] BUY @100 Qty : 5 | 1 is [Mathced with 3
