/*******************************************************************************
Author : Aneesh Muralidharan
E-Mail : aneesh@thegeek.in
*******************************************************************************/

#ifndef SYMBIANPROFILEACCESS_H
#define SYMBIANPROFILEACCESS_H

#include <QObject>
#include <QSystemDeviceInfo>

using namespace QtMobility;

struct SymbianProfileAccessData;
class SymbianProfileAccess : public QObject
{
    Q_OBJECT

public:
    SymbianProfileAccess(QObject *parent = 0);
    ~SymbianProfileAccess();

    QStringList profilesInDevice() const;
    int profileCount() const;
    QString profileAt(int index) const;
    int indexOfProfile(const QString& profileName) const;
    int currentProfile() const;

    bool isInitialized() const;

protected:
    void init();

public slots:
    bool setProfile(const QString& profileName);
    bool setProfile(int profileIndex);

protected slots:
    void slotSystemProfileChanged(QSystemDeviceInfo::Profile);

signals:
    void initialized();
    void currentProfileChanged(int);

private:
    SymbianProfileAccessData* d;
};

#endif // SYMBIANPROFILEACCESS_H
