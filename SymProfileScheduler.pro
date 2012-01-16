# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE003384B

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:{

    TARGET.CAPABILITY = ReadUSerData \
        WriteUserData \
        NetworkServices \
        LocalServices \
        WriteDeviceData \
        UserEnvironment

    INCLUDEPATH += C:/QtSDK/Symbian/SDKs/Symbian3Qt474/epoc32/include \
                   C:/QtSDK/Symbian/SDKs/Symbian3Qt474/epoc32/include/platform \
                   C:/QtSDK/Symbian/SDKs/Symbian3Qt474/epoc32/include/app \
                   C:/QtSDK/Symbian/SDKs/Symbian3Qt474/epoc32/include/mw

    LIBS += -LC:\QtSDK\Symbian\SDKs\Symbian3Qt474\epoc32\release\armv5\lib -lprofileengine \
       -letel3rdparty \
       -lfeatdiscovery
}


# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += systeminfo

SOURCES += \
    MainWindow.cpp \
    Main.cpp \
    SymbianProfileAccess.cpp \
    ProfileControllerWindow.cpp
HEADERS += \
    MainWindow.h \
    SymbianProfileAccess.h \
    ProfileControllerWindow.h
FORMS +=

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()









