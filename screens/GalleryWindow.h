#ifndef GALLERYWINDOW_H
#define GALLERYWINDOW_H

#include <string>
#include <vector>
#include <map>
#include <QMainWindow>

#include "widgets/Thumbnnail.h"
#include "screens/AskFolderWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GalleryWindow; }
QT_END_NAMESPACE

class GalleryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GalleryWindow(QWidget *parent = nullptr);
    ~GalleryWindow();

private:
    Ui::GalleryWindow *ui;
    AskFolderWindow *askFolderWindow;
    string galleryPath;
    map<string, vector<Thumbnnail*>> *Thumbnails;

    void setupSlots();
    void resetThumbnails();
    void askForGalleryPath();
    bool tryOpenGallery(string galleryPath);

private slots:
    void askForGallery_Accepted();
    void actionOpen_Triggered();
};

#endif // GALLERYWINDOW_H
