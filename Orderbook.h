#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "Order.h"
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

class Orderbook {
private:
  std::map<Price, std::list<Order>, std::greater<Price>> buyOrders;
  std::map<Price, std::list<Order>> sellOrders;
  std::unordered_map<OrderId, std::pair<Price, std::list<Order>::iterator>>
      orderMap;
  void matchOrder(Order order, bool IsLimit);
public:
  void addOrder(const Order &order);
  void cancelOrder(OrderId orderid);
  void modifyOrder(OrderId id, Price newprice);
  
  void printDepth() const;
};

#endif
