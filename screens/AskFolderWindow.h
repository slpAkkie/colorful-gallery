#ifndef ASKFOLDERWINDOW_H
#define ASKFOLDERWINDOW_H

#include <string>

#include <QDialog>
#include <QLineEdit>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class AskFolderWindow; }
QT_END_NAMESPACE

class AskFolderWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AskFolderWindow(QWidget *parent = nullptr);
    ~AskFolderWindow();

    string getPath();

private:
    Ui::AskFolderWindow *ui;
};

#endif // ASKFOLDERWINDOW_H
