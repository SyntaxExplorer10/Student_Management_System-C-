#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <limits>
using namespace std;

class Student {
private:
    string Name, City;
    int Age;
    char Grade;
public:
    Student(string name, int age, char grade, string city) {
        Name = name; Age = age; Grade = grade; City = city;
    }
    string getName() const { return Name; }
    int    getAge()  const { return Age; }
    char   getGrade()const { return Grade; }
    string getCity() const { return City; }

    void Display() {
        cout << "Student Name  : " << Name  << endl;
        cout << "Student Age   : " << Age   << endl;
        cout << "Student Grade : " << Grade << endl;
        cout << "Student City  : " << City  << endl;
        cout << "------------------" << endl;
    }
};

void Savetofile(vector<Student> &student) {
    ofstream outFile("Students.txt");
    if (!outFile) { cout << "Error, File cannot open!" << endl; return; }
    for (auto &s : student)
        outFile << s.getName() << "," << s.getAge() << ","
                << s.getGrade() << "," << s.getCity() << "\n";
    outFile.close();
}

void loadFromFile(vector<Student> &students) {
    ifstream inFile("Students.txt");
    if (!inFile) return;
    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;
        string name  = line.substr(0, p1);
        int    age   = stoi(line.substr(p1 + 1, p2 - p1 - 1));
        char   grade = line.substr(p2 + 1, p3 - p2 - 1)[0];
        string city  = line.substr(p3 + 1);
        students.emplace_back(name, age, grade, city);
    }
    inFile.close();
}

// NEW FUNCTION: Delete all students at once
void DeleteAllStudents(vector<Student> &students) {
    if (students.empty()) {
        cout << "No students to delete!" << endl;
        return;
    }

    char confirm;
    cout << "  WARNING: You are about to delete ALL " << students.size()
         << " students! " << endl;
    cout << "Are you sure? (y/N): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        students.clear();
        Savetofile(students);
        cout << " All students have been deleted successfully!" << endl;
    } else {
        cout << " Deletion cancelled." << endl;
    }
}

void HSortByAge(vector<Student> &s) { sort(s.begin(), s.end(), [](Student a, Student b){ return a.getAge() > b.getAge(); }); }
void LSortByAge(vector<Student> &s) { sort(s.begin(), s.end(), [](Student a, Student b){ return a.getAge() < b.getAge(); }); }
void SortByName(vector<Student> &s) { sort(s.begin(), s.end(), [](Student a, Student b){ return a.getName() < b.getName(); }); }
void SortByGrade(vector<Student> &s){ sort(s.begin(), s.end(), [](Student a, Student b){ return a.getGrade() < b.getGrade(); }); }

void showSummary(vector<Student> &students) {
    if (students.empty()) { cout << "No Students Exist!" << endl; return; }
    int total = students.size();
    map<char, int> gradeCount;
    map<string, int> cityCount;
    for (auto &s : students) {
        gradeCount[s.getGrade()]++;
        cityCount[s.getCity()]++;
    }
    cout << "\n------ Summary ------" << endl;
    cout << "Total Students : " << total << endl;
    cout << "\n.... Grade ...." << endl;
    for (auto &p : gradeCount)
        cout << "  Grade " << p.first << " : " << p.second << endl;
    cout << "\n.... City ...." << endl;
    for (auto &p : cityCount)
        cout << "  " << p.first << " : " << p.second << endl;
}

int main() {
    vector<Student> student;
    loadFromFile(student);

    int choice;
    do {
        cout << "\n====Student Manager====" << endl;
        cout << "1. Add Student"  << endl;
        cout << "2. Display"      << endl;
        cout << "3. Delete"       << endl;
        cout << "4. Search"       << endl;
        cout << "5. Sort"         << endl;
        cout << "6. Summary"      << endl;
        cout << "7. Delete ALL Students" << endl;
        cout << "8. Exit"         << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice) {

        case 1: {
            int n;
            cout << "Enter number of students to add: ";
            cin >> n;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string name, city; int age; char grade;
            for (int i = 0; i < n; i++) {
                cout << "\n-- Student " << i+1 << " --" << endl;
                cout << "Enter Name  : "; getline(cin, name);
                cout << "Enter City  : "; getline(cin, city);
                cout << "Enter Age   : "; cin >> age;
                cout << "Enter Grade : "; cin >> grade;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                student.push_back(Student(name, age, grade, city));
                cout << "Student Added!" << endl;
            }
            Savetofile(student);
            break;
        }

        case 2: {
            if (student.empty()) { cout << "No students yet!" << endl; break; }
            cout << "\n--- All Students ---" << endl;
            int index = 1;
            for (auto &s : student) {
                cout << "Student #" << index++ << endl;
                s.Display();
            }
            break;
        }

        case 3: {
            if (student.empty()) { cout << "No students to delete!" << endl; break; }
            cout << "\n--- All Students ---" << endl;
            int index = 1;
            for (auto &s : student) {
                cout << "Student #" << index++ << endl;
                s.Display();
            }
            int num;
            cout << "Enter number to delete: ";
            cin >> num;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (num >= 1 && num <= (int)student.size()) {
                student.erase(student.begin() + (num - 1));
                cout << "Deleted!" << endl;
                Savetofile(student);
            } else {
                cout << "Invalid number!" << endl;
            }
            cout << "\n--- After Delete ---" << endl;
            index = 1;
            for (auto &s : student) {
                cout << "Student #" << index++ << endl;
                s.Display();
            }
            break;
        }

        case 4: {
            if (student.empty()) { cout << "No students to search!" << endl; break; }
            string Key; bool found = false; int i = 1;
            cout << "Enter keyword to search: ";
            getline(cin, Key);
            cout << "Searching for: [" << Key << "]" << endl;
            for (auto &w : student) {
                if (w.getName().find(Key) != string::npos ||
                    w.getCity().find(Key) != string::npos) {
                    cout << "Found in Student #" << i << "!" << endl;
                    w.Display();
                    found = true;
                }
                i++;
            }
            if (!found) cout << "Not Found!" << endl;
            break;
        }

        case 5: {
            if (student.empty()) { cout << "No students to sort!" << endl; break; }
            int sortChoice;
            cout << "\n1. Age (low to high)" << endl;
            cout << "2. Age (high to low)"  << endl;
            cout << "3. Name (A to Z)"      << endl;
            cout << "4. Grade (A to Z)"     << endl;
            cout << "Enter sort choice: ";
            cin >> sortChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch(sortChoice) {
                case 1: LSortByAge(student);  cout << "Sorted by Age (low-high)!"  << endl; break;
                case 2: HSortByAge(student);  cout << "Sorted by Age (high-low)!"  << endl; break;
                case 3: SortByName(student);  cout << "Sorted by Name (A-Z)!"      << endl; break;
                case 4: SortByGrade(student); cout << "Sorted by Grade (A-Z)!"     << endl; break;
                default: cout << "Invalid choice!" << endl;
            }
            Savetofile(student);
            int idx = 1;
            for (auto &s : student) {
                cout << "Student #" << idx++ << endl;
                s.Display();
            }
            break;
        }

        case 6:
            showSummary(student);
            break;

        case 7:
            DeleteAllStudents(student);
            break;

        case 8:
            cout << "Thank you for using Student Manager! Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice! Enter 1-8." << endl;
        }

    } while (choice != 8);

    return 0;
}
