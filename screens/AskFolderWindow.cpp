#include "ui_AskFolderWindow.h"
#include "screens/AskFolderWindow.h"

string AskFolderWindow::getPath()
{
    return ui->InputPath->text().toStdString();
}

// Constructor of the Window
AskFolderWindow::AskFolderWindow(QWidget *parent)
    // Inherit Frameless window and UI that was compiled from .ui file constructors
    : QDialog(parent), ui(new Ui::AskFolderWindow)
{
    // Setup UI from compiled .ui file
    this->ui->setupUi(this);

    QPoint dialogCenter = mapToGlobal(rect().center());
    QPoint parentWindowCenter = parent->window()->mapToGlobal(
        parent->window()->rect().center());
    move(parentWindowCenter - dialogCenter);
}

// Destructor of the Window
AskFolderWindow::~AskFolderWindow()
{
    // Delete UI to free up memory
    delete this->ui;
}
