#ifndef GALLERYWINDOW_H
#define GALLERYWINDOW_H

#define DEFAULT_COLUMN_AMOUNT 3
#define DEFAULT_THUMBNAIL_LIST "default"

#include <QMainWindow>
#include <QProgressBar>

#include "widgets/Thumbnail.h"
#include "screens/AskFolderWindow.h"

using namespace std;

// Setup Ui namespace for Ui part of this window generated due to building
QT_BEGIN_NAMESPACE
namespace Ui { class GalleryWindow; }
QT_END_NAMESPACE

typedef vector<Thumbnail*> ThumbnailList;
typedef map<string, ThumbnailList*> ThumbnailsContainer;

class GalleryWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief GalleryWindow
     *      Constructor of the window
     * @param parent
     *      parent widget for this one
     */
    explicit GalleryWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor of the Window
     */
    ~GalleryWindow();

private:
    /**
     * @brief ui
     *      Ui class part with all the widgets
     */
    Ui::GalleryWindow *ui;

    /**
     * @brief progessBar
     *      Progress bar in the status bar
     */
    QProgressBar *progessBar = nullptr;

    /**
     * @brief askFolderWindow
     *      pointer to the window which asks user to gallery path
     */
    AskFolderWindow *askFolderWindow = nullptr;

    /**
     * @brief galleryPath
     *      path to the gallery specified by the user
     */
    string galleryPath;

    /**
     * @brief Thumbnails
     *      available thumbnail lists
     */
    ThumbnailsContainer *Thumbnails = nullptr;

    /**
     * @brief currentList
     *      list of thumbnails that now displayed
     */
    string currentList = DEFAULT_THUMBNAIL_LIST;

    /**
     * @brief columnAmount
     *      amount of column in grid layout
     */
    int columnAmount = DEFAULT_COLUMN_AMOUNT;

    /**
     * @brief previewPixmap
     *      pixmap that currently on preview label
     */
    QPixmap *previewPixmap = nullptr;

    /**
     * @brief previewThumbnail
     *  Thumbnail that currently using for preview
     */
    Thumbnail *previewThumbnail = nullptr;

    /**
     * @brief setupSlots
     *      connects the slots to window's widgets and signals
     */
    void setupSlots();

    /**
     * @brief newThumbnailList
     *      created new list in Thumbnails
     * @param name
     *      name for new list
     */
    void newThumbnailList(string name);

    /**
     * @brief addThumbnail
     *      add Thumbnail ibject to the list by nameMAGIC NUMBER: It fix theme scrollbar that too wide due to hide the thumbnail widgets a little bit
     * @param list_name
     * @param thumbnail
     */
    void addThumbnail(string list_name, Thumbnail *thumbnail);

    /**
     * @brief clearThumbnails
     *      resets *Thumbnails to it's initial state
     */
    void clearThumbnails();

    /**
     * @brief askForGalleryPath
     *      asks user to enter path for which gallery will be opened
     */
    void askForGalleryPath();

    /**
     * @brief tryOpenGallery
     *      trying to open gallery for provided path and returns true on successful
     * @param galleryPath
     *      path to the folder for which gallery will be opened
     * @return true or false according to successfuly opened provided path
     */
    bool tryOpenGallery(string galleryPath);

    /**
     * @brief createProgressBar
     *      creates new progress bar in statusbar of the window
     * @param max
     *      maximum value that progress bar can be set
     */
    QProgressBar* createProgressBar(int max);

    /**
     * @brief deleteProgressBar
     *      deletes progress bar
     */
    void deleteProgressBar();

    /**
     * @brief getThumbnailColumnWidth
     *      returns width to resize thumbnail for
     * @return column width according to current window size
     */
    int getThumbnailColumnWidth();

    /**
     * @brief loadGallery
     *      loads images from the directory
     */
    void loadGallery();

    /**
     * @brief renderThumbnails
     *      renders thumbnail widgets from current list
     */
    void renderThumbnails();

    /**
     * @brief clearThumbnailPreview
     *      clears the tumbnail preview label pixmap and content at all
     */
    void clearThumbnailPreview();

    /**
     * @brief renderPreviewForThumbnail
     *      renders a preview image for provided Thumbnail object
     * @param thumbnail
     */
    void renderPreviewForThumbnail(Thumbnail *thumbnail = nullptr);

    /**
     * @brief resizeLayout
     *      resizes layout according to current window size
     */
    void resizeLayout();

    /**
     * @brief resetGallery
     *      closes the opened gallery and clear all the content from it
     */
    void resetGallery();

protected:
    /**
     * @brief resizeEvent
     *      reimplements default empty resizeEvent
     */
    void resizeEvent(QResizeEvent* event) override;

private slots:
    /**
     * @brief askForGallery_Accepted
     *      handle signal when askFolderWindow is accepted
     */
    void askForGallery_Accepted();

    /**
     * @brief actionOpen_Triggered
     *      handle signal when menu action Open is triggered
     */
    void actionOpen_Triggered();

    /**
     * @brief actionClose_Triggered
     *      handle signal when menu action Close is triggered
     */
    void actionClose_Triggered();

    /**
     * @brief thumbnail_Clicked
     *      handle signal when any thumbnail was clicked
     */
    void thumbnail_Clicked();

    /**
     * @brief splitterBody_Moved
     *      handle splitter move event
     * @param pos
     * @param index
     */
    void splitterBody_Moved(int pos, int index);
};

#endif // GALLERYWINDOW_H
