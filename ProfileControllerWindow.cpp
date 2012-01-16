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
#include <QDebug>

struct ProfileControllerWindowData
{
    QComboBox* comboBox;
    SymbianProfileAccess symProAccess;
    QMap<QTime, int> timeProfileMap;
    QLabel* scheduleInfoLabel;

    QTimeEdit* teStartTime ;
    QTimeEdit* teEndTime ;
    QComboBox* cbProfileChooser  ;
};

ProfileControllerWindow::ProfileControllerWindow(QWidget *parent) :
    QWidget(parent)
{
    d = new ProfileControllerWindowData;

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(new QLabel(tr("Set current profile")));

    d->comboBox = new QComboBox;
    d->comboBox->addItems(d->symProAccess.profilesInDevice());
    d->comboBox->setCurrentIndex(d->symProAccess.currentProfile());
    hLayout->addWidget(d->comboBox);

    QGridLayout* gLayout = new QGridLayout;
    d->teStartTime = new QTimeEdit;
    d->teEndTime = new QTimeEdit;
    d->cbProfileChooser  = new QComboBox;
    d->cbProfileChooser->addItems(d->symProAccess.profilesInDevice());
    QPushButton* pb = new QPushButton(tr("SET"));
    gLayout->addWidget(new QLabel(tr("StartTime")), 0, 0);
    gLayout->addWidget(d->teStartTime, 0, 1);
    gLayout->addWidget(new QLabel(tr("EndTime")), 1, 0);
    gLayout->addWidget(d->teEndTime, 1, 1);
    gLayout->addWidget(new QLabel(tr("Profile")), 2, 0);
    gLayout->addWidget(d->cbProfileChooser, 2, 1);
    gLayout->addWidget(pb, 3, 0, 1, 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(addProfileConfiguration()));

    d->scheduleInfoLabel = new QLabel;
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setLayout(new QHBoxLayout);
    scrollArea->layout()->addWidget(d->scheduleInfoLabel);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout);
    vLayout->addLayout(gLayout);
    vLayout->addWidget(scrollArea);

    d->timeProfileMap[QTime(0,0,0)] = 0;
    d->timeProfileMap[QTime(23, 59, 59)] = 0;
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
    d->timeProfileMap[d->teStartTime->time()] = d->cbProfileChooser->currentIndex();
    if(!d->timeProfileMap.contains(d->teEndTime->time()))
        d->timeProfileMap[d->teEndTime->time()] = 0;

    QString info;
    for(int i=0; i<d->timeProfileMap.keys().count()-1; ++i)
    {
        QTime startTime = d->timeProfileMap.keys().at(i);
        QTime endTime = d->timeProfileMap.keys().at(i+1);
        QString timeStamp = startTime.toString() + " - " + endTime.toString();
        QString profileName = d->symProAccess.profileAt(d->timeProfileMap.value(startTime));
        info += timeStamp + " : " + profileName + "\n";
    }
    d->scheduleInfoLabel->setText(info);
    this->switchCurrentProfile();
}

void ProfileControllerWindow::switchCurrentProfile()
{
    QTime currentTime = QTime::currentTime();
    QTime indexTime;
    unsigned long long int durationToWait = -1;
    for(int i=0; i<d->timeProfileMap.count()-1; ++i)
    {
        QTime startTime = d->timeProfileMap.keys().at(i);
        QTime endTime = d->timeProfileMap.keys().at(i+1);
        if(currentTime >= startTime && currentTime < endTime)
        {
            indexTime = startTime;
            durationToWait = QTime::currentTime().msecsTo(endTime);

            d->symProAccess.setProfile(d->timeProfileMap.value(startTime));
            break;
        }
    }
    qDebug() << "Changed profile at : " << currentTime;
    qDebug() << "Next change in [" << durationToWait << "] msecs or [" << durationToWait / 1000 << "] secs" ;
    QTimer::singleShot(durationToWait, this, SLOT(switchCurrentProfile()));
}
