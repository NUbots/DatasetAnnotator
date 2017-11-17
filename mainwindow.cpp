#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <iostream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent,QWidget *child) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // Setup the UI object.
    // The following block hides elements while the File Interface is open, as they are only needed for the image interface.
    ui->file_list->hide();  // List of files.
    ui->label->hide();  // Feature label.
    ui->prev_button->hide();    // Previous button.
    ui->occlusion_label->hide();    // Label with Occlusion written in it.
    ui->occlusion_spin->hide();     // Occlusion spinner.
    ui->truncate_edit->hide();      // Truncate line edit field.
    ui->truncate_label->hide();     // Label that has Truncated written in it.
    // End block.
    current_child = child;  // Set the child from the parameter given.
    end_flag = false;   // Since it is the start, it shouldn't be the end.
    this->set_current_child(current_child); // Actually sets the child up as the central child of the main window.
    // Line below connects the next button in the button region of the main window to the update widget forward function.
    connect(this->findChild<QWidget*>("button_widget")->findChild<QPushButton*>("next_button"),SIGNAL(clicked()),this,SLOT(update_widget_forward()));
    // Line below connects the previous button in the button region of the main window to the update widget backward function.
    connect(ui->prev_button,SIGNAL(clicked(bool)),this,SLOT(update_widget_backward()));
}

void MainWindow::set_current_child(QWidget* child)
{
    current_child = child;  // ensure that the current child has the reference to the child object that was passed through.
    QWidget* main_widget = this->centralWidget()->findChild<QWidget*>("main_widget");   // Find the main widget of the main window.
    main_widget->layout()->addWidget(child);    // Add the child widget to the layout that the main widget has.
}

bool MainWindow::update_widget_forward()
{
    if(this->centralWidget()->findChild<QWidget*>("main_widget")->findChild<FileOpen*>("FileOpen")) // If the current child is a FileOpen widget, it means that it's information needs to be processed.
    {
        FileOpen* file_widget = (FileOpen*)(this->centralWidget()->findChild<QWidget*>("main_widget")->findChild<FileOpen*>("FileOpen"));   // Retrieve a reference to the File Open widget.
        FileOpen* temp = file_widget;   // Give it a temp value?
        QDir test_dir(file_widget->get_file_string());  // Create a directory to ensure that the directory exists.
        if(!test_dir.exists())  // If the directory does not exist.
        {
            return false; // Operation failed, return to the FileOpen screen.
        }
        this->directory_path = file_widget->get_file_string();  // Retrieve the directory path from the File interface and store it in the corresponding local variable.
        ui->label->setText(QString("Feature: ") + file_widget->get_feature());  // Set the label that informa the user about the feature
        this->feature = file_widget->get_feature(); // Store the user entered feature that is being classified, for later use in data output.
        controller = new FileController(this->directory_path);  // Create a new file controller using the current directory path.
        if(controller->directory_size() == 0)
        {
            return false;   // Fail result, since the directory is empty.
        }
        this->centralWidget()->findChild<QWidget*>("main_widget")->layout()->removeWidget(file_widget); // This removes the file interface widget. Needs to be done before adding the replacing widget.
        delete temp;
        set_current_child(new DisplayImages(this,controller,directory_path));   // Create a new Display Images interface and set that to be the current child of main window.
        ui->file_list->setMinimumWidth(ui->file_list->sizeHintForColumn(0));    // Keeps the List Widget from becoming too wide.
        // The following block shows all of the hidden elements from when the widget had initially started.
        ui->file_list->show();
        ui->prev_button->show();
        ui->label->show();
        ui->occlusion_label->show();
        ui->occlusion_spin->show();
        ui->truncate_edit->show();
        ui->truncate_label->show();
        // End block.
        ui->occlusion_spin->setValue(0);    // Default 0.
        ui->truncate_edit->setPlaceholderText("Integer: 0-99"); // This instructs the user on accepted input for truncated.
        for(int i = 0; i < controller->directory_size(); i++)   // For each file in the directory that the file controller has.
        {
            ui->file_list->addItem((controller->filename_at(i)));   // Add the filename to the list widget.
        }
        ui->file_list->setCurrentRow(0);    // Used to scroll through the list, start at the beginning.
    }
    else    // The application is currently in the middle of the classification process.
    {
        if(controller->end_directory() || end_flag == true) // If the file that output is being written for is the last in the directory, this is a special case.
        {
            DisplayImages* temp = (DisplayImages*)(ui->centralWidget->findChild<DisplayImages*>("DisplayImages"));  // Retrieve the current Display Images object.
            if(temp == nullptr) // This is a failsafe that shouldn't happen.
            {
                std::cout << "Error";
            }
            else    // Good to go!
            {
                if(temp->get_box_x() >=0) // Since the fail return is -1, anything greater than that indicates a bounding box exists for the Image.
                {
                    //Get valaues from the image object.
                    int x = temp->get_box_x();
                    int y = temp->get_box_y();
                    int length = temp->get_box_length();
                    int depth = temp->get_box_depth();
                    // Since the truncated score is entered as a string of an integer, some processing is required.
                    QString input = ui->truncate_edit->text().trimmed();    // Retrieve the text and trim it.
                    float truncated = 0;    // Initialise value.
                    try{
                        // This try block tries to convert the string to an integer, then to a float. If any part of this fails, will catch.
                        int value = input.toInt();
                        truncated = (float)value/100.00;
                    }
                    catch(std::exception e) // Invalid input.
                    {
                        // Resets the input and ignores the next call, forcing correct input.
                        ui->truncate_edit->setText("Please enter a number between 0 and 99.");
                        return false;
                    }
                    input = ui->occlusion_spin->text(); //Retrieves occluded score from spinner. This doesn't need a catch because inut has to be an int.
                    int occlusion = input.toInt();
                    controller->feature_existence(true,controller->get_current_index());    // Indicate that the feature exists in this image.
                    controller->set_feature_boundingbox(controller->get_current_index(),x,y,length,depth);  // Set the bounding box that contains the feature in the image.
                    controller->set_parameters(controller->get_current_index(),truncated,occlusion);    // Set the occluded and truncated scores for the image.
                }
                else    // No x value, no feature.
                {
                    controller->feature_existence(false,controller->get_current_index());   // Only need to indicate that feature does not exist.
                }
            }
            controller->write_output(controller->get_current_index(), feature); // Write the output data for the current image.
            delete temp;
            QApplication::quit();   // Currently quits at end. TODO: just restart?
        }
        else    // somewhere in the directory that is not the beginning or the end.
        {
            DisplayImages* temp = (DisplayImages*)(ui->centralWidget->findChild<DisplayImages*>("DisplayImages"));  // Retrieve the current Display Images object.
            if(temp == nullptr) // This is a failsafe that shouldn't happen.
            {
                std::cout << "Error";
            }
            else
            {
                if(temp->get_box_x() >=0) // Since the fail return is -1, anything greature than that indicates a bounding box exists for the Image.
                {
                    //Get valaues from the image object.
                    int x = temp->get_box_x();
                    int y = temp->get_box_y();
                    int length = temp->get_box_length();
                    int depth = temp->get_box_depth();
                    // Since the truncated score is entered as a string of an integer, some processing is required.
                    QString input = ui->truncate_edit->text().trimmed();    // Retrieve the text and trim it.
                    float truncated = 0;    // Initialise value.
                    try{
                        // This try block tries to convert the string to an integer, then to a float. If any part of this fails, will catch.
                        int value = input.toInt();
                        truncated = (float)value/100.00;
                    }
                    catch(std::exception e) // Invalid input.
                    {
                        // Resets the input and ignores the next call, forcing correct input.
                        ui->truncate_edit->setText("Please enter a number between 0 and 99.");
                        return false;
                    }
                    input = ui->occlusion_spin->text(); //Retrieves occluded score from spinner. This doesn't need a catch because inut has to be an int.
                    int occlusion = input.toInt();
                    controller->feature_existence(true,controller->get_current_index());    // Indicate that the feature exists in this image.
                    controller->set_feature_boundingbox(controller->get_current_index(),x,y,length,depth);  // Set the bounding box that contains the feature in the image.
                    controller->set_parameters(controller->get_current_index(),truncated,occlusion);    // Set the occluded and truncated scores for the image.
                }
                else    // No x value, no feature.
                {
                    controller->feature_existence(false,controller->get_current_index());   // Only need to indicate that feature does not exist.
                }
                controller->write_output(controller->get_current_index(), feature); // Write the output data for the current image.

                controller->increment_index(1); // Since there are still more images to process, continue on.
                ui->main_widget->layout()->removeWidget(temp);  // Remove the widget that is to be deleted.
                delete temp;
                set_current_child(new DisplayImages(this,controller,directory_path));   // Create a new Display Images for the next file and sets it as the current child.
                ui->file_list->setCurrentRow(controller->get_current_index());  // Keeps the list updated to be the current image showing.
                ui->occlusion_spin->setValue(0);    // Reset occluded element value.
                ui->truncate_edit->setText("");     // Reset truncated element value.
                ui->truncate_edit->setPlaceholderText("Integer: 0-99"); // Reset truncated placeholder text.
                if(controller->end_directory()) // If this is the last image, then set the end flag.
                {
                    ui->next_button->setText("Finish"); // Indicates that the application will finish when clicked on.
                    end_flag = true;    // Boolean flag used above set to indicate the end of the images.
                }
            }
        }
    }
    return true;    // Success.
}

bool MainWindow::update_widget_backward()
{
    end_flag = false;   // Since we are moving backwards, it is automatically not the last file.
    ui->next_button->setText("Next");   // For the same reason as above, automatically not the finish.
    if(controller->get_current_index() != 0)    // If the current index is 0, then cannot move any further back in the list, already at the start.
    {
        DisplayImages* temp = (DisplayImages*)(ui->centralWidget->findChild<DisplayImages*>("DisplayImages"));  // Retrieve the current Display Images object.
        if(temp == nullptr) // This is a failsafe that shouldn't happen.
        {
            std::cout << "Error";
        }
        else    // Good to go.
        {
            controller->increment_index(-1);    // Incrementing backwards is just incrementing in a negative direction.
            ui->main_widget->layout()->removeWidget(temp);  // Remove the current image.
            delete temp;    // We don't need any information, so can delete it directly.
            set_current_child(new DisplayImages(this,controller,directory_path));   // New image file with previous image, set as current child.
        }
    }
    return true; //Succes!
}

MainWindow::~MainWindow()
{
    // Delete all new pointers.
    delete ui;
    delete current_child;
    delete controller;
}
