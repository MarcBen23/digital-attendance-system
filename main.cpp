#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <limits>

using namespace std;

// ==============================
// 1. STUDENT CLASS
// ==============================
class Student {
private:
    string indexNumber;
    string name;
    
public:
    Student() : indexNumber(""), name("") {}
    
    Student(string idx, string n) : indexNumber(idx), name(n) {}
    
    // Getters
    string getIndex() const { return indexNumber; }
    string getName() const { return name; }
    
    // Setters
    void setIndex(string idx) { indexNumber = idx; }
    void setName(string n) { name = n; }
    
    // Display student info
    void display() const {
        cout << left << setw(15) << indexNumber 
             << setw(25) << name << endl;
    }
    
    // Convert to string for file storage
    string toString() const {
        return indexNumber + "," + name;
    }
    
    // Load from string (for file reading)
    static Student fromString(const string& data) {
        size_t commaPos = data.find(',');
        if (commaPos != string::npos) {
            string idx = data.substr(0, commaPos);
            string name = data.substr(commaPos + 1);
            return Student(idx, name);
        }
        return Student("", "");
    }
};


