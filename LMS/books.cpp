#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream file("books.csv");

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    file << "Serial Number,Book Title,Author,Publisher,Year,ISBN Number,Book Status\n";

    file << "1,The Pragmatic Programmer,Andrew Hunt,Addison-Wesley,1999,978-0-201-61622-4,Issued\n";
    file << "2,1984,George Orwell,Secker & Warburg,1949,978-0-452-28423-4,Issued\n";
    file << "3,The Great Gatsby,F. Scott Fitzgerald,Charles Scribner's Sons,1925,978-0-7432-7356-5,Available\n";
    file << "4,To Kill a Mockingbird,Harper Lee,J. B. Lippincott & Co.,1960,978-0-06-112008-4,Available\n";
    file << "5,Feluda Samagra,Satyajit Ray,Ananda Publishers,1965,978-93-80341-45-6,Reserved\n";
    file << "6,Byomkesh Bakshi Stories,Sharadindu Bandyopadhyay,Ananda Publishers,1932,978-93-80341-46-3,IAvailable\n";
    file << "7,Professor Shonku Samagra,Satyajit Ray,Ananda Publishers,1961,978-93-80341-47-0,Available\n";
    file << "8,Harry Potter and the Sorcerer's Stone,J.K. Rowling,Bloomsbury,1997,978-0-7475-3269-9,Available\n";
    file << "9,A Brief History of Time,Stephen Hawking,Bantam Books,1988,978-0-553-10953-5,Available\n";
    file << "10,The Theory of Everything,Albert Einstein,Princeton University Press,1950,978-0-691-16410-3,Available\n";
    file<< "11,The Guide,R. K. Narayan,Indian Thought Publications,1958,978-81-7223-002-4,Issued\n";

    file.close();

    return 0;
}

