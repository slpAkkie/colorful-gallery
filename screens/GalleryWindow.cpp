#include <filesystem>

#include <QDebug>
#include <QString>
#include <QMessageBox>

#include "ui_GalleryWindow.h"
#include "screens/GalleryWindow.h"

/**
 * @brief setupSlots
 *      connects the slots to window's widgets and signals
 */
void GalleryWindow::setupSlots()
{
    this->connect(this->ui->ActionOpen, SIGNAL(triggered()), this, SLOT(actionOpen_Triggered()));
}

/**
 * @brief newThumbnailList
 *      created new list in Thumbnails
 * @param name
 *      name for new list
 */
void GalleryWindow::newThumbnailList(string name)
{
    if (this->Thumbnails == nullptr)
    {
        this->Thumbnails = new ThumbnailsContainer();
    }

    this->Thumbnails->emplace(name, new ThumbnailList());
}

/**
 * @brief addThumbnail
 *      add Thumbnail ibject to the list by name
 * @param list_name
 * @param thumbnail
 */
void GalleryWindow::addThumbnail(string list_name, Thumbnail *thumbnail)
{
    if (!this->Thumbnails->contains(list_name))
    {
        this->newThumbnailList(list_name);
    }

    this->Thumbnails->at(list_name)->push_back(thumbnail);
}

/**
 * @brief clearThumbnails
 *      resets *Thumbnails to it's initial state
 */
void GalleryWindow::clearThumbnails()
{
    this->newThumbnailList("default");
    this->newThumbnailList("dominantColour");
}

/**
 * @brief askForGalleryPath
 *      asks user to enter path for which gallery will be opened
 */
void GalleryWindow::askForGalleryPath()
{
    if (this->askFolderWindow == nullptr)
    {
        this->askFolderWindow = new AskFolderWindow(this);
    }

    this->connect(this->askFolderWindow, SIGNAL(accepted()), this, SLOT(askForGallery_Accepted()));

    this->askFolderWindow->show();
}

/**
 * @brief tryOpenGallery
 *      trying to open gallery for provided path and returns true on successful
 * @param galleryPath
 *      path to the folder for which gallery will be opened
 * @return true or false according to successfuly opened provided path
 */
bool GalleryWindow::tryOpenGallery(string galleryPath)
{
    if (!filesystem::exists(galleryPath)) return false;

    this->galleryPath = galleryPath;
    this->setWindowTitle(QString().fromStdString(this->galleryPath));

    return true;
}

/**
 * @brief loadGallery
 *      loads images from the directory
 */
void GalleryWindow::loadGallery()
{
    for (filesystem::directory_entry const &entry : filesystem::directory_iterator(this->galleryPath))
    {
        if (filesystem::is_directory(entry)) continue;

        string entry_path = entry.path().c_str();
        this->addThumbnail("default", new Thumbnail (this, entry_path));
    }
}

/**
 * @brief askForGallery_Accepted
 *      holds signal when askFolderWindow is accepted
 */
void GalleryWindow::askForGallery_Accepted()
{
    string inputPath = this->askFolderWindow->getPath();

    if (!this->tryOpenGallery(inputPath))
    {
        QMessageBox::critical(this, this->tr("message-wrong.gallery-path.title"), this->tr("message-wrong.gallery-path.text"));

        return;
    }

    this->loadGallery();
}

/**
 * @brief actionOpen_Triggered
 *      holds signal when menu action Open is triggered
 */
void GalleryWindow::actionOpen_Triggered()
{
    this->askForGalleryPath();
}

/**
 * @brief GalleryWindow
 *      Constructor of the window
 * @param parent
 *      parent widget for this one
 */
GalleryWindow::GalleryWindow(QWidget *parent)
    // Inherit Frameless window and UI that was compiled from .ui file constructors
    : QMainWindow(parent), ui(new Ui::GalleryWindow)
{
    // Setup UI from compiled .ui file
    this->ui->setupUi(this);

    this->setupSlots();

    this->clearThumbnails();
    this->askForGalleryPath();
}

/**
 * @brief Destructor of the Window
 */
GalleryWindow::~GalleryWindow()
{
    // Delete UI to free up memory
    delete this->ui;
    delete this->Thumbnails;
    delete this->askFolderWindow;
}
