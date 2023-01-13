#include <vector>

#include <QToolButton>
#include <QPixmap>
#include <QString>
#include <QPoint>
#include <QSize>
#include <QMenu>

#ifndef Thumbnail_H
#define Thumbnail_H

#define MAX_SIDE_SIZE 480
#define SIZE_FOR_COLOR_CODE_DETERMINE 128
#define EMPTY_COLOR_CODE_VALUE ""
// -----
#define COLOR_STEP 2
#define MIN_SATURATION 5
#define MIN_LIGHTNESS 5
// -----
#define COLOR_CODE_LEADING_SYMB "0"
#define COLOR_CODE_PART_LEN 3
// -----
#define INC_VALUE(m, v) \
    { \
        int _ = v & COLOR_STEP; \
        v -= _; \
        if (_ > COLOR_STEP / 2) \
            v += COLOR_STEP; \
        \
        if (v >= 0) { \
            if (m->contains(v)) \
                m->at(v) += 1; \
            else \
                m->emplace(v, 1); \
        }\
    }

#define BETWEEN(min, val, max) \
    ((min <= val) && (val <= max))

#define MAP_KEY_W_MAX_VAL(m) \
    std::max_element(m->begin(), m->end(), \
        [](const auto & p1, const auto & p2) { \
            return p1.second < p2.second; \
        } \
    )->first

#define LEADING_SYMB(str) \
    if (COLOR_CODE_PART_LEN - str.length() > 0) \
        str = QString(COLOR_CODE_LEADING_SYMB).repeated(COLOR_CODE_PART_LEN - str.length()) + str \

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
    explicit Thumbnail(QWidget *parent = nullptr, QString path = nullptr);

    /**
     * Destructor of the widget
     */
    ~Thumbnail();

    /**
     * @brief srcSize
     *      size of source image
     */
    QSize *srcSize = nullptr;

    /**
     * @brief colorString
     *      string representing the characteristics
     *      of the colors in the image as a whole
     */
    QString colorCode = EMPTY_COLOR_CODE_VALUE;

    /**
     * @brief load
     *      loads image into pixmap
     */
    void load();

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
    QString getImagePath();

    /**
     * @brief getSrcSizeAsString
     *      returns source file resolution
     * @return
     */
    QString getResolution();

    /**
     * @brief determineColorCode
     *      Proceed the image and determine the color code
     */
    void determineColorCode();

    /**
     * @brief getColorCode
     *      returns a string representing the characteristics
     *      of the colors in the image as a whole
     * @param load
     * @return
     */
    QString getColorCode(bool load = true);

    /**
     * @brief deleteSourceFile
     *      deletes source file from disk
     */
    void deleteSourceFile();

private:
    /**
     * @brief image_path
     *      path to the image
     */
    QString imagePath;

    /**
     * @brief customContextMenu
     *      Thumbnail's custom context menu
     */
    QMenu *customContextMenu = nullptr;

    /**
     * @brief customContextMenuActions
     *      Thumbnail's custom context menu actions
     */
    vector<QAction*> customContextMenuActions;

    /**
     * @brief setupSlots
     *      connects the slots to window's widgets and signals
     */
    void setupSlots();

    /**
     * @brief createCustomContextMenu
     *      define and configure the context menu
     */
    void createCustomContextMenu();

    /**
     * @brief deleteCustomContextMenu
     *      delete the context menu
     */
    void deleteCustomContextMenu();

signals:
    void deleted();

private slots:
    /**
     * @brief customContextMenu_Requested
     *      handle signal when context menu requested
     * @param point
     */
    void customContextMenu_Requested(QPoint point);

    /**
     * @brief deleteAction_Triggered
     *      handle when delete action is triggered
     */
    void deleteAction_Triggered();
};

#endif // Thumbnail_H
