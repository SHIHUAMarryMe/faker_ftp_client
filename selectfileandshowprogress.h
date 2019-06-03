#ifndef SELECTFILEANDSHOWPROGRESS_H
#define SELECTFILEANDSHOWPROGRESS_H

#include <QWidget>

class QFileDialog;
class QLineEdit;
class QProgressBar;
class QPushButton;
class SelectFileAndShowProgressWidget final : public QWidget
{
public :
    SelectFileAndShowProgressWidget(QWidget* const parent = nullptr);

    // forbit copy and move.
    SelectFileAndShowProgressWidget(const SelectFileAndShowProgressWidget&) = delete;
    SelectFileAndShowProgressWidget& operator=(const SelectFileAndShowProgressWidget&) = delete;


private slots:
    void onSelectedButtonClicked(bool checked);

private:
    QFileDialog* file_dialog_;
    QLineEdit* line_editor_;
    QProgressBar* progress_bar_;
    QPushButton* select_button_;
    QPushButton* start_uploading_;
};




#endif // SELECTFILEANDSHOWPROGRESS_H
