#include "shell.h"
#include "matrix.h"
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <queue>
#include <sstream>
#include <iomanip>

using std::cout; 
using std::endl;
using std::cin; 

std::vector<Matrix *> matrices(5, NULL); 

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
        //cout << "Optional paramaters will also be in curly braces instead" << endl;
        cout << endl; 
        cout << "CREATE MATRIX - matrix [matrix#] [rows] [cols] [numbers]" << endl;
        cout << "SET ENTRY - set [matrix#] [row#] [col#] [number]" << endl;
        cout << "SET ROW - setrow [matrix#] [row#] [numbers]" << endl;
        cout << "SET COLUMN - setcol [matrix#] [col#] [numbers]" << endl;
        cout << "TRANSPOSE - transpose [matrix#] [store_matrix#]" << endl;
        cout << "INVERT - invert [matrix#] [store_matrix#]" << endl;
        cout << "MULTIPLY - mult [matrix#] [matrix#] [store_matrix#]" << endl;
        cout << "DOT - dot [matrix#] [matrix#] {store_matrix#}" << endl;
        cout << "REDUCED ECHELON - rechelon [matrix#] [store_matrix#]" << endl;
        //cout << "VECTOR MULTIPLY - vecmult [matrix#] [matrix-vector#] [store_matrix#]" << endl;
        cout << "LINEAR INDEPENDENCE - chkind [matrix#]" << endl;
        cout << "SPANNING - chkspan [matrix#]" << endl;
        cout << "DETERMINANT - det [matrix#]" << endl;
        cout << "SHOW MATRIX/VECTOR - show [matrix#]" << endl; 
        cout << "COPY - copy [source_matrix#] [dest_matrix#]" << endl; 
        cout << endl; 
    }
}

//"CREATE MATRIX - matrix [matrix#] [rows] [cols] {numbers}"
void create_matrix(std::queue<std::string> &command) {
    command.pop();

    int n_matrix = stoi(command.front());
    command.pop();
    int rows = stoi(command.front());
    command.pop();
    int cols = stoi(command.front());
    command.pop();
    
    if (matrices[n_matrix] != NULL) free(matrices[n_matrix]);
    matrices[n_matrix] = new Matrix(rows, cols);

    for (int i = 0; i < rows * cols; i++) {
        matrices[n_matrix]->addEntry(i%rows, i/cols, stod(command.front()));
        command.pop();
    }

}


//"SET ENTRY - set [matrix#] [row#] [col#] [number]"
void set(std::queue<std::string> &command) {
    command.pop();
    
    int mat = stoi(command.front());
    command.pop();
    int row = stoi(command.front());
    command.pop();
    int col = stoi(command.front());
    command.pop();

    double num = stod(command.front());
    command.pop();

    matrices[mat]->addEntry(row, col, num);
}

//"TRANSPOSE - transpose [matrix#] {store_matrix#}"
void transpose(std::queue<std::string> &command) {
    command.pop();

    int mat = stoi(command.front());
    command.pop();
    int st_mat = stoi(command.front());
    command.pop();
    if (matrices[st_mat] != NULL) free(matrices[st_mat]);

    matrices[st_mat] = new Matrix(matrices[mat]->getTranspose());
}

//"INVERT - invert [matrix#] {store_matrix#}"
void invert(std::queue<std::string> &command) {
    command.pop();

    int mat = stoi(command.front());
    command.pop();
    int st_mat = stoi(command.front());
    command.pop();
    if (matrices[st_mat] != NULL) free(matrices[st_mat]);

    matrices[st_mat] = new Matrix(matrices[mat]->inverse());
}

//"REDUCED ECHELON - rechelon [matrix#] {store_matrix#}"
void rechelon(std::queue<std::string> &command) {
    command.pop();

    int src_matrix = stoi(command.front());
    command.pop();

    int dest_matrix = stoi(command.front());
    command.pop();
    
    if (matrices[dest_matrix] != NULL) free(matrices[dest_matrix]);
    
    if (matrices[src_matrix] != NULL) {
        matrices[dest_matrix] = new Matrix(matrices[src_matrix]->reducedEchelon());
    } else {
        cout << "Matrix #" << src_matrix << " does not exist" << endl;
    }
}

//"LINEAR INDEPENDENCE - chkind [matrix#]"
void chkind(std::queue<std::string> &command) {
    command.pop();

    int mat = stoi(command.front());
    command.pop();

    if (matrices[mat] != NULL) {
        cout << "Linear independence: " << matrices[mat]->linearIndependence() << endl;
    } else {
        cout << "Matrix #" << mat << " does not exist" << endl; 
    }
}

//"SPANNING - chkspan [matrix#]"
void chkspan(std::queue<std::string> &command) {
    command.pop();

    int mat = stoi(command.front());
    command.pop();

    if (matrices[mat] != NULL) {
        cout << "Spanning: " << matrices[mat]->spanning() << endl;
    } else {
        cout << "Matrix #" << mat << " does not exist" << endl; 
    }
}

//"DETERMINANT - det [matrix#]"
void det(std::queue<std::string> &command) {
    command.pop();

    int mat = stoi(command.front());
    command.pop();

    if (matrices[mat] != NULL) {
        cout << "Determinant = " << matrices[mat]->determinant() << endl; 
    } else {
        cout << "Matrix #" << mat << " does not exist" << endl;
    }
}

//"COPY - copy [source_matrix#] [dest_matrix#]"
void copy(std::queue<std::string> &command) {
    command.pop();

    int src_matrix = stoi(command.front());
    command.pop();
    int dest_matrix = stoi(command.front());
    command.pop();

    if (matrices[dest_matrix] != NULL) free(matrices[dest_matrix]);
    
    if (matrices[src_matrix] != NULL) {
        matrices[dest_matrix] = new Matrix(*matrices[src_matrix]);
    }
}

//"SHOW MATRIX/VECTOR - show [matrix#]"
void show(std::queue<std::string> &command) {
    command.pop();

    int n_matrix = stoi(command.front());
    command.pop();
    
    if (matrices[n_matrix] != NULL) {
        matrices[n_matrix]->printMatrix();
    } else {
        cout << "Matrix " << n_matrix << " does not exist." << endl;
    }
}

//"MULTIPLY - mult [matrix#] [matrix#] {store_matrix#}"
void multiply(std::queue<std::string> &command) {
    command.pop();
    
    int mat1 = stoi(command.front());
    command.pop();
    int mat2 = stoi(command.front());
    command.pop();
    int dest_matrix = stoi(command.front());
    command.pop();

    if (matrices[dest_matrix] != NULL) free(matrices[dest_matrix]);

    matrices[dest_matrix] = new Matrix(matrices[mat1]->multiply(matrices[mat2]));
}


