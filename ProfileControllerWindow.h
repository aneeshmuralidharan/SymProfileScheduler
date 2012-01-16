/*******************************************************************************
Author : Aneesh Muralidharan
E-Mail : aneesh@thegeek.in
*******************************************************************************/

#ifndef PROFILECONTROLLERWINDOW_H
#define PROFILECONTROLLERWINDOW_H

#include <QWidget>

struct ProfileControllerWindowData;
class ProfileControllerWindow : public QWidget
{
    Q_OBJECT
public:
    ProfileControllerWindow(QWidget *parent = 0);
    ~ProfileControllerWindow();

protected:
    void paintEvent(QPaintEvent *);

protected slots:
    void addProfileConfiguration();

private:
    ProfileControllerWindowData* d;
};

#endif // PROFILECONTROLLERWINDOW_H
