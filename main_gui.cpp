// wxWidgets "Hello world" Program

#include <wx/wx.h>
#include "gui.h"
#include <iostream>
#include "matrix.h"

enum {
    INIT_BUTTON = 1,
    SHOW_BUTTON = 2
};

wxBEGIN_EVENT_TABLE(MatrixFrame, wxFrame)
    EVT_BUTTON(INIT_BUTTON,  MatrixFrame::createEditFrame)
    EVT_BUTTON(SHOW_BUTTON, MatrixFrame::createShowFrame)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MatrixApp);

bool MatrixApp::OnInit() {
    MatrixFrame *frame = new MatrixFrame( "Hello World", wxPoint(50, 50), wxSize(640, 480) );
    frame->Show( true );
    return true;
}

MatrixFrame::MatrixFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size) {
    
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
    wxBoxSizer * panelSizer = new wxBoxSizer(wxHORIZONTAL);
	matrixOp = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE|wxHSCROLL|wxVSCROLL );
    matrixOp->SetScrollRate(5, 5);
    
    matrixListPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE|wxTAB_TRAVERSAL );

    wxBoxSizer * operations = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer * new_box = new wxBoxSizer(wxHORIZONTAL);
    new_box->SetMinSize( wxSize( -1,40 ) );
    new_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("New: ")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	name = new wxTextCtrl(matrixOp, wxID_ANY, "(Name)",wxDefaultPosition, wxDefaultSize);
    new_box->Add(name, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    new_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("(r)"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    rows = new wxTextCtrl(matrixOp, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(35, -1), 0L,  wxTextValidator(wxFILTER_NUMERIC));
    new_box->Add(rows, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    new_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("(c)"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    cols = new wxTextCtrl(matrixOp, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(35, -1), 0L,  wxTextValidator(wxFILTER_NUMERIC));
    new_box->Add(cols, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	initButton = new wxButton(matrixOp, INIT_BUTTON, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    new_box->Add(initButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * multiply_box = new wxBoxSizer(wxHORIZONTAL);
    multiply_box->SetMinSize( wxSize( -1,40 ) );
    multiply_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Multiply: ")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mult1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    multiply_box->Add(mult1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    multiply_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mult2 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    multiply_box->Add(mult2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_multiply = new wxButton(matrixOp, wxID_ANY, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    multiply_box->Add(go_multiply, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * echelon_box = new wxBoxSizer(wxHORIZONTAL);
    echelon_box->SetMinSize( wxSize( -1,40 ) );
    echelon_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Echelon: ")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    echelon1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    echelon_box->Add(echelon1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_echelon = new wxButton(matrixOp, wxID_ANY, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    echelon_box->Add(go_echelon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * row_echelon_box = new wxBoxSizer(wxHORIZONTAL);
    row_echelon_box->SetMinSize( wxSize( -1,40 ) );
    row_echelon_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Row Echelon: ")), 0, wxALIGN_CENTER_VERTICAL| wxALL, 5);
    row_echelon1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    row_echelon_box->Add(row_echelon1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_row_echelon = new wxButton(matrixOp, wxID_ANY, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    row_echelon_box->Add(go_row_echelon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * inverse_box = new wxBoxSizer(wxHORIZONTAL);
    inverse_box->SetMinSize( wxSize( -1,40 ) );
    inverse_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Inverse: ")), 0, wxALIGN_CENTER_VERTICAL| wxALL, 5);
    inverse1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    inverse_box->Add(inverse1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_inverse = new wxButton(matrixOp, wxID_ANY, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    inverse_box->Add(go_inverse, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * determinant_box = new wxBoxSizer(wxHORIZONTAL);
    determinant_box->SetMinSize( wxSize( -1,40 ) );
    determinant_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Determinant: ")), 0, wxALIGN_CENTER_VERTICAL| wxALL, 5);
    determinant1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    determinant_box->Add(determinant1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_determinant = new wxButton(matrixOp, wxID_ANY, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    determinant_box->Add(go_determinant, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * spanning_box = new wxBoxSizer(wxHORIZONTAL);
    spanning_box->SetMinSize( wxSize( -1,40 ) );
    spanning_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Spanning: ")), 0, wxALIGN_CENTER_VERTICAL| wxALL, 5);
    spanning1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    spanning_box->Add(spanning1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_spanning = new wxButton(matrixOp, wxID_ANY, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    spanning_box->Add(go_spanning, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * linear_independence_box = new wxBoxSizer(wxHORIZONTAL);
    linear_independence_box->SetMinSize( wxSize( -1,40 ) );
    linear_independence_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Linear Independence: ")), 0, wxALIGN_CENTER_VERTICAL| wxALL, 5);
    linear_independence1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    linear_independence_box->Add(linear_independence1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_linear_independence = new wxButton(matrixOp, wxID_ANY, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    linear_independence_box->Add(go_linear_independence, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    operations->Add(new_box, 1, wxEXPAND, 5);
    operations->Add(multiply_box, 1, wxEXPAND, 5);
    operations->Add(echelon_box, 1, wxEXPAND, 5);
    operations->Add(row_echelon_box, 1, wxEXPAND, 5);
    operations->Add(inverse_box, 1, wxEXPAND, 5);
    operations->Add(determinant_box, 1, wxEXPAND, 5);
    operations->Add(spanning_box, 1, wxEXPAND, 5);
    operations->Add(linear_independence_box, 1, wxEXPAND, 5);
    matrixOp->SetSizer(operations);
    matrixOp->Layout();
    operations->Fit(matrixOp);
    
    panelSizer->Add(matrixOp, 2, wxEXPAND | wxALL, 5);

    wxBoxSizer * listSizer = new wxBoxSizer(wxVERTICAL);
    matrixListBox = new wxListBox(matrixListPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	listSizer->Add(matrixListBox, 5, wxEXPAND | wxALL, 5 );

    wxBoxSizer * listButtonsBox = new wxBoxSizer(wxHORIZONTAL);
	matrixShowButton = new wxButton( matrixListPanel, SHOW_BUTTON, wxT("Show"), wxDefaultPosition, wxDefaultSize, 0 );
	listButtonsBox->Add( matrixShowButton, 0, wxALL, 5 );
	matrixDeleteButton = new wxButton( matrixListPanel, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	listButtonsBox->Add( matrixDeleteButton, 0, wxALL, 5 );

    listSizer->Add(listButtonsBox, 1, wxALIGN_BOTTOM | wxALL |wxEXPAND, 5);

    matrixListPanel->SetSizer(listSizer);
    matrixListPanel->Layout();
    listSizer->Fit(matrixListPanel);
	panelSizer->Add(matrixListPanel, 1, wxEXPAND | wxALL, 5);

    this->SetSizer(panelSizer);
    this->Layout();
    this->Centre( wxBOTH );

    matrixList.setParent(this);
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

void MatrixFrame::createShowFrame(wxCommandEvent& event) {
    wxString name = matrixListBox->GetString(matrixListBox->GetSelection());
    Matrix * mat = matrixList.get(name);
    MatrixShowFrame * frame = new MatrixShowFrame(this, name, mat, wxDefaultPosition, wxDefaultSize);
    frame->Show();
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
    this->Bind(wxEVT_CLOSE_WINDOW, &MatrixEditFrame::OnClose, this);
}

void MatrixEditFrame::createMatrix(wxCommandEvent& event) {
    Matrix * mat = new Matrix(mat_rows, mat_cols);
    double value = 0; 
    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            (inputBoxes[i * mat_rows + j]->GetValue()).ToDouble(&value);
            mat->addEntry(i, j, value);
            inputBoxes[i * mat_rows + j]->Destroy();
            std::cout << "Added entry to matrix: " << value << std::endl; 
        }
    }

    m_parent->matrixList.add(mat_name, mat);
    this->Close();
}

void MatrixEditFrame::OnClose(wxCloseEvent &event) {
    m_parent->initButton->Enable(true);
    this->Destroy();
}

void MatrixManager::add(wxString& str, Matrix * matrix) {
    matrices.insert({str.ToStdString(), matrix});
    matrix_names.Add(str);
    m_parent->matrixListBox->Append(str);
}

void MatrixManager::setParent(MatrixFrame * parent) {
    m_parent = parent; 
}

Matrix * MatrixManager::get(wxString & str) {
    return matrices.at(str.ToStdString());
}

MatrixShowFrame::MatrixShowFrame(MatrixFrame * parent, const wxString& title, Matrix * mat, const wxPoint& pos, const wxSize& size) 
    : wxFrame(parent, wxID_ANY, title, pos, size) {

    wxFlexGridSizer * matrix_grid = new wxFlexGridSizer(mat->m, mat->n, wxDefaultSize);
    for (int i = 0; i < mat->m; i++) {
        for (int j = 0; j < mat->n; j++) {
            wxString entry;
            entry << mat->getEntry(i,j);
            matrix_grid->Add(new wxStaticText(this, wxID_ANY, entry), 1, wxALL); 
        }
    }
    this->SetSizer(matrix_grid);
    this->Centre();

}
