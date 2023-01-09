#include "Thumbnail.h"

/**
 * @brief Thumbnail
 *      Constructor of the widget
 * @param parent
 *      parent widget for this one
 */
Thumbnail::Thumbnail(QWidget *parent, string path)
    : QToolButton(parent)
{
    this->image_path = path;
}

/**
 * Destructor of the widget
 */
Thumbnail::~Thumbnail()
{
    //
}
