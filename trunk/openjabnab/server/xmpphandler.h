#ifndef _XMPPHANDLER_H_
#define _XMPPHANDLER_H_

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>
#include "pluginmanager.h"

class XmppHandler : public QObject
{
	Q_OBJECT
	
public:
	XmppHandler(QTcpSocket *, PluginManager * p);

protected:
	virtual ~XmppHandler() {};

private slots:
	void HandleBunnyXmppMessage();
	void HandleVioletXmppMessage();
	void OnDisconnect();

private:
	QList<QByteArray> XmlParse(QByteArray const&);
	void WriteToBunny(QByteArray const&);

	QTcpSocket * incomingXmppSocket;
	QTcpSocket outgoingXmppSocket;
	PluginManager * pluginManager;
};

#endif
