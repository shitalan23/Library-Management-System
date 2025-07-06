#include<bits/stdc++.h>
#include<ctime>
#include<sstream>
#define BORROW_HISTORY_FILE "borrow_history.csv"

using namespace std;

// borrowing history of Users
struct IssuedBook{
    string bookTitle, date_of_issue;
    int total_days, fine;
};

// // hashmap to store user and issued books
unordered_map<string, vector<IssuedBook>> borrow_history;




// current date in string format
string getcurrentdate(){  // reference ; https://www.w3schools.com/cpp/cpp_date.asp
    time_t now = time(0);
    tm *localtm = localtime(&now);

    int day = localtm->tm_mday;
    int month = 1 + localtm->tm_mon;  
    int year = 1900 + localtm->tm_year; 

    return to_string(day) + "-" + to_string(month) + "-" + to_string(year);
}
// convert string to date
tm stringTodate(const string &datestr){
    tm date = {};
    istringstream ss(datestr);
    char dash;
    ss >> date.tm_mday >> dash >> date.tm_mon >> dash >> date.tm_year;
    date.tm_mon -= 1;  
    date.tm_year -= 1900; 
    return date;
}
// calculate total days issued of a book
int cal_total_days(string &date_of_issue){
    string curDate = getcurrentdate();

    tm issueDate = stringTodate(date_of_issue);
    tm current = stringTodate(curDate);

    time_t issueTime = mktime(&issueDate);
    time_t currentTime = mktime(&current);

    int totalDays = (difftime(currentTime, issueTime) / (60 * 60 * 24)) ;
    return totalDays;
}
// to lower case
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}
// generating user ID when a new user signs up
int generateUserID(string userID){
    // hash<string> hasher;
    // string name_t = toLowerCase(name);
    // size_t hashedValue = hasher(name); 
    // int randomness = rand()%100;
    // return name_t.substr(0, 6) + "_" + to_string((hashedValue+randomness) % 1000);

    ifstream file("users.csv");
        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            return 5;
        }

        string line;
        cin.ignore();
        getline(file, line); 

        // cin.ignore();

        while (getline(file, line)) {
            stringstream ss(line);
            string serial, name, storedUserID, storedPassword, role;

            getline(ss, serial, ','); 
            getline(ss, name, ','); 
            getline(ss, storedUserID, ','); 
            getline(ss, storedPassword, ','); 
            getline(ss, role, ',');   

            // Check if userID and password matches
            if (storedUserID == userID) {
                file.close();
                cout<<"This userID is not available\n";

                return 0;
            }
        }

        file.close();
        return 1 ;

}
// to get last serial number from users.csv
int getSerialNumber() {
    ifstream file("users.csv");
    string line;
    int count = 0;

    while (getline(file, line)) {
        if (!line.empty()) 
        count++;
    }
    file.close();


    return (count > 1) ? count : 1; 
}
// last serial number from "books.csv"
int getSerialNumber_books(){
    ifstream file("books.csv");
    string line;
    int count = 0;

    // cin.ignore();

    while (getline(file>>ws, line)) {
        if (!line.empty()) 
        count++;
    }
    file.close();

    return (count > 1) ? count : 1; 
}

// add new user to "users.csv"
bool addUser(const string &name, const string &userID, const string &password, const string &role) {
    ofstream file("users.csv", ios::app);  

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return false;
    }

    int serialNumber = getSerialNumber();

    // cout<<"UserID : "<<userID<<endl;

    file << serialNumber << "," << name << "," << userID << "," << password << "," << role << "\n";
    if (file.fail()) {
        file.close();
        return false; 
    }
    file.close();

    return true; 
}

enum BookStatus{Available, Borrowed, Reserved, NotFound};

class Book{
public:
    string title, author, publisher, ISBN;
    int year;
    BookStatus status;
    void updateBookStatus(string bookname, string newStatus, string usserID);
    void addToborrow_history(string userID, string bookname){
        // update borrow_history
        IssuedBook newbook = {bookname,getcurrentdate(),0,0};
        borrow_history[userID].push_back(newbook);
        return;
    }
    // Checks BookStuatus
    BookStatus checkstatus(string &bookname){
        ifstream file("books.csv");

        if(!file.is_open()){
            cerr<<"Error opening books.csv.\n";
            return Available;
        }
        string line;

        cin.ignore();
        getline(file, line);

        string lowerCaseBookname = toLowerCase(bookname);

        while(getline(file,line)){
            stringstream ss(line);
            string serial, title, author, publisher, year, isbn, status;
            getline(ss, serial, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, publisher, ',');
            getline(ss, year, ',');
            getline(ss, isbn, ',');
            getline(ss, status, ','); 

            title = toLowerCase(title);

            if (title == lowerCaseBookname) {
                file.close();
                string bookstatus = toLowerCase(status);
                if(bookstatus=="available") return Available;
                else if(bookstatus=="issued") return Borrowed;
                else if(bookstatus=="reserved")  return Reserved;
                return NotFound;
            }
        }
        return NotFound;
    }
    
};

class Account{
public:
    // make a file to keep track of all the fines collected
    struct finedetails{
        int finecollected;
        string date_of_collection;
    };
    unordered_map<string, vector<finedetails>> fine_details;

    void fines_collection(string userID, int fine){
        int finecolleted = fine;
        string date_of_collection = getcurrentdate();

        finedetails fineD = {fine, date_of_collection};

        fine_details[userID].push_back(fineD);
        return;
    }
    
    //function to know total fine collected
    int total_fine_collected(){
        int total_fine = 0;
        for(const auto &it: fine_details){
            for(const auto &j :it.second){
                total_fine += j.finecollected;
            }
        }
        return total_fine;
    }
    

    // currently borrowed books file "cur_borrowed.csv"

    vector<string> splitCSVLine(const string &line) {
        vector<string> tokens;
        stringstream ss(line);
        string token;
        cin.ignore();
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void update_cur_borrowed(string UserID, string bookname){
        ifstream file("books.csv");
        if (!file.is_open()) {
            cout << "Error opening file!\n";
            return;
        }
        string line;
        string booktitle;
        string author,publisher, date_of_issue, isbn;
        // cin.ignore();
        bool found = false;
        while (getline(file, line)) {
            vector<string> details = splitCSVLine(line);
            if (details.size() < 7) continue;

            booktitle = details[1];
            if(toLowerCase(booktitle)==toLowerCase(bookname)){
                author = details[2];
                publisher = details[3];
                isbn = details[5];

                found = true;
                break;

            }
        }
        file.close();
        if (!found) {
            cout << "Book not found in books.csv!\n";
            return;
        }
        ofstream cur_file("cur_borrowed_books.csv",ios::app);
        if(!cur_file.is_open()){
            cout<<"Error opening cur_borrowed_books.csv!\n";
            return;
        }

        cur_file << bookname << "," << author << ","
             << publisher << "," << getcurrentdate() << "," <<isbn<<","<< UserID << "\n";

        cur_file.close();
        return;
    }
};

class Library{
public:
    void see_available_books();
    void stu_borrow_books(string userID);
    void fac_borrow_books(string userID);
    void return_book(string userID);
    void search_book(string book_name);

    void loadFiles(){
        ifstream booksFile("books.txt");
        ifstream userFile("Users.txt");
    }
    
    
    int calc_fine(string role, int total_days){
        if(role=="Faculty") return 0;
        return (total_days>15)*(total_days-15)*10;
    }
    void addIssuedBook(string userID, string &bookTitle, string &date_of_issue, string &role){
        int total_days = cal_total_days(date_of_issue);
        int fine = calc_fine(role, total_days);
        borrow_history[userID].push_back({bookTitle, date_of_issue, total_days, fine});
    }
    void see_borrow_history(string userID, string role){
        cout << "Checking history for user: '" << userID << "'" << endl;
        if(borrow_history.find(userID)==borrow_history.end()){
            cout<<"No books have been issued!\n";
        return;
        }
    
        cout<<"Books issued by you: \n";
        cout << "--------------------------------------------\n";
        cout << "Book Title | Date Issued | Days Issued | Fine\n";
        for (const auto &book : borrow_history[userID]) {
            if(role=="Faculty"){
                cout << book.bookTitle << " | " << book.date_of_issue << " | " 
                << book.total_days << " | Rs. " << 0 << "\n"; 
            } else{
            cout << book.bookTitle << " | " << book.date_of_issue << " | " 
             << book.total_days << " | Rs. " << book.fine << "\n";
            }
        }
        cout << "--------------------------------------------\n";
        return;

    }
    // see total_fine
    int total_fine(const string &userID) {
        if (borrow_history.find(userID) == borrow_history.end()) {
            cout << "User not found in borrowing history.\n";
            return 0;
        }
    
        int totalFine = 0;
        for (const auto &book : borrow_history[userID]) {
            totalFine += book.fine;
        }
    
        return totalFine;
    }

    unordered_map<string, string> user_roles;
    void loadUserRoles() {
        ifstream file("users.csv");
        if (!file.is_open()) {
            cerr << "Error: Could not open users.csv\n";
            return;
        }
    
        string line, userID, role;

        // cin.ignore();
        
        getline(file>>ws, line);
        
        while (getline(file, line)) {
            stringstream ss(line);
            string serial, name, username, password;
    
            getline(ss, serial, ',');
            getline(ss, name, ',');
            getline(ss, userID, ',');
            getline(ss, password, ',');
            getline(ss, role, ',');
    
            user_roles[userID] = role;
        }
    
        file.close();
    }

    // we need a function which will update fine and total_days in borrow_history everytime the code runs
    void updateBorrowHistory(){

        ifstream file("cur_borrowed_books.csv");
        if(!file.is_open()){
            cerr<<"Error opening cur_borrowed_books.csv!\n";
            return;
        }
        string line;
        getline(file,line);
        while (getline(file, line)) {
            stringstream ss(line);
            string  title, author, publisher, date_of_issue, isbn, userID;

            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, publisher, ',');
            getline(ss, date_of_issue, ',');
            getline(ss, isbn, ',');
            getline(ss, userID, ',');
            
            IssuedBook book{title, date_of_issue, 0, 0};
            book.total_days = cal_total_days(date_of_issue);
            book.fine = calc_fine(user_roles[userID], book.total_days);
            
            borrow_history[userID].push_back(book);
        }
        
    }
    
};


void Library::return_book(string userID) {
    cout << "Write the title of the book to be returned: ";
    string bookname;
    getline(cin >> ws, bookname);

    if (borrow_history.find(userID) == borrow_history.end() || borrow_history[userID].empty()) {
        cout << "You have not issued any book!\n";
        return;
    }

    // Removing the book from borrow_history
    auto &books = borrow_history[userID];
    for (auto it = books.begin(); it != books.end();) {
        if (toLowerCase(it->bookTitle) == toLowerCase(bookname)) {
            // Pay fine if any
            if (it->fine != 0) {
                cout << "Pay your fine: " << it->fine << " INR\n";
                cout << "Once done, press 1!\n";
                char ch;
                cin >> ch;
                if (ch != '1') {
                    cout << "Fine not paid.\n";
                    return;
                }
                it->fine = 0;
            }
            it = books.erase(it);
            break;
        } else {
            ++it;
        }
    }

    // Updating the "books.csv"
    Book b;
    b.updateBookStatus(bookname, "Available", userID);

    // Updating the "cur_borrowed_books.csv"
    ifstream cur_file("cur_borrowed_books.csv");
    ofstream temp_file("temp_cur_borrowed_books.csv");
    if (!cur_file.is_open() || !temp_file.is_open()) {
        cerr << "Error opening files!\n";
        return;
    }
    string line;
    getline(cur_file, line);
    temp_file << line << "\n";

    while (getline(cur_file, line)) {
        stringstream ss(line);
        string title, author, publisher, date_of_issue, isbn, userID;
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, publisher, ',');
        getline(ss, date_of_issue, ',');
        getline(ss, isbn, ',');
        getline(ss, userID, ',');

        if (toLowerCase(title) != toLowerCase(bookname)) {
            temp_file << line << "\n";
        }
    }

    cur_file.close();
    temp_file.close();

    remove("cur_borrowed_books.csv");
    rename("temp_cur_borrowed_books.csv", "cur_borrowed_books.csv");

    cout << bookname << " has been successfully returned.\n";
}

class User{
private:
    string password;
public:
    int serial_number;
    string userID, name, role;
    int max_days, max_books;
    vector<string> isbn_issued;
    void user_menu();
    User(){};
    
};

class Faculty: public User{
    public:
        void faculty_menu(string userID) {
        char ch;
        do {
            cout << "---------------------------------------------------------------------\n";
            cout << "Press 1 to see available books.\n";
            cout << "Press 2 to see your borrowing history.\n";
            cout << "Press 3 to see your fine.\n";
            cout << "Press 4 to issue a book.\n";
            cout << "Press 5 to return books.\n";
            cout << "Press 6 to see availability of a particular book.\n";
            cout << "Press 7 to logout.\n";
            cout << "---------------------------------------------------------------------\n";
            cin >> ch;
            cin.ignore();  
    
            Library lb;
            User u;
            switch (ch) {
                case '1':
                    lb.see_available_books();
                    faculty_menu(userID);
                    return;
                case '2':
                    lb.see_borrow_history(userID, "Faculty");
                    faculty_menu(userID);
                    return;
                case '3': {
                    // int fine = lb.total_fine(userID);
                    cout << "Your total fine is: " << 0 << " INR\n";
                    faculty_menu(userID);  
                    return;
                }
                case '4':
                    lb.fac_borrow_books(userID);
                    faculty_menu(userID); 
                    return;
                case '5':
                    lb.return_book(userID);
                    faculty_menu(userID); 
                    return;
                case '6': {
                    cout << "Enter the book name you want to search for: ";
                    string book_name;
                    getline(cin, book_name);
                    lb.search_book(book_name);
                    faculty_menu(userID); 
                    return;
                }
                case '7':
                    cout << "You are logged out.\n";
                    user_menu();  
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    faculty_menu(userID);  
                    return;
            }
        } while (ch != '7');

    }
};

void Library::search_book(string bookname){
    ifstream file("books.csv");

    if(!file.is_open()){
        cerr<<"Error opening books.csv.\n";
        return;
    }
    string line;
    // cin.ignore();
    getline(file>>ws, line);


    bool found = false;
    // cin.ignore();
    while(getline(file>>ws,line)){
        stringstream ss(line);
        string serial, title, author, publisher, year, isbn, status;
        getline(ss, serial, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, publisher, ',');
        getline(ss, year, ',');
        getline(ss, isbn, ',');
        getline(ss, status, ','); 

        if (toLowerCase(bookname) == toLowerCase(title)) {
            found = true;
            cout << serial << ". " << title << " by " << author << " (" << year << ")"<<"isbn number : "<<isbn<<","<<"publisher: "<<publisher<<" and "<<"availablity: "<<status<<endl;
        }
    }
    if (!found) {
        cout << "Searched book is not vailable in the Library! :(\n";
    }

    file.close();
}

void Library::see_available_books(){
    ifstream file("books.csv");

    if(!file.is_open()){
        cerr<<"Error opening books.csv.\n";
        return;
    }
    string line;
    // cin.ignore();
    getline(file, line);

    cout<<"Available books:\n";
    cout<<"----------------------------------------------\n";

    bool found = false;
    while(getline(file,line)){
        stringstream ss(line);
        string serial, title, author, publisher, year, isbn, status;
        getline(ss, serial, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, publisher, ',');
        getline(ss, year, ',');
        getline(ss, isbn, ',');
        getline(ss, status, ','); 

        if (status == "Available") {
            found = true;
            cout << serial << ". " << title << " by " << author << " (" << year << ")\n";
        }
    }
    if (!found) {
        cout << "No books are currently available.\n";
    }

    cout << "---------------------------------------------------------------\n";
    file.close();
}

class Librarian: public Library, public User, public Account{
public:
    void addNewBooks(){
        cout<<"Title of the new book : ";
        string bookname;
        // cin.ignore();
        getline(cin>>ws,bookname);

        cout<<"Author of the book : ";
        string author;
        // cin.ignore();
        getline(cin>>ws, author);

        cout<<"Publisher of the book : ";
        string publisher;
        // cin.ignore();
        getline(cin>>ws, publisher);

        cout<<"ISBN number: ";
        string isbn;
        // cin.ignore();
        getline(cin>>ws, isbn);

        cout<<"Year of the book: ";
        int year;
        cin>>year;

        ifstream readfile("books.csv");
        ofstream writefile("books.csv",ios::app);

        if(!writefile || !readfile){
            cout<<"Can't open file.\n";
            return;
        }

        string line;
        int lastSerial = 0;

        // Read the file to get the last serial number

        int newserial = getSerialNumber_books();

        writefile<<newserial<<","<<bookname<<","<< author << "," 
        << publisher << "," << year << "," << isbn << ",Available\n";

        writefile.close();
        cout << bookname << " has been successfully added.\n";

        return;

    }
    void librarian_menu(string userID);
};

void Librarian::librarian_menu(string userID){
        cout<<"---------------------------------------------------------------------\n";
        cout<<"Press 1 to see all books.\n";
        cout<<"Press 2 to search for an user.\n";
        cout<<"Press 3 to add new books to the Library.\n";
        cout<<"Press 4 to search for a particular book.\n";
        cout<<"Press 5 to logout.\n";
        cout<<"---------------------------------------------------------------------\n";
        char ch;
        cin>>ch;
        Librarian l;
        Library lb;
        if(ch=='1'){
            // open "books.csv" to see all the books
            ifstream file("books.csv");
            if (!file.is_open()) {
                cerr << "Error opening books.csv!" << endl;
                l.librarian_menu(userID);
                exit(1);
            }
            string line;
            getline(file, line); // Read and ignore the header line
            cout << "\nList of Books:\n";
            cout << "--------------------------------------------------------------" << endl;

            while (getline(file, line)) {
                stringstream ss(line);
                string serial, title, author, publisher, year, isbn, status;
                
                getline(ss, serial, ',');
                getline(ss, title, ',');
                getline(ss, author, ',');
                getline(ss, publisher, ',');
                getline(ss, year, ',');
                getline(ss, isbn, ',');
                getline(ss, status, ',');
                
                cout << "Serial Number: " << serial << "\n";
                cout << "Title: " << title << "\n";
                cout << "Author: " << author << "\n";
                cout << "Publisher: " << publisher << "\n";
                cout << "Year: " << year << "\n";
                cout << "ISBN: " << isbn << "\n";
                cout << "Status: " << status << "\n";
                cout << "--------------------------------------------------------------" << endl;
            }
            file.close();
            l.librarian_menu(userID);
        }else if(ch=='2'){
            // search for an user in "users.csv"
            string userid;
            cout<<"Enter userID: ";
            cin>>userid;
            ifstream file("users.csv");
    
            if (!file.is_open()) {
                cerr << "Error opening users.csv!" << endl;
                l.librarian_menu(userID);
                exit(1);
            }

            string line;
            getline(file, line);
            
            bool found = false;
            while (getline(file, line)) {
                stringstream ss(line);
                string serial, name, id, password, role;
                
                getline(ss, serial, ',');
                getline(ss, name, ',');
                getline(ss, id, ',');
                getline(ss, password, ','); // Read password but do not use it
                getline(ss, role, ',');
                
                if (id == userid) {
                    cout << "User Name: " << name << "\n";
                    cout << "User ID: " << id << "\n";
                    cout << "Role: " << role << "\n";
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                cout << "User ID not found!" << endl;
            }
            
            file.close();

            l.librarian_menu(userID);
        }else if(ch=='3'){
            l.addNewBooks();
            l.librarian_menu(userID);
        }else if(ch=='4'){
            cout<<"Enter the book name you want to search for: ";
            string book_name;
            cin.ignore();
            getline(cin>>ws, book_name);
            lb.search_book(book_name);
            l.librarian_menu(userID);
        }
        else if(ch=='5'){
            cout<<"You are logged out.\n";
            User u;
            u.user_menu();
        }

}




class Student: public User{
public:
    void student_menu(string userID){
        cout<<"---------------------------------------------------------------------\n";
        cout<<"Press 1 to see available books.\n";
        cout<<"Press 2 to see your borrowing history.\n";
        cout<<"Press 3 to see your fine.\n";
        cout<<"Press 4 to issue a book.\n";
        cout<<"Press 5 to return books and clear fine.\n";
        cout<<"Press 7 to see availabilty of a particular book.\n";
        cout<<"Press 8 to logout.\n";
        cout<<"---------------------------------------------------------------------\n";
        char ch;
        cin>>ch;
        Student s;
        Library lb;
        if(ch=='1'){
            lb.see_available_books();
            s.student_menu(userID);
        }else if(ch=='2'){
            lb.see_borrow_history(userID,"Student");
            s.student_menu(userID);
        }
        else if(ch=='3'){
            int fine = lb.total_fine(userID);
            cout<<"Your total fine is : "<<fine<<" INR"<<endl;
            s.student_menu(userID);
        }else if(ch=='4'){
            lb.stu_borrow_books(userID);
            
            s.student_menu(userID);
        }else if(ch=='5'){
            lb.return_book(userID);
            s.student_menu(userID);
        }else if (ch=='7'){
            // search in the "books.txt" if the book is availble or not
            cout<<"Enter the book name you want to search for: ";
            string book_name;
            getline(cin>>ws, book_name);
            Library lb;
            lb.search_book(book_name);
            s.student_menu(userID);
        }
        else if(ch=='8'){
            cout<<"You are logged out.\n";
            User u;
            u.user_menu();
        }
        return;

    }
};

// Borrowing books
void Library::stu_borrow_books(string userID){
    // if total_fine==0 and total_books_borrowed <3 then only the student can borrow further books
    // number of books borrowed till now can be obtained from "borrow_history.txt"
    int totalFineOFUser = total_fine(userID);
    int total_books_issued = borrow_history[userID].size();

    if(totalFineOFUser!=0 ){
        cout<<"Sorry! your fine is pending.\n";
        return;
    }else if(total_books_issued>=3){
        cout<<"You have already issued 3 books. Return atleast one to issue new.\n";
        return;
    }
    cout<<"----------------------------------------------\n";
    cout<<"Enter the name of the book you want to issue: ";
    string bookname;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    getline(cin, bookname);
    Book b;
    BookStatus status = b.checkstatus(bookname);
    if(status == Reserved){
        cout<<"You can only read this book with Library premise.\n";
        return;

    }
    else if(status == Borrowed){
        cout<<"Come another time! The book is not available for issue now.\n";
        return;
    }else if(status==NotFound){
        cout<<"Sorry the book is not present in the library.\n";
        return;
    }

    b.updateBookStatus(bookname, "Issued", userID);
    b.addToborrow_history(userID, bookname);

    Librarian l;
    l.update_cur_borrowed(userID,bookname);

    cout<<"You have issued successfully! Please collect your book from the Library.\n";
    

    return;
}

string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}


bool bookFound(string name){
    string book_name = toLowerCase(trim(name));
    ifstream ogfile("books.csv");

    if(!ogfile.is_open()){
        cerr<<"Error in files!\n";
        return false;
    }

    string line;
    bool foundbook = false;

    getline(ogfile, line);

    while(getline(ogfile,line)){
        stringstream ss(line);
        string serial, title, author, publisher, year, isbn, status;
        getline(ss, serial, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, publisher, ',');
        getline(ss, year, ',');
        getline(ss, isbn, ',');
        getline(ss, status, ',');
        title = toLowerCase(title);
        if(title==book_name){
            foundbook = true;
            ogfile.close();
            break;
        }   
    }
    // cout<<book_name;
    ogfile.close();
    return foundbook;
    
}

void Book::updateBookStatus(string bookname, string newstatus, string userID){
    ifstream ogfile("books.csv");
    ofstream tempfile("temp.csv");

    if(!ogfile.is_open()||!tempfile.is_open()){
        cerr<<"Error in files!\n";
        return;
    }

    string line;
    bool foundbook = false;
    // cin.ignore();
    getline(ogfile, line);
    tempfile<<line<<"\n";

    // cin.ignore();
    while(getline(ogfile,line)){
        stringstream ss(line);
        string serial, title, author, publisher, year, isbn, status;
        getline(ss, serial, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, publisher, ',');
        getline(ss, year, ',');
        getline(ss, isbn, ',');
        getline(ss, status, ',');
        if(toLowerCase(title)==toLowerCase(bookname)){
            foundbook = true;
            status = newstatus;
        }
        
        tempfile<<serial<<","<<title<<","<<author<<","<<publisher<<","<<year<<","<<isbn<<","<<status<<endl;
        
    }
    ogfile.close();
    tempfile.close();
    if (!foundbook) {
        cerr << "Book not found!\n";
        remove("temp.csv");
    } 
    else {
        remove("books.csv"); 
        rename("temp.csv", "books.csv");
    }
    return;
}

void Library::fac_borrow_books(string userID){
   // if borrowed_days-current day>60 or  number of borrowed books == 5: new books cannot be issued

//    int totalFineOFUser = total_fine(userID);
    int total_books_issued = borrow_history[userID].size();
    bool crossed60 = false;
    string book60;
    for(const IssuedBook &book: borrow_history[userID]){
        if(book.total_days >= 60){
            crossed60 = true;
            book60 = book.bookTitle;
        }
    }
    if(total_books_issued>=5){
        cout<<"You have already issued 5 books. Return atleast one to issue new.\n";
        return;
    }else if(crossed60){
        cout<<"The book "<<book60<<"has been issued for over 60 days.\n";
        return;
    }
    cout<<"----------------------------------------------\n";
    cout<<"Enter the name of the book you want to issue: ";
    string bookname;
    // cin.ignore();
    getline(cin>>ws, bookname);
    Book b;
    bool found = bookFound(bookname);

    if(found==false){
        cout<<"This book is not available in library\n";
        return;
    }
    BookStatus status = b.checkstatus(bookname);
    if(status == Reserved || status == Borrowed){
        cout<<"Come another time! The book is not available for issue now.\n";
        return;
    }

    b.updateBookStatus(bookname, "Issued", userID);
    b.addToborrow_history(userID, bookname);

    Librarian l;
    l.update_cur_borrowed(userID,bookname);

    cout<<"Issued Successfully\n";
    return;

}


// Authenticate the user
class UserLogin{
    public:
    string authenticate(const string &userID, const string &password) {
        ifstream file("users.csv");
        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            return "Invalid";
        }

        string line;
        cin.ignore();
        getline(file, line); 

        // cin.ignore();

        while (getline(file, line)) {
            stringstream ss(line);
            string serial, name, storedUserID, storedPassword, role;

            getline(ss, serial, ','); 
            getline(ss, name, ','); 
            getline(ss, storedUserID, ','); 
            getline(ss, storedPassword, ','); 
            getline(ss, role, ',');   

            // Check if userID and password matches
            if (storedUserID == userID && storedPassword == password) {
                file.close();
                return role; 
            }
        }

        file.close();
        return "Invalid"; 
    }
};

// This will be displayed on starting the code
void User::user_menu(){
    cout<<"---------------------------------------------------------------------\n";
    cout<<"Welcome to IITK Library"<<endl;
    cout << "Enter 1 to Login\nEnter 2 to Sign Up\nEnter 3 to exit\n";
    cout<<"---------------------------------------------------------------------\n";
    char ch;
    cin>>ch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    User u;
    if(ch=='1'){
        // Login
        User u1;
        string user_id, password;
        cout<<"Enter your id: "<<endl;
        cin>> user_id;
        cout<<"Enter your password: "<<endl;
        cin>>password;

        UserLogin loginsystem;
        string role = loginsystem.authenticate(user_id, password);

        if(role == "Student"){
            cout<< "You have been logged in as a student.\n";
            Student s;
            s.student_menu(user_id);
        }
        else if(role=="Faculty"){
            cout<<"You have been logged in as a faculty.\n";
            Faculty f;
            f.faculty_menu(user_id);
        }else if(role=="Librarian"){
            cout<<"You have been logged in as Librarian.\n";
            Librarian l;
            l.librarian_menu(user_id);
        }
        else{
            cout<<"Invalid User.\n";
            u.user_menu();
        }
    }else if(ch=='2'){
        // Sign Up
        // assuming Librarian are registered by default
        string name;
        cout<<"Enter your name: ";
        // cin.ignore();
        getline(cin>>ws,name);
        int idx;
        cout<<"Enter 1 if you are a faculty or 2 for student: ";
        cin>>idx;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string role = (idx==1)? "Faculty":"Student";
        string password;
        cout<<"Enter your password: ";
        // cin.ignore();
        getline(cin>>ws,password);

        

        int mark = 10;
        string userID;
        while(mark!=1 || mark==5){
            cout<<"Enter your userID: ";
            getline(cin>>ws,userID);
            mark = generateUserID(userID);
        }
         
        if(mark==1){
            if(addUser(name, userID, password, role)){
                cout<<"Registration Successful!\n";
            }
        }
        else{
            cout<<"Registration failed!\n";
        }
        User u2;
        u2.user_menu();
    }
    else {
        cout<<"Bye!!!\n";
        exit(1);
    }
}


int main() {

    Library lb;
    lb.updateBorrowHistory();

    User u;
    u.user_menu();

    return 0;
}