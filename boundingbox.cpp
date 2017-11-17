#include "boundingbox.h"

BoundingBox::BoundingBox()
{
    // Empty because it shouldn't be used. Bounding Boxes are Immutable objects.
}

BoundingBox::BoundingBox(int x, int y, int length, int depth)
{
    // Set the member variables from the parameters provided.
    x_start = x;
    y_start = y;
    x_region = length;
    y_region = depth;
}

BoundingBox::~BoundingBox()
{
    // No pointers belong to this class.
}

int BoundingBox::get_x_start()
{
    return x_start; // Return value of member variable.
}

int BoundingBox::get_y_start()
{
    return y_start; // Return value of member variable.
}

int BoundingBox::get_y_region()
{
    return y_region; // Return value of member variable.
}
int BoundingBox::get_x_region()
{
    return x_region; // Return value of member variable.
}
