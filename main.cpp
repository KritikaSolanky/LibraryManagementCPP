#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>
#include <conio.h> // For password masking on Windows

using namespace std;

// ---------------- Structs ----------------
struct Book {
    string serial;
    string title;
    string author;
    bool isMovie;   // 0-Book,1-Movie
    bool available;
};

struct Membership {
    string memberName;
    string membershipNo;
    string startDate;
    string endDate;
};

struct User {
    string username;
    string password;
    string name;
    string role; // admin or user
};

struct Issue {
    string username;
    string serial;
    string issueDate;
    string dueDate;
    string returnDate;
    double fine;
    bool finePaid;
};

// ---------------- Utility Functions ----------------
string todayDate() {
    time_t t = time(0);
    tm* now = localtime(&t);
    char buf[11];
    sprintf(buf, "%04d-%02d-%02d", now->tm_year+1900, now->tm_mon+1, now->tm_mday);
    return string(buf);
}

string addDays(string date, int days) {
    tm tmDate = {};
    sscanf(date.c_str(), "%d-%d-%d", &tmDate.tm_year, &tmDate.tm_mon, &tmDate.tm_mday);
    tmDate.tm_year -= 1900; tmDate.tm_mon -= 1;
    time_t t = mktime(&tmDate) + days*24*3600;
    tm* newDate = localtime(&t);
    char buf[11];
    sprintf(buf, "%04d-%02d-%02d", newDate->tm_year+1900, newDate->tm_mon+1, newDate->tm_mday);
    return string(buf);
}

void pause() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ---------------- Sample Data ----------------
vector<User> users = { {"admin","admin123","Administrator","admin"},
                       {"user1","user123","User One","user"} };

vector<Book> books = { {"S001","The Hobbit","J.R.R. Tolkien",0,true},
                       {"S002","1984","George Orwell",0,true},
                       {"S003","Interstellar","Christopher Nolan",1,true} };

vector<Membership> memberships;
vector<Issue> issues;

// ---------------- Password Input with Mask ----------------
string inputPassword() {
    string password;
    char ch;
    while((ch = _getch()) != 13) { // Enter key
        if(ch == 8) { // Backspace
            if(password.length() > 0) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password.push_back(ch);
            cout << "*";
        }
    }
    cout << endl;
    return password;
}

// ---------------- Login ----------------
User login() {
    string u,p;
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    p = inputPassword();
    for(auto &usr : users)
        if(usr.username==u && usr.password==p)
            return usr;
    return {"","","",""};
}

// ---------------- Reports ----------------
void reportBooks() {
    cout << "\n--- Books Available ---\n";
    for(auto &b:books)
        cout << b.title << " by " << b.author << " | Serial:" << b.serial
             << " | " << (b.available?"Available":"Issued") << endl;
    pause();
}

void reportActiveIssues() {
    cout << "\n--- Active Issues ---\n";
    for(auto &i:issues)
        if(i.returnDate=="")
            cout << "User: " << i.username << " | Serial: " << i.serial
                 << " | Issue: " << i.issueDate << " | Due: " << i.dueDate << endl;
    pause();
}

// ---------------- Transactions ----------------
void issueBook(User &user) {
    string serial;
    cout << "Enter Serial No of Book to Issue: ";
    cin >> serial;
    Book* bptr=nullptr;
    for(auto &b:books) if(b.serial==serial) bptr=&b;
    if(!bptr) { cout << "Book not found.\n"; pause(); return; }
    if(!bptr->available) { cout << "Book currently issued.\n"; pause(); return; }

    string issueDate = todayDate();
    string dueDate = addDays(issueDate,15);
    issues.push_back({user.username, serial, issueDate, dueDate,"",0.0,false});
    bptr->available=false;
    cout << "Book issued. Due date: " << dueDate << endl;
    pause();
}

void returnBook(User &user) {
    string serial;
    cout << "Enter Serial No of Book to Return: ";
    cin >> serial;
    Issue* iptr=nullptr;
    for(auto &i:issues) if(i.serial==serial && i.username==user.username && i.returnDate=="") iptr=&i;
    if(!iptr) { cout << "No such issued book.\n"; pause(); return; }

    iptr->returnDate = todayDate();

    // Fine calculation
    tm tmDue={}, tmRet={};
    sscanf(iptr->dueDate.c_str(), "%d-%d-%d",&tmDue.tm_year,&tmDue.tm_mon,&tmDue.tm_mday);
    sscanf(iptr->returnDate.c_str(), "%d-%d-%d",&tmRet.tm_year,&tmRet.tm_mon,&tmRet.tm_mday);
    tmDue.tm_year-=1900; tmDue.tm_mon-=1;
    tmRet.tm_year-=1900; tmRet.tm_mon-=1;
    time_t tDue = mktime(&tmDue);
    time_t tRet = mktime(&tmRet);
    double fine = 0.0;
    if(difftime(tRet,tDue)>0) fine = difftime(tRet,tDue)/(24*3600)*1.0;
    iptr->fine=fine;

    // Checkbox
    char choice;
    if(fine > 0) {
        cout << "Fine due: " << fine << ". Has fine been paid? (y/n): ";
        cin >> choice;
        iptr->finePaid = (choice=='y' || choice=='Y');
        if(!iptr->finePaid) { cout << "Cannot return book until fine is paid.\n"; pause(); return; }
    } else {
        iptr->finePaid = true;
    }

    for(auto &b:books) if(b.serial==serial) b.available=true;
    cout << "Book returned successfully.\n";
    pause();
}

// ---------------- Maintenance ----------------
void addBook() {
    string serial,title,author;
    int type;
    cout << "Enter Serial No: "; cin >> serial;
    cin.ignore();
    cout << "Enter Title: "; getline(cin,title);
    cout << "Enter Author: "; getline(cin,author);
    cout << "Type? 0-Book,1-Movie (default Book): "; cin >> type;
    books.push_back({serial,title,author,type==1,true});
    cout << "Book added.\n"; pause();
}

void updateBook() {
    string serial;
    cout << "Enter Serial No to update: "; cin >> serial;
    Book* bptr=nullptr;
    for(auto &b:books) if(b.serial==serial) bptr=&b;
    if(!bptr) { cout << "Book not found.\n"; pause(); return; }
    cin.ignore();
    cout << "New Title: "; getline(cin,bptr->title);
    cout << "New Author: "; getline(cin,bptr->author);
    cout << "Updated.\n"; pause();
}

void addMembership() {
    string name; int dur;
    cin.ignore();
    cout << "Member Name: "; getline(cin,name);
    cout << "Select Duration 6/12/24 months (default 6): "; cin >> dur;
    if(dur!=6 && dur!=12 && dur!=24) dur=6;
    string memNo = "MB" + to_string(rand()%10000);
    memberships.push_back({name,memNo,todayDate(),addDays(todayDate(),dur*30)});
    cout << "Membership added: " << memNo << endl; pause();
}

// ---------------- Menus ----------------
void adminMenu(User &user) {
    int ch;
    do {
        cout << "\n--- Admin Menu ---\n1.Maintenance 2.Transactions 3.Reports 4.Logout\nSelect: ";
        cin >> ch;
        if(ch==1) {
            int m;
            cout << "1.Add Book 2.Update Book 3.Add Membership 4.Back\nSelect: ";
            cin >> m;
            if(m==1) addBook();
            else if(m==2) updateBook();
            else if(m==3) addMembership();
        }
        else if(ch==2) {
            int t; 
            cout << "1.Issue Book 2.Return Book 3.Back\nSelect: "; cin >> t;
            if(t==1) issueBook(user);
            else if(t==2) returnBook(user);
        }
        else if(ch==3) { reportBooks(); reportActiveIssues(); }
    } while(ch!=4);
}

void userMenu(User &user) {
    int ch;
    do {
        cout << "\n--- User Menu ---\n1.Transactions 2.Reports 3.Logout\nSelect: ";
        cin >> ch;
        if(ch==1) {
            int t;
            cout << "1.Issue Book 2.Return Book 3.Back\nSelect: "; cin >> t;
            if(t==1) issueBook(user);
            else if(t==2) returnBook(user);
        }
        else if(ch==2) { reportBooks(); reportActiveIssues(); }
    } while(ch!=3);
}

// ---------------- Main ----------------
int main() {
    cout << "--- Library Management System ---\n";
    User user = login();
    if(user.username=="") { cout << "Login failed.\n"; return 0; }
    if(user.role=="admin") adminMenu(user);
    else userMenu(user);
    cout << "Bye.\n";
    return 0;
}
