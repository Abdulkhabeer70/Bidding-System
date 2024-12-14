#include <iostream>
#include <string>
using namespace std;

// Node structure for singly linked list
struct node
{
    int itemId;
    int bidderId;
    int amount;
    node *next;

    node()
    {
        itemId = bidderId = amount = 0;
        next = nullptr;
    }

    node(int i, int b, int a)
    {
        itemId = i;
        bidderId = b;
        amount = a;
        next = nullptr;
    }
};

// Linked list to store sales history
class list
{
private:
    node *start;
    int size;

public:
    list()
    {
        start = nullptr;
        size = 0;
    }

    ~list()
    {
        node *ptr = start;
        while (start != nullptr)
        {
            start = start->next;
            delete ptr;
            ptr = start;
        }
    }

    void insert(int itemId, int bidderId, int amount)
    {
        node *p = new node(itemId, bidderId, amount);
        p->next = start;
        start = p;
        size++;
    }

    void print()
    {
        node *ptr = start;
        if (!ptr)
        {
            cout << "No sales history available." << endl;
            return;
        }
        while (ptr != nullptr)
        {
            cout << "Item ID: " << ptr->itemId << ", Bidder ID: " << ptr->bidderId
                 << ", Amount: $" << ptr->amount << endl;
            ptr = ptr->next;
        }
    }
};

// Bidder structure
struct Bidder
{
    int id;
    string name;

    Bidder()
    {
        id = 0;
        name = "";
    }

    Bidder(int i, string n)
    {
        id = i;
        name = n;
    }
};

// Item structure
struct Item
{
    int id;
    string name;
    int ownerId;
    int highestBid;
    int highestBidderId;

    Item()
    {
        id = ownerId = highestBid = highestBidderId = 0;
        name = "";
    }

    Item(int i, string n, int o)
    {
        id = i;
        name = n;
        ownerId = o;
        highestBid = 0;
        highestBidderId = -1;
    }
};

// Bidding system class
class BiddingSystem
{
private:
    Bidder bidders[10];
    Item items[10];
    list salesHistory;
    int bidderCount;
    int itemCount;
    const double platformFeePercentage = 5.0;

public:
    BiddingSystem()
    {
        bidderCount = 0;
        itemCount = 0;
    }

    void addBidder(int id, string name)
    {
        if (bidderCount < 10)
        {
            bidders[bidderCount++] = Bidder(id, name);
        }
        else
        {
            cout << "Bidder limit reached." << endl;
        }
    }

    void addItem(int id, string name, int ownerId)
    {
        if (itemCount < 10)
        {
            items[itemCount++] = Item(id, name, ownerId);
        }
        else
        {
            cout << "Item limit reached." << endl;
        }
    }

    void placeBid(int itemId, int bidderId, int amount)
    {
        Item *item = nullptr;
        Bidder *bidder = nullptr;

        for (int i = 0; i < itemCount; i++)
        {
            if (items[i].id == itemId)
            {
                item = &items[i];
                break;
            }
        }

        for (int i = 0; i < bidderCount; i++)
        {
            if (bidders[i].id == bidderId)
            {
                bidder = &bidders[i];
                break;
            }
        }

        if (!item)
        {
            cout << "Item ID " << itemId << " does not exist." << endl;
            return;
        }
        if (!bidder)
        {
            cout << "Bidder ID " << bidderId << " does not exist." << endl;
            return;
        }

        if (amount > item->highestBid)
        {
            item->highestBid = amount;
            item->highestBidderId = bidderId;
            cout << "Bid placed successfully by " << bidder->name << " on item "
                 << item->name << " for $" << amount << "." << endl;
        }
        else
        {
            cout << "Bid amount must be higher than the current highest bid of $"
                 << item->highestBid << "." << endl;
        }
    }

    void finalizeSale(int itemId)
    {
        Item *item = nullptr;

        for (int i = 0; i < itemCount; i++)
        {
            if (items[i].id == itemId)
            {
                item = &items[i];
                break;
            }
        }

        if (!item)
        {
            cout << "Item ID " << itemId << " does not exist." << endl;
            return;
        }

        if (item->highestBid == 0)
        {
            cout << "No bids placed for item " << item->name << "." << endl;
            return;
        }

        double platformFee = (item->highestBid * platformFeePercentage) / 100;
        double earnings = item->highestBid - platformFee;

        salesHistory.insert(itemId, item->highestBidderId, item->highestBid);

        cout << "Item " << item->name << " sold to Bidder ID " << item->highestBidderId
             << " for $" << item->highestBid << "." << endl;
        cout << "Platform fee: $" << platformFee
             << ". Earnings for the owner: $" << earnings << "." << endl;

        // Reset item for future bidding
        item->highestBid = 0;
        item->highestBidderId = -1;
    }

    void displaySalesHistory()
    {
        salesHistory.print();
    }
};

int main()
{
    BiddingSystem system;

    system.addBidder(1, "Alice");
    system.addBidder(2, "Bob");

    system.addItem(101, "Painting", 1);
    system.addItem(102, "Vase", 2);

    system.placeBid(101, 1, 100);
    system.placeBid(101, 2, 150);
    system.placeBid(102, 1, 200);

    system.finalizeSale(101);
    system.finalizeSale(102);

    system.displaySalesHistory();

    return 0;
}
