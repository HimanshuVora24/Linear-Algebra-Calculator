#ifndef GUI_H
#define GUI_H

#include <wx/wx.h>
#include <vector>
#include <unordered_map>
#include "matrix.h"
#include <string>

class MatrixEditFrame;
class MatrixFrame; 
class MatrixManager; 
class MatrixShowFrame;

class MatrixApp: public wxApp {

public:
    virtual bool OnInit();

};

class MatrixEditFrame : public wxFrame {

public:
    MatrixFrame * m_parent;
    wxString mat_name; 
    int mat_rows;
    int mat_cols; 
    std::vector<wxTextCtrl *> inputBoxes; 
    wxButton * confirmButton;
    MatrixEditFrame(MatrixFrame * parent, const wxString& title, const wxPoint& pos, const wxSize& size, wxString name, int rows, int cols);
    void createMatrix(wxCommandEvent& event);
    void OnClose(wxCloseEvent &event);
};

class MatrixShowFrame : public wxFrame {

public:
    MatrixFrame * m_parent;
    wxString mat_name; 
    int mat_rows;
    int mat_cols; 
    std::vector<wxTextCtrl *> showBoxes; 
    wxButton * confirmButton;
    MatrixShowFrame(MatrixFrame * parent, const wxString& title, Matrix * mat, const wxPoint& pos, const wxSize& size);
    void OnClose(wxCloseEvent &event);
};


class MatrixManager {
public: 
    MatrixFrame * m_parent; 
    wxArrayString matrix_names; 
    std::unordered_map<std::string, Matrix *> matrices;

    void add(wxString& str, Matrix * mat);
    void remove(wxString &matrix);
    Matrix * get(wxString &matrix);
    void setParent(MatrixFrame * parent);
};


class MatrixFrame: public wxFrame {

private:
    void OnExit(wxCommandEvent& event);
    void createEditFrame(wxCommandEvent& event);
    void createShowFrame(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

public:
    wxButton * initButton;
    wxTextCtrl * name; 
    wxTextCtrl * rows; 
    wxTextCtrl * cols; 
    
    wxScrolledWindow * matrixOp;

    wxChoice * mult1;
    wxChoice * mult2; 
    wxButton * go_multiply; 
    
    wxChoice * echelon1;
    wxButton * go_echelon;

    wxChoice * row_echelon1; 
    wxButton * go_row_echelon;

    wxChoice * inverse1; 
    wxButton * go_inverse; 

    wxChoice * determinant1;
    wxButton * go_determinant; 

    wxChoice * spanning1; 
    wxButton * go_spanning; 

    wxChoice * linear_independence1; 
    wxButton * go_linear_independence;

    wxPanel * matrixListPanel;
    wxListBox * matrixListBox; 
    wxButton * matrixShowButton;
    wxButton * matrixDeleteButton; 
    MatrixEditFrame * editFrame; 
    Matrix * mat; 
    MatrixManager matrixList;
    MatrixFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
};


#endif