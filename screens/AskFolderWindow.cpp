#include <QFileDialog>
#include <QShortcut>

#include "ui_AskFolderWindow.h"
#include "screens/AskFolderWindow.h"

/**
 * @brief getPath
 *      get path that was input be the user
 * @return path from the input widget
 */
QString AskFolderWindow::getPath()
{
    return ui->InputPath->text();
}

/**
 * @brief setupSlots
 *      connects the slots to window's widgets and signals
 */
void AskFolderWindow::setupSlots()
{
    QObject::connect(this->ui->ButtonBrowse, SIGNAL(clicked()), this, SLOT(buttonBrowse_clicked()));
}

/**
 * @brief openFileDialog
 *      opens a file dialog and paste it's result to the input
 */
void AskFolderWindow::openFileDialog()
{
    QFileDialog fileDialog(this, this->tr("title.select-folder"), this->ui->InputPath->text());
    fileDialog.setFileMode(QFileDialog::FileMode::Directory);
    fileDialog.setOption(QFileDialog::ShowDirsOnly);

    if (fileDialog.exec())
    {
        this->ui->InputPath->setText(fileDialog.selectedFiles().at(0));
    }
}

/**
 * @brief buttonBrowse_clicked
 *      handle signal when select folder button is clicked
 */
void AskFolderWindow::buttonBrowse_clicked()
{
    this->openFileDialog();
}

/**
 * @brief AskFolderWindow
 *      Constructor of the window
 * @param parent
 *      parent widget for this one
 */
AskFolderWindow::AskFolderWindow(QWidget *parent, QString initialPath)
    // Inherit Frameless window and UI that was compiled from .ui file constructors
    : QDialog(parent), ui(new Ui::AskFolderWindow)
{
    // Setup UI from compiled .ui file
    this->ui->setupUi(this);
    this->ui->InputPath->setText(initialPath);

//    QPoint dialogCenter = mapToGlobal(rect().center());
//    QPoint parentWindowCenter = parent->window()->mapToGlobal(
//                parent->window()->rect().center()
//    );
//    move(parentWindowCenter - dialogCenter);

    this->setupSlots();

}

/**
 * @brief Destructor of the Window
 */
AskFolderWindow::~AskFolderWindow()
{
    // Delete UI to free up memory
    delete this->ui;
}
