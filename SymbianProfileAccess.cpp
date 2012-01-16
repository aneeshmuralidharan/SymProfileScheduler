/*******************************************************************************
Author : Aneesh Muralidharan
E-Mail : aneesh@thegeek.in
*******************************************************************************/

#include "SymbianProfileAccess.h"
#include <QDebug>
#include <QString>
#include <QStringList>

#include <MProEngEngine.h>
#include <ProEngFactory.h>
#include <profiler.h>
#include <MProEngProfileNameArray.h>
#include <MProEngProfile.h>
#include <MProEngProfileName.h>
#include <asclisession.h>

struct SymbianProfileAccessData
{
    SymbianProfileAccessData()
        : profileEngine(0),
          currentProfile(-1),
          initialized(false){}

    MProEngEngine* profileEngine;
    QStringList deviceProfileList;
    int currentProfile;
    bool initialized;

    QtMobility::QSystemDeviceInfo systemDeviceInfo;

    QString descToName(const TDesC& tdesc)
    {
        return QString::fromUtf16(tdesc.Ptr(), tdesc.Length());
    }
};

SymbianProfileAccess::SymbianProfileAccess(QObject *parent) :
    QObject(parent)
{
    d = new SymbianProfileAccessData;
    d->profileEngine = ProEngFactory::NewEngineL();
    this->init();
    connect(&d->systemDeviceInfo, SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)), this, SLOT(slotSystemProfileChanged(QSystemDeviceInfo::Profile)));
}

SymbianProfileAccess::~SymbianProfileAccess()
{
    if(d->profileEngine)
        d->profileEngine->Release();
    delete d;
}

QStringList SymbianProfileAccess::profilesInDevice() const
{
    return d->deviceProfileList;
}

int SymbianProfileAccess::profileCount() const
{
    d->deviceProfileList.count();
}

QString SymbianProfileAccess::profileAt(int index) const
{
    if(index < 0 || index >= d->deviceProfileList.count())
        return QString();

    return d->deviceProfileList.at(index);
}

int SymbianProfileAccess::indexOfProfile(const QString& profileName) const
{
    return d->deviceProfileList.indexOf(profileName);
}

int SymbianProfileAccess::currentProfile() const
{
    return d->profileEngine->ActiveProfileId();
}

bool SymbianProfileAccess::isInitialized() const
{
    return d->initialized;
}

void SymbianProfileAccess::init()
{
    if(!d->profileEngine)
        return;

    d->deviceProfileList.clear();
    for(int i=0; i<d->profileEngine->ProfileNameArrayLC()->MdcaCount(); i++)
        d->deviceProfileList << d->descToName(d->profileEngine->ProfileLC(i)->ProfileName().Name());

    d->currentProfile = d->profileEngine->ActiveProfileId();

    d->initialized = true;
    emit initialized();
}

bool SymbianProfileAccess::setProfile(const QString& profileName)
{
    return setProfile(d->deviceProfileList.indexOf(profileName));
}

bool SymbianProfileAccess::setProfile(int profileIndex)
{
    if(profileIndex < 0 || profileIndex >= d->deviceProfileList.count())
        return false;

    if(profileIndex == this->currentProfile())
        return false;

    int iError;
    TRAP(iError,d->profileEngine->SetActiveProfileL(profileIndex);)

    if(iError != KErrNone)
        return false;

    d->currentProfile = profileIndex;
    emit currentProfileChanged(d->currentProfile);
}

void SymbianProfileAccess::slotSystemProfileChanged(QSystemDeviceInfo::Profile val)
{
    // TODO
    /*
    d->currentProfile = val;
    emit currentProfileChanged(d->currentProfile);
    */
}
