#include "Thumbnail.h"

/**
 * @brief load
 *      loads image into pixmap
 */
void Thumbnail::load()
{
    this->pixmap = new QPixmap(QString::fromStdString(this->imagePath));
    this->srcSize = new QSize(this->pixmap->size());

    if (this->srcSize->width() > this->srcSize->height())
    {
        if (this->srcSize->width() > MAX_SIDE_SIZE)
        {
            *this->pixmap = QPixmap(this->pixmap->scaledToWidth(MAX_SIDE_SIZE));
        }
    }
    else {
        if (this->srcSize->height() > MAX_SIDE_SIZE)
        {
            *this->pixmap = QPixmap(this->pixmap->scaledToHeight(MAX_SIDE_SIZE));
        }
    }

    this->setIcon(QIcon(*this->pixmap));
}

/**
 * @brief resizeToWidth
 *      resize tool button keep image ratio according to width
 */
void Thumbnail::resizeToWidth(int width)
{
    int height = this->srcSize->height() * width / this->srcSize->width();

    QSize newSize(
        width,
        height < this->srcSize->height() ? height : this->srcSize->height()
    );

    this->setFixedSize(newSize);
    this->setIconSize(newSize);
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
    delete this->pixmap;
}
