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
