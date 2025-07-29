#include "Orderbook.h"
#include "TimeUtils.h"
void Orderbook::addOrder(const Order &order) {
  std::cout << TimeUtils::timePointToString(order.gettimestamp()) << " ";
  std::cout << (order.getSide() == Buy ? "[ BUY ] " : "[ SELL ] ")
            << order.getPrice() << " x " << order.getInitialQuantity()
            << std::endl;

  switch (order.getordertype()) {
  case LimitOrder:
    matchOrder(order, true);
    break;
  case MarketOrder:
    matchOrder(order, false);
    break;
  case ModifyOrder:
    modifyOrder(order.getOrderId(), order.getPrice());
    break;
  case CancelOrder:
    cancelOrder(order.getOrderId());
    break;
  }
}

void Orderbook::cancelOrder(OrderId orderid) {
  auto found = orderMap.find(orderid);
  if (found == orderMap.end()) {
    std::cout << "[ CANCEL FAILED ] OrderId " << orderid << " not found\n";
    return;
  }

  Price price = found->second.first;
  auto it = found->second.second;
  Side side = it->getSide();

  std::cout << "[ CANCEL ] OrderId " << orderid << " Price: " << price
            << " Qty: " << it->getRemainingQuantity() << "\n";

  if (side == Buy) {
    auto &lst = buyOrders[price];
    lst.erase(it);
    if (lst.empty())
      buyOrders.erase(price);
  } else {
    auto &lst = sellOrders[price];
    lst.erase(it);
    if (lst.empty())
      sellOrders.erase(price);
  }

  orderMap.erase(found);
}

void Orderbook::matchOrder(Order order, bool IsLimit) {
  if (order.getSide() == Buy) {
    for (auto it = sellOrders.begin();
         it != sellOrders.end() && order.getRemainingQuantity() > 0;) {

      Price sellPrice = it->first;
      if (order.getPrice() < sellPrice && IsLimit)
        break;

      auto &sellList = it->second;
      for (auto sellIt = sellList.begin();
           sellIt != sellList.end() && order.getRemainingQuantity() > 0;) {

        Quantity matchQty = std::min(order.getRemainingQuantity(),
                                     sellIt->getRemainingQuantity());
        std::cout << TimeUtils::timePointToString(order.gettimestamp()) << " ";
        std::cout << "[ TRADE ]  BUY @" << sellPrice << " Qty : " << matchQty
                  << " | " << order.getOrderId() << " is Mathced with "
                  << sellIt->getOrderId() << "\n";

        order.reduceQuantity(matchQty);
        sellIt->reduceQuantity(matchQty);

        if (sellIt->getRemainingQuantity() == 0) {
          sellIt = sellList.erase(sellIt);
        } else {
          ++sellIt;
        }
      }

      if (sellList.empty()) {
        it = sellOrders.erase(it);
      } else {
        ++it;
      }
    }

    if (order.getRemainingQuantity() > 0) {
      buyOrders[order.getPrice()].push_back(order);
      auto it = --buyOrders[order.getPrice()].end();
      orderMap[order.getOrderId()] = {order.getPrice(), it};
    }

  } else if (order.getSide() == Sell) {
    for (auto it = buyOrders.begin();
         it != buyOrders.end() && order.getRemainingQuantity() > 0;) {

      Price buyPrice = it->first;
      if (order.getPrice() > buyPrice)
        break;

      auto &buyList = it->second;
      for (auto buyIt = buyList.begin();
           buyIt != buyList.end() && order.getRemainingQuantity();) {

        Quantity matchQty = std::min(order.getRemainingQuantity(),
                                     buyIt->getRemainingQuantity());
        std::cout << TimeUtils::timePointToString(order.gettimestamp()) << " ";
        std::cout << "[ TRADE ]  SELL @" << buyPrice << " Qty :" << matchQty
                  << " | " << order.getOrderId() << " is Matched with "
                  << buyIt->getOrderId() << "\n";

        order.reduceQuantity(matchQty);
        buyIt->reduceQuantity(matchQty);

        if (buyIt->getRemainingQuantity() == 0) {
          buyIt = buyList.erase(buyIt);
        } else {
          ++buyIt;
        }
      }

      if (buyList.empty()) {
        it = buyOrders.erase(it);
      } else {
        ++it;
      }
    }

    if (order.getRemainingQuantity() > 0) {
      sellOrders[order.getPrice()].push_back(order);
      auto it = --sellOrders[order.getPrice()].end();
      orderMap[order.getOrderId()] = {order.getPrice(), it};
    }
  }
}

void Orderbook::modifyOrder(OrderId id, Price newprice) {
  if (orderMap.find(id) == orderMap.end()) {
    std::cout << "[MODIFY FAILES ] OrderId " << id << " not found\n";
    return;
  }

  auto found = orderMap.find(id);
  Price oldprice = found->second.first;
  auto oldIt = found->second.second;
  Order &oldOrder = *oldIt;
  Side side = oldOrder.getSide();
  OrderType type = oldOrder.getordertype();
  Quantity oldQty = oldOrder.getRemainingQuantity();

  std::cout << "[ MODIFY ] OrderId " << id << " -> NEW PRICE " << newprice
            << "\n";

  auto &orderList = (side == Buy) ? buyOrders[oldprice] : sellOrders[oldprice];
  orderList.erase(oldIt);
  if (orderList.empty()) {
    if (side == Buy) {
      buyOrders.erase(oldprice);
    } else {
      sellOrders.erase(oldprice);
    }
  }

  orderMap.erase(found);
  Order newOrder(type, newprice, oldQty, side);
  addOrder(newOrder);
}

void Orderbook::printDepth() const {
  std::cout << "Buy Orders: \n";
  for (const auto &price_list : buyOrders) {
    std::cout << "Price: " << price_list.first << " ";
    for (const auto &order : price_list.second) {
      std::cout << "Order: " << order.getRemainingQuantity() << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\nSell Orders: \n";
  for (const auto &price_list : sellOrders) {
    std::cout << "Price: " << price_list.first << " ";
    for (const auto &order : price_list.second) {
      std::cout << "Order: " << order.getRemainingQuantity() << " ";
    }
    std::cout << "\n";
  }
}
