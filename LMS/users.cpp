#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;

int main() {
    ofstream file("users.csv");

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    file << "User Name,User ID,Password,Role\n";

    file << "1,Durbasmriti Saha,durba_23," << "100@abc" << ",Student\n";
    file << "2,Kanika Kumari,kanika_23," << "100@345" << ",Student\n";
    file << "3,Indranil Saha,isaha," << "100@abc" << ",Faculty\n";
    file << "4,Shital Niras,shital_23," << "abx@321" << ",Student\n";
    file << "5,Gargee Dhalkari,gargee_23," << "500#ab" << ",Student\n";
    file << "6,Debopriya Roy,dbroy," << "1234" << ",Faculty\n";
    file << "7,Rohini Sharma,rohan_s," << "bh3@abc" << ",Librarian\n";
    file << "8,Konkona Sen,kankan_s," << "1!rght3" << ",Student\n";
    file << "9,Shonku,ishonku_p," << "prof_shonku12#4" << ",Faculty\n";
    file << "10,Naman Agarwal,Naman_a23," << "agarwal45@3" << ",Librarian\n";
    

    file.close();

    return 0;
}
