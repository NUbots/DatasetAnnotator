#ifndef TRAININGIMAGE_H
#define TRAININGIMAGE_H
#include <QString>
#include <boundingbox.h>

class TrainingImage
{
public:
    /**
     * @brief TrainingImage Default Constructor. Use the one that sets the filename instead.
     */
    TrainingImage();
    /**
     * @brief TrainingImage Constructor that sets the filename for the Image.
     * @param filename String that is passed to link the object to a file in the directory that is being used.
     */
    TrainingImage(QString filename);
    /**
     * Deconstructor. Used to delte the Bounding Box pointer if it exists.
     */
    ~TrainingImage();
    /**
     * @brief get_filename Retrieves the file name of the Training Image.
     * @return The value stored in the filename member variable.
     */
    QString get_filename() const;
    /**
     * @brief feature_exists Used to determine whether the feature being classified exists within the Image.
     * @return The value that is stored in the feature_existing member variable.
     */
    bool feature_exists();
    /**
     * @brief set_filename Sets the filename of the Image, this will relate to the image being retrieved when the time comes.
     * @param filename The string that contains the filename that the Image is being changed to.
     */
    void set_filename(QString filename);
    /**
     * @brief set_feature_exists Sets whether the feature exists in this image or not.
     * @param existence The value of feature existence. True if exists.
     */
    void set_feature_exists(bool existence);
    /**
     * @brief set_bounding_box Used to set the values of the bounding box of the image, this occurs if the feature exists.
     * @param x The starting x position for the bounding box.
     * @param y The starting y position for the bounding box.
     * @param length The length of the bounding box.
     * @param depth The depth/height of the the bounding box.
     * @return True if the operation was successful. False otherwise.
     */
    bool set_bounding_box(int x, int y, int length, int depth);
    /**
     * @brief set_truncated Sets the truncated score of the image.
     * @param score The score that is to be used for the truncated score. [.00,1.00).
     * @return True if the operation was a success, false otherwise.
     */
    bool set_truncated(float score);
    /**
     * @brief set_occluded Sets the occluded score of the image.
     * @param score The score that is used for the occlusion of the image. [0,3]
     * @return True if the operation was a success, false otherwise.
     */
    bool set_occluded(int score);
    /**
     * @brief get_truncated Retrieves the truncated score for the image.
     * @return The value stored in the truncated member variable. -1 if not set.
     */
    float get_truncated();
    /**
     * @brief get_occluded Retrieves the occluded score for the image.
     * @return The value stored in the occluded member variable. -1 if not set.
     */
    int get_occluded();
    /**
     * @brief get_bounding_box Retrieves the Bounding Box for the image.
     * @return A pointer to the bounding box object for the image. Null pointer if it is not set.
     */
    BoundingBox* get_bounding_box();
private:
    bool feature_existing;  // Flag to indicate if the feature exists in this image.
    QString filename;   // Filename of the image that this object is linked to.
    BoundingBox* box;   // Bounding box object that stores information about the image bounding box if it exists.
    float truncated;    // Stores the truncated score for the image if it exists.
    int occluded;   // Store the occluded score for the image if it exists.
};

#endif // TRAININGIMAGE_H
