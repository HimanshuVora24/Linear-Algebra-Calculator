#include "shell.h"
#include "matrix.h"
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include <queue>
#include <sstream>
#include <iomanip>

using std::cout; 
using std::endl;
using std::cin; 

std::unordered_map<std::string, Matrix *> matrices; 

void getCommandString(std::queue<std::string> &command_string) {
    std::string str; 
    
    cout << "Enter Command: ";
    getline(cin, str);

    std::istringstream strstream(str);

    while(strstream >> str) { 
        //if (str.compare("\n")) break;
        command_string.push(str); 
    }
    cout << endl; 
}


void help(std::queue<std::string> & command) {
    
    command.pop();
    if (!command.empty()) {
        std::string cmd_help = command.front();
        command.pop();
    } else {
        cout << "--Welcome to Matrix Manipulation Tool v0.1--" << endl;
        cout << endl; 
        cout << "As the name suggests, this tool allows to perform operations on inputed matrices. ";
        cout << "In this version, you can store up to 5 matrices at once. Note that vectors are also considered as matrices. ";
        cout << "Below are a list of commands you can utilize in this program. Please pay attention to the formatting of each one. ";
        cout << "The parameters of each command are provided in square brackets, which should be omitted when using" << endl; 
        cout << "Matrix names should not have any spaces" << endl;
        //cout << "Optional paramaters will also be in curly braces instead" << endl;
        cout << endl; 
        cout << "CREATE MATRIX - matrix [matrix_name] [rows] [cols] [numbers]" << endl;
        cout << "SET ENTRY - set [matrix_name] [row#] [col#] [number]" << endl;
        cout << "SET ROW - setrow [matrix_name] [row#] [numbers]" << endl;
        cout << "SET COLUMN - setcol [matrix_name] [col#] [numbers]" << endl;
        cout << "TRANSPOSE - transpose [matrix_name] [store_matrix_name]" << endl;
        cout << "INVERT - invert [matrix_name] [store_matrix_name]" << endl;
        cout << "MULTIPLY - mult [matrix_name] [matrix_name] [store_matrix_name]" << endl;
        cout << "DOT - dot [matrix_name] [matrix_name] {store_matrix_name}" << endl;
        cout << "REDUCED ECHELON - rechelon [matrix_name] [store_matrix_name]" << endl;
        //cout << "VECTOR MULTIPLY - vecmult [matrix_name] [matrix-vector#] [store_matrix_name]" << endl;
        cout << "LINEAR INDEPENDENCE - chkind [matrix_name]" << endl;
        cout << "SPANNING - chkspan [matrix_name]" << endl;
        cout << "DETERMINANT - det [matrix_name]" << endl;
        cout << "SHOW MATRIX/VECTOR - show [matrix_name]" << endl; 
        cout << "COPY - copy [source_matrix_name] [dest_matrix_name]" << endl; 
        cout << endl; 
    }
}

//"CREATE MATRIX - matrix [matrix_name] [rows] [cols] {numbers}"
void create_matrix(std::queue<std::string> &command) {
    command.pop();

    std::string n_matrix = command.front();
    command.pop();
    int rows = stoi(command.front());
    command.pop();
    int cols = stoi(command.front());
    command.pop();
    
    if (matrices.count(n_matrix)) delete matrices[n_matrix];
    matrices[n_matrix] = new Matrix(rows, cols);

    for (int i = 0; i < rows * cols; i++) {
        matrices[n_matrix]->addEntry(i%rows, i/cols, stod(command.front()));
        command.pop();
    }

}


//"SET ENTRY - set [matrix_name] [row#] [col#] [number]"
void set(std::queue<std::string> &command) {
    command.pop();
    
    std::string mat = command.front();
    command.pop();
    int row = stoi(command.front());
    command.pop();
    int col = stoi(command.front());
    command.pop();

    double num = stod(command.front());
    command.pop();

    matrices[mat]->addEntry(row, col, num);
}

//"TRANSPOSE - transpose [matrix_name] {store_matrix_name}"
void transpose(std::queue<std::string> &command) {
    command.pop();

    std::string mat = command.front();
    command.pop();
    std::string st_mat = command.front();
    command.pop();
    if (matrices.count(st_mat)) delete matrices[st_mat];

    matrices[st_mat] = new Matrix(matrices[mat]->getTranspose());
}

//"INVERT - invert [matrix_name] {store_matrix_name}"
void invert(std::queue<std::string> &command) {
    command.pop();

    std::string mat = command.front();
    command.pop();
    std::string st_mat = command.front();
    command.pop();
    if (!matrices.count(st_mat)) delete matrices[st_mat];

    matrices[st_mat] = new Matrix(matrices[mat]->inverse());
}

//"REDUCED ECHELON - rechelon [matrix_name] {store_matrix_name}"
void rechelon(std::queue<std::string> &command) {
    command.pop();

    std::string src_matrix = command.front();
    command.pop();

    std::string dest_matrix = command.front();
    command.pop();
    
    if (matrices.count(dest_matrix)) delete matrices[dest_matrix];

    if (matrices.count(src_matrix)) {
        matrices[dest_matrix] = new Matrix(matrices[src_matrix]->reducedEchelon());
    } else {
        cout << "Matrix #" << src_matrix << " does not exist" << endl;
    }
}

//"LINEAR INDEPENDENCE - chkind [matrix_name]"
void chkind(std::queue<std::string> &command) {
    command.pop();

    std::string mat = command.front();
    command.pop();

    if (matrices.count(mat)) {
        cout << "Linear independence: " << matrices[mat]->linearIndependence() << endl;
    } else {
        cout << "Matrix " << mat << " does not exist" << endl; 
    }
}

//"SPANNING - chkspan [matrix_name]"
void chkspan(std::queue<std::string> &command) {
    command.pop();

    std::string mat = command.front();
    command.pop();

    if (matrices.count(mat)) {
        cout << "Spanning: " << matrices[mat]->spanning() << endl;
    } else {
        cout << "Matrix " << mat << " does not exist" << endl; 
    }
}

//"DETERMINANT - det [matrix_name]"
void det(std::queue<std::string> &command) {
    command.pop();

    std::string mat = command.front();
    command.pop();

    if (matrices.count(mat)) {
        cout << "Determinant = " << matrices[mat]->determinant() << endl; 
    } else {
        cout << "Matrix #" << mat << " does not exist" << endl;
    }
}

//"COPY - copy [source_matrix_name] [dest_matrix_name]"
void copy(std::queue<std::string> &command) {
    command.pop();

    std::string src_matrix = command.front();
    command.pop();

    std::string dest_matrix = command.front();
    command.pop();
    
    if (matrices.count(dest_matrix)) delete matrices[dest_matrix];

    if (matrices.count(src_matrix)) {
        matrices[dest_matrix] = new Matrix(matrices[src_matrix]->reducedEchelon());
    } else {
        cout << "Matrix " << src_matrix << " does not exist" << endl;
    }
}

//"SHOW MATRIX/VECTOR - show [matrix_name]"
void show(std::queue<std::string> &command) {
    command.pop();

    std::string n_matrix = command.front();
    command.pop();
    
    if (matrices.count(n_matrix)) {
        matrices[n_matrix]->printMatrix();
    } else {
        cout << "Matrix " << n_matrix << " does not exist." << endl;
    }
}

//"MULTIPLY - mult [matrix_name] [matrix_name] {store_matrix_name}"
void multiply(std::queue<std::string> &command) {
    command.pop();
    
    std::string mat1 = command.front();
    command.pop();
    std::string mat2 = command.front();
    command.pop();
    std::string dest_matrix = command.front();
    command.pop();

    if (matrices.count(dest_matrix)) delete matrices[dest_matrix];

    matrices[dest_matrix] = new Matrix(matrices[mat1]->multiply(matrices[mat2]));
}

void quit() {
    for (auto i = matrices.begin(); i != matrices.end(); ++i) {
        delete (i->second);
    }
}


