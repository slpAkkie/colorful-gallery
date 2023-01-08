#include "ui_AskFolderWindow.h"
#include "screens/AskFolderWindow.h"

QString AskFolderWindow::getPath()
{
    return ui->InputPath->text();
}

// Constructor of the Window
AskFolderWindow::AskFolderWindow(QWidget *parent)
    // Inherit Frameless window and UI that was compiled from .ui file constructors
    : QDialog(parent), ui(new Ui::AskFolderWindow)
{
    // Setup UI from compiled .ui file
    this->ui->setupUi(this);
}

// Destructor of the Window
AskFolderWindow::~AskFolderWindow()
{
    // Delete UI to free up memory
    delete this->ui;
}
