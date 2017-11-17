#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fileopen.h"
#include "displayimages.h"
#include "clickablelabel.h"
#include <QListWidgetItem>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow Default Constructor. Called by default, no need for other parameters.
     * @param parent The parent of the MainWindow. This should realy only be the application calling it.
     * @param child Child of the MainWindow. This will be used to set the current child.
     */
    explicit MainWindow(QWidget *parent = 0,QWidget *child = 0);
    /**
     * Default Constructor. Will be used to delete the current child, UI object, and controller object if they still exist.
     */
    ~MainWindow();
    /**
     * @brief set_current_child Sets the current child pointer to point to the current central view.
     * @param child The QWidget that is going to be the central view.
     */
    void set_current_child(QWidget* child);
public slots:
    /**
     * @brief update_widget_forward Slot that is used to advance to the next image when the user clicks next. Also used to proceed from File interface.
     * @return True if the operation was successful. False otherwise.
     */
    bool update_widget_forward();
    /**
     * @brief update_widget_backward Slot that is used to return to the previously classified image.
     * @return True. Currently there are no critical errors that stop the funnction and program from operating.
     * TODO: 'Remember' the previously entered data.
     */
    bool update_widget_backward();
private:
    Ui::MainWindow *ui; // User interface object generated from the form.
    QWidget *current_child; // Pinter to the current central widget.
    QString directory_path; // Stores the directory path that the program operates in.
    FileController* controller; // Pointer to the file controller object for the application.
    bool end_flag; // A flag to know when the application has eached the end of the direction.
    QString feature;    // A string that stores the user-given featre that is being classified.
};

#endif // MAINWINDOW_H
