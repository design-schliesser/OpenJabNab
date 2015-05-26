#include <QDateTime>
#include <QCryptographicHash>
#include <QXmlStreamReader>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMapIterator>
#include <QRegExp>

#include <memory>
#include "bunny.h"
#include "bunnymanager.h"
#include "httprequest.h"
#include "log.h"
#include "cron.h"
#include "messagepacket.h"

#include "plugin_topbottom.h"

#include "settings.h"
#include "ttsmanager.h"

Q_EXPORT_PLUGIN2(plugin_TopBottom, PluginTopBottom)

PluginTopBottom::PluginTopBottom():PluginInterface("TopBottom", "Oben / Unten Busansage")
{
}

PluginTopBottom::~PluginTopBottom() {}

void PluginTopBottom::OnBunnyConnect(Bunny *)
{
}

void PluginTopBottom::OnBunnyDisconnect(Bunny *)
{
}

void PluginTopBottom::OnInitPacket(const Bunny *, AmbientPacket &, SleepPacket &)
{
}

bool PluginTopBottom::OnClick(Bunny * b, PluginInterface::ClickType type)
{
        if (type == PluginInterface::SingleClick || type == PluginInterface::DoubleClick) {
                int eichberg = 9001081;
                int ludwigbeck = 9001078;


                depEichberg = 0;
                depLudwigBeck = 0;

                getSchedule(b, eichberg);
                getSchedule(b, ludwigbeck);

                /*QString text = "";
                if(depEichberg < depLudwigBeck && depEichberg != 0 && depLudwigBeck != 0)
                {
                    text += QString("Der naechste Bus faehrt in %1 Minuten an der Haltestelle Eichberg").arg(depEichberg);
                }
                else
                {
                    text += QString("Der naechste Bus faehrt in %1 Minuten an der Haltestelle Ludwig-Beck-Strasse").arg(depLudwigBeck);
                }
                QByteArray file = TTSManager::CreateNewSound(text, "anna");
                QByteArray message = "MU " + file + "\nPL 3\nMW\n";
                b->SendPacket(MessagePacket(message));*/
                return true;
        }
        return false;
}

/*bool PluginTopBottom::OnRFID(Bunny * b, QByteArray const& tag)
{
        int eichberg = 9001081;
        //int ludwigbeck = 9001078;
        LogDebug("RFID");
        getSchedule(b, eichberg);
        return true;
}*/

void PluginTopBottom::getSchedule(Bunny * b,int busstop)
{
        Q_UNUSED(b);

        QString sb = QString("http://www.ding.eu/ding2/XML_DM_REQUEST?laguage=de&typeInfo_dm=stopID&nameInfo_dm=%1&deleteAssignedStops_dm=1&useRealtime=1&mode=direct").arg(busstop);
        QUrl url(sb);
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        manager->setProperty("BunnyID", b->GetID());
        manager->setProperty("busstopID", busstop);
        connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(analyseXml(QNetworkReply*)));
        manager->get(QNetworkRequest(url));
}

void PluginTopBottom::analyseXml(QNetworkReply* networkReply)
{
        if (!networkReply->error()) {
                Bunny * bunny = BunnyManager::GetBunny(this, networkReply->parent()->property("BunnyID").toByteArray());
                int busstop = networkReply->parent()->property("busstopID").toInt();
                if(bunny) {
                        PluginTopBottom_Worker * p = new PluginTopBottom_Worker(this, bunny, QString::fromUtf8(networkReply->readAll()), busstop);
                        connect(p, SIGNAL(done(bool,Bunny*,QString, int)), this, SLOT(analyseDone(bool,Bunny*,QString, int)));
                        connect(p, SIGNAL(finished()), p, SLOT(deleteLater()));
                        p->start();
                }
        }
        networkReply->deleteLater();
        networkReply->parent()->deleteLater();
}


void PluginTopBottom::analyseDone(bool ret, Bunny * b, QString ausgabe, int busstop)
{
        LogInfo(QString("Busstop in analyseDone: %1").arg(busstop));
        if(ret && b)
        {
            if(busstop == 9001081)
            {
                 depEichberg = ausgabe.toInt();
            }
            if(busstop == 9001078)
            {
                depLudwigBeck = ausgabe.toInt();
            }
            LogInfo(QString("depEichberg, depLudwigBeck in analyseDone: %1, %2").arg(depEichberg).arg(depLudwigBeck));
            if(depEichberg != 0 && depLudwigBeck != 0 && b->IsIdle())
            {
                LogInfo(QString("Send Message!"));
                QString text = "Der naechste Bus faehrt in ";
                if(depEichberg < depLudwigBeck)
                {
                    text += QString("%1 Minuten an der Haltestelle Eichberg").arg(depEichberg);
                }
                else
                {
                    text += QString("%1 Minuten an der Haltestelle Ludwig-Beck-Strasse").arg(depLudwigBeck);
                }
                QByteArray file = TTSManager::CreateNewSound(text, "anna");
                QByteArray message = "MU " + file + "\nPL 3\nMW\n";
                b->SendPacket(MessagePacket(message));
                //Reset
                depEichberg = 0;
                depLudwigBeck = 0;
            }
        }
}

/*void PluginTopBottom::InitApiCalls()
{
        DECLARE_PLUGIN_BUNNY_API_CALL("addrfid(tag)", PluginTopBottom, Api_AddRFID);
        DECLARE_PLUGIN_BUNNY_API_CALL("removerfid(tag)", PluginTopBottom, Api_RemoveRFID);
}


PLUGIN_BUNNY_API_CALL(PluginTopBottom::Api_AddRFID)
{
        Q_UNUSED(account);

        bunny->SetPluginSetting(GetName(), QString("RFIDRatp/%1").arg(hRequest.GetArg("tag")));

        return new ApiManager::ApiOk(QString("Add Top/Bottom for RFID '%1', bunny '%2'").arg(hRequest.GetArg("tag"), QString(bunny->GetID())));
}

PLUGIN_BUNNY_API_CALL(PluginTopBottom::Api_RemoveRFID)
{
        Q_UNUSED(account);

        bunny->RemovePluginSetting(GetName(), QString("RFIDRatp/%1").arg(hRequest.GetArg("tag")));

        return new ApiManager::ApiOk(QString("Remove RFID '%1' for bunny '%3'").arg(hRequest.GetArg("tag"), QString(bunny->GetID())));
}*/

/* WORKER THREAD */
PluginTopBottom_Worker::PluginTopBottom_Worker(PluginTopBottom * p, Bunny * bu, QString b, int stop):plugin(p),bunny(bu),buffer(b.replace("&amp;", "and").replace("&gt;","").replace("&nbsp;","")),busstop(stop)
{

}

void PluginTopBottom_Worker::run()
{
    QXmlStreamReader xml;
    xml.clear();
    xml.addData(buffer);
    LogInfo(QString("Busstop in Worker: %1").arg(busstop));

    //QString currentTag;
    QString ausgabe = "";
    while (!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement())
        {
            if(xml.name() == "itdDeparture")
            {
                foreach(const QXmlStreamAttribute &attr, xml.attributes())
                {
                    //LogInfo(QString("Attr.name(): ").arg(attr.name().toString()));
                    if(attr.name().toString() == QLatin1String("platform"))
                    {

                        if(attr.value().toString() == QLatin1String("2"))
                        {
                            LogInfo(QString("platform == 2"));
                            foreach(const QXmlStreamAttribute &attr2, xml.attributes())
                            {
                                //LogInfo(QString("Attr2.name(): ").arg(attr.name().toString()));
                                if(attr2.name().toString() == QLatin1String("countdown"))
                                {
                                    QString attr2_value = attr2.value().toString();
                                    if(attr2_value.toInt() >= 2)
                                    {
                                        QString countdown = attr2.value().toString();
                                        LogInfo(QString("Attribute Value: %1").arg(countdown));
                                        ausgabe += countdown;
                                        // Stop xml reading
                                        xml.clear();
                                    }
                                }
                            }
                        }
                    }
                    /*if(attr.name().toString() == QLatin1String("platform"))
                    {
                        if(attr.value().toString() == QLatin1String("2"))
                        {
                            LogInfo(QString("platform == 2"));
                            foreach(const QXmlStreamAttribute &attr2, xml.attributes())
                            {
                                LogInfo(QString("countdown found"));
                                if(attr2.name().toString() == QLatin1String("countdown") && attr2.value().toString() >= QLatin1String("2"))
                                {
                                    QString countdown = attr2.value().toString();
                                    LogInfo(QString("Attribute Value: %1").arg(countdown));
                                    ausgabe += countdown;
                                    // Stop xml reading
                                    xml.clear();
                                }
                            }
                        }
                    }*/
                }
            }
        }
    }
    LogInfo(QString("TopBottom: %1").arg(ausgabe));
    /*QByteArray file = TTSManager::CreateNewSound(ausgabe, "anna");
    QByteArray message = "MU " + file + "\nPL 3\nMW\n";

    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError)
            emit done(false, bunny, QByteArray());
    else
            emit done(true, bunny, message);*/

    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError)
            emit done(false, bunny, QString(), busstop);
    else
            emit done(true, bunny, ausgabe, busstop);
}
