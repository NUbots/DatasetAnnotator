#include "filecontroller.h"
#include <QPixmap>
#include <QDebug>

FileController::FileController()
{
    // Default constructor which doesn't do anything, shouldn't be used.
}

FileController::~FileController()
{
    while(!image_data.isEmpty())    // While there are still entries in the list of training images.
    {
        TrainingImage* data = &(image_data.first());    // Get the reference to the first training image.
        image_data.removeFirst();   // Delete that training image from the list.
        delete data;    // Delete it, to avoid memory leaks.
    }
}
FileController::FileController(QString dir_string)
{
    /*
     * NOTE: This section uses standard C file access. This could be changed, but this works well.
     */
    DIR* image_directory;   // Directory pointer object.
    struct dirent *entry;   // Get a structure for a directory entry.
    image_directory = opendir(dir_string.toUtf8().constData()); // Open the directory that is fed in from user input.
    QRegExp image_files("[^/,\\,?,%,*,:,|,\",<,>,.]*.(png|jpg|bmp|jpeg|pbm|pgm|ppm|xbm|xpm)");  // A regex that ensure that the filenale is a supported image for QPixmap.
    if(image_directory != NULL) // If the directory opening was a success.
    {
        while((entry = readdir(image_directory)))   // While there are still more objects in the directory.
        {
            if(entry->d_type == DT_REG && image_files.exactMatch((QString)entry->d_name))   // If the filename matches the regex for image files.
            {
                TrainingImage* image_file = new TrainingImage(entry->d_name);   // Create a new Training image object linked to the current file.
                image_data.push_back(*image_file);  // Add that training image object to the list.
            }
        }
    }
    else
    {
        perror("Directory does not exist or cannot be opened.");    // Print an error to a console.
    }
    delete entry;   // Deleting this doesn't break program, but deleting the DIR pointer did. To explore?
    current_index = 0;  // Set the index of the controller to the first image in it's list.
    directory = dir_string; // Set the directory path to use later.
}

QString FileController::filename_at(int index)
{
    if(index < image_data.size() && index > -1) // If the index given is a valid index for the list.
    {
        return image_data.at(index).get_filename(); // Return the filename of the image at the index.
    }
    else
    {
        return nullptr; // Fail return value.
    }
}

bool FileController::feature_existence(bool exists,int index)
{
    if(index < image_data.size() && index > -1) // If the index given is a valid index for the list.
    {
        image_data[index].set_feature_exists(exists); // Set the value.
        return true;    // Success.
    }
    else
    {
        return false;  // Return failure result.
    }
}

bool FileController::determine_feature_existence(int index)
{
    if(index < image_data.size() && index > -1) // If the index given is a valid index for the list.
    {
        return image_data[index].feature_exists(); // Return the whether the feature exists in the image at the index.
    }
    else
    {
        return false;  // Return failure result.
    }
}

int FileController::get_current_index()
{
    return current_index; // Return the current index.
}

bool FileController::increment_index(int incremental)
{
    if(current_index+incremental < image_data.size() && current_index+incremental > -1) // If the increment given gives a valid index for the list.
    {
        current_index += incremental;   // Increment the current index by the value of increment.
        return true;    // Success.
    }
    return false;  // Return failure result.
}

bool FileController::end_directory()
{
    return (current_index >= image_data.size()-1);  // Return the whether the current image objject is the last in the list..
}

int FileController::directory_size()
{
    return image_data.size();   // Return the length of the image list.
}

bool FileController::set_feature_boundingbox(int index, int x, int y, int length, int depth)
{
    if(index < image_data.size() && index > -1) // If the index given is a valid index for the list.
    {
        image_data[index].set_bounding_box(x,y,length,depth);   // Set the bounding box of the image at the index.
        return true;    // Success.
    }
    return false;  // Return failure result.
}

BoundingBox* FileController::get_feature_boundingbox(int index)
{
    if(index < image_data.size() && index > -1) // If the index given is a valid index for the list.
    {
        return image_data[index].get_bounding_box();    // Retrieve and return the bounding box for the image at the index sepcified.
    }
    return nullptr;  // Return failure result.

}

bool FileController::set_parameters(int index, float truncated, int occluded)
{
    if(index < image_data.size() && index > -1) // If the index given is a valid index for the list.
    {
        return image_data[index].set_truncated(truncated) && image_data[index].set_occluded(occluded);  // Set the values. If there is any problems, it will return false.
    }
    return false;  // Return failure result.
}

int FileController::get_image_occlusion(int index)
{
    if(index < image_data.size() && index > -1) // If the index given is a valid index for the list.
    {
        return image_data[index].get_occluded();    // Retrieve and return the occluded score for the image at the index specified.
    }
    else return 3;  // Fail value indicates Unknown.
}

float FileController::get_image_truncation(int index)
{
    if(index < image_data.size() && index > -1) // If the index given is a valid index for the list.
    {
        return image_data[index].get_truncated();   // Retrieve and return the truncated score for the image at the index specified.
    }
    else return 0;  // Return failure result.
}

bool FileController::write_output(int index, QString feature)
{
    if(index >= image_data.size() || index < 0) // If the index given is not a valid index for the list.
    {
        return false;   // Failure, return before doing anything.
    }
    QString output_file = QString(directory);   // Copy the directory string into a new one.
    output_file.chop(6);    // if the directory is of the form ..../images, then chops off the images part.
    if(QDir(output_file).exists() && output_file.compare("/") != 0)  // If the format is in the expected format.
    {
        output_file.append("labels/");  // Set up for the directory for labels, assuming correct setup.
        if(!QDir(output_file).exists()) // If the format is as expected, continue.
        {
            QDir new_dir;   // Needed to make a new directory, in case labels doesn't exist..
            new_dir.mkdir(output_file); // Make the new directory.
        }
    }
    else    // The format is not as expected.
    {
        output_file = QString(directory);   // Will simply save data files into the same directory.
        output_file.append("/label_data/");
        if(!QDir(output_file).exists()) // If the format is as expected, continue.
        {
            QDir new_dir;   // Needed to make a new directory, in case labels doesn't exist..
            new_dir.mkdir(output_file); // Make the new directory.
        }
    }
    output_file.append(image_data[index].get_filename());   // Set up the output file to have the same filename as the image.
    QString out_file = QString(output_file);    // Copy strings again.
    int pos = out_file.lastIndexOf('.',-1); // Find the last occurrence of '.' character in the string.
    int file_ext_length = out_file.size()-pos;  // Retrieves the lenth of the current file extionsion, since it could be variable.
    out_file.chop(file_ext_length); // Cut off the file extension.
    out_file.append(".txt");    // Give the current file a text file extension.
    QFile data_file(out_file);  // Create a file object for the current filepath.
    if(data_file.open(QIODevice::WriteOnly | QIODevice::Text))  // Tries to open the file with Write permissions. Will create it if it does not exist.
    {
        QTextStream file_out(&data_file);   // Create an output stream that pipes into the text file that just opened.
        BoundingBox* current_box = image_data[index].get_bounding_box();    // Get a reference to the bounding box for the indexed image.
        if(current_box != nullptr)  // If the bounding box is not null, then there is meaningful data to writ.
        {
            QString x_string, y_string, length_string, depth_string, occlusion_string, truncated_string;    // Strings used to convert numbers to strings.
            x_string.setNum((float)current_box->get_x_start(),'f',2);   // Convert the starting x coord to a float, then to a string for writing.
            y_string.setNum((float)current_box->get_y_start(),'f',2);   // Convert the starting y coord to a float, then to a string for writing.
            length_string.setNum((float)current_box->get_x_start() + (float)current_box->get_x_region(),'f',2); // Get the end x coord, and change it to a string of a float for writing.
            depth_string.setNum((float)current_box->get_y_start() + (float)current_box->get_y_region(),'f',2);  // Get the end y coord, and change it to a string of a float for writing.
            occlusion_string.setNum(image_data[index].get_occluded());  // Convert the occluded score to a string for writing.
            truncated_string.setNum(image_data[index].get_truncated(),'f',2);   // Convert the truncated score to a string for writing.
            // The following 'line' outputs the available information into the KITTI data format.
            file_out << "'" << feature << "' " << truncated_string <<" " << occlusion_string << " 0.00 " << x_string << " "<< y_string << " "
                     << length_string << " " << depth_string << " 0.00 0.00 0.00 0.00 0.00 0.00 0.00 1.00";
        }
        else    // No features exist in the image.
        {
            QString path(directory);    // Get the path to the images.
            path.append("/").append(image_data[index].get_filename());  // Retrieve the full path to the relevant image.
            QPixmap image(path);    // Create the image from the path again.
            QString max_width, max_height;  // Strings for printing the image width and height.
            max_width.setNum((float)image.width(),'f',2);   // Get the value of the image width, then conver it to a string (in float format) so that it can be printed.
            max_height.setNum((float)image.height(),'f',2); // Get the value of the image height, then convert it to a string (in float format) so that it can be printed.
            qDebug() << "W: " << max_width << " H: " << max_height << "\n";
            // The following line outputs the available information into the KITTI data format.
            // This is the case where the user doesn't care about the image. Therefore the entire image is set as a DontCare rectangle.
            file_out << "'DontCare' 0.00 0 0.00 0.00 0.00 " << max_width << " " << max_height << " 0.00 0.00 0.00 0.00 0.00 0.00 1.00";
        }
        return true;    // Success.
    }
    else return false;  // Return failure result.
}
