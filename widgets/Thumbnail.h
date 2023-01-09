#ifndef Thumbnail_H
#define Thumbnail_H

#define MAX_SIDE_SIZE 480

#include <QToolButton>
#include <QPixmap>
#include <QSize>

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

    /**
     * @brief load
     *      loads image into pixmap
     */
    void load();

    /**
     * @brief srcSize
     *      size of source image
     */
    QSize *srcSize = nullptr;

    /**
     * @brief resizeToWidth
     *      resize tool button keep image ratio according to width
     */
    void resizeToWidth(int width);

    /**
     * @brief getImagePath
     *      returns source image path
     * @return
     */
    string getImagePath();

private:
    /**
     * @brief image_path
     *      path to the image
     */
    string imagePath;
};

#endif // Thumbnail_H
