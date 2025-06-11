#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

enum Side {
    Buy,
    Sell,
};

enum OrderType {
    LimitOrder,
    MarketOrder,
    CancelOrder,
    ModifyOrder,
};

using Price = std::uint32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint32_t;
using timestamp = std::chrono::system_clock::time_point;

OrderId global_order_id = 1;

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
    Order(OrderType ordertype, Price price, Quantity quantity, Side side) {
        ordertype_ = ordertype;
        price_ = price;
        quantity_ = quantity;
        side_ = side;
        initial_quantity_ = quantity;
        remaining_quantity_ = quantity;
        orderId_ = global_order_id++;
        timestamp_ = std::chrono::system_clock::now();
    }

    Side getSide() const { return side_; }
    timestamp gettimestamp() const { return timestamp_; }
    OrderType getordertype() const { return ordertype_; }
    Price getPrice() const { return price_; }
    OrderId getOrderId() const { return orderId_; }
    Quantity getRemainingQuantity() const { return remaining_quantity_; }
    Quantity getInitialQuantity() const { return initial_quantity_; }

    void reduceQuantity(Quantity qty) { remaining_quantity_ -= qty; }
};

class Orderbook {
private:
    std::map<Price, std::list<Order>, std::greater<Price>> buyOrders;
    std::map<Price, std::list<Order>> sellOrders;
    std::unordered_map<OrderId, std::pair<Price, std::list<Order>::iterator>> orderMap;

public:
    void addOrder(const Order &order) {
        std::cout << (order.getSide() == Buy ? "[ BUY ] " : "[ SELL ] ")
                  << order.getPrice() << " x " << order.getInitialQuantity() << std::endl;

        switch (order.getordertype()) {
            case LimitOrder:
                matchOrder(order, true);
                break;
            case MarketOrder:
                matchOrder(order, false);
                break;
            case ModifyOrder:
                modifyOrder(order.getOrderId(), order.getPrice(), order.getInitialQuantity());
                break;
            case CancelOrder:
                cancelOrder(order.getOrderId());
                break;
        }
    }

    void cancelOrder(OrderId orderid) {
        auto found = orderMap.find(orderid);
        if (found == orderMap.end()) {
            std::cout << " [ CANCEL FAILED ] OrderId " << orderid << " not found\n";
            return;
        }

        Price price = found->second.first;
        auto it = found->second.second;
        Side side = it->getSide();

        std::cout << " [ CANCEL ] OrderId " << orderid << " Price: " << price
                  << " Qty: " << it->getRemainingQuantity() << "\n";

        if (side == Buy) {
            auto &lst = buyOrders[price];
            lst.erase(it);
            if (lst.empty()) buyOrders.erase(price);
        } else {
            auto &lst = sellOrders[price];
            lst.erase(it);
            if (lst.empty()) sellOrders.erase(price);
        }

        orderMap.erase(found);
    }

    void matchOrder(Order order, bool IsLimit) {
        if (order.getSide() == Buy) {
            for (auto it = sellOrders.begin();
                 it != sellOrders.end() && order.getRemainingQuantity() > 0;) {

                Price sellPrice = it->first;
                if (order.getPrice() < sellPrice && IsLimit) break;

                auto &sellList = it->second;
                for (auto sellIt = sellList.begin();
                     sellIt != sellList.end() && order.getRemainingQuantity() > 0;) {

                    Quantity matchQty = std::min(order.getRemainingQuantity(),
                                                 sellIt->getRemainingQuantity());

                    std::cout << " [ TRADE ]  BUY @" << sellPrice << " Qty : " << matchQty << " | "
                              << order.getOrderId() << " is [Mathced with " << sellIt->getOrderId() << "\n";

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
                if (order.getPrice() > buyPrice) break;

                auto &buyList = it->second;
                for (auto buyIt = buyList.begin();
                     buyIt != buyList.end() && order.getRemainingQuantity();) {

                    Quantity matchQty = std::min(order.getRemainingQuantity(),
                                                 buyIt->getRemainingQuantity());

                    std::cout << " [ TRADE ]  SELL @" << buyPrice << " Qty :" << matchQty << " | "
                              << order.getOrderId() << " is Matched with " << buyIt->getOrderId() << "\n";

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

    void modifyOrder(OrderId id, Price newprice, Quantity newQty) {
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

        std::cout << " [ MODIFY ] OrderId " << id << " -> NEW PRICE " << newprice
                  << ", New qty : " << newQty << "\n";

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
        Order newOrder(type, newprice, newQty, side);
        addOrder(newOrder);
    }

    void printDepth() const {
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
};

int main() {
    Orderbook ob;

    std::cout << "\n=== Test: Price-Time Priority ===\n";

    // Older buy orders (OrderId = 1, 2)
    ob.addOrder(Order(LimitOrder, 100, 5, Buy));   // OrderId 1
    ob.addOrder(Order(LimitOrder, 101, 5, Buy));   // OrderId 2

    // Newer buy order at better price (OrderId = 3)
    ob.addOrder(Order(LimitOrder, 102, 5, Buy));   // OrderId 3

    // Older sell orders (OrderId = 4, 5)
    ob.addOrder(Order(LimitOrder, 105, 4, Sell));  // OrderId 4
    ob.addOrder(Order(LimitOrder, 104, 4, Sell));  // OrderId 5

    // Newer sell at better price (OrderId = 6)
    ob.addOrder(Order(LimitOrder, 103, 4, Sell));  // OrderId 6

    // Market order to buy, will match against best sell prices
    ob.addOrder(Order(MarketOrder, 0, 10, Buy));   // Should match OrderId 6 → 5 → 4

    // Market order to sell, will match best buy prices
    ob.addOrder(Order(MarketOrder, 0, 12, Sell));  // Should match OrderId 3 → 2 → 1

    // Add duplicate price buy orders for time priority check
    ob.addOrder(Order(LimitOrder, 99, 6, Buy));    // OrderId 9
    ob.addOrder(Order(LimitOrder, 99, 4, Buy));    // OrderId 10 (later)

    // Matching sell at 98 to hit 99 (OrderId 9 then 10)
    ob.addOrder(Order(LimitOrder, 98, 5, Sell));

    std::cout << "\n=== Final Order Book ===\n";
    ob.printDepth();

    return 0;
}

