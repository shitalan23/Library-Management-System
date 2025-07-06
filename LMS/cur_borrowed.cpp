#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream file("cur_borrowed_books.csv");

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    file << "Book Title,Author,Publisher,Date of issue,ISBN Number,userID\n";

    file << "1984,George Orwell,Secker & Warburg,25-2-2015,978-0-452-28423-4,kanika_23\n";
    file << "The Pragmatic Programmer,Andrew Hunt,Addison-Wesley,20-2-2025,978-0-201-61622-4,isaha\n";
    file << "The guide,R. K. Narayan,Indian Thought Publications,10-3-2025,978-81-7223-002-4,durba_23\n";

    file.close();

    return 0;
}