#ifndef Thumbnail_H
#define Thumbnail_H

#include <QToolButton>

using namespace std;

class Thumbnail : public QToolButton
{
    Q_OBJECT

public:
    /**
     * @brief Thumbnail
     *      Constructor of the widget
     * @param parent
     *      parent widget for this one
     */
    explicit Thumbnail(QWidget *parent = nullptr, string path = nullptr);

    /**
     * Destructor of the widget
     */
    ~Thumbnail();

private:
    /**
     * @brief image_path
     *      path to the image
     */
    string image_path;
};

#endif // Thumbnail_H
