#include <iostream>
#include <fstream>
#include <string>

class ExecutionRepDetails
{
public:
    std::string orderID;
    std::string clientOrderID;
    std::string instrument;
    int side;
    std::string executionStatus;
    int quantity;
    double price;
    ExecutionRepDetails *previous;
    ExecutionRepDetails *next;

    ExecutionRepDetails(std::string oID, std::string cID, std::string inst, int s, std::string status, int qty, double pr)
    {
        orderID = oID;
        clientOrderID = cID;
        instrument = inst;
        side = s;
        executionStatus = status;
        quantity = qty;
        price = pr;
        previous = nullptr;
        next = nullptr;
    }
};

// Function to add ExecutionRepDetails to the linked list
void addToLinkedList(ExecutionRepDetails *&head, std::string oID, std::string cID, std::string inst, int s, std::string status, int qty, double pr)
{
    ExecutionRepDetails *newNode = new ExecutionRepDetails(oID, cID, inst, s, status, qty, pr);

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
        newNode->previous = temp;
    }
}

// Function to create a linked list of ExecutionRepDetails
ExecutionRepDetails *createLinkedList()
{
    ExecutionRepDetails *head = nullptr;

    addToLinkedList(head, "123", "456", "Instrument A", 1, "Executed", 10, 99.99);
    addToLinkedList(head, "789", "012", "Instrument B", 0, "Pending", 5, 50.0);
    addToLinkedList(head, "345", "678", "Instrument C", 1, "Rejected", 20, 199.99);

    return head;
}

// Function to write data to a CSV file
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

    const ExecutionRepDetails *temp = head;
    while (temp != nullptr)
    {
        file << temp->orderID << ","
             << temp->clientOrderID << ","
             << temp->instrument << ","
             << temp->side << ","
             << temp->executionStatus << ","
             << temp->quantity << ","
             << temp->price << "\n";

        temp = temp->next;
    }

    file.close(); // Close the file
    std::cout << "Data has been written to the file: " << filename << std::endl;
}

// Function to free the memory of the linked list
void freeLinkedList(ExecutionRepDetails *&head)
{
    ExecutionRepDetails *temp = head;
    while (temp != nullptr)
    {
        ExecutionRepDetails *current = temp;
        temp = temp->next;
        delete current;
    }
    head = nullptr;
}

int main()
{
    ExecutionRepDetails *head = createLinkedList();

    // Write the linked list data to a CSV file
    std::string filename = "execution_details.csv";
    writeToCSV(filename, head);

    // Free the memory of the linked list
    freeLinkedList(head);

    return 0;
}
