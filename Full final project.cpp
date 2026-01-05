#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <limits>

using namespace std;

// ---------------- Structures ----------------
struct Room {
    int roomNo;
    char type[10];
    int price;
    int isBooked;
};

struct Customer {
    char name[30];
    char phone[15];
    int roomNo;
    int days;
    char preference[20];
};

// ---------------- Function Declarations ----------------
bool adminLogin();

void addRoom();
void viewRooms();
void bookRoom();
void editOrCancelBooking();
void availability();
void searchCustomerByName();
void sortRoomsByPrice();
void searchRoomByNumber();
void searchRoomByType();

// ---------------- Admin Login ----------------
bool adminLogin() {
    char user[20], pass[20];

    cout << "===== ADMIN LOGIN =====\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    if (strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0) {
        cout << "Login successful!\n";
        return true;
    } else {
        cout << "Invalid login. Access denied.\n";
        return false;
    }
}

// ---------------- Functions ----------------

// Add Room
void addRoom() {
    Room r;
    ofstream fout("rooms.txt", ios::app);
    if (!fout) { cout << "Error opening rooms file!\n"; return; }

    cout << "\n--- Add New Room ---\n";
    cout << "Enter Room Number: ";
    cin >> r.roomNo;
    cin.ignore();

    cout << "Enter Room Type (AC / Non-AC): ";
    cin.getline(r.type, 10);

    cout << "Enter Price per Day: ";
    cin >> r.price;
    cin.ignore();

    r.isBooked = 0;
    fout << r.roomNo << " " << r.type << " " << r.price << " " << r.isBooked << "\n";
    fout.close();

    cout << "Room added successfully!\n";
}

// View Rooms
void viewRooms() {
    Room r;
    ifstream fin("rooms.txt");
    if (!fin) { cout << "No room records found!\n"; return; }

    cout << "\nRoomNo\tType\tPrice\tStatus\n";
    while (fin >> r.roomNo >> r.type >> r.price >> r.isBooked) {
        cout << r.roomNo << "\t" << r.type << "\t" << r.price << "\t"
             << (r.isBooked ? "Booked" : "Available") << endl;
    }
    fin.close();
}

// Book Room
void bookRoom() {
    Room r;
    Customer c;

    ifstream fin("rooms.txt");
    ofstream temp("temp_rooms.txt");
    ofstream custFile("customers.txt", ios::app);

    if (!fin || !temp || !custFile) {
        cout << "File error!\n";
        return;
    }

    int roomNo;
    bool found = false;

    cout << "Enter Room Number to Book: ";
    cin >> roomNo;
    cin.ignore();

    while (fin >> r.roomNo >> r.type >> r.price >> r.isBooked) {
        if (r.roomNo == roomNo && r.isBooked == 0) {
            cout << "Enter Customer Name: ";
            cin.getline(c.name, 30);

            cout << "Enter Phone: ";
            cin.getline(c.phone, 15);

            cout << "Enter Days of Stay: ";
            cin >> c.days;
            cin.ignore();

            cout << "Enter Preference: ";
            cin.getline(c.preference, 20);

            c.roomNo = roomNo;
            r.isBooked = 1;

            custFile << c.name << " " << c.phone << " " << c.roomNo << " "
                     << c.days << " " << c.preference << "\n";

            int total = r.price * c.days;
            cout << "\n----- BILL -----\n";
            cout << "Customer Name : " << c.name << "\n";
            cout << "Phone         : " << c.phone << "\n";
            cout << "Room No       : " << c.roomNo << "\n";
            cout << "Room Type     : " << r.type << "\n";
            cout << "Days Stayed   : " << c.days << "\n";
            cout << "Price/Day     : " << r.price << "\n";
            cout << "Total Amount  : " << total << "\n";

            found = true;
        }
        temp << r.roomNo << " " << r.type << " " << r.price << " " << r.isBooked << "\n";
    }

    fin.close();
    temp.close();
    custFile.close();

    remove("rooms.txt");
    rename("temp_rooms.txt", "rooms.txt");

    if (!found) cout << "Room not available!\n";
}

// Cancel Booking
void editOrCancelBooking() {
    Room r;
    ifstream fin("rooms.txt");
    ofstream temp("temp_rooms.txt");

    if (!fin || !temp) {
        cout << "Room file not found!\n";
        return;
    }

    int roomNo;
    bool found = false;

    cout << "Enter Room Number to Cancel Booking: ";
    cin >> roomNo;
    cin.ignore();

    while (fin >> r.roomNo >> r.type >> r.price >> r.isBooked) {
        if (r.roomNo == roomNo && r.isBooked == 1) {
            r.isBooked = 0;
            found = true;
        }
        temp << r.roomNo << " " << r.type << " " << r.price << " " << r.isBooked << "\n";
    }

    fin.close();
    temp.close();
    remove("rooms.txt");
    rename("temp_rooms.txt", "rooms.txt");

    if (found) cout << "Booking cancelled successfully!\n";
    else cout << "No booking found for this room!\n";
}

// Availability
void availability() {
    Room r;
    ifstream fin("rooms.txt");
    if (!fin) { cout << "Room file not found!\n"; return; }

    cout << "\n--- Available Rooms ---\n";
    while (fin >> r.roomNo >> r.type >> r.price >> r.isBooked) {
        if (r.isBooked == 0)
            cout << "Room No: " << r.roomNo
                 << " | Type: " << r.type
                 << " | Price: " << r.price << "\n";
    }
    fin.close();
}

// Search Customer by Name
void searchCustomerByName() {
    Customer c;
    ifstream fin("customers.txt");
    if (!fin) { cout << "Customer file not found!\n"; return; }

    char searchName[30];
    cout << "Enter Customer Name to Search: ";
    cin.ignore();
    cin.getline(searchName, 30);

    bool found = false;
    while (fin >> c.name >> c.phone >> c.roomNo >> c.days >> c.preference) {
        if (strcmp(c.name, searchName) == 0) {
            cout << "\n--- Customer Details ---\n";
            cout << "Name: " << c.name << "\nPhone: " << c.phone
                 << "\nRoom No: " << c.roomNo
                 << "\nDays: " << c.days
                 << "\nPreference: " << c.preference << "\n";
            found = true;
            break;
        }
    }

    if (!found) cout << "Customer not found!\n";
    fin.close();
}

// Sort Rooms by Price
void sortRoomsByPrice() {
    Room r[100];
    int count = 0;

    ifstream fin("rooms.txt");
    if (!fin) { cout << "No room records found!\n"; return; }

    while (fin >> r[count].roomNo >> r[count].type >> r[count].price >> r[count].isBooked)
        count++;
    fin.close();

    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < count - i - 1; j++)
            if (r[j].price > r[j + 1].price)
                swap(r[j], r[j + 1]);

    cout << "\n--- Rooms Sorted by Price ---\n";
    for (int i = 0; i < count; i++)
        cout << "Room No: " << r[i].roomNo
             << " | Type: " << r[i].type
             << " | Price: " << r[i].price
             << " | Status: " << (r[i].isBooked ? "Booked" : "Available") << "\n";
}

// Search Room by Number
void searchRoomByNumber() {
    Room r;
    int roomNo;

    cout << "Enter Room Number to Search: ";
    cin >> roomNo;
    cin.ignore();

    ifstream fin("rooms.txt");
    if (!fin) { cout << "No room records found!\n"; return; }

    bool found = false;
    while (fin >> r.roomNo >> r.type >> r.price >> r.isBooked) {
        if (r.roomNo == roomNo) {
            cout << "\nRoom No: " << r.roomNo
                 << "\nType: " << r.type
                 << "\nPrice: " << r.price
                 << "\nStatus: " << (r.isBooked ? "Booked" : "Available") << "\n";
            found = true;
            break;
        }
    }

    if (!found) cout << "Room not found!\n";
    fin.close();
}

// Search Room by Type
void searchRoomByType() {
    Room r;
    char searchType[10];

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Room Type (AC / Non-AC): ";
    cin.getline(searchType, 10);

    for (int i = 0; searchType[i]; i++)
        searchType[i] = toupper(searchType[i]);

    ifstream fin("rooms.txt");
    if (!fin) { cout << "No room records found!\n"; return; }

    bool found = false;
    cout << "\n--- Rooms of type " << searchType << " ---\n";

    while (fin >> r.roomNo >> r.type >> r.price >> r.isBooked) {
        char roomType[10];
        strcpy(roomType, r.type);
        for (int i = 0; roomType[i]; i++)
            roomType[i] = toupper(roomType[i]);

        if (strcmp(roomType, searchType) == 0) {
            cout << "Room No: " << r.roomNo
                 << " | Price: " << r.price
                 << " | Status: " << (r.isBooked ? "Booked" : "Available") << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "No rooms found of this type.\n";

    fin.close();
}

// ---------------- MAIN ----------------
int main() {

    if (!adminLogin()) return 0;   // 🔐 LOGIN FIRST

    int choice;
    do {
        cout << "\n===== Welcome to The Titan Hotel =====\n";
        cout << "1. Add Room\n2. View Rooms\n3. Book Room + Payment\n4. Cancel Booking\n";
        cout << "5. Available Rooms\n6. Search Customer by Name\n7. Sort Rooms by Price\n";
        cout << "8. Search Room by Room Number\n9. Search Room by Type\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addRoom(); break;
            case 2: viewRooms(); break;
            case 3: bookRoom(); break;
            case 4: editOrCancelBooking(); break;
            case 5: availability(); break;
            case 6: searchCustomerByName(); break;
            case 7: sortRoomsByPrice(); break;
            case 8: searchRoomByNumber(); break;
            case 9: searchRoomByType(); break;
            case 0: cout << "Thank You!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}

