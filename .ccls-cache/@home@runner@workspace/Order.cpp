#include "Order.h"


static OrderId global_order_id = 1;


Order :: Order(OrderType ordertype, Price price, Quantity quantity, Side side) {
    ordertype_ = ordertype;
    price_ = price;
    quantity_ = quantity;
    side_ = side;
    initial_quantity_ = quantity;
    remaining_quantity_ = quantity;
    orderId_ = global_order_id++;
    timestamp_ = std::chrono::system_clock::now();
}
Side  Order::getSide() const { return side_; }
timestamp Order::gettimestamp() const { return timestamp_; }
OrderType Order::getordertype() const { return ordertype_; }
Price Order::getPrice() const { return price_; }
OrderId Order::getOrderId() const { return orderId_; }
Quantity Order::getRemainingQuantity() const { return remaining_quantity_; }
Quantity Order::getInitialQuantity() const { return initial_quantity_; }
void Order::reduceQuantity(Quantity qty){ remaining_quantity_-=qty;}
