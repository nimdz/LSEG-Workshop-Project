#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ClientDetails
{
public:
    string orderID;
    string clientOrderID;
    string instrument;
    int side;
    int quantity;
    double price;
    ClientDetails *previous;

    // Constructor
    ClientDetails(string oID, string cID, string inst, int s, int qty, double pr)
    {
        orderID = oID;
        clientOrderID = cID;
        instrument = inst;
        side = s;
        quantity = qty;
        price = pr;
        previous = nullptr;
    }
};

class ExecutionRepDetails
{
public:
    string orderID;
    string clientOrderID;
    string instrument;
    int side;
    string executionStatus;
    int quantity;
    double price;
    ExecutionRepDetails *next;

    ExecutionRepDetails(string oID, string cID, string inst, int s, string status, int qty, double pr)
    {
        orderID = oID;
        clientOrderID = cID;
        instrument = inst;
        side = s;
        executionStatus = status;
        quantity = qty;
        price = pr;
        next = nullptr;
    }
};

ExecutionRepDetails *head = nullptr;

void addToExecutionRepDetailsLinkedList(ExecutionRepDetails *&head, ExecutionRepDetails *newNode)
{
    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        ExecutionRepDetails *temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->next = nullptr;
    }
}

void writeToCSV(const std::string &filename, const ExecutionRepDetails *head)
{
    std::ofstream file(filename); // Open the file

    if (!file.is_open()) // Check if the file was opened successfully
    {
        std::cout << "Failed to open the file: " << filename << std::endl;
        return;
    }

    // Write the CSV header
    file << "Order ID,Client Order ID,Instrument,Side,Execution Status,Quantity,Price\n";

    const ExecutionRepDetails *temp2 = head;
    while (temp2 != nullptr)
    {
        file << temp2->orderID << ","
             << temp2->clientOrderID << ","
             << temp2->instrument << ","
             << temp2->side << ","
             << temp2->executionStatus << ","
             << temp2->quantity << ","
             << temp2->price << "\n";

        temp2 = temp2->next;
    }

    file.close(); // Close the file
    std::cout << "Data has been written to the file: " << filename << std::endl;
}

// Function to free the memory of the ExecutionRepDetails linked list
void freeLinkedList(ExecutionRepDetails *&head)
{
    ExecutionRepDetails *temp2 = head;
    while (temp2 != nullptr)
    {
        ExecutionRepDetails *current = temp2;
        temp2 = temp2->next;
        delete current;
    }
    head = nullptr;
}

// Stack class
class Stack
{
private:
    ClientDetails *top;

public:
    // Constructor
    Stack()
    {
        top = nullptr;
    }

    // Push a new element to the top of the stack
    void push(string oID, string cID, string inst, int s, int qty, double pr)
    {
        ClientDetails *newNode = new ClientDetails(oID, cID, inst, s, qty, pr);
        newNode->previous = top;
        top = newNode;
        cout << "Client order ID " << cID << " pushed to the stack." << endl;
    }

    // Pop the top element from the stack
    void pop()
    {
        if (top == nullptr)
        {
            cout << "The stack is empty." << endl;
            return;
        }

        ClientDetails *temp = top;
        top = top->previous;
        string poppedOrderID = temp->clientOrderID;
        delete temp;
        cout << "Client order ID " << poppedOrderID << " popped from the stack." << endl;
    }

    // Remove an element from the stack based on client order ID, instrument, and quantity
    void remove(string cID, string inst, int qty)
    {
        if (top == nullptr)
        {
            cout << "The stack is empty." << endl;
            return;
        }

        ClientDetails *current = top;
        ClientDetails *previous = nullptr;

        while (current != nullptr)
        {
            if (current->clientOrderID == cID && current->instrument == inst && current->quantity == qty)
            {
                if (previous == nullptr)
                {
                    top = current->previous;
                }
                else
                {
                    previous->previous = current->previous;
                }

                cout << "Client order ID " << cID << " removed from the stack." << endl;
                delete current;
                return;
            }

            previous = current;
            current = current->previous;
        }

        cout << "Element not found in the stack." << endl;
    }

    // Search for an element in the stack based on instrument and price
    ClientDetails *search(string inst, double pr)
    {
        ClientDetails *current = top;
        while (current != nullptr)
        {
            if (current->instrument == inst && current->price == pr)
            {
                return current;
            }
            current = current->previous;
        }
        return nullptr; // Element not found
    }

    // Print the elements of the stack
    void printStack()
    {
        if (top == nullptr)
        {
            cout << "The stack is empty." << endl;
            return;
        }

        cout << "Elements in the stack:" << endl;
        ClientDetails *current = top;
        while (current != nullptr)
        {
            cout << "Client Order ID: " << current->clientOrderID << endl;
            cout << "Instrument: " << current->instrument << endl;
            cout << "Side: " << current->side << endl;
            cout << "Quantity: " << current->quantity << endl;
            cout << "Price: " << current->price << endl;
            cout << endl;
            current = current->previous;
        }
    }
};

void ExchangeCalc()
{
    Stack buyer;
    Stack seller;

    ifstream ip("order.csv");

    if (!ip.is_open())
        cout << "ERROR: File Open" << '\n';

    int orderCount = 1;

    while (ip.good())
    {
        string clientOrderID;
        string instrument;
        string sSide;
        string sPrice;
        string sQuantity;

        getline(ip, clientOrderID, ',');
        getline(ip, instrument, ',');
        getline(ip, sSide, ',');
        getline(ip, sQuantity, ',');
        getline(ip, sPrice, '\n');

        int side = 0;
        int quantity = 0;
        double price = 0.0;
        try
        {
            side = stoi(sSide);
            quantity = stoi(sQuantity);
            price = stod(sPrice);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid price value: " << sPrice << std::endl;
            // Handle the error, such as skipping the current line or exiting the function
            continue; // Skip to the next iteration
        }

        string orderID = "ord" + to_string(orderCount);
        orderCount++;

        if (side == 1)
        {
            buyer.push(orderID, clientOrderID, instrument, side, quantity, price);

            ClientDetails *temp = seller.search(instrument, price);

            while (temp != nullptr && buyer.search(instrument, price)->quantity > 0)
            {
                if (temp != nullptr && instrument == temp->instrument && quantity == temp->quantity && price >= temp->price)
                {
                    ExecutionRepDetails *buyerExecution = new ExecutionRepDetails(orderID, clientOrderID, instrument, side, "Fill", quantity, price);
                    ExecutionRepDetails *sellerExecution = new ExecutionRepDetails(temp->orderID, temp->clientOrderID, instrument, temp->side, "Fill", quantity, price);

                    addToExecutionRepDetailsLinkedList(head, buyerExecution);
                    addToExecutionRepDetailsLinkedList(head, sellerExecution);

                    buyer.pop();
                    seller.remove(temp->clientOrderID, temp->instrument, temp->quantity);

                    cout << "Buyer and seller orders filled completely." << endl;
                }
                // Check if the top buyer's quantity is less than the seller's quantity
                else if (temp != nullptr && instrument == temp->instrument && quantity < temp->quantity && price >= temp->price)
                {
                    ExecutionRepDetails *buyerExecution = new ExecutionRepDetails(orderID, clientOrderID, instrument, side, "Fill", quantity, price);
                    ExecutionRepDetails *sellerExecution = new ExecutionRepDetails(temp->orderID, temp->clientOrderID, instrument, temp->side, "Pfill", quantity, price);

                    addToExecutionRepDetailsLinkedList(head, buyerExecution);
                    addToExecutionRepDetailsLinkedList(head, sellerExecution);

                    temp->quantity -= quantity;
                    buyer.pop();

                    cout << "Buyer order partially filled." << endl;
                }
                else if (temp != nullptr && instrument == temp->instrument && quantity > temp->quantity && price >= temp->price)
                {
                    ExecutionRepDetails *buyerExecution = new ExecutionRepDetails(orderID, clientOrderID, instrument, side, "Pfill", quantity, price);
                    ExecutionRepDetails *sellerExecution = new ExecutionRepDetails(temp->orderID, temp->clientOrderID, instrument, temp->side, "Fill", quantity, price);

                    seller.remove(temp->clientOrderID, temp->instrument, temp->quantity);
                    buyer.search(instrument, price)->quantity -= temp->quantity;

                    addToExecutionRepDetailsLinkedList(head, buyerExecution);
                    addToExecutionRepDetailsLinkedList(head, sellerExecution);
                }
                else
                {
                    ExecutionRepDetails *buyerExecution = new ExecutionRepDetails(orderID, clientOrderID, instrument, side, "New", quantity, price);
                    addToExecutionRepDetailsLinkedList(head, buyerExecution);
                }
            }
        }
        else if (side == 2)
        {
            seller.push(orderID, clientOrderID, instrument, side, quantity, price);

            ClientDetails *temp = buyer.search(instrument, price);

            while (temp != nullptr && seller.search(instrument, price)->quantity > 0)
            {
                if (temp != nullptr && instrument == temp->instrument && quantity == temp->quantity && price <= temp->price)
                {
                    ExecutionRepDetails *sellerExecution = new ExecutionRepDetails(orderID, clientOrderID, instrument, side, "Fill", quantity, price);
                    ExecutionRepDetails *buyerExecution = new ExecutionRepDetails(temp->orderID, temp->clientOrderID, instrument, temp->side, "Fill", quantity, price);

                    addToExecutionRepDetailsLinkedList(head, sellerExecution);
                    addToExecutionRepDetailsLinkedList(head, buyerExecution);

                    buyer.pop();
                    seller.remove(temp->clientOrderID, temp->instrument, temp->quantity && price <= temp->price);

                    cout << "Buyer and seller orders filled completely." << endl;
                }
                // Check if the top buyer's quantity is less than the seller's quantity
                else if (temp != nullptr && instrument == temp->instrument && quantity < temp->quantity && price <= temp->price)
                {
                    ExecutionRepDetails *sellerExecution = new ExecutionRepDetails(orderID, clientOrderID, instrument, side, "Fill", quantity, price);
                    ExecutionRepDetails *buyerExecution = new ExecutionRepDetails(temp->orderID, temp->clientOrderID, instrument, temp->side, "Pfill", quantity, price);

                    addToExecutionRepDetailsLinkedList(head, sellerExecution);
                    addToExecutionRepDetailsLinkedList(head, buyerExecution);

                    temp->quantity -= quantity;
                    buyer.pop();

                    cout << "Seller order partially filled." << endl;
                }
                else if (temp != nullptr && instrument == temp->instrument && quantity > temp->quantity)
                {
                    ExecutionRepDetails *sellerExecution = new ExecutionRepDetails(orderID, clientOrderID, instrument, side, "Pfill", quantity, price);
                    ExecutionRepDetails *buyerExecution = new ExecutionRepDetails(temp->orderID, temp->clientOrderID, instrument, temp->side, "Fill", quantity, price);

                    seller.remove(temp->clientOrderID, temp->instrument, temp->quantity);
                    buyer.search(instrument, price)->quantity -= temp->quantity;

                    addToExecutionRepDetailsLinkedList(head, sellerExecution);
                    addToExecutionRepDetailsLinkedList(head, buyerExecution);
                }
                else
                {
                    ExecutionRepDetails *sellerExecution = new ExecutionRepDetails(orderID, clientOrderID, instrument, side, "New", quantity, price);
                    addToExecutionRepDetailsLinkedList(head, sellerExecution);
                }
            }
        }
    }

    string fileName = "execution_rep.csv";
    writeToCSV(fileName, head);

    freeLinkedList(head);

    buyer.printStack();
    seller.printStack();
}

// Example usage
int main()
{

    ExchangeCalc();

    return 0;
}
