#include "displayimages.h"
#include "ui_displayimages.h"
#include <QPixmap>
#include <iostream>
#include "clickablelabel.h"
#include <cmath>

DisplayImages::DisplayImages(QWidget *parent) : QWidget(parent), ui(new Ui::DisplayImages)
{
    ui->setupUi(this);  // Creates the user interface.
}

DisplayImages::DisplayImages(QWidget *parent, FileController* controller, QString path) : QWidget(parent), ui(new Ui::DisplayImages)
{
    ui->setupUi(this);  // Creates the user interface.
    ui->gridLayout->setHorizontalSpacing(1);    // Set the spacing. This fixes an issue where the grid would keep growing.
    ui->gridLayout->setVerticalSpacing(1);  // Fixes the same issue mentioned about.
    controller_ref = controller;    // Creates a reference to the file controller object for future operations to Traiining Images.
    mapper = new QSignalMapper(this);   // A mapper is used to handle many signal slot connections elegantly.
    ClickableLabel* grids[16][16];  // An array to easily step through the clickabel labels. Currently the grid is hardcoded at 16*16. TODO: A variable size grid.
    for(int i = 0; i < 16; i++) // Step through each row of the grid.
    {
        for(int j = 0; j < 16; j++) // Step through each column of the row.
        {
            grids[i][j] = (ClickableLabel*)ui->gridLayout->itemAtPosition(i,j)->widget();   // Get the reference and map it to the local array.
        }
    }
    QString filepath = path.append("/").append(controller->filename_at(controller->get_current_index()));   // Get the full file path for the image to be loaded.
    QPixmap original_img(filepath); // Initialise a QPixmap object, which handles the image processing from the file given.
    region_width = original_img.width()/16; // Since the grid is 16*16, the image needs to be divided into 16 parts to display in a grid. This is along the x axis.
    region_height = original_img.height()/16;   // See the above comment. This is for the y axis though.
    double width_ratio = 1024/(double)original_img.width(); // Currently all images are displayed along a 1024 x axis, so have to determine the ratio of that and the x axis.
    double height_ratio = 512/(double)original_img.height();    // Currently all images are displayed along a 512 y axis, so have to determin the ratio of that and y axis of the image.
    int section_width = std::ceil(region_width*width_ratio);    // This is the width of each label.
    int section_height = std::ceil(region_height*height_ratio); // This is the height of each label.
    for(int i = 0; i < 16; i++) // For each row in the grid.
    {
        for(int j = 0; j < 16; j++) // For each column in the row.
        {
            grids[i][j]->setMaximumWidth(section_width);    // Set the maximum width of the label to that generated from the ratios.
            grids[i][j]->setMaximumHeight(section_height);  // Set the maximum heigh of the label to that generated from the ratios.
            QPixmap region_img = original_img.copy((j*region_width),(i*region_height),region_width,region_height);
            // The above line sets the pixmap that the label displays as the corresponding section of the image when divided into a corresponding grid.
            grids[i][j]->setPixmap(region_img.scaled(section_width,section_height));    // Scall the image section to fit the size of the label that is in use.
            connect(grids[i][j], SIGNAL(clicked()), mapper, SLOT(map()));   // Maps the current label to the mapper, so that the signal can be used.
            mapper->setMapping(grids[i][j],((i*16)+j)); // Makes a mapping to an integer that is computed based on the row and column that the current grid exists in.
            flags[i][j] = false;    // Sets the grid as not being selected.
        }
    }
    connect(mapper,SIGNAL(mapped(int)),this,SLOT(image_region_clicked(int)));   // This connect statement links all of the mappings in the mapper to the function that is the slot.
}

DisplayImages::~DisplayImages()
{
    // Delete the pointers that are new.
    delete ui;
    delete mapper;
}

void DisplayImages::image_region_clicked(int indicator)
{
    int row = indicator/16; // Get the row by using integer arithmetic, cutting off any remainder, so you get what row of 16.
    int column = indicator%16;  // Get the column by using modulo arithmetic, which cuts off the row part.
    ClickableLabel* label = (ClickableLabel*)ui->gridLayout->itemAtPosition(row,column)->widget();  // Retrieve the label that is in the location row, column.
    if(flags[row][column])  // If the region is currently set as including the feature.
    {
        label->setStyleSheet("border: 0px solid;"); // Remove the selected indicator.
        flags[row][column] = false; // Set the value so that it is no longer included.
    }
    else    // The region has not been selected.
    {
        label->setStyleSheet("border: 1px solid #7fff00;"); // Enter the selected indicator.
        flags[row][column] = true;  // Set the value so that it does include the feature.
    }
}

int DisplayImages::get_box_x()
{
    for(int i = 0; i < 16; i++) // For each column.
    {
        for(int j = 0; j < 16; j++) // Traverse each row in the column.
        {
            if(flags[j][i]) // If the flag for the current section is set, this is the first occurrence of an x position from (0,0).
            {
                return i*region_width;  // Return the column index by the region width, this gives the location on the image that this x occurred at.
            }
        }
    }
    return -1;  // -1 can't be reached natually, so makes a good fail result.
}

int DisplayImages::get_box_y()
{
    for(int i = 0; i < 16; i++) // For each row.
    {
        for(int j = 0; j < 16; j++) // Traverse each column in the row.
        {
            if(flags[i][j]) // If the flag for the current section is set, this is the first occurrence of a y postion from (0,0).
            {
                return i*region_height; // Return the row index by the region height, this gives the location on the image that this y occurred.
            }
        }
    }
    return -1;  // -1 can't be reached naturally, so makes a good fail result.
}

int DisplayImages::get_box_length()
{
    int start = this->get_box_x();  // The length starts at the starting x position, so retrieve that.
    int end = 0;    // Initialise the end.
    for(int i = 0; i < 16; i++) // For each column.
    {
        for(int j = 0; j < 16; j++) // Traverse each row in the column.
        {
            if(flags[j][i]) // If the flag is set, this indicates the region is part of the box.
            {
                int test = (i+1)*region_width;  // Get the furthermost x point of this region.
                if(test > end)  // If the furthermost poing of this region is greater than the current region.
                {
                    end = test; // Set the furthermost point as that of the current region.
                }
            }
        }
    }
    return end-start;   // Return the end subtract the start, which gets the length of the box.
}

int DisplayImages::get_box_depth()
{
    int start = this->get_box_y();  // The depth/height starts at the starting y position, so retrieve that.
    int end = 0;    // Initialise the end.
    for(int i = 0; i < 16; i++) // For each row.
    {
        for(int j = 0; j < 16; j++) // Traverse each column in the row.
        {
            if(flags[i][j]) // If the flag is set, this indicates the region is part of the box.
            {
                int test = (i+1)*region_height;  // Get the furthermost y point of this region.
                if(test > end)  // If the furthermost poing of this region is greater than the current region.
                {
                    end = test; // Set the furthermost point as that of the current region.
                }
            }
        }
    }
    return end-start;   // Return the end subtract the start, which gets the depth/height of the box.
}
