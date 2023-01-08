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
    QString gallery_path;
    map<std::string, std::vector<Thumbnnail*>> *Thumbnails;

    void resetThumbnails();
    void askForGalleryPath();

private slots:
    void askForGallery_Accepted();
};

#endif // GALLERYWINDOW_H
