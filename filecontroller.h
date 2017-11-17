#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QList>
#include "trainingimage.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <QDir>
#include <QFile>
#include <QTextStream>

class FileController
{
public:
    /**
     * @brief FileController Default Constructor. Shouldn't be used.
     */
    FileController();
    /**
     * @brief FileController Actual constructor. Loads a list of image files from a directory, and stores the list.
     * @param dir_string The string that indicates the directory to load images from.
     */
    FileController(QString dir_string);
    /**
     * Deconstructor. Needed for consistency, no personal pointers to delete.
     */
    ~FileController();
    /**
     * @brief filename_at Gets the name of the file that is stored at a point in the list.
     * @param index The location that will be used to retrieve the file name from the list.
     * @return A string that contains the file name. This does not include the directory path.
     */
    QString filename_at(int index);
    /**
     * @brief feature_existence Sets the boolean value to indicate if the feature was present in the image.
     * @param exists The value that the flag is to be set to, true if the feature exists, false otherwise.
     * @param index The index of the TrainingImage object that the feature flag is to be set for.
     * @return True if the operation was a success (index was within bounds), false if failure.
     */
    bool feature_existence(bool exists,int index);
    /**
     * @brief determine_feature_existence Function used to evaluate whether the feature exists for a given object.
     * @param index The index of the TrainingImage object in the list that the retrieval is occurring for.
     * @return The boolean value that is stored in the TrainingImage object that is being accessed.
     */
    bool determine_feature_existence(int index);
    /**
     * @brief get_current_index Used to return the current index of the file being processed.
     * @return The integer value dependent on the current index. Stored in the member variable current index.
     */
    int get_current_index();\
    /**
     * @brief increment_index Increments the current index by some value. Note that to backtrack, a negative number is used.
     * @param incremental the value that the current index is to be incremented by.
     * @return An indicator of whether the operation occurred. For instance, if the increment would the ist out of it's bounds,
     *          the operation would fail.
     */
    bool increment_index(int incremental);
    /**
     * @brief end_directory Determines if the controller is currently at the end of the directory used.
     * @return The boolean value, true if and only if the current file is the last in the directory.
     */
    bool end_directory();
    /**
     * @brief directory_size Returns the number of relevant files in the current directory.
     * @return An integer that is the count of relevant files in the directory being used.
     */
    int directory_size();
    /**
     * @brief set_feature_boundingbox This sets the Bounding Box for a given training image.
     * @param index The index of the Training Image that the bounding box is being set for.
     * @param x The starting x position, maps to the bounding box.
     * @param y The starting y position, maps to the bounding box.
     * @param length The length of the x-axis, maps to the bounding box.
     * @param depth The depth of the y-axis, maps to the bounding box.
     * @return True if the operation was successful, false otherwise.
     */
    bool set_feature_boundingbox(int index,int x,int y,int length, int depth);
    /**
     * @brief get_feature_boundingbox Returns a reference to the Bounding box object that is linked to a Training Image.
     * @param index The index of the Training Image that is having its bounding box retrieved.
     * @return The BoundingBox object of the Training Image. null is the BoundingBox object doesn't exist.
     */
    BoundingBox* get_feature_boundingbox(int index);
    /**
     * @brief set_parameters Sets the additional parameters that are needed for the KITTI data format.
     * @param index The index of the Training Image that the parameters are being set for.
     * @param truncated The truncated score that was given from user input.
     * @param occluded The occluded score that was given from user input.
     * @return True if the parameters were set, that is that the index was valid.
     */
    bool set_parameters(int index,float truncated, int occluded);
    /**
     * @brief get_image_occlusion This retrieves the image occluded score that a Training image has, if it has it.
     * @param index The index of the Training Image that the occluded score is being retrieved from.
     * @return The occluded score for the Image. Will return -1 if the score doesn't exist or the index was invalid.
     */
    int get_image_occlusion(int index);
    /**
     * @brief get_image_truncation Retrieves the truncated score that a Training Image has, if it exists.
     * @param index The index of the Training Image that the truncated score is being retrieved from.
     * @return The truncated score for the Image. Will return -1 if the score does not exist or the index was iinvalid.
     */
    float get_image_truncation(int index);
    /**
     * @brief write_output Used to write the output data file for an Image.
     * @param index The index of the Training Image that is having output data generated for it.
     * @param feature The feature that is being identified but the main application. This is one of the only details that the file controller does not have direct access to.
     * @return True if the operation succeeds, false otherwise. Causes for fail: File access issues, invalid index.
     */
    bool write_output(int index, QString feature);

private:
    QList<TrainingImage> image_data;    // The list that is used to hold the IMage objects that the application operates one to retrieve and store data.
    int current_index;  // The file controller keeps track of the progress through a directory with this flag.
    QString directory;  // This is the directory path that the cntroller is providing read and write access to.
};

#endif // FILECONTROLLER_H
