#include "Order.h"
#include "Orderbook.h"

int main() {
  Orderbook ob;

  std::cout << "\n=== Test: Price-Time Priority ===\n";

  // Older buy orders (OrderId = 1, 2)
  ob.addOrder(Order(LimitOrder, 100, 5, Buy)); // OrderId 1
  ob.addOrder(Order(LimitOrder, 101, 5, Buy)); // OrderId 2

  // Newer buy order at better price (OrderId = 3)
  ob.addOrder(Order(LimitOrder, 102, 5, Buy)); // OrderId 3

  // Older sell orders (OrderId = 4, 5)
  ob.addOrder(Order(LimitOrder, 105, 4, Sell)); // OrderId 4
  ob.addOrder(Order(LimitOrder, 104, 4, Sell)); // OrderId 5

  // Newer sell at better price (OrderId = 6)
  ob.addOrder(Order(LimitOrder, 103, 4, Sell)); // OrderId 6

  // Market order to buy, will match against best sell prices
  ob.addOrder(Order(MarketOrder, 0, 10, Buy)); // Should match OrderId 6 → 5 → 4

  // Market order to sell, will match best buy prices
  ob.addOrder(
      Order(MarketOrder, 0, 12, Sell)); // Should match OrderId 3 → 2 → 1

  // Add duplicate price buy orders for time priority check
  ob.addOrder(Order(LimitOrder, 99, 6, Buy)); // OrderId 9
  ob.addOrder(Order(LimitOrder, 99, 4, Buy)); // OrderId 10 (later)

  // Matching sell at 98 to hit 99 (OrderId 9 then 10)
  ob.addOrder(Order(LimitOrder, 98, 5, Sell));

  std::cout << "\n=== Final Order Book ===\n";
  ob.printDepth();

  return 0;
}
