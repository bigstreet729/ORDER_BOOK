#ifndef ORDER_H
#define ORDER_H

#include <chrono>
#include <cstdint>

enum Side { Buy, Sell };

enum OrderType { LimitOrder, MarketOrder, CancelOrder, ModifyOrder };

using Price = std::uint32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint32_t;
using timestamp = std::chrono::system_clock::time_point;
class Order {
private:
  Price price_;
  Quantity quantity_;
  Side side_;
  OrderType ordertype_;
  Quantity initial_quantity_;
  Quantity remaining_quantity_;
  OrderId orderId_;
  timestamp timestamp_;

public:
  Order(OrderType ordertype, Price price, Quantity quantity, Side side);

  Side getSide() const;
  timestamp gettimestamp() const;
  OrderType getordertype() const;
  Price getPrice() const;
  OrderId getOrderId() const;
  Quantity getRemainingQuantity() const;
  Quantity getInitialQuantity() const;
  void reduceQuantity(Quantity qty);
};

#endif