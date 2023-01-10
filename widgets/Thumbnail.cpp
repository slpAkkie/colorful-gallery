#include "Thumbnail.h"

/**
 * @brief load
 *      loads image into pixmap
 */
void Thumbnail::load()
{
    QPixmap pixmap(QString::fromStdString(this->imagePath));
    this->srcSize = new QSize(pixmap.size());

    if (this->srcSize->width() > this->srcSize->height())
    {
        if (this->srcSize->width() > MAX_SIDE_SIZE)
        {
            pixmap = QPixmap(pixmap.scaledToWidth(MAX_SIDE_SIZE));
        }
    }
    else {
        if (this->srcSize->height() > MAX_SIDE_SIZE)
        {
            pixmap = QPixmap(pixmap.scaledToHeight(MAX_SIDE_SIZE));
        }
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
string Thumbnail::getImagePath()
{
    return this->imagePath;
}

/**
 * @brief Thumbnail
 *      Constructor of the widget
 * @param parent
 *      parent widget for this one
 */
Thumbnail::Thumbnail(QWidget *parent, string path)
    : QToolButton(parent)
{
    this->imagePath = path;
}

/**
 * Destructor of the widget
 */
Thumbnail::~Thumbnail()
{
    delete this->srcSize;
}
