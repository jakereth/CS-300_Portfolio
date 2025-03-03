
// Jacob Rethmeier 2/23/2025 CS 300


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000;

struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;

        Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
    };

    Node* root;
    void inOrder(Node* node);

public:
    BinarySearchTree() { root = nullptr; }
    void PrintCourseList();
    void Insert(Course aCourse);
    Course Search(string courseId);
};

void BinarySearchTree::PrintCourseList() {
    inOrder(root);
}

void BinarySearchTree::Insert(Course aCourse) {
    if (!root) {
        root = new Node(aCourse);
    } else {
        Node* currentNode = root;
        while (true) {
            if (aCourse.courseId < currentNode->course.courseId) {
                if (!currentNode->left) {
                    currentNode->left = new Node(aCourse);
                    break;
                }
                currentNode = currentNode->left;
            } else {
                if (!currentNode->right) {
                    currentNode->right = new Node(aCourse);
                    break;
                }
                currentNode = currentNode->right;
            }
        }
    }
}

Course BinarySearchTree::Search(string courseId) {
    Node* currentNode = root;
    while (currentNode) {
        if (currentNode->course.courseId == courseId) return currentNode->course;
        currentNode = (courseId < currentNode->course.courseId) ? currentNode->left : currentNode->right;
    }
    return Course();
}

void BinarySearchTree::inOrder(Node* node) {
    if (!node) return;
    inOrder(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    inOrder(node->right);
}

vector<string> Split(string lineFeed) {
    vector<string> tokens;
    string temp = "";
    lineFeed += ',';
    for (char ch : lineFeed) {
        if (ch == ',') {
            tokens.push_back(temp);
            temp = "";
        } else {
            temp += ch;
        }
    }
    return tokens;
}

void loadCourses(string csvPath, BinarySearchTree* courseList) {
    ifstream inFS(csvPath);
    string line;
    vector<string> tokens;

    if (!inFS.is_open()) {
        cout << "Could not open file." << endl;
        return;
    }

    while (getline(inFS, line)) {
        Course aCourse;
        tokens = Split(line);

        if (tokens.size() < 2) {
            cout << "\nSkipping malformed line." << endl;
            continue;
        }

        aCourse.courseId = tokens[0];
        aCourse.courseName = tokens[1];

        for (size_t i = 2; i < tokens.size(); i++)
            aCourse.preList.push_back(tokens[i]);

        courseList->Insert(aCourse);
    }

    inFS.close();
}

void displayCourse(Course aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: " << (aCourse.preList.empty() ? "None" : "");

    for (size_t i = 0; i < aCourse.preList.size(); i++) {
        cout << aCourse.preList[i];
        if (i < aCourse.preList.size() - 1) cout << ", ";
    }
    cout << endl;
}

void convertCase(string &toConvert) {
    for (char &ch : toConvert) if (isalpha(ch)) ch = toupper(ch);
}

int main(int argc, char* argv[]) {
    string csvPath = "FinalProject_Sample_input.csv";
    string aCourseKey;
    BinarySearchTree* courseList = new BinarySearchTree();
    Course course;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure.\n";
        cout << "2. Print Course List.\n";
        cout << "3. Print Course.\n";
        cout << "9. Exit\n";
        cout << "What would you like to do? ";

        cin >> choice;
        system("cls");

        switch (choice) {
            case 1:
                loadCourses(csvPath, courseList);
                cout << "Data loaded successfully.\n";
                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 2:
                cout << "Here is a sample schedule:\n";
                courseList->PrintCourseList();
                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 3:
                cout << "What course do you want to know about? ";
                cin >> aCourseKey;
                convertCase(aCourseKey);
                course = courseList->Search(aCourseKey);

                if (course.courseId.empty()) {
                    cout << "\nCourse not found.\n";
                } else {
                    displayCourse(course);
                }
                Sleep(GLOBAL_SLEEP_TIME);
                break;

            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;

            default:
                cout << choice << " is not a valid option.\n";
        }

        cin.clear();
        cin.ignore();
    }

    return 0;
}