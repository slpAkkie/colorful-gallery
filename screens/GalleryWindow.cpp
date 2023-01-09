#include <filesystem>

#include <QString>
#include <QMessageBox>

#include "ui_GalleryWindow.h"
#include "screens/GalleryWindow.h"

void GalleryWindow::resetThumbnails()
{
    this->Thumbnails = new map<std::string, std::vector<Thumbnnail*>>();

    this->Thumbnails->emplace(piecewise_construct,
                       make_tuple("default"),
                       make_tuple());
    this->Thumbnails->emplace(piecewise_construct,
                       make_tuple("sortedByDominantColour"),
                       make_tuple());
}

void GalleryWindow::askForGalleryPath()
{
    this->askFolderWindow = new AskFolderWindow(this);
    this->connect(this->askFolderWindow, SIGNAL(accepted()), this, SLOT(askForGallery_Accepted()));

    this->askFolderWindow->show();
}

void GalleryWindow::askForGallery_Accepted()
{
    string inputPath = this->askFolderWindow->getPath();

    if (!this->tryOpenGallery(inputPath))
    {
        QMessageBox::critical(this, this->tr("message-wrong.gallery-path.title"), this->tr("message-wrong.gallery-path.text"));
    }

    delete this->askFolderWindow;
}

bool GalleryWindow::tryOpenGallery(string galleryPath)
{
    if (!filesystem::exists(galleryPath)) return false;

    this->galleryPath = galleryPath;
    this->setWindowTitle(QString().fromStdString(this->galleryPath));

    return true;
}

void GalleryWindow::setupSlots()
{
    this->connect(this->ui->ActionOpen, SIGNAL(triggered()), this, SLOT(actionOpen_Triggered()));
}

void GalleryWindow::actionOpen_Triggered()
{
    this->askForGalleryPath();
}

// Constructor of the Window
GalleryWindow::GalleryWindow(QWidget *parent)
    // Inherit Frameless window and UI that was compiled from .ui file constructors
    : QMainWindow(parent), ui(new Ui::GalleryWindow)
{
    // Setup UI from compiled .ui file
    this->ui->setupUi(this);

    this->setupSlots();

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
