#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H


class BoundingBox
{
public:
    /**
     * @brief BoundingBox Default Constructor. Should never realistically be used.
     */
    BoundingBox();
    /**
     * @brief BoundingBox Constructor that creates a bounding box properly, initialising all member variables.
     * @param x The starting x co-ordinate of the bounding box. This is in the top left corner.
     * @param y The starting y co-ordinate of the bounding box. This is in the top left corner.
     * @param length The lenth of the bounding box. This is the distance between the two bounding x values.
     * @param depth The depth/height of the bounding box. This is the distance between the two bounding y values.
     */
    BoundingBox(int x, int y, int length, int depth);
    /**
     * @brief BoundingBox Deconstructor. Effectively empty.
     */
    ~BoundingBox();

    /* Member functions. */

    /**
     * @brief get_x_start Getter for the x_start member variable.
     * @return The value stored in x_start.
     */
    int get_x_start();
    /**
     * @brief get_y_start Getter for the y_start member variable.
     * @return The value stored in y_start.
     */
    int get_y_start();
    /**
     * @brief get_x_region Getter for the x_region member variable.
     * @return The value stored in x_region.
     */
    int get_x_region();
    /**
     * @brief get_y_region Getter for the y_region member variable.
     * @return The value stored in y_region.
     */
    int get_y_region();

private:
    /**
     * @brief x_start The value for x in the top left point of the bounding box.
     */
    int x_start;
    /**
     * @brief y_start The value for y in the top left point of the bounding box.
     */
    int y_start;
    /**
     * @brief x_region The value for the distance between the two x values of the bounding box.
     */
    int x_region;
    /**
     * @brief y_region The value for the distance between the two y values of the bounding box.
     */
    int y_region;
};

#endif // BOUNDINGBOX_H
