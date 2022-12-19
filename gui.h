#ifndef GUI_H
#define GUI_H

#include <wx/wx.h>
#include <vector>
#include "matrix.h"


class MatrixEditFrame;
class MatrixFrame; 
class MatrixOpPanel;

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
};

class MatrixFrame: public wxFrame {

private:
    void OnExit(wxCommandEvent& event);
    void createEditFrame(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

public:
    wxButton * initButton;
    wxTextCtrl * name; 
    wxTextCtrl * rows; 
    wxTextCtrl * cols; 
    wxPanel * MatrixOpPanel;
    wxPanel * MatrixListPanel;
    MatrixEditFrame * editFrame; 
    Matrix * mat; 
    MatrixFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
};

class MatrixOpPanel : public wxPanel {

public: 
    MatrixFrame * parent;
    
    MatrixOpPanel(wxWindow * parent);   
};

class MatrixListPanel : public wxPanel {

public:
    MatrixFrame * parent;
    MatrixListPanel(wxWindow * parent);
};

#endif