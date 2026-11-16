#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;

// Base class
class Student {
protected:
    string studentID;
    string name;

public:
    Student(string id, string n) {
        studentID = id;
        name = n;
    }

    // Virtual function
    virtual void display() const = 0;

    virtual ~Student() {}
};

// Derived class
class Result : public Student {
private:
    int marks1, marks2, marks3;

public:
    Result(string id, string n, int m1, int m2, int m3)
        : Student(id, n) {
        marks1 = m1;
        marks2 = m2;
        marks3 = m3;
    }

    // Calculate total marks
    int totalMarks() const {
        return marks1 + marks2 + marks3;
    }

    // Determine pass or fail
    string getResult() const {
        if (marks1 >= 35 && marks2 >= 35 &&
            marks3 >= 35)
            return "PASS";

        return "FAIL";
    }

    // Overriding virtual function
    void display() const override {
        cout << "\nStudent ID : " << studentID;
        cout << "\nName       : " << name;
        cout << "\nSubject 1  : " << marks1;
        cout << "\nSubject 2  : " << marks2;
        cout << "\nSubject 3  : " << marks3;
        cout << "\nTotal Marks: " << totalMarks()
             << " / 300";
        cout << "\nPercentage : "
             << fixed << setprecision(2)
             << totalMarks() / 3.0 << "%";
        cout << "\nResult     : " << getResult();
        cout << "\n-----------------------------\n";
    }

    // Save one record to file
    void save(ofstream& file) const {
        file << studentID << "|"
             << name << "|"
             << marks1 << "|"
             << marks2 << "|"
             << marks3 << "\n";
    }
};

// Exception handling for marks
int readMark(int subject) {
    int mark;

    while (true) {
        cout << "Enter marks for Subject "
             << subject << " (0-100): ";

        string input;
        getline(cin, input);

        try {
            size_t pos;
            mark = stoi(input, &pos);

            if (pos != input.length())
                throw invalid_argument("Invalid input.");

            if (mark < 0 || mark > 100)
                throw out_of_range(
                    "Marks must be between 0 and 100.");

            return mark;
        }
        catch (const exception& e) {
            cout << "Error: " << e.what()
                 << " Please enter again.\n";
        }
    }
}

// Template function
template <typename T>
void displayCount(const vector<T>& records) {
    cout << "\nTotal number of student records: "
         << records.size() << "\n";
}

// File handling: save records
void saveToFile(const vector<Result>& records) {
    ofstream file("students.txt");

    if (!file) {
        cout << "Unable to open file for writing.\n";
        return;
    }

    for (const auto& student : records) {
        student.save(file);
    }

    file.close();
    cout << "\nRecords saved successfully.\n";
}

// File handling: retrieve records
void loadFromFile(vector<Result>& records) {
    ifstream file("students.txt");

    if (!file) {
        cout << "Unable to open file for reading.\n";
        return;
    }

    records.clear();
    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string id, name, m1, m2, m3;

        if (getline(ss, id, '|') &&
            getline(ss, name, '|') &&
            getline(ss, m1, '|') &&
            getline(ss, m2, '|') &&
            getline(ss, m3, '|')) {

            try {
                records.emplace_back(
                    id, name,
                    stoi(m1), stoi(m2), stoi(m3)
                );
            }
            catch (const exception&) {
                cout << "Skipping invalid file record.\n";
            }
        }
    }

    file.close();
    cout << "\nRecords retrieved from file.\n";
}

// Main function
int main() {
    vector<Result> students;

    int n;

    cout << "STUDENT RESULT MANAGEMENT SYSTEM\n";
    cout << "Enter number of students: ";

    while (!(cin >> n) || n <= 0) {
        cout << "Enter a valid positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Accept student details
    for (int i = 0; i < n; i++) {
        string id, name;

        cout << "\nEnter details of Student "
             << i + 1 << "\n";

        cout << "Enter Student ID: ";
        getline(cin, id);

        cout << "Enter Student Name: ";
        getline(cin, name);

        int m1 = readMark(1);
        int m2 = readMark(2);
        int m3 = readMark(3);

        students.emplace_back(id, name, m1, m2, m3);
    }

    // Display record count using template
    displayCount(students);

    // Save records
    saveToFile(students);

    // Retrieve records from file
    loadFromFile(students);

    // Runtime polymorphism using base class pointer
    cout << "\nALL STUDENT RESULTS\n";

    for (auto& student : students) {
        Student* ptr = &student;
        ptr->display();
    }

    // Identify students above specified marks
    int limit;

    cout << "\nEnter minimum total marks: ";
    while (!(cin >> limit) || limit < 0 || limit >= 300) {
        cout << "Enter a valid mark between 0 and 299: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<Result> selected;

    // STL algorithm with lambda expression
    copy_if(students.begin(), students.end(),
            back_inserter(selected),
            [limit](const Result& s) {
                return s.totalMarks() > limit;
            });

    cout << "\nSTUDENTS SCORING ABOVE "
         << limit << " MARKS\n";

    if (selected.empty()) {
        cout << "No students found.\n";
    }
    else {
        for (const auto& student : selected) {
            student.display();
        }
    }

    cout << "\nProgram executed successfully.\n";

    return 0;
}
