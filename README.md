🧮 Limit Order Book Matching Engine (C++)
This project implements a realistic, high-performance Limit Order Book Matching Engine in C++, suitable for trading simulations, backtesting strategies, or learning how modern exchanges work.

The engine supports:
Limit Orders
Market Orders
Modify Orders
Cancel Orders
All matching is handled using Price-Time Priority, which mimics real-world order matching behavior.

Features
Price-Time Priority Matching
Ensures fairness in execution, prioritizing best price and earliest time.
Buy/Sell Order Support
Handles both sides of the book with full order life cycle:
Order submission
Order cancellation
Order modification
High Performance
Efficient data structures allow the engine to process ~1,000,000 orders/second on typical modern CPUs.
Underlying Data Structures
std::map<Price, std::list<Order>>
Used for the order book Provides O(log n) access to price levels
Maintains sorted price levels naturally
std::unordered_map<OrderID, {Price, iterator}> Tracks orders for fast lookup during cancellation/modification Allows O(1) access to any order by its ID
Running the Project
Compile

g++ main.cpp Orderbook.cpp Order.cpp TimeUtils.cpp -o orderbook
Run
bash
Copy
Edit
./orderbook
📂 Sample Output
Sample output demonstrating trades, order matches, and orderbook state is available in the Output file.

File Structure
├── main.cpp          # Entry point
├── Order.cpp/.h      # Order structure and logic
├── Orderbook.cpp/.h  # Core matching engine
├── TimeUtils.cpp/.h  # Utilities for timestamping/logging
├── Output            # Sample output of engine
Concepts Demonstrated
Realistic market microstructure
Efficient order matching using STL containers
Clean C++ OOP design with modular components
Time complexity optimizations for fast execution
💡 Ideal For
Trading infrastructure simulations, Good source of heavy tick data based on orders placing
Learning exchange-level matching logic
Benchmarking engine performance
Building a foundation for HFT / execution strategies
