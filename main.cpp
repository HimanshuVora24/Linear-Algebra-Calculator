#include "matrix.h"
#include "shell.h"
#include <map>
#include <iostream>
#include <string>
#include <stdexcept> 
#include <functional>
#include <queue>

using std::cout; 
using std::endl;
using std::cin; 

std::map<std::string, std::function<void(std::queue<std::string> &)>> function_map {
    {"help", &help},
    {"matrix", &create_matrix},
    {"copy", &copy},
    {"show", &show},
    {"multiply", &multiply},
    {"transpose", &transpose},
    {"invert", &invert},
    {"set", &set},
    {"rechelon", &rechelon},
    {"chkind", &chkind},
    {"chkspan", &chkspan},
    {"det", &det}
};


int main() {
    /*Matrix matrix(3,3);
    matrix.changeRow(0, std::vector<double>{1, 1, 2}); 
    matrix.changeRow(1, std::vector<double>{1, 1, 0});
    matrix.changeRow(2, std::vector<double>{2, 2, 0});
    //matrix.sortByPivot(0);
    matrix.printMatrix();
    matrix.reducedEchelon().printMatrix();
    //matrix.reducedEchelon().printMatrix();
    cout << matrix.determinant() << endl;

    return 0; */
    cout << "Welcome to Matrix Calculator. Please type \'help\' to see all the commands" << endl;
    while (true) {
        cout << endl; 
        std::queue<std::string> command_string;
        getCommandString(command_string);
        if (command_string.empty()) continue; 

        if (command_string.front().compare("quit") == 0 || command_string.front().compare("q") == 0) break; 
        //cout << command_string.front() << endl;
        try {
            (function_map.at(command_string.front()))(command_string);            
        } catch (const std::out_of_range &exp) {
            cout << "Command does not exist. Please use \"help\" to see commands" << endl; 
        }
    }

    return 0; 
}