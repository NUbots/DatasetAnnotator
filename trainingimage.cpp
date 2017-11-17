#include "trainingimage.h"

TrainingImage::TrainingImage()
{
    filename = "";  // Default is empty.
    feature_existing = false;   // Default is false.
    box = nullptr;  // Default is null.
    truncated = 0;  // Default is not truncated.
    occluded = 3;   // Default is unknown.
}

TrainingImage::TrainingImage(QString filename)
{
    feature_existing = false;   // Default is false;
    this->filename = filename;  // Set the file name for the file that is being represented.
    box = nullptr;  // Default is null.
    truncated = 0;  // Default is not truncated.
    occluded = 3;   // Default is unknown.
}

TrainingImage::~TrainingImage()
{
    if(box != nullptr)  // If box has been set, needs to be deleted.
    {
        delete box;
    }
}
void TrainingImage::set_feature_exists(bool existence)
{
    feature_existing = existence;   // Set the existence to whatever has been passed as the parameter.
    return;
}

void TrainingImage::set_filename(QString filename)
{
    this->filename = filename;  // Set the filename as the string passed in by parameter.
    return;
}

QString TrainingImage::get_filename() const
{
    return filename;
}

bool TrainingImage::feature_exists()
{
    return feature_existing;
}

bool TrainingImage::set_bounding_box(int x, int y, int length, int depth)
{
    bool ret = false;
    if(box != nullptr)  // If the box exists, it needs to be deleted to create a new one.
    {
        delete box;
        ret = true;
    }
    box = new BoundingBox(x,y,length,depth);    // Create the bounding box for the image.
    return ret;
}

BoundingBox* TrainingImage::get_bounding_box()
{
    return box;
}

bool TrainingImage::set_occluded(int score)
{
    if(score >= 0 && score < 4) // Score must be valid.
    {
        occluded = score;   // Set from parameter.
        return true;
    }
    else
    {
        return false;
    }
}

bool TrainingImage::set_truncated(float score)
{
    if(score >= 0 && score < 1) // Score must be valid.
    {
        truncated = score;  // Set from parameter.
        return true;
    }
    else
    {
        return false;
    }
}

float TrainingImage::get_truncated()
{
    return truncated;
}

int TrainingImage::get_occluded()
{
    return occluded;
}
