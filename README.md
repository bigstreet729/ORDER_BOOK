# Limit Order Book Matching Engine (C++)

This project implements a realistic, high-performance Limit Order Book Matching Engine in C++, suitable for trading simulations, backtesting strategies, or learning how modern exchanges work. The engine supports Limit Orders, Market Orders, Modify Orders, and Cancel Orders. All matching is handled using Price-Time Priority, which mimics real-world order matching behavior and ensures fairness in execution.

The engine is designed for high performance. Efficient use of STL containers allows it to process approximately 1,000,000 orders per second on modern hardware. It supports the full lifecycle of orders, including submission, cancellation, and modification, for both BUY and SELL sides of the book.

The order book is internally represented using a `std::map<Price, std::list<Order>>`, which maintains price levels in sorted order and allows O(log n) access for insertion and traversal. Each order is tracked using a `std::unordered_map<OrderID, {Price, iterator}>` that provides O(1) access to any existing order, enabling efficient cancellation and modification.

To compile the project, use the following command:
g++ main.cpp Orderbook.cpp Order.cpp TimeUtils.cpp -o orderbook 
./orderbook



Sample output demonstrating order placement, trade execution, and final order book state can be found in the `Output` file provided with the project.

The code is organized into multiple files for clarity and modularity:
- `main.cpp`: Entry point of the program
- `Order.cpp` / `Order.h`: Contains the order structure and helper functions
- `Orderbook.cpp` / `Orderbook.h`: Implements the core matching engine logic
- `TimeUtils.cpp` / `TimeUtils.h`: Utility functions for time-stamping and logging
- `Output`: A sample file showing typical output from a sample session

The project demonstrates core concepts of market microstructure, such as how price-time priority ensures fairness, how STL containers like `map` and `unordered_map` can be leveraged for performance, and how to structure a matching engine using clean, modular C++ code. It can serve as a solid base for anyone building a trading infrastructure simulation, generating synthetic high-frequency data from order events, or studying the internal workings of an exchange.

This engine is ideal for:
- Simulating trading and exchange environments
- Generating and replaying heavy tick-level synthetic data
- Learning how matching engines operate
- Benchmarking execution logic for latency and throughput
- Building foundational components for algorithmic or high-frequency trading systems
