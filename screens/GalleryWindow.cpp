#include <QString>

#include "ui_GalleryWindow.h"
#include "screens/GalleryWindow.h"

void GalleryWindow::resetThumbnails()
{
    this->Thumbnails = new map<std::string, std::vector<Thumbnnail*>>();

    this->Thumbnails->emplace(std::piecewise_construct,
                       std::make_tuple("default"),
                       std::make_tuple());
    this->Thumbnails->emplace(std::piecewise_construct,
                       std::make_tuple("sortedByDominantColour"),
                       std::make_tuple());
}

void GalleryWindow::askForGalleryPath()
{
    this->askFolderWindow = new AskFolderWindow(this);
    this->connect(this->askFolderWindow, SIGNAL(accepted()), this, SLOT(askForGallery_Accepted()));

    this->askFolderWindow->show();
}

void GalleryWindow::askForGallery_Accepted()
{
    this->gallery_path = this->askFolderWindow->getPath();

    if (this->gallery_path == "") return;

    this->setWindowTitle(this->gallery_path);
}

// Constructor of the Window
GalleryWindow::GalleryWindow(QWidget *parent)
    // Inherit Frameless window and UI that was compiled from .ui file constructors
    : QMainWindow(parent), ui(new Ui::GalleryWindow)
{
    // Setup UI from compiled .ui file
    this->ui->setupUi(this);

    this->resetThumbnails();
    this->askForGalleryPath();
}

// Destructor of the Window
GalleryWindow::~GalleryWindow()
{
    // Delete UI to free up memory
    delete this->ui;
    delete this->Thumbnails;
    if (this->askFolderWindow != NULL) {
        delete this->askFolderWindow;
    }
}
