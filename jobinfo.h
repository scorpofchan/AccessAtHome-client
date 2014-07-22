#ifndef JOBINFO_H
#define JOBINFO_H

#include <QDialog>

namespace Ui {
class JobInfo;
}

class JobInfo : public QDialog
{
    Q_OBJECT

public:
    explicit JobInfo(QWidget *parent = 0);
    ~JobInfo();
    void setname(QString);
    void setdescription(QString);

private:
    Ui::JobInfo *ui;
};

#endif // JOBINFO_H
