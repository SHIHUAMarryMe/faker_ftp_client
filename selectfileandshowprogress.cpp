
#include <QLineEdit>
#include <QFileDialog>
#include <QProgressBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>

#include "selectfileandshowprogress.h"


SelectFileAndShowProgressWidget::SelectFileAndShowProgressWidget(QWidget * const parent)
    :QWidget{ parent },
      file_dialog_{new QFileDialog},
      line_editor_{new QLineEdit},
    progress_bar_{new QProgressBar},
    select_button_{new QPushButton},
    start_uploading_{new QPushButton}
{
    this->file_dialog_->setWindowTitle(tr("select a file"));
    this->file_dialog_->setAcceptMode(QFileDialog::AcceptOpen);
    this->file_dialog_->setFileMode(QFileDialog::ExistingFiles);
    this->file_dialog_->setViewMode(QFileDialog::Detail);

    this->select_button_->setText(tr("select a file"));
    this->select_button_->setFixedWidth(150);

    this->line_editor_->setPlaceholderText("push button for selecting a file.");
//    this->line_editor_->setFixedWidth(500);


    this->start_uploading_->setText(tr("start uploading"));
    this->start_uploading_->setFixedWidth(150);

    this->progress_bar_->setRange(0, 100);
    this->progress_bar_->setValue(0);

    QVBoxLayout* total_layout{ new QVBoxLayout };

    QHBoxLayout* button_editor_layout{ new QHBoxLayout };
    button_editor_layout->addWidget(this->select_button_);
    button_editor_layout->addWidget(this->line_editor_);

    QHBoxLayout* button_progress_layout{ new QHBoxLayout };
    button_progress_layout->addWidget(this->start_uploading_);
    button_progress_layout->addWidget(this->progress_bar_);


    total_layout->addLayout(button_editor_layout);
    total_layout->addLayout(button_progress_layout);

    this->setLayout(total_layout);




    QObject::connect(this->select_button_, &QPushButton::clicked, this, &SelectFileAndShowProgressWidget::onSelectedButtonClicked);

}

void SelectFileAndShowProgressWidget::onSelectedButtonClicked(bool checked)
{
    (void)checked;

    this->file_dialog_->setDirectory(".");

    if(this->file_dialog_->exec() == QDialog::Accepted)
    {
        QString file_path_str{ this->file_dialog_->selectedFiles()[0] };
        this->line_editor_->setText(file_path_str);

    }else{
        QMessageBox msgBox;
        msgBox.setText(tr("You'd better select a file."));
        msgBox.exec();

        this->line_editor_->clear();
    }
}
