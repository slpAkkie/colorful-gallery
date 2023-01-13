#include <QFile>

#include "Thumbnail.h"

/**
 * @brief setupSlots
 *      connects the slots to window's widgets and signals
 */
void Thumbnail::setupSlots()
{
    QObject::connect(
        this, SIGNAL(customContextMenuRequested(QPoint)),
        this, SLOT(customContextMenu_Requested(QPoint))
    );
}

/**
 * @brief createCustomContextMenu
 *      define and configure the context menu
 */
void Thumbnail::createCustomContextMenu()
{
    if (this->customContextMenu != nullptr) return;

    this->customContextMenu = new QMenu(this);
    this->customContextMenuActions.clear();

    QAction *deleteAction = new QAction(tr("menu-action.delete"), this);
    QObject::connect(
        deleteAction, SIGNAL(triggered()),
        this, SLOT(deleteAction_Triggered())
    );
    this->customContextMenuActions.push_back(deleteAction);

    for (QAction *action : this->customContextMenuActions)
    {
        this->customContextMenu->addAction(action);
    }
}

/**
 * @brief deleteCustomContextMenu
 *      delete the context menu
 */
void Thumbnail::deleteCustomContextMenu()
{
    for (QAction *action : this->customContextMenuActions)
    {
        delete action;
    }

    delete this->customContextMenu;
    this->customContextMenu = nullptr;
}

/**
 * @brief load
 *      loads image into pixmap
 */
void Thumbnail::load()
{
    QPixmap pixmap(this->imagePath);
    this->srcSize = new QSize(pixmap.size());

    bool needResize = this->srcSize->width() > MAX_SIDE_SIZE || this->srcSize->height() > MAX_SIDE_SIZE;

    if (this->srcSize->width() > this->srcSize->height())
    {
        pixmap = needResize ? QPixmap(pixmap.scaledToWidth(MAX_SIDE_SIZE)) : pixmap;
    }
    else
    {
        pixmap = needResize ? QPixmap(pixmap.scaledToHeight(MAX_SIDE_SIZE)) : pixmap;
    }

    this->setIcon(QIcon(pixmap));
}

/**
 * @brief resizeToWidth
 *      resize tool button keep image ratio according to width
 */
void Thumbnail::resizeToWidth(int width)
{
    int height = this->srcSize->height() * width / this->srcSize->width();
    int srcHeight = this->srcSize->height();

    if (height > srcHeight) height = srcHeight;

    QSize newSize(
        width,
        height < MAX_SIDE_SIZE
            ? height
            : MAX_SIDE_SIZE
    );

    this->setFixedSize(newSize);
    this->setIconSize(newSize);
}

/**
 * @brief getImagePath
 *      returns source image path
 * @return
 */
QString Thumbnail::getImagePath()
{
    return this->imagePath;
}

/**
 * @brief getSrcSizeAsString
 *      returns source file resolution
 * @return
 */
QString Thumbnail::getResolution()
{
    return QString::fromStdString(to_string(this->srcSize->width()) + "x" + to_string(this->srcSize->height()));
}

/**
 * @brief deleteSourceFile
 *      deletes source file from disk
 */
void Thumbnail::deleteSourceFile()
{
    QFile::moveToTrash(this->imagePath);

    emit this->deleted();
}

/**
 * @brief customContextMenu_Requested
 *      handle signal when context menu requested
 * @param point
 */
void Thumbnail::customContextMenu_Requested(QPoint point)
{
    this->createCustomContextMenu();
    this->customContextMenu->exec(this->mapToGlobal(point));
    this->deleteCustomContextMenu();
}

/**
 * @brief deleteAction_Triggered
 *      handle when delete action is triggered
 */
void Thumbnail::deleteAction_Triggered()
{
    this->deleteSourceFile();
}

/**
 * @brief Thumbnail
 *      Constructor of the widget
 * @param parent
 *      parent widget for this one
 */
Thumbnail::Thumbnail(QWidget *parent, QString path) : QToolButton(parent)
{
    this->imagePath = path;

    this->setupSlots();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

/**
 * Destructor of the widget
 */
Thumbnail::~Thumbnail()
{
    delete this->srcSize;
}
