#include "Order.h"
#include "Orderbook.h"
#include <chrono>
#include <iostream>
#include <random>

void stressTest(Orderbook &ob, int numOrders) {
  std::default_random_engine rng;
  std::uniform_real_distribution<double> priceDist(90.0, 110.0); // Price range
  std::uniform_int_distribution<int> qtyDist(1, 100); // Quantity range
  std::uniform_int_distribution<int> sideDist(0, 1);  // 0 = Buy, 1 = Sell

  for (int i = 0; i < numOrders; ++i) {
    double price = priceDist(rng);
    int qty = qtyDist(rng);
    Side side = sideDist(rng) == 0 ? Buy : Sell;

    ob.addOrder(Order(LimitOrder, price, qty, side));
  }
}

int main() {
  Orderbook ob;
  int numOrders = 100000;

  std::cout << "Starting stress test with " << numOrders << " limit orders...\n"
  ;

  auto start = std::chrono::high_resolution_clock::now();

  stressTest(ob, numOrders);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "✅ Orders processed in " << duration.count() << " ms\n";

  // std::cout << "\nFinal Order Book Snapshot:\n";
  // ob.printDepth();

  return 0;
}
