#include "fileopen.h"
#include "ui_fileopen.h"
#include <QFileDialog>

FileOpen::FileOpen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileOpen)
{
    ui->setupUi(this);  // Create the UI element.
    QObject::connect(this->findChild<QPushButton*>("browse_button"),SIGNAL(clicked()), this, SLOT(get_path())); // Connect the browse button to the slot that opens the file dialog.
}

QString FileOpen::get_file_string()
{
    return this->file_string;   // Returns the file string, when a correct one is selected.
}

void FileOpen::get_path()
{
    // The line below opens a file dilog that only shows and accepts directories. Starts in the root directory to support a degree of cross-platform.
    file_string = QFileDialog::getExistingDirectory(this, tr("Open directory for images."),"/",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    this->findChild<QLineEdit*>("filepath")->setText(file_string);  // Set the text of the text field as the path to the selected directory.
}

QString FileOpen::get_feature()
{
    return ui->training_label_edit->text(); // Returns the value that the user entered in the feature field.
}

FileOpen::~FileOpen()
{
    delete ui;  // Deletes the UI pointer object.
}
