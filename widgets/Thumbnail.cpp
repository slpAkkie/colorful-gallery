#include <QImage>
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
 * @brief determineColorCode
 *      Proceed the image and determine the color code
 */
void Thumbnail::determineColorCode()
{
    QImage *image = new QImage(QImage(this->imagePath).scaled(
        QSize(SIZE_FOR_COLOR_CODE_DETERMINE, SIZE_FOR_COLOR_CODE_DETERMINE),
        Qt::KeepAspectRatio
   ));

    int width = image->width(), height = image->height();
    map<int, int>
            *h = new map<int, int>(),
            *s = new map<int, int>(),
            *l = new map<int, int>();

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            QColor pixel = image->pixelColor(x, y);
            int hue, saturation, lightness;
            pixel.getHsl(&hue, &saturation, &lightness);

            if (saturation < MIN_SATURATION || lightness < MIN_LIGHTNESS || lightness / 2.2 > saturation)
                continue;

            INC_VALUE(h, hue);
            INC_VALUE(s, saturation);
            INC_VALUE(l, lightness);
        }
    }

    delete image;

    QString
        _h = QString::number(MAP_KEY_W_MAX_VAL(h)),
        _s = QString::number(MAP_KEY_W_MAX_VAL(s)),
        _l = QString::number(MAP_KEY_W_MAX_VAL(l));

    delete h;
    delete s;
    delete l;

    LEADING_SYMB(_h);
    LEADING_SYMB(_s);
    LEADING_SYMB(_l);

    this->colorCode = _h + _s + _l;
}

/**
 * @brief getColorStringForSort
 *      returns a string representing the characteristics
 *      of the colors in the image as a whole
 * @return
 */
QString Thumbnail::getColorCode(bool load)
{
    if (this->colorCode == EMPTY_COLOR_CODE_VALUE && load)
    {
        this->determineColorCode();
    }

    return this->colorCode;
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
