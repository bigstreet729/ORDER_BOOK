#include "Order.h"
#include "Orderbook.h"
#include <chrono>
#include <iostream>
#include <random>

void stressTest(Orderbook &ob, int numOrders) {
  std::default_random_engine rng;
  std::uniform_real_distribution<double> priceDist(90.0, 110.0);
  std::uniform_int_distribution<int> qtyDist(1, 1000);
  std::uniform_int_distribution<int> sideDist(0, 1);
  std::uniform_int_distribution<int> typeDist(0, 1);

  for (uint32_t i = 0; i < numOrders; ++i) {
    Side side = sideDist(rng) == 0 ? Buy : Sell;
    int qty = qtyDist(rng);
    OrderType type = typeDist(rng) == 0 ? LimitOrder : MarketOrder;
    if (type == LimitOrder) {
      double price = priceDist(rng);
      ob.addOrder(Order(LimitOrder, price, qty, side));
    } else {
      ob.addOrder(Order(MarketOrder, 0.0, qty, side));
    }
  }
}

int main() {
  Orderbook ob;
  uint32_t numOrders = 1000000;

  std::cout << "Starting stress test with " << numOrders
            << " limit orders...\n";

  auto start = std::chrono::high_resolution_clock::now();

  stressTest(ob, numOrders);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Orders processed in " << duration.count() << " ms\n";

  // std::cout << "\nFinal Order Book Snapshot:\n";
  // ob.printDepth();

  return 0;
}
