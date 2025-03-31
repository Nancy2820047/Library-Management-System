// LibraFlow.cpp
// Name: Nancy Ahmed Mohamed
// ID: 231000470
// DATE: 11/28/2024
// LIBRARY MANAGEMENT SYSTEM

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>
using namespace std;

class Book {
protected:
    string title;
    string author;
    string ISBN;

public:
    Book(string t, string a, string i) : title(t), author(a), ISBN(i) {}
    virtual ~Book() = default;  // Virtual destructor for polymorphism

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return ISBN; }
};

class Fiction : public Book {
public:
    Fiction(string t, string a, string i) : Book(t, a, i) {}
};

class NonFiction : public Book {
public:
    NonFiction(string t, string a, string i) : Book(t, a, i) {}
};

class RecentlyAddedStack {
    struct Node {
        Book* book;
        Node* next;
        Node(Book* b) : book(b), next(nullptr) {}
    };

    Node* top;
    int count;
    const int MAX_SIZE = 5;

    // Disable copying
    RecentlyAddedStack(const RecentlyAddedStack&) = delete;
    RecentlyAddedStack& operator=(const RecentlyAddedStack&) = delete;

public:
    RecentlyAddedStack() : top(nullptr), count(0) {}

    ~RecentlyAddedStack() {
        while (top != nullptr) {
            popBook();
        }
    }

    void pushBook(Book* book) {
        if (count == MAX_SIZE) {
            popBook();
        }
        Node* newNode = new Node(book);
        newNode->next = top;
        top = newNode;
        count++;
    }

    void popBook() {
        if (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp->book;  // Free book memory
            delete temp;
            count--;
        }
    }

    void displayRecentBooks() const {
        Node* current = top;
        if (current == nullptr) {
            cout << "No recently added books.\n";
            return;
        }
        cout << "===== Recently Added Books =====\n";
        while (current != nullptr) {
            cout << "- " << current->book->getTitle()
                 << " by " << current->book->getAuthor()
                 << " (ISBN: " << current->book->getISBN() << ")\n";
            current = current->next;
        }
    }
};

class BorrowQueue {
    struct Node {
        Book* book;
        Node* next;
        Node(Book* b) : book(b), next(nullptr) {}
    };

    Node* front;
    Node* rear;
    int count;
    const int MAX_SIZE = 10;

    // Disable copying
    BorrowQueue(const BorrowQueue&) = delete;
    BorrowQueue& operator=(const BorrowQueue&) = delete;

public:
    BorrowQueue() : front(nullptr), rear(nullptr), count(0) {}

    ~BorrowQueue() {
        while (front != nullptr) {
            dequeueBook();
        }
    }

    void enqueueBook(Book* book) {
        if (count == MAX_SIZE) {
            dequeueBook();
        }
        Node* newNode = new Node(book);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }

    void dequeueBook() {
        if (front != nullptr) {
            Node* temp = front;
            front = front->next;
            if (front == nullptr) {
                rear = nullptr;
            }
            delete temp->book;  // Free book memory
            delete temp;
            count--;
        }
    }

    void displayBorrowedBooks() const {
        Node* current = front;
        if (current == nullptr) {
            cout << "No borrowed books.\n";
            return;
        }
        cout << "===== Borrowed Books History =====\n";
        while (current != nullptr) {
            cout << "- " << current->book->getTitle()
                 << " (ISBN: " << current->book->getISBN() << ")\n";
            current = current->next;
        }
    }
};

class Library {
    vector<Book*> books;
    RecentlyAddedStack recentlyAddedStack;
    BorrowQueue borrowQueue;

public:
    ~Library() {
        for (auto& book : books) {
            delete book;
        }
    }

    void addBook(string type, string title, string author, string ISBN) {
        // Check for duplicate ISBN
        for (const auto& book : books) {
            if (book->getISBN() == ISBN) {
                cout << "Error: ISBN already exists!\n";
                return;
            }
        }

        // Convert type to lowercase
        transform(type.begin(), type.end(), type.begin(), ::tolower);

        Book* newBook = nullptr;
        if (type == "fiction") {
            newBook = new Fiction(title, author, ISBN);
        } else if (type == "nonfiction") {
            newBook = new NonFiction(title, author, ISBN);
        } else {
            cout << "Invalid book type!\n";
            return;
        }

        books.push_back(newBook);
        recentlyAddedStack.pushBook(newBook);
        cout << "Book added successfully.\n";
    }

    void removeBook(string ISBN) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if ((*it)->getISBN() == ISBN) {
                delete *it;  // Free memory
                books.erase(it);
                cout << "Book with ISBN " << ISBN << " removed.\n";
                return;
            }
        }
        cout << "Error: Book not found!\n";
    }

    void borrowBook(string ISBN) {
        for (auto& book : books) {
            if (book->getISBN() == ISBN) {
                borrowQueue.enqueueBook(book);
                cout << "Book '" << book->getTitle() << "' borrowed.\n";
                return;
            }
        }
        cout << "Error: Book not found!\n";
    }

    Book* searchBook(string key) const {
        for (auto& book : books) {
            if (book->getISBN() == key || book->getTitle() == key) {
                return book;
            }
        }
        return nullptr;
    }

    void viewRecentlyAddedBooks() const {
        recentlyAddedStack.displayRecentBooks();
    }

    void viewBorrowingHistory() const {
        borrowQueue.displayBorrowedBooks();
    }
};
int getIntegerInput() {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a number: ";
    }
    cin.ignore();
    return value;
}


int main() {
    Library library;
    int choice;
    string type, title, author, ISBN;

    do {
        cout << "\n===== LibraFlow Library System =====\n";
        cout << "1. Add a book\n";
        cout << "2. Borrow a book\n";
        cout << "3. Remove a book by ISBN\n";
        cout << "4. Search for a book\n";
        cout << "5. View Recently Added Books\n";
        cout << "6. View Borrowing History\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        choice = getIntegerInput();

        switch (choice) {
            case 1: {
                cout << "Enter book type (Fiction/NonFiction): ";
                getline(cin, type);
                cout << "Enter title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter ISBN: ";
                getline(cin, ISBN);
                library.addBook(type, title, author, ISBN);
                break;
            }

            case 2: {
                cout << "Enter ISBN to borrow: ";
                getline(cin, ISBN);
                library.borrowBook(ISBN);
                break;
            }

            case 3: {
                cout << "Enter ISBN to remove: ";
                getline(cin, ISBN);
                library.removeBook(ISBN);
                break;
            }

            case 4: {
                cout << "Enter title/ISBN to search: ";
                getline(cin, ISBN);
                Book* foundBook = library.searchBook(ISBN);
                if (foundBook) {
                    cout << "Found: " << foundBook->getTitle()
                         << " by " << foundBook->getAuthor()
                         << " (ISBN: " << foundBook->getISBN() << ")\n";
                } else {
                    cout << "Book not found!\n";
                }
                break;
            }

            case 5:
                library.viewRecentlyAddedBooks();
                break;

            case 6:
                library.viewBorrowingHistory();
                break;

            case 7:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 7);

    return 0;
}