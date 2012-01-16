/*******************************************************************************
Author : Aneesh Muralidharan
E-Mail : aneesh@thegeek.in
*******************************************************************************/

#include "ProfileControllerWindow.h"
#include "SymbianProfileAccess.h"
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPaintEvent>
#include <QTimeEdit>
#include <QMap>
#include <QTimer>
#include <QPushButton>
#include <QScrollArea>

struct ProfileControllerWindowData
{
    QComboBox* comboBox;
    SymbianProfileAccess symProAccess;
    QMap<QTime, int> timeProfileMap;
    QLabel* scheduleInfoLabel;

    QTimeEdit* te11 ;
    QTimeEdit* te12 ;
    QComboBox* cb1  ;
};

ProfileControllerWindow::ProfileControllerWindow(QWidget *parent) :
    QWidget(parent)
{
    d = new ProfileControllerWindowData;

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(new QLabel("Set current profile"));

    d->comboBox = new QComboBox;
    d->comboBox->addItems(d->symProAccess.profilesInDevice());
    d->comboBox->setCurrentIndex(d->symProAccess.currentProfile());
    hLayout->addWidget(d->comboBox);

    QGridLayout* gLayout = new QGridLayout;
    d->te11 = new QTimeEdit;
    d->te12 = new QTimeEdit;
    d->cb1  = new QComboBox;
    d->cb1->addItems(d->symProAccess.profilesInDevice());
    QPushButton* pb = new QPushButton("SET");
    gLayout->addWidget(d->te11, 0, 0);
    gLayout->addWidget(d->te12, 0, 1);
    gLayout->addWidget(d->cb1, 0, 2);
    gLayout->addWidget(pb, 0, 3);
    connect(pb, SIGNAL(clicked()), this, SLOT(addProfileConfiguration()));

    d->scheduleInfoLabel = new QLabel;
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setLayout(new QHBoxLayout);
    scrollArea->layout()->addWidget(d->scheduleInfoLabel);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addLayout(gLayout);
    vLayout->addWidget(scrollArea);

    connect(&d->symProAccess, SIGNAL(currentProfileChanged(int)), d->comboBox, SLOT(setCurrentIndex(int)));
    connect(d->comboBox, SIGNAL(currentIndexChanged(int)), &d->symProAccess, SLOT(setProfile(int)));
}

ProfileControllerWindow::~ProfileControllerWindow()
{
    delete d;
}

void ProfileControllerWindow::paintEvent(QPaintEvent *e)
{
    d->comboBox->setCurrentIndex(d->symProAccess.currentProfile());
    QWidget::paintEvent(e);
}

void ProfileControllerWindow::addProfileConfiguration()
{
    d->timeProfileMap[d->te11->time()] = d->cb1->currentIndex();
    if(!d->timeProfileMap.contains(d->te12->time()))
        d->timeProfileMap[d->te12->time()] = 0;

    QString info;
    for(int i=0; i<d->timeProfileMap.keys().count()-1; ++i)
    {
        QTime startTime = d->timeProfileMap.key(i);
        QTime endTime = d->timeProfileMap.key(i+1);
        QString timeStamp = startTime.toString() + " - " + endTime.toString();
        QString profileName = d->symProAccess.profileAt(d->timeProfileMap.value(startTime));
        info += timeStamp + " : " + profileName + "\n";
    }
    d->scheduleInfoLabel->setText(info);
}
