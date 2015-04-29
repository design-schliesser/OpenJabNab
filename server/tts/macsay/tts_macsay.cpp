#include <QDateTime>
#include <QUrl>
#include <QCryptographicHash>
#include <QMapIterator>
#include "tts_macsay.h"
#include "log.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

Q_EXPORT_PLUGIN2(tts_macsay, TTSMacsay)

TTSMacsay::TTSMacsay():TTSInterface("macsay", "Macsay")
{

	// German
        voiceList.insert("Anna", "Anna");
        voiceList.insert("Markus", "Markus");
        voiceList.insert("Petra", "Petra");
        voiceList.insert("Yannick", "Yannick");
}

TTSMacsay::~TTSMacsay()
{
}

QByteArray TTSMacsay::CreateNewSound(QString text, QString voice, bool forceOverwrite)
{
    QEventLoop loop;

    if(!voiceList.contains(voice))
            voice = "Anna";

    // Check (and create if needed) output folder
    QDir outputFolder = ttsFolder;
    if(!outputFolder.exists(voice))
            outputFolder.mkdir(voice);

    if(!outputFolder.cd(voice))
    {
            LogError(QString("Cant create TTS Folder : %1").arg(ttsFolder.absoluteFilePath(voice)));
            return QByteArray();
    }

    // Compute fileName
    QString fileName = QCryptographicHash::hash(text.toAscii(), QCryptographicHash::Md5).toHex().append(".mp3");
    QString filePath = outputFolder.absoluteFilePath(fileName);

    if(!forceOverwrite && QFile::exists(filePath))
            return ttsHTTPUrl.arg(voice, fileName).toAscii();

    // Fetch MP3
    QHttp http("192.168.2.99");
    QObject::connect(&http, SIGNAL(done(bool)), &loop, SLOT(quit()));

    QByteArray ContentData;
    ContentData += "b=123456789abc&f=test1024&t=" + QUrl::toPercentEncoding(text) + "&v=" + voice;

    QHttpRequestHeader Header;
    Header.addValue("Host", "192.168.2.99");

    Header.setContentLength(ContentData.length());
    Header.setRequest("GET", "/tts.php", 1, 1);

    http.request(Header, ContentData);
    loop.exec();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
            LogError("Cannot open sound file for writing");
            return QByteArray();
    }
    file.write(http.readAll());
    file.close();
    return ttsHTTPUrl.arg(voice, fileName).toAscii();

}

