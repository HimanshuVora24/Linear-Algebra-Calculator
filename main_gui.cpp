// wxWidgets "Hello world" Program

#include <wx/wx.h>
#include "gui.h"
#include <iostream>
#include "matrix.h"
#include <cmath>

enum {
    INIT_BUTTON = 1,
    SHOW_BUTTON = 2,
    DELETE_BUTTON = 3,
    SAVE_BUTTON = 4,
    INVERSE,
    ECHELON,
    ROW_ECHELON,
    LINEAR_INDEPENDENCE,
    DETERMINANT
};

wxBEGIN_EVENT_TABLE(MatrixFrame, wxFrame)
    EVT_BUTTON(INIT_BUTTON,  MatrixFrame::createEditFrame)
    EVT_BUTTON(SHOW_BUTTON, MatrixFrame::createShowFrame)
    EVT_BUTTON(DELETE_BUTTON, MatrixFrame::deleteMatrix)
    EVT_BUTTON(INVERSE, MatrixFrame::inverseMatrix)
    EVT_BUTTON(ROW_ECHELON, MatrixFrame::rowEchelonMatrix)
    EVT_BUTTON(LINEAR_INDEPENDENCE, MatrixFrame::linearIndepedentMatrix)
    EVT_BUTTON(DETERMINANT, MatrixFrame::determinantMatrix)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MatrixApp);
 
// https://stackoverflow.com/a/41434490/19170967
wxString decimalToFraction(double x) {
    wxString str = "";

    if (x < 0){
        return "-" + decimalToFraction(-x);
    }
    double tolerance = 1.0E-6;
    double h1=1; double h2=0;
    double k1=0; double k2=1;
    double b = x;
    do {
        double a = floor(b);
        double aux = h1; h1 = a*h1+h2; h2 = aux;
        aux = k1; k1 = a*k1+k2; k2 = aux;
        b = 1/(b-a);
    } while (abs(x-h1/k1) > x*tolerance);
    if (k1 == 1) str << x; 
    else str << h1 << "/" << k1; 
    return str; 
}

bool MatrixApp::OnInit() {
    MatrixFrame *frame = new MatrixFrame( "Hello World", wxPoint(50, 50), wxSize(640, 480) );
    frame->Show( true );
    return true;
}

void MatrixFrame::OnClose(wxCloseEvent &event) {
    for (int i = 0; i < (int) matrixList->matrix_names->GetCount(); i++) {
        Matrix * m = matrixList->get((*(matrixList->matrix_names))[i]);
        delete m; 
    }
    matrixList->matrix_names->Clear();
    delete matrixList->matrix_names; 
    delete matrixList->matrices;
    delete matrixList; 
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
	go_echelon = new wxButton(matrixOp, ECHELON, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    echelon_box->Add(go_echelon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * row_echelon_box = new wxBoxSizer(wxHORIZONTAL);
    row_echelon_box->SetMinSize( wxSize( -1,40 ) );
    row_echelon_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Row Echelon: ")), 0, wxALIGN_CENTER_VERTICAL| wxALL, 5);
    row_echelon1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    row_echelon_box->Add(row_echelon1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_row_echelon = new wxButton(matrixOp, ROW_ECHELON, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    row_echelon_box->Add(go_row_echelon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * inverse_box = new wxBoxSizer(wxHORIZONTAL);
    inverse_box->SetMinSize( wxSize( -1,40 ) );
    inverse_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Inverse: ")), 0, wxALIGN_CENTER_VERTICAL| wxALL, 5);
    inverse1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    inverse_box->Add(inverse1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_inverse = new wxButton(matrixOp, INVERSE, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
    inverse_box->Add(go_inverse, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer * determinant_box = new wxBoxSizer(wxHORIZONTAL);
    determinant_box->SetMinSize( wxSize( -1,40 ) );
    determinant_box->Add(new wxStaticText(matrixOp, wxID_ANY, wxT("Determinant: ")), 0, wxALIGN_CENTER_VERTICAL| wxALL, 5);
    determinant1 = new wxChoice(matrixOp, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    determinant_box->Add(determinant1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	go_determinant = new wxButton(matrixOp, DETERMINANT, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
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
	go_linear_independence = new wxButton(matrixOp, LINEAR_INDEPENDENCE, wxT("Go"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
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
	matrixDeleteButton = new wxButton( matrixListPanel, DELETE_BUTTON, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	listButtonsBox->Add( matrixDeleteButton, 0, wxALL, 5 );

    listSizer->Add(listButtonsBox, 1, wxALIGN_BOTTOM | wxALL |wxEXPAND, 5);

    matrixListPanel->SetSizer(listSizer);
    matrixListPanel->Layout();
    listSizer->Fit(matrixListPanel);
	panelSizer->Add(matrixListPanel, 1, wxEXPAND | wxALL, 5);

    this->SetSizer(panelSizer);
    this->Layout();
    this->Centre( wxBOTH );

    matrixList = new MatrixManager();
    matrixList->setParent(this);
    matrixList->matrix_names = new wxArrayString();
    matrixList->matrices = new std::unordered_map<std::string, Matrix *>();
    
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
    int selected = matrixListBox->GetSelection();
    if (selected != wxNOT_FOUND) {
        wxString name = matrixListBox->GetString(matrixListBox->GetSelection());
        Matrix * mat = matrixList->get(name);
        MatrixShowFrame * frame = new MatrixShowFrame(this, name, mat, wxDefaultPosition, wxDefaultSize);
        frame->Show();
    }
}

void MatrixFrame::deleteMatrix(wxCommandEvent & event) {
    int selected = matrixListBox->GetSelection();
    if (selected != wxNOT_FOUND) {
        wxString name = matrixListBox->GetString(matrixListBox->GetSelection());
        matrixList->remove(name, selected);
    }
}

void MatrixFrame::createEditFrame(wxCommandEvent& event) {
    //std::cout << "Hello" << std::endl; 
    wxString input_name = name->GetValue();
    int input_rows = wxAtoi(rows->GetValue());
    int input_cols = wxAtoi(cols->GetValue());
    //editFrame = new MatrixEditFrame(this, input_name, wxDefaultPosition, wxSize(input_rows*45 + 5 * input_rows + 5, (input_cols + 1) * 45 + 5 * input_cols + 5 + 10), input_name, input_rows, input_cols);
    editFrame = new MatrixEditFrame(this, input_name, wxDefaultPosition, wxDefaultSize, input_name, input_rows, input_cols);
    
    editFrame->Show(true);
}

MatrixEditFrame::MatrixEditFrame(MatrixFrame * parent, const wxString& title, const wxPoint& pos, const wxSize& size, wxString name, int rows, int cols) 
        : wxFrame(parent, wxID_ANY, title, pos, size) {
    m_parent = parent; 
    mat_name = name; 
    mat_rows = rows;
    mat_cols = cols; 
    m_parent->initButton->Enable(false);
    int h_size = 35;
    wxBoxSizer * matrixBox = new wxBoxSizer(wxVERTICAL);
    wxGridSizer * matrix_grid = new wxGridSizer(mat_rows, mat_cols, 3, 3);
    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            wxTextCtrl * text = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(h_size, h_size), 0L,  wxTextValidator(wxFILTER_NUMERIC));
            matrix_grid->Add(text, 1, wxALL | wxEXPAND);
            inputBoxes.push_back(text);
        }
    }
    confirmButton = new wxButton(this, wxID_ANY, "Create", wxDefaultPosition, wxSize(-1, 45));
    //confirmButton->SetMaxSize(wxSize(-1, 35));
    confirmButton->Bind(wxEVT_BUTTON, &MatrixEditFrame::createMatrix, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &MatrixEditFrame::OnClose, this);
    
    matrixBox->Add(matrix_grid, mat_rows, wxALL | wxEXPAND);
    matrixBox->Add(confirmButton, 0, wxALL | wxEXPAND);
    this->SetSizer(matrixBox);
    this->Layout();
    matrixBox->Fit(this);
}

void MatrixEditFrame::createMatrix(wxCommandEvent& event) {
    Matrix * mat = new Matrix(mat_rows, mat_cols);
    double value = 0; 
    for (int i = 0; i < mat_rows; i++) {
        for (int j = 0; j < mat_cols; j++) {
            (inputBoxes[i * mat_cols + j]->GetValue()).ToDouble(&value);
            mat->addEntry(i, j, value);
            inputBoxes[i * mat_cols + j]->Destroy();
            //std::cout << "Added entry to matrix: " << value << std::endl; 
        }
    }

    m_parent->matrixList->add(mat_name, mat);
    this->Close();
}

void MatrixEditFrame::OnClose(wxCloseEvent &event) {
    m_parent->initButton->Enable(true);
    this->Destroy();
}

void MatrixManager::add(wxString& str, Matrix * matrix) {
    matrices->insert({str.ToStdString(), matrix});
    matrix_names->Add(str);
    m_parent->matrixListBox->Append(str);
    m_parent->inverse1->Append(str);
    m_parent->row_echelon1->Append(str);
    m_parent->linear_independence1->Append(str);
    m_parent->determinant1->Append(str);
}

void MatrixManager::setParent(MatrixFrame * parent) {
    m_parent = parent; 

}

Matrix * MatrixManager::get(wxString & str) {
    return matrices->at(str.ToStdString());
}

void MatrixManager::remove(wxString & str, int num_pos) {
    matrix_names->Remove(str);
    Matrix * mat = (*matrices)[str.ToStdString()];
    matrices->erase(str.ToStdString());
    m_parent->matrixListBox->Delete(num_pos);
    m_parent->inverse1->Delete(num_pos);
    m_parent->row_echelon1->Delete(num_pos);
    m_parent->linear_independence1->Delete(num_pos);
    m_parent->determinant1->Delete(num_pos);
    delete mat; 
}

MatrixShowFrame::MatrixShowFrame(MatrixFrame * parent, const wxString& title, Matrix * mat, const wxPoint& pos, const wxSize& size) 
    : wxFrame(parent, wxID_ANY, title, pos, size) {

    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetMinSize(wxSize(-1, -1));
    wxBoxSizer * frame_box = new wxBoxSizer(wxVERTICAL);
    wxPanel * main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxBORDER_SUNKEN);
    wxFlexGridSizer * matrix_grid = new wxFlexGridSizer(mat->m, mat->n, 18,18);
    matrix_grid->SetFlexibleDirection(wxBOTH);
    matrix_grid->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    for (int i = 0; i < mat->m; i++) {
        for (int j = 0; j < mat->n; j++) {
            wxString entry;
            entry << "[" << decimalToFraction(mat->getEntry(i,j)) << "]";
            wxPanel * numPanel = new wxPanel(main_panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 45));
            wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);
            //box->SetMinSize(wxSize(-1, 45));
            box->Add(new wxStaticText(numPanel, wxID_ANY, entry), 0, wxALL | wxALIGN_CENTER, 5); 
            numPanel->SetSizer(box);
            numPanel->Layout();
            matrix_grid->Add(numPanel, 0, wxEXPAND, 15);
        }
    }
    main_panel->SetSizer(matrix_grid);
    main_panel->Layout();
    matrix_grid->Fit(main_panel);
    frame_box->Add(main_panel, 0, wxEXPAND | wxALL, 5);
    this->SetSizer(frame_box);
    this->Layout();
    frame_box->Fit(this);
    this->Centre(wxBOTH);

}

MatrixSaveFrame::MatrixSaveFrame(MatrixFrame * parent, const wxString& title, Matrix * mat, const wxPoint& pos, const wxSize& size) 
    : wxFrame(parent, wxID_ANY, title, pos, size) {

    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->SetMinSize(wxSize(-1, -1));
    m_parent = parent; 
    matrix = mat; 
    saved = false; 
    wxBoxSizer * frame_box = new wxBoxSizer(wxVERTICAL);
    wxPanel * main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxBORDER_SUNKEN);
    wxFlexGridSizer * matrix_grid = new wxFlexGridSizer(mat->m, mat->n, 18,18);
    matrix_grid->SetFlexibleDirection(wxBOTH);
    matrix_grid->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    for (int i = 0; i < mat->m; i++) {
        for (int j = 0; j < mat->n; j++) {
            wxString entry;
            entry << "[" << mat->getEntry(i,j) << "]";
            wxPanel * numPanel = new wxPanel(main_panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 45));
            wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);
            //box->SetMinSize(wxSize(-1, 45));
            box->Add(new wxStaticText(numPanel, wxID_ANY, entry), 0, wxALL | wxALIGN_CENTER, 5); 
            numPanel->SetSizer(box);
            numPanel->Layout();
            matrix_grid->Add(numPanel, 0, wxEXPAND, 15);
        }
    }
    main_panel->SetSizer(matrix_grid);
    main_panel->Layout();
    matrix_grid->Fit(main_panel);
    frame_box->Add(main_panel, 0, wxEXPAND | wxALL, 5);
    
    wxBoxSizer * save_box = new wxBoxSizer(wxHORIZONTAL);
    input_name = new wxTextCtrl(this, wxID_ANY, title);
    wxButton * saveButton = new wxButton(this, wxID_ANY, "Save Matrix"); 
    saveButton->Bind(wxEVT_BUTTON, &MatrixSaveFrame::saveMatrix, this);
    save_box->Add(input_name);
    save_box->Add(saveButton);

    frame_box->Add(save_box, 0, wxALL);
    this->SetSizer(frame_box);
    this->Layout();
    frame_box->Fit(this);
    this->Centre(wxBOTH);

    this->Bind(wxEVT_CLOSE_WINDOW, &MatrixSaveFrame::OnClose, this);
}

void MatrixSaveFrame::OnClose(wxCloseEvent &event) {
    if (!saved) {
        delete matrix;
    } 
    this->Destroy();
}

void MatrixSaveFrame::saveMatrix(wxCommandEvent & event) {
    wxString name = input_name->GetValue();
    (m_parent->matrixList)->add(name, matrix);
    saved = true; 
    this->Close();
}

void MatrixFrame::inverseMatrix(wxCommandEvent & event) {
    int selected = inverse1->GetSelection();
    //printf("%d\n", selected);
    if (selected != wxNOT_FOUND) {
        wxString name = inverse1->GetString(selected);
        Matrix * mat = matrixList->get(name);
        Matrix * mat_inverse = new Matrix(mat->inverse());
        MatrixSaveFrame * saveFrame = new MatrixSaveFrame(this, name+"_inverse", mat_inverse, wxDefaultPosition, wxDefaultSize);
        saveFrame->Show();
    }
} 

/*void MatrixFrame::echelonMatrix(wxCommandEvent & event) {
    int selected = echelon1->GetSelection();
    //printf("%d\n", selected);
    if (selected != wxNOT_FOUND) {
        wxString name = echelon1->GetString(selected);
        Matrix * mat = matrixList->get(name);
        Matrix * mat_echelon = new Matrix(mat->echelon());
        MatrixSaveFrame * saveFrame = new MatrixSaveFrame(this, name+"_echelon", mat_echelon, wxDefaultPosition, wxDefaultSize);
        saveFrame->Show();
    }
}*/

void MatrixFrame::rowEchelonMatrix(wxCommandEvent & event) {
    int selected = row_echelon1->GetSelection();
    //printf("%d\n", selected);
    if (selected != wxNOT_FOUND) {
        wxString name = row_echelon1->GetString(selected);
        Matrix * mat = matrixList->get(name);
        Matrix * mat_row_echelon = new Matrix(mat->reducedEchelon());
        MatrixSaveFrame * saveFrame = new MatrixSaveFrame(this, name+"_row_echelon", mat_row_echelon, wxDefaultPosition, wxDefaultSize);
        saveFrame->Show();
    }
}

void MatrixFrame::linearIndepedentMatrix(wxCommandEvent & event) {
    int selected = linear_independence1->GetSelection();
    if (selected != wxNOT_FOUND) {
        wxString name = linear_independence1->GetString(selected);
        Matrix * mat = matrixList->get(name);
        wxString mat_inverse = (mat->linearIndependence()) ? "True" : "False";
        //MatrixSaveFrame * saveFrame = new MatrixSaveFrame(this, name+"_inverse", mat_inverse, wxDefaultPosition, wxDefaultSize);
        wxDialog * dialog = new wxDialog(this, wxID_ANY, "linear independence", wxDefaultPosition, wxSize(200,200));
        wxBoxSizer * dialogSizer = new wxBoxSizer(wxVERTICAL);
        dialogSizer->Add(new wxStaticText(dialog, wxID_ANY, "Matrix: " + name), 0, wxALIGN_CENTER_HORIZONTAL);
        dialogSizer->Add(new wxStaticText(dialog, wxID_ANY, mat_inverse), 0, wxALIGN_CENTER_HORIZONTAL);
        dialog->SetSizer(dialogSizer);
        dialog->ShowModal();
    }
} 

void MatrixFrame::determinantMatrix(wxCommandEvent & event) {
    int selected = determinant1->GetSelection();
    if (selected != wxNOT_FOUND) {
        wxString name = determinant1->GetString(selected);
        Matrix * mat = matrixList->get(name);
        wxString mat_determinant = decimalToFraction(mat->determinant());
        //MatrixSaveFrame * saveFrame = new MatrixSaveFrame(this, name+"_inverse", mat_inverse, wxDefaultPosition, wxDefaultSize);
        wxDialog * dialog = new wxDialog(this, wxID_ANY, "Determinant", wxDefaultPosition, wxSize(200,200));
        wxBoxSizer * dialogSizer = new wxBoxSizer(wxVERTICAL);
        dialogSizer->Add(new wxStaticText(dialog, wxID_ANY, "Matrix: " + name), 0, wxALIGN_CENTER_HORIZONTAL);
        dialogSizer->Add(new wxStaticText(dialog, wxID_ANY, "Determinant" + mat_determinant), 0, wxALIGN_CENTER_HORIZONTAL);
        dialog->SetSizer(dialogSizer);
        dialog->ShowModal();
    }
} 