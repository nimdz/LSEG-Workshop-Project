

# Order Matching System

This project consists of two main C++ programs that handle client orders, match buy and sell orders, and generate execution reports. The system uses a linked list to store execution details and a stack to manage client orders.

## Files

- `main.cpp`: Contains the core logic for reading orders from a CSV file, processing them, and generating execution reports.
- `temp.cpp`: Contains additional functionality for managing client orders using a stack and creating execution reports.

## Features

- Reads orders from an `order.csv` file.
- Matches buy and sell orders based on instrument and price.
- Generates execution reports and saves them to `execution_rep.csv`.
- Manages client orders using a stack.

## How to Run

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/nimdz/LSEG-Workshop-Project
   ```

2. **Compile the Code:**
   ```bash
   g++ main.cpp -o main
   g++ temp.cpp -o temp
   ```

3. **Prepare the Input File:**
   - Create an `order.csv` file with the following format:
     ```
     clientOrderID,instrument,side,quantity,price
     ```

4. **Run the Programs:**

   To execute the main logic:
   ```bash
   ./main
   ```

   To run additional functionality:
   ```bash
   ./temp
   ```

5. **Check the Output:**
   - Execution details will be saved to `execution_rep.csv`.

## File Structure

- `main.cpp`: Main source code for order processing and execution report generation.
- `temp.cpp`: Additional source code for stack-based client order management and report generation.
- `order.csv`: Input file with order details.
- `execution_rep.csv`: Output file with execution reports.

## License

This project is open-source and available under the MIT License.

---

This `README.md` provides an overview of both `main.cpp` and `temp.cpp`, explaining how they work together and how to run them. Adjust any specifics as needed for your project.