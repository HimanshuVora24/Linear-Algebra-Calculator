// wxWidgets "Hello world" Program

#include <wx/wx.h>
#include "gui.h"
#include <iostream>
#include "matrix.h"

enum {
    INIT_BUTTON = 1
};

wxBEGIN_EVENT_TABLE(MatrixFrame, wxFrame)
    EVT_BUTTON(INIT_BUTTON,  MatrixFrame::createEditFrame)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MatrixApp);

bool MatrixApp::OnInit() {
    MatrixFrame *frame = new MatrixFrame( "Hello World", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show( true );
    return true;
}

MatrixFrame::MatrixFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {
    
    wxBoxSizer * panelSizer = new wxBoxSizer(wxHORIZONTAL);
    

    /*wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    name = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    rows = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0L,  wxTextValidator(wxFILTER_NUMERIC));
    cols = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0L,  wxTextValidator(wxFILTER_NUMERIC));
    initButton = new wxButton(this, INIT_BUTTON, "Create Matrix");
    vbox->Add(name);
    vbox->Add(rows);
    vbox->Add(cols);
    vbox->Add(initButton);
    this->SetSizer(vbox);*/

    //initButton->Bind(wxEV)
    /*int h_size = 45;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            inputBoxes.push_back(new wxTextCtrl(this, wxID_ANY, "0", wxPoint(i*h_size + 5 * i, j*h_size + 5 * j),
             wxSize(h_size, h_size)));
        }
    }*/
    
    /*wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );*/
}

void MatrixFrame::OnExit(wxCommandEvent& event) {
    Close( true );
}

void MatrixFrame::createEditFrame(wxCommandEvent& event) {
    std::cout << "Hello" << std::endl; 
    wxString input_name = name->GetValue();
    int input_rows = wxAtoi(rows->GetValue());
    int input_cols = wxAtoi(cols->GetValue());
    editFrame = new MatrixEditFrame(this, input_name, wxDefaultPosition, wxSize(input_rows*45 + 5 * input_rows + 5, (input_cols + 1) * 45 + 5 * input_cols + 5 + 10), input_name, input_rows, input_cols);
    editFrame->Show(true);
}

MatrixEditFrame::MatrixEditFrame(MatrixFrame * parent, const wxString& title, const wxPoint& pos, const wxSize& size, wxString name, int rows, int cols) 
        : wxFrame(parent, wxID_ANY, title, pos, size) {
    m_parent = parent; 
    mat_name = name; 
    mat_rows = rows;
    mat_cols = cols; 
    m_parent->initButton->Enable(false);
    int h_size = 45;
    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            inputBoxes.push_back(new wxTextCtrl(this, wxID_ANY, "0", wxPoint(i*h_size + 5 * i + 5, j*h_size + 5 * j + 5),
             wxSize(h_size, h_size), 0L,  wxTextValidator(wxFILTER_NUMERIC)));
        }
    }
    confirmButton = new wxButton(this, wxID_ANY, "Create", wxPoint(0, cols * h_size + 30));
    confirmButton->Bind(wxEVT_BUTTON, &MatrixEditFrame::createMatrix, this);

}

void MatrixEditFrame::createMatrix(wxCommandEvent& event) {
    Matrix * mat = new Matrix(mat_rows, mat_cols);
    double value = 0; 
    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            (inputBoxes[i * 3 + j]->GetValue()).ToDouble(&value);
            mat->addEntry(i, j, value);
            inputBoxes[i * 3 + j]->Destroy();
            std::cout << "Added entry to matrix: " << value << std::endl; 
        }
    }
    if (m_parent->mat != NULL) delete m_parent->mat;
    m_parent->mat = mat; 
    m_parent->initButton->Enable(true);
    this->Destroy();
}

