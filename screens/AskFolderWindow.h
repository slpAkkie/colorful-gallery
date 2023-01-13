#ifndef ASKFOLDERWINDOW_H
#define ASKFOLDERWINDOW_H

#include <QDialog>
#include <QString>
#include <QLineEdit>

using namespace std;

// Setup Ui namespace for Ui part of this window generated due to building
QT_BEGIN_NAMESPACE
namespace Ui { class AskFolderWindow; }
QT_END_NAMESPACE

class AskFolderWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief AskFolderWindow
     *      Constructor of the window
     * @param parent
     *      parent widget for this one
     */
    explicit AskFolderWindow(QWidget *parent = nullptr, QString initialPath = "");

    /**
     * @brief Destructor of the Window
     */
    ~AskFolderWindow();

    /**
     * @brief getPath
     *      get path that was input be the user
     * @return path from the input widget
     */
    QString getPath();

private:
    /**
     * @brief ui
     *      Ui class part with all the widgets
     */
    Ui::AskFolderWindow *ui;

    /**
     * @brief setupSlots
     *      connects the slots to window's widgets and signals
     */
    void setupSlots();

    /**
     * @brief openFileDialog
     *      opens a file dialog and paste it's result to the input
     */
    void openFileDialog();

private slots:
    /**
     * @brief buttonBrowse_clicked
     *      handle signal when select folder button is clicked
     */
    void buttonBrowse_clicked();
};

#endif // ASKFOLDERWINDOW_H
