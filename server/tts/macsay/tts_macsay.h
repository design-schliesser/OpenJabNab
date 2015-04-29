#ifndef _TTSMACSAY_H_
#define _TTSMACSAY_H_

#include <QHttp>
#include <QMultiMap>
#include <QTextStream>
#include <QThread>
#include "ttsinterface.h"
	
class TTSMacsay : public TTSInterface
{
	Q_OBJECT
	Q_INTERFACES(TTSInterface)
	
public:
	TTSMacsay();
	virtual ~TTSMacsay();
        QByteArray CreateNewSound(QString, QString, bool);

private:
};

#endif
