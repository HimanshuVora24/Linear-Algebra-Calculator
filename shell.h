#ifndef SHELL_H
#define SHELL_H

#include <queue> 
#include <string> 

void getCommandString(std::queue<std::string> &command_string);
void help(std::queue<std::string> & command);
void create_matrix(std::queue<std::string> &command);
void set(std::queue<std::string> &command);
void transpose(std::queue<std::string> &command);
void invert(std::queue<std::string> &command);
void rechelon(std::queue<std::string> &command);
void chkind(std::queue<std::string> &command);
void chkspan(std::queue<std::string> &command);
void det(std::queue<std::string> &command);
void copy(std::queue<std::string> &command);
void show(std::queue<std::string> &command);
void multiply(std::queue<std::string> &command);
void quit();

#endif