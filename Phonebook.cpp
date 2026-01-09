#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include<conio.h>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

// Function to change text color
void color(int c) {
    SetConsoleTextAttribute(h, c);
}

struct Contact {
    string fullname;
    string number;
};

class Phonebook {
private:
    vector<Contact> contacts;

public:
    Phonebook() { loadFromFile(); }
    bool isValidNumber(const string &num) {
    if (num.length() != 10)
        return false;

    for (char c : num) {
        if (!isdigit(c))
            return false;
    }
    return true;
    }

    void addContact() {
        Contact c;
        color(11); cout << "Enter Full Name: "; color(15);
        cin.ignore();
        getline(cin, c.fullname);

        color(11); cout << "Enter Number: "; color(15);
        cin >> c.number;
        // Validate number
        if (!isValidNumber(c.number)) {
          color(12);
          cout << "Error: Phone number must be exactly 10 digits and numeric only!\n";
          color(15);
          return;
        }

        // Prevent duplicate number
        for (auto &x : contacts) {
            if (x.number == c.number) {
                color(12); cout << "Error: Number already exists!\n"; color(15);
                return;
            }
        }

        contacts.push_back(c);
        saveToFile();

        color(10); cout << "Contact Added Successfully.\n"; color(15);
    }
    void deleteByNumber() {
    string num;
    color(11); cout << "Enter Number to Delete: "; color(15);
    cin >> num;

    for (auto it = contacts.begin(); it != contacts.end(); ++it) {
        if (it->number == num) {
            contacts.erase(it);
            saveToFile();
            color(10); cout << "Contact Deleted Successfully.\n"; color(15);
            return;
        }
    }

    color(12); cout << "Contact Not Found.\n"; color(15);
   }

   void deleteByName() {
    string name;
    color(11); cout << "Enter Full Name to Delete: "; color(15);
    cin.ignore();
    getline(cin, name);

    for (auto it = contacts.begin(); it != contacts.end(); ++it) {
        if (it->fullname == name) {
            contacts.erase(it);
            saveToFile();
            color(10); cout << "Contact Deleted Successfully.\n"; color(15);
            return;
        }
    }

    color(12); cout << "Contact Not Found.\n"; color(15);
   }

   void deleteContact() {
    int option;
    color(14);
    cout << "\nDelete Contact By:\n";
    cout << "1. Phone Number\n";
    cout << "2. Full Name\n";
    cout << "3. Back\n";
    cout << "4. Exit\n";
    color(11);
    cout << "Enter choice: ";
    color(15);
    cin >> option;

    if (option == 1)
        deleteByNumber();
    else if (option == 2)
        deleteByName();
    else if(option == 3)
        return;
    else if (option == 4)
        exit(1);
    else {
        color(12);
        cout << "Invalid delete option!\n";
        color(15);
    }
    if(option!=3)
        getch();
}


    void searchByNumber() {
        string num;
        color(11); cout << "Enter Number: "; color(15);
        cin >> num;

        for (auto &c : contacts) {
            if (c.number == num) {
                color(10);
                cout << "Full Name: " << c.fullname << "\nNumber: " << c.number << "\n";
                color(15);
                return;
            }
        }
        color(12); cout << "Contact Not Found.\n"; color(15);
    }

    void searchByName() {
        string name;
        color(11); cout << "Enter Full Name: "; color(15);
        cin.ignore();
        getline(cin, name);

        for (auto &c : contacts) {
            if (c.fullname == name) {
                color(10);
                cout << "Full Name: " << c.fullname << "\nNumber: " << c.number << "\n";
                color(15);
                return;
            }
        }
        color(12); cout << "Contact Not Found.\n"; color(15);
    }
    void searchContact() {
    int option;
    color(14);
    cout << "\nSearch Contact By:\n";
    cout << "1. Phone Number\n";
    cout << "2. Full Name\n";
    cout << "3. Back\n";
    cout << "4. Exit\n";
    color(11);
    cout << "Enter choice: ";
    color(15);
    cin >> option;

    if (option == 1)
        searchByNumber();
    else if (option == 2)
        searchByName();
    else if(option==3)
        return;
    else if(option==4)
         exit(1);
    else {
        color(12);
        cout << "Invalid search option!\n";
        color(15);
    }
    if(option!=3)
        getch();
 }

  void editContact() {
    string oldNum;
    color(11); cout << "Enter Number of Contact to Edit: "; color(15);
    cin >> oldNum;

    for (auto &c : contacts) {
        if (c.number == oldNum) {

            color(11); cout << "Enter New Full Name: "; color(15);
            cin.ignore();
            getline(cin, c.fullname);

            string newNumber;
            color(11); cout << "Enter New 10-digit Number: "; color(15);
            cin >> newNumber;

            // Validate number format
            if (!isValidNumber(newNumber)) {
                color(12);
                cout << "Error: Phone number must be exactly 10 digits and numeric only!\n";
                color(15);
                return;
            }

            // Check duplicate number (ignore current contact)
            for (auto &x : contacts) {
                if (x.number == newNumber && x.number != oldNum) {
                    color(12);
                    cout << "Error: This number already exists!\n";
                    color(15);
                    return;
                }
            }

            // Update number
            c.number = newNumber;

            saveToFile();
            color(10); cout << "Contact Updated Successfully.\n"; color(15);
            return;
        }
    }

    color(12); cout << "Contact Not Found.\n"; color(15);
}


    void sortContacts() {
        sort(contacts.begin(), contacts.end(),
            [](Contact a, Contact b) { return a.fullname < b.fullname; });

        saveToFile();
        color(10); cout << "Contacts Sorted Alphabetically.\n"; color(15);
    }

    void displayContacts() {
        color(14);
        cout << "\n--- CONTACT LIST ---\n";
        color(15);

        for (auto &c : contacts) {
            color(10);
            cout << "Full Name: " << c.fullname << " | Number: " << c.number << endl;
            color(15);
        }
    }

    void saveToFile() {
        ofstream file("phonebook.txt");
        for (auto &c : contacts) {
            file << c.fullname << "|" << c.number << endl;
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file("phonebook.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            size_t pos = line.find("|");
            if (pos != string::npos) {
                Contact c;
                c.fullname = line.substr(0, pos);
                c.number = line.substr(pos + 1);
                contacts.push_back(c);
            }
        }
        file.close();
    }
};

int main() {
    Phonebook pb;
    int choice;

    while (true) {
        color(14);
        cout << "\n==== PHONEBOOK MENU ====\n";
        cout << "1. Add Contact\n";
        cout << "2. Delete Contact\n";
        cout << "3. Search Contact\n";
        //cout << "4. Search by Name\n";
        cout << "4. Edit Contact\n";
        cout << "5. Sort Contacts\n";
        cout << "6. Display Contacts\n";
        cout << "7. Exit\n";
        color(11);
        cout << "Enter your choice: ";
        color(15);

        cin >> choice;

        switch (choice) {
            case 1: pb.addContact(); break;
            case 2: pb.deleteContact(); break;
            case 3: pb.searchContact(); break;
            case 4: pb.editContact(); break;
            case 5: pb.sortContacts(); break;
            case 6: pb.displayContacts(); break;
            case 7:
                color(10);
                cout << "Exiting Phonebook. Goodbye!\n";
                color(15);
                return 0;
            default:
                color(12);
                cout << "Invalid Option!\n";
                color(15);

        }
        if(choice!=2&&choice!=3)
        getch();
        system("cls");
    }
}

