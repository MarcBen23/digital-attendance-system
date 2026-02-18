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

// ==============================
// 2. ATTENDANCE RECORD CLASS
// ==============================
class AttendanceRecord {
private:
    string studentIndex;
    char status; // 'P' = Present, 'A' = Absent, 'L' = Late
    
public:
    AttendanceRecord() : studentIndex(""), status('A') {}
    
    AttendanceRecord(string idx, char s) : studentIndex(idx), status(s) {}
    
    // Getters
    string getStudentIndex() const { return studentIndex; }
    char getStatus() const { return status; }
    
    // Setters
    void setStatus(char s) { status = s; }
    
    // Display record
    void display(const vector<Student>& students) const {
        // Find student name
        string studentName = "Unknown";
        for (const auto& student : students) {
            if (student.getIndex() == studentIndex) {
                studentName = student.getName();
                break;
            }
        }
        
        string statusText;
        switch (status) {
            case 'P': statusText = "Present"; break;
            case 'A': statusText = "Absent"; break;
            case 'L': statusText = "Late"; break;
            default: statusText = "Unknown";
        }
        
        cout << left << setw(15) << studentIndex 
             << setw(25) << studentName 
             << setw(10) << statusText << endl;
    }
    
    // Convert to string for file storage
    string toString() const {
        return studentIndex + "," + status;
    }
    
    // Load from string
    static AttendanceRecord fromString(const string& data) {
        size_t commaPos = data.find(',');
        if (commaPos != string::npos) {
            string idx = data.substr(0, commaPos);
            char status = data.substr(commaPos + 1)[0];
            return AttendanceRecord(idx, status);
        }
        return AttendanceRecord("", 'A');
    }
};

// ==============================
// 3. ATTENDANCE SESSION CLASS
// ==============================
class AttendanceSession {
private:
    string courseCode;
    string date;
    string startTime;
    int duration; // in hours
    vector<AttendanceRecord> records;
    
public:
    AttendanceSession() : courseCode(""), date(""), startTime(""), duration(0) {}
    
    AttendanceSession(string course, string d, string time, int dur) 
        : courseCode(course), date(d), startTime(time), duration(dur) {}
    
    // Getters
    string getCourseCode() const { return courseCode; }
    string getDate() const { return date; }
    string getStartTime() const { return startTime; }
    int getDuration() const { return duration; }
    vector<AttendanceRecord> getRecords() const { return records; }
    
    // Get filename for this session
    string getFilename() const {
        // Replace hyphens with underscores for filename
        string formattedDate = date;
        replace(formattedDate.begin(), formattedDate.end(), '-', '_');
        return "session_" + courseCode + "_" + formattedDate + ".txt";
    }
    
    // Add a record
    void addRecord(const AttendanceRecord& record) {
        records.push_back(record);
    }
    
    // Update a record
    bool updateRecord(string studentIndex, char status) {
        for (auto& record : records) {
            if (record.getStudentIndex() == studentIndex) {
                record.setStatus(status);
                return true;
            }
        }
        return false;
    }
    
    // Initialize records for all students
    void initializeRecords(const vector<Student>& students) {
        records.clear();
        for (const auto& student : students) {
            records.push_back(AttendanceRecord(student.getIndex(), 'A')); // Default: Absent
        }
    }
    
    // Display session info
    void displayHeader() const {
        cout << "\n==========================================" << endl;
        cout << "SESSION: " << courseCode << endl;
        cout << "DATE: " << date << endl;
        cout << "TIME: " << startTime << " (" << duration << " hours)" << endl;
        cout << "==========================================" << endl;
    }
    
    // Display attendance list
    void displayAttendance(const vector<Student>& students) const {
        cout << "\nATTENDANCE LIST:\n";
        cout << left << setw(15) << "Index Number" 
             << setw(25) << "Student Name" 
             << setw(10) << "Status" << endl;
        cout << string(50, '-') << endl;
        
        for (const auto& record : records) {
            record.display(students);
        }
    }
    
    // Generate summary statistics
    void displaySummary(const vector<Student>& students) const {
        int present = 0, absent = 0, late = 0;
        
        for (const auto& record : records) {
            switch (record.getStatus()) {
                case 'P': present++; break;
                case 'A': absent++; break;
                case 'L': late++; break;
            }
        }
        
        int total = present + absent + late;
        
        cout << "\nATTENDANCE SUMMARY:\n";
        cout << "==========================================" << endl;
        cout << "Course: " << courseCode << " (" << date << ")\n";
        cout << "Total Students: " << total << endl;
        cout << "Present: " << present << " (" 
             << (total > 0 ? (present * 100 / total) : 0) << "%)" << endl;
        cout << "Absent: " << absent << " (" 
             << (total > 0 ? (absent * 100 / total) : 0) << "%)" << endl;
        cout << "Late: " << late << " (" 
             << (total > 0 ? (late * 100 / total) : 0) << "%)" << endl;
        cout << "==========================================" << endl;
    }
    
    // Save session to file
    bool saveToFile() const {
        ofstream file(getFilename());
        if (!file.is_open()) {
            cout << "Error: Could not save session to file!" << endl;
            return false;
        }
        
        // Save session header
        file << "COURSE:" << courseCode << endl;
        file << "DATE:" << date << endl;
        file << "TIME:" << startTime << endl;
        file << "DURATION:" << duration << endl;
        file << "ATTENDANCE_RECORDS:" << endl;
        
        // Save each record
        for (const auto& record : records) {
            file << record.toString() << endl;
        }
        
        file.close();
        cout << "Session saved to: " << getFilename() << endl;
        return true;
    }
    
    // Load session from file
    bool loadFromFile(const string& filename, const vector<Student>& students) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not load session file!" << endl;
            return false;
        }
        
        records.clear();
        string line;
        bool readingRecords = false;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            if (line == "ATTENDANCE_RECORDS:") {
                readingRecords = true;
                continue;
            }
            
            if (!readingRecords) {
                // Read session info
                if (line.find("COURSE:") == 0) {
                    courseCode = line.substr(7);
                } else if (line.find("DATE:") == 0) {
                    date = line.substr(5);
                } else if (line.find("TIME:") == 0) {
                    startTime = line.substr(5);
                } else if (line.find("DURATION:") == 0) {
                    duration = stoi(line.substr(9));
                }
            } else {
                // Read attendance records
                AttendanceRecord record = AttendanceRecord::fromString(line);
                records.push_back(record);
            }
        }
        
        file.close();
        
        // If no records were loaded, initialize with current students
        if (records.empty()) {
            initializeRecords(students);
        }
        
        cout << "Session loaded from: " << filename << endl;
        return true;
    }
};
