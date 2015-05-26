#ifndef _PLUGINTOPBOTTOM_H_
#define _PLUGINTOPBOTTOM_H_

#include <QUrl>
#include <QNetworkAccessManager>
#include <QMultiMap>
#include <QTextStream>
#include <QThread>
#include "plugininterface.h"
	
class PluginTopBottom : public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

private slots:
        void analyseXml(QNetworkReply* networkReply);
        void analyseDone(bool, Bunny*, QString, int);

public:
        PluginTopBottom();
        virtual ~PluginTopBottom();

        bool OnClick(Bunny *, PluginInterface::ClickType);
        //bool OnRFID(Bunny * b, QByteArray const& tag);
	void OnBunnyConnect(Bunny *);
	void OnBunnyDisconnect(Bunny *);
	void OnInitPacket(const Bunny *, AmbientPacket &, SleepPacket &);
        int depEichberg;
        int depLudwigBeck;

        // API
        //void InitApiCalls();
        //PLUGIN_BUNNY_API_CALL(Api_AddRFID);
        //PLUGIN_BUNNY_API_CALL(Api_RemoveRFID);

private:
        void getSchedule(Bunny * b, int busstop);
        //int busstop;

};

class PluginTopBottom_Worker : public QThread
{
        Q_OBJECT

signals:
        void done(bool, Bunny*, QString, int);

public:
        PluginTopBottom_Worker(PluginTopBottom * , Bunny *, QString, int);
        virtual ~PluginTopBottom_Worker() {}
        void run();

private:
        PluginTopBottom * plugin;
        Bunny * bunny;
        QString buffer;
        int busstop;

};

#endif
