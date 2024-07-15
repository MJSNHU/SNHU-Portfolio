//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Matt Jackson
// Version     : 1.0
// Copyright   : Copyright © 2024 SNHU COCE
// Description : Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

//============================================================================
// Course structure definition
//============================================================================

struct Course {
    string courseId; // unique identifier
    string courseName;
    vector<string> prereqs;
    void PrintCourse(Course course);
};

/**
* Given course, print ID, name, and prereqs
*
* @param course Course to print
*
*/
void Course::PrintCourse(Course course){
    cout << course.courseId << ": " << course.courseName << endl;
    if (course.prereqs.size() > 0) {
        cout << "  Prereqs:";
        for (int i = 0; i < course.prereqs.size(); i++) {
            cout << " " << course.prereqs.at(i);
        }
        cout << endl;
    }
    else {
        cout << "  No prereqs" << endl;
    }
}

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

// define a tree node structure
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

private:
    void addNode(Node* node,Course course);
    void inOrder(Node* node);
    vector<string> courseIds;

public:
    Node* root;
    BinarySearchTree();
    void PrintAll();
    void Insert(Course course);
    Course Search(string courseId);
};

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node,Course course) {
   
    // if node is larger then add to left
    if (node->course.courseId.compare(course.courseId) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    // else add node to the left
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        // recurse down the right node
        else {
            this->addNode(node->right, course);
        }
    }
}

/**
* Traverse the tree in order and print out the course details
* 
* @param node Current node in tree
*/
void BinarySearchTree::inOrder(Node* node) {
    
    if (node != nullptr) {
        inOrder(node->left);
        node->course.PrintCourse(node->course);
        inOrder(node->right);
    }
}

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initiate root to nullptr
    this->root = nullptr;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::PrintAll() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
    if (root == nullptr) {
        // set rootequal to new node with course
        root = new Node(course);
    }
    // else add Node with course starting at root
    else {
        addNode(root, course);
    }
}

/**
 * Search for a course
 * 
 * @param courseId Course to search for
 */
Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* current = root;

    // keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseId.compare(courseId) == 0) {
            return current->course;
        }
        // if courseId is smaller than current node then traverse left
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        }
        // else courseId is larger so traverse right
        else {
            current = current->right;
        }
    }
    //if no match, return empty course
    return Course();
}
   
/**
* Check that all prereqs for course exsist, if not prints error
* and removes course from course list
* 
* @param courses vector of courses to search through
*/
vector<Course> CheckPrereqs(vector<Course> courses) {
   // for each course in the list (i)
   // for each prereq in the course (j)
   // for each course in the list (k)
   // check that prereq[j] == some course[k].courseId  
    int removed = 0;
    int i = 0;
   
    for (int i = 0; i < courses.size(); i++) {
        int prereqsize = courses.at(i).prereqs.size();
        for (int j = 0; j < prereqsize; j++) {
            bool exists = false;
            for (int k = 0; k < courses.size(); k++) {
      
                if (courses.at(i).prereqs.at(j) == courses.at(k).courseId) {
                    exists = true;
                    break;
                }
            }
            // if the prereq does not exsist, print error message and remove the course          
            if (!exists) {
                cout << "Prereq " << courses.at(i).prereqs.at(j)
                    << " for " << courses.at(i).courseId
                    << " not in course list. Removing " << courses.at(i).courseId << " from list." << endl;
                courses.erase(courses.begin() + i);
                removed += 1;
                prereqsize -= 1;
            }
        }
    }
    if (removed == 0) {
        cout << "All prereqs valid." << endl;
    }
    else {
        cout << removed << " course(s) removed from list." << endl;
    }
    return courses;
}



/**
* Parse course from line in file
* returns a vector of all courses, including incorrect prereqs
* 
* @param filename The absolute location of the file to parse
*/
vector <Course> parseCourses(string filename) {

    vector <Course> courses;

    // check that filestream is properly opened
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Could not open file: " << filename << endl;
        return courses;
    }
    
    for (string line; getline(inFile, line);) {
        if (line.size() == 0) {
            continue;
        }
        vector<string> row;

        istringstream sstream(move(line));

        Course course;

        // split string into tokens delineated by a comma
        for (string token; getline(sstream, token, ',');) {
            row.push_back(token);
        }

        // if there is only one token, it is an invalid line,
        // must be at least a courseId and courseName (format of either not checked)
        if (row.size() <= 1) {
            cout << "Entry " << line << " invalid; skipping" << endl;
        }
        else {
            course.courseId = row.at(0);    // first token
            course.courseName = row.at(1);  // second token
            
            // if there are more than two tokens, the rest are prereqs
            if (row.size() > 2) {
                string token;
                for (int i = 2; i < row.size(); i++) {
                    // ignores unprintable character tokens 
                    // else add the token
                    token = row.at(i);
                    char ch = token[0];
                    if (!isprint(ch)) {
                        continue;
                    }
                    else {
                        course.prereqs.push_back(token);
                    }
                }
                
            }
            courses.push_back(course);            
        }   
    }
    inFile.close();
    return courses;
}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    
    // Declare variables
    BinarySearchTree* bst;
    bst = new BinarySearchTree();

    Course course;
    vector <Course> courses;

    string courseId;

    string csvPath;

    int choice = 0;
    while (choice != 9) {
     
        cout << "Menu:" << endl;
        cout << "  1. Load courses" << endl;
        cout << "  2. Search for a course and display information" << endl;
        cout << "  3. Print all courses sorted" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // ensure entry is of type int 
        // (reads decimals as multiple inputs; clears screen to avoid seeing multiple outputs)
        while (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            system("CLS");
            continue;
        }

        switch (choice) {

        case 1:
            cout << "Enter full path and filename: ";
            cin >> csvPath;
            cout << endl;
            courses = parseCourses(csvPath);
            //
            if (courses.empty()) {
                cout << endl;
                continue;
            }
            courses = CheckPrereqs(courses);
            for (int i = 0; i < courses.size(); i++) {
                bst->Insert(courses.at(i));
            }
            cout << courses.size() << " course(s) loaded into data structure." << endl;
            cout << endl;
            break;
        case 2:
            cout << "Enter CourseId (case sensitve): ";
            cin >> courseId;
            cout << endl;
            course = bst->Search(courseId);

            if (!course.courseId.empty()) {
                course.PrintCourse(course);
            }
            else {
                cout << "CourseId " << courseId << " not found." << endl;
            }
            cout << endl;
            break;

        case 3:
            cout << endl;
            if (bst->root == nullptr) {
                cout << "Tree is empty." << endl;
            }
            else {
                bst->PrintAll();
            }
            cout << endl;
            break;

        case 9:
            cout << "Good bye." << endl;
            return 0;

        default:
            break;
        }
    }   
}