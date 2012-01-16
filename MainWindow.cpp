/*******************************************************************************
Author : Aneesh Muralidharan
E-Mail : aneesh@thegeek.in
*******************************************************************************/

#include "MainWindow.h"
#include "ProfileControllerWindow.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setCentralWidget(new ProfileControllerWindow);
}
