
# Project Title

The Library management system is designed to help users managing books and user records, issuing books and returning them efficiently. Users consists of Students, faculties and authorities like Librarian. 


## File Structure

LMS\

│── books.cpp              
│── books.csv                          
│── cur_borrowed.cpp        
│── cur_borrowed_borrowed.csv       
│── main.cpp                             
│── users.cpp              
│── users.csv                            
│── README.md               
## Deployment

To deploy this project on Windows

```bash
  cd path\to\LMS
```
```bash
  g++ main.cpp books.cpp cur_borrowed.cpp users.cpp -o LMS.exe
```
```bash
  LMS.exe
```


To deploy this project on Linux

```bash
  cd path\to\LMS
```
```bash
  g++ main.cpp books.cpp cur_borrowed.cpp users.cpp -o LMS.exe
```
```bash
  ./LMS
```


## Features

- Login and sign-up system for users. (Librarian is assumed to signed up by default)
- View available books, check book status and search books by title
- Track overdue books and calculate fine
- Reserved books can only be read within the Library premise and can't be issued.
- Faculties are not required to pay fines



## Known Issues/Runtime Issues

- Sometimes, during runtime, after entering input, you may need to press "Enter" a couple of times before the output appears. This could be due to input buffering issues in cin or how different operating systems handle console input.
- I tried to solve this issues as much as possible but still there may arise this issue again. Please follow the first point in that case.