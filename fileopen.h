#ifndef FILEOPEN_H
#define FILEOPEN_H

#include <QWidget>

namespace Ui {
class FileOpen;
}

class FileOpen : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief FileOpen Default Constructor. Since this is a graphic user interface oobject, this is automatically used.
     * @param parent The parent of the object being created, default is null.
     */
    explicit FileOpen(QWidget *parent = 0);
    /**
     * @brief Deconstructor, used to destroy the UI object of the interface.
     */
    ~FileOpen();
    /**
     * @brief get_file_string Retrieves the user-entered directory string.
     * @return The QString object which is stored from user input.
     */
    QString get_file_string();
    /**
     * @brief get_feature Retrieves the user-entered feature that is being identified.
     * @return THe QString object which is stored from user input.
     */
    QString get_feature();
public slots:
    /**
     * @brief get_path This is the slot that launches when the user browses for the directory. This laucches a QFileDialog.
     */
    void get_path();

private:
    Ui::FileOpen *ui; //User interface object for displaying.
    QString file_string;    // QString which stores the file string that the user enters.
};

#endif // FILEOPEN_H
