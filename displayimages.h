#ifndef DISPLAYIMAGES_H
#define DISPLAYIMAGES_H

#include <QWidget>
#include "filecontroller.h"
#include <QLabel>
#include <QGraphicsPixmapItem>
#include <QSignalMapper>

namespace Ui {
class DisplayImages;
}

class DisplayImages : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief DisplayImages Default constructor. Shouldn't be used.
     * @param parent The widget that owns this widget. This is typically the MainWindow in some way, but can be others.
     */
    explicit DisplayImages(QWidget *parent = 0);
    /**
     * @brief DisplayImages Actual Constructor, this initialises all of the items needed to display a meaningful interface.
     * @param parent The widget that owns this widget. This is typically the MainWindow in some way, but can be others.
     * @param controller This is the file controller that this object uses to load images from a file path.
     * @param path This is the directory path to the directory that the images are to be read from.
     */
    DisplayImages(QWidget *parent,FileController* controller, QString path);
    /**
     * Deconstructor. Needed to delete owned pointers such as ui and mapper.
     */
    ~DisplayImages();
    /**
     * @brief get_box_x This is used to return the starting x value that the user indicated through interaction.
     * @return The value that indicates the start of the bounding box on the image, in regards to the x-axis. This is in pixels.
     *          If there is no bounding box entered by the user, then output is -1.
     */
    int get_box_x();
    /**
     * @brief get_box_y This is used to return the starting y value that the user indicated through interaction.
     * @return The value that indicates the start of the bounding box on the image, in regards to the y-axis. This is in pixels.
     *          If there is no bounding box entered by the user, then output is -1.
     */
    int get_box_y();
    /**
     * @brief get_box_length This is used to return the length of the bounding box that the user entered through interaction.
     * @return The value that indicates length of the bounding box, along the x axis. This is in pixels.
     *          If there is no bounding box entered by the user, then output is -1.
     */
    int get_box_length();
    /**
     * @brief get_box_depth This is used to return the depth/height of the bounding box that the user entered through interaction.
     * @return The value that indicates depth/height of the bounding box, along the y axis. This is in pixels.
     *          If there is no bounding box entered by the user, then output is -1.
     */
    int get_box_depth();

private slots:
    /**
     * @brief image_region_clicked This is used to set a region of the image that was clicked as part of a bounding box or not.
     * @param indicator This is an integer value that maps to the image that was clicked by the user.
     */
    void image_region_clicked(int indicator);


private:
    /**
     * @brief ui This is the UI object which displays as per the DisplayImages form.
     */
    Ui::DisplayImages *ui;
    /**
     * @brief display_name This is the name of the files that is currently being viewed.
     */
    QString display_name;
    // UNNEEDED? unsigned int size;
    /**
     * @brief controller_ref Pointer to the file controller that is being used by the application.
     */
    FileController* controller_ref;
    /**
     * @brief mapper This is an object that is used to map signals to a slot using integer keys. This saves having 256 slots and connect statements.
     */
    QSignalMapper* mapper;
    /**
     * @brief flags A 2d array that maps to the grid the image is displayed in. Used mostly for cell highlighting and determining the bounding box.
     */
    bool flags[16][16];
    /**
     * @brief region_height The height of each image region. This is image_height/16.
     */
    int region_height;
    /**
     * @brief region_width The width of each image region. This is image_width/16.
     */
    int region_width;
};

#endif // DISPLAYIMAGES_H
