#include <filesystem>

#include <QFileInfo>
#include <QMessageBox>

#include "ui_GalleryWindow.h"
#include "screens/GalleryWindow.h"

/**
 * @brief resizeEvent
 *      reimplements default empty resizeEvent
 */
void GalleryWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   this->resizeLayout();
}

/**
 * @brief setupSlots
 *      connects the slots to window's widgets and signals
 */
void GalleryWindow::setupSlots()
{
    QObject::connect(
        this->ui->ActionOpen, SIGNAL(triggered()),
        this, SLOT(actionOpen_Triggered())
    );
    QObject::connect(
        this->ui->ActionReload, SIGNAL(triggered()),
        this, SLOT(actionReload_Triggered())
    );
    QObject::connect(
        this->ui->ActionClose, SIGNAL(triggered()),
        this, SLOT(actionClose_Triggered())
    );
    QObject::connect(
        this->ui->ActionMoreColumns, SIGNAL(triggered()),
        this, SLOT(actionMoreColumns_Triggered())
    );
    QObject::connect(
        this->ui->ActionLessColumns, SIGNAL(triggered()),
        this, SLOT(actionLessColumns_Triggered())
    );

    QObject::connect(
        this->ui->SplitterBody, SIGNAL(splitterMoved(int,int)),
        this, SLOT(splitterBody_Moved(int,int))
    );
}

/**
 * @brief readHistoryFile
 *      returns first line from history file
 * @return
 */
QString GalleryWindow::readHistoryFile()
{
    QFile historyFile(PATH_SAVE_FILENAME);
    QString lastGalleryPath = "";

    if (historyFile.open(QIODevice::ReadOnly))
    {
        lastGalleryPath = historyFile.readLine();
        historyFile.close();
    }

    return lastGalleryPath;
}

/**
 * @brief writeHistoryFile
 *      writes string into the history file
 * @param str
 */
void GalleryWindow::writeHistoryFile(QString str)
{
    QFile historyFile(PATH_SAVE_FILENAME);

    if (historyFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        historyFile.write(str.toUtf8());
        historyFile.close();
    }
}

/**
 * @brief askForGalleryPath
 *      asks user to enter path for which gallery will be opened
 */
void GalleryWindow::askForGalleryPath()
{
    if (this->askFolderWindow == nullptr)
    {
        this->askFolderWindow = new AskFolderWindow(this, this->readHistoryFile());

        QObject::connect(
            this->askFolderWindow, SIGNAL(accepted()),
            this, SLOT(askForGallery_Accepted())
        );
    }

    this->askFolderWindow->show();
}

/**
 * @brief tryOpenGallery
 *      trying to open gallery for provided path and returns true on successful
 * @param galleryPath
 *      path to the folder for which gallery will be opened
 * @return true or false according to successfuly opened provided path
 */
bool GalleryWindow::tryOpenGallery(QString galleryPath)
{
    if (!filesystem::exists(galleryPath.toStdString())) return false;

    this->galleryPath = galleryPath;
    this->setWindowTitle(this->galleryPath);

    return true;
}

/**
 * @brief resetGallery
 *      closes the opened gallery and clear all the content from it
 */
void GalleryWindow::resetGallery()
{
    this->clearThumbnailPreview();
    this->clearThumbnails();

    this->setWindowTitle(tr("window.title"));
}

/**
 * @brief clearThumbnailPreview
 *      clears the tumbnail preview label pixmap and content at all
 */
void GalleryWindow::clearThumbnailPreview()
{
    if (this->previewPixmap != nullptr)
    {
        delete this->previewPixmap;
        this->previewPixmap = nullptr;

        this->ui->ThumbnailPreview->clear();
    }

    if (this->previewThumbnail != nullptr)
    {
        this->previewThumbnail = nullptr;
    }

    this->ui->StatusBar->clearMessage();
}

/**
 * @brief clearThumbnails
 *      resets *Thumbnails to it's initial state
 */
void GalleryWindow::clearThumbnails()
{
    if (this->Thumbnails != nullptr && this->Thumbnails->contains(DEFAULT_THUMBNAIL_LIST)) {
        for (Thumbnail *thumbnail : *this->Thumbnails->at(DEFAULT_THUMBNAIL_LIST))
        {
            this->ui->ThumbnailListContainerLayout->removeWidget(thumbnail);
            delete thumbnail;
        }

        this->Thumbnails->clear();
    }

    this->newThumbnailList(DEFAULT_THUMBNAIL_LIST);
    this->newThumbnailList("dominantColour");
}

/**
 * @brief addThumbnail
 *      add Thumbnail ibject to the list by name
 * @param list_name
 * @param thumbnail
 */
void GalleryWindow::addThumbnail(QString list_name, Thumbnail *thumbnail)
{
    if (!this->Thumbnails->contains(list_name))
    {
        this->newThumbnailList(list_name);
    }

    this->Thumbnails->at(list_name)->push_back(thumbnail);
}

/**
 * @brief createProgressBar
 *      creates new progress bar in statusbar of the window
 * @param max
 *      maximum value that progress bar can be set
 */
QProgressBar* GalleryWindow::createProgressBar(int max)
{
    if (this->progessBar == nullptr)
    {
        this->progessBar = new QProgressBar(this);
        this->progessBar->setMaximum(max);
        this->progessBar->setMinimum(0);
        this->progessBar->setValue(0);
    }

    this->ui->StatusBar->addWidget(this->progessBar);

    return this->progessBar;
}

/**
 * @brief deleteProgressBar
 *      deletes progress bar
 */
void GalleryWindow::deleteProgressBar()
{
    this->ui->StatusBar->removeWidget(this->progessBar);

    delete this->progessBar;
    this->progessBar = nullptr;
}

/**
 * @brief loadGallery
 *      loads images from the directory
 */
void GalleryWindow::loadGallery()
{
    this->clearThumbnails();

    // Create all the Thumbnails with the paths
    for (filesystem::directory_entry const &entry : filesystem::directory_iterator(this->galleryPath.toStdString()))
    {
        if (filesystem::is_directory(entry)) continue;

        QString entryPath = QString::fromStdString(entry.path());
        Thumbnail *thumbnail = new Thumbnail(this, entryPath);
        QObject::connect(thumbnail, SIGNAL(clicked()), this, SLOT(thumbnail_Clicked()));
        QObject::connect(thumbnail, SIGNAL(deleted()), this, SLOT(thumbnail_Deleted()));

        this->addThumbnail(DEFAULT_THUMBNAIL_LIST, thumbnail);
    }

    // Then load images to it
    int thumbnailCount = this->Thumbnails->at(DEFAULT_THUMBNAIL_LIST)->size();

    QProgressBar *progressBar = this->createProgressBar(thumbnailCount);

    for (Thumbnail *thumbnail : *this->Thumbnails->at(DEFAULT_THUMBNAIL_LIST))
    {
        thumbnail->load();
        progressBar->setValue(progressBar->value() + 1);
    }

    this->deleteProgressBar();
}

/**
 * @brief renderThumbnails
 *      renders thumbnail widgets from current list
 */
void GalleryWindow::renderThumbnails(int startFrom)
{
    int thumbnailCount = this->Thumbnails->at(this->currentList)->size();
    ThumbnailList *thumbnailList = this->Thumbnails->at(this->currentList);

    for (int i = startFrom; i < thumbnailCount; i++)
    {
        Thumbnail *thumbnail = thumbnailList->at(i);
        thumbnail->resizeToWidth(this->getThumbnailColumnWidth());

        int row = i / this->columnAmount;
        int column = i % this->columnAmount;

        this->ui->ThumbnailListContainerLayout->addWidget(thumbnail, row, column, Qt::AlignmentFlag::AlignVCenter);
    }
}

/**
 * @brief newThumbnailList
 *      created new list in Thumbnails
 * @param name
 *      name for new list
 */
void GalleryWindow::newThumbnailList(QString name)
{
    if (this->Thumbnails == nullptr)
    {
        this->Thumbnails = new ThumbnailsContainer();
    }

    this->Thumbnails->emplace(name, new ThumbnailList());
}

/**
 * @brief openGallery
 *      opens gallery for provided path string
 * @param path
 */
void GalleryWindow::openGallery(QString inputPath)
{
    if (!this->tryOpenGallery(inputPath))
    {
        QMessageBox::critical(
            this,
            this->tr("message-wrong.gallery-path.title"),
            this->tr("message-wrong.gallery-path.text")
        );

        return;
    }

    this->writeHistoryFile(inputPath);
    this->resetGallery();
    this->loadGallery();
    this->renderThumbnails();
}

/**
 * @brief getThumbnailColumnWidth
 *      returns width to resize thumbnail for
 * @return column width according to current window size
 */
int GalleryWindow::getThumbnailColumnWidth()
{
    /** MAGIC NUMBER: It fix theme scrollbar that too wide due to hide the thumbnail widgets a little bit */
    return (this->ui->ThumbnailsList->width() - 64 ) / this->columnAmount;
}

/**
 * @brief renderPreviewForThumbnail
 *      renders a preview image for provided Thumbnail object
 * @param thumbnail
 */
void GalleryWindow::renderPreviewForThumbnail(Thumbnail *thumbnail)
{
    if (thumbnail == nullptr && this->previewThumbnail == nullptr) return;

    if (thumbnail != nullptr)
    {
        if (thumbnail != this->previewThumbnail)
        {
            this->clearThumbnailPreview();
            this->previewThumbnail = thumbnail;
            this->previewPixmap = new QPixmap(this->previewThumbnail->getImagePath());

            QString statusBarMessage = QFileInfo(this->previewThumbnail->getImagePath()).fileName() + "    [" + this->previewThumbnail->getResolution() + "]";

            this->ui->StatusBar->showMessage(statusBarMessage);
        }
    }

    this->ui->ThumbnailPreview->setPixmap(
        this->previewPixmap->scaled(this->ui->ThumbnailPreview->size(), Qt::KeepAspectRatio)
    );
}

/**
 * @brief resizeLayout
 *      resizes layout according to current window size
 */
void GalleryWindow::resizeLayout()
{
    if (this->Thumbnails == nullptr) return;

    this->renderPreviewForThumbnail();

    for (Thumbnail *thumbnail : *this->Thumbnails->at(DEFAULT_THUMBNAIL_LIST))
    {
        thumbnail->resizeToWidth(this->getThumbnailColumnWidth());
    }
}

/**
 * @brief removeThumbnail
 *      removes thumbnail from all lists and screen
 * @param thumbnail
 */
void GalleryWindow::removeThumbnail(Thumbnail *thumbnail)
{
    this->ui->ThumbnailListContainerLayout->removeWidget(thumbnail);

    ThumbnailList *currentList = this->Thumbnails->at(this->currentList);

    int deletedThumbnailIndex = std::find(
        currentList->begin(), currentList->end(), thumbnail
    ) - currentList->begin();

    for (const auto & entry: *this->Thumbnails)
    {
        ThumbnailList *list = entry.second;
        list->erase(std::remove(list->begin(), list->end(), thumbnail), list->end());
    }

    thumbnail->deleteLater();
    this->renderThumbnails(deletedThumbnailIndex);
}

/**
 * @brief askForGallery_Accepted
 *      holds signal when askFolderWindow is accepted
 */
void GalleryWindow::askForGallery_Accepted()
{
    this->openGallery(this->askFolderWindow->getPath());
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
 * @brief actionReload_Triggered
 *      handle signal then menu action Reffresh is triggered
 */
void GalleryWindow::actionReload_Triggered()
{
    this->openGallery(this->galleryPath);
}

/**
 * @brief actionClose_Triggered
 *      handle signal when menu action Close is triggered
 */
void GalleryWindow::actionClose_Triggered()
{
    this->resetGallery();
    this->writeHistoryFile("");
}

/**
 * @brief actionMoreColumns_Triggered
 *      handle signal when menu action more columns is triggered
 */
void GalleryWindow::actionMoreColumns_Triggered()
{
    this->columnAmount++;

    if (this->columnAmount > MAX_COLUMN_AMOUNT) this->columnAmount = MAX_COLUMN_AMOUNT;

    this->renderThumbnails();
}

/**
 * @brief actionMoreColumns_Triggered
 *      handle signal when menu action more columns is triggered
 */
void GalleryWindow::actionLessColumns_Triggered()
{
    this->columnAmount--;

    if (this->columnAmount < MIN_COLUMN_AMOUNT) this->columnAmount = MIN_COLUMN_AMOUNT;

    this->renderThumbnails();
}

/**
 * @brief thumbnail_Clicked
 *      handle signal when any thumbnail was clicked
 */
void GalleryWindow::thumbnail_Clicked()
{
    Thumbnail *thumbnail = dynamic_cast<Thumbnail*>(this->sender());
    if (thumbnail == nullptr) return;

    this->renderPreviewForThumbnail(thumbnail);
}

/**
 * @brief resized
 *      handle signal that something resized.
 *      Spliter or Window or something else change the
 *      layout size
 */
void GalleryWindow::splitterBody_Moved([[maybe_unused]] int pos, [[maybe_unused]] int index)
{
    this->resizeLayout();
}

/**
 * @brief thumbnail_Deleted
 *      handle signal when user deleted a thumbnail
 */
void GalleryWindow::thumbnail_Deleted()
{
    if (dynamic_cast<Thumbnail*>(this->sender()) == nullptr) return;

    Thumbnail *deletedThumbnail = dynamic_cast<Thumbnail*>(this->sender());

    if (this->previewThumbnail == deletedThumbnail)
    {
        this->clearThumbnailPreview();
    }

    this->removeThumbnail(deletedThumbnail);
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

    this->askForGalleryPath();
}

/**
 * @brief Destructor of the Window
 */
GalleryWindow::~GalleryWindow()
{
    delete this->ui;
    delete this->progessBar;
    delete this->askFolderWindow;
    delete this->Thumbnails;
    delete this->previewPixmap;
    delete this->previewThumbnail;
}
