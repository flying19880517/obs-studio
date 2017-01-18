#ifndef WEBDIALOG_HPP
#define WEBDIALOG_HPP

#include <QDialog>

namespace Ui {
class XimalayaWebDialog;
}

class XimalayaWebDialog : public QDialog
{
    Q_OBJECT

public:
    explicit XimalayaWebDialog(QWidget *parent = 0);
    ~XimalayaWebDialog();

    bool SetUrl(QUrl url);
private:
    Ui::XimalayaWebDialog *ui;
};

#endif // WEBDIALOG_HPP
