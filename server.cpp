#include "server.h"

#include <QtWebSockets>
#include <QtCore>
#include <QDebug>

#include <cstdio>
using namespace std;

QT_USE_NAMESPACE

static QString getIdentifier(QWebSocket *peer)
{
    return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(),
                                       QString::number(peer->peerPort()));
}

Server::Server(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Chat Server"),
                                            QWebSocketServer::NonSecureMode,
                                            this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {

        qDebug() << "Server's connecting URL : " << m_pWebSocketServer->serverUrl();
        QTextStream(stdout) << "Chat Server listening on port " << port << '\n';
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
    }
}

Server::~Server()
{

}

void Server::onNewConnection()
{
    auto pSocket = m_pWebSocketServer->nextPendingConnection();
    QTextStream(stdout) << getIdentifier(pSocket) << " connected!\n";
    pSocket->setParent(this);

    connect(pSocket, &QWebSocket::textMessageReceived,
            this, &Server::processMessage);

//    connect(pSocket, &QWebSocket::binaryMessageReceived,
//            this, &Server::onBinaryMessage);

    connect(pSocket, &QWebSocket::disconnected,
            this, &Server::socketDisconnected);

    pSocket->sendTextMessage("Hello wassup bro!");

    m_clients << pSocket;
}

//void Server::onBinaryMessage(QByteArray message){
//    // should be done in parallel
//    QJsonDocument item_doc = QJsonDocument::fromJson(message);
//    QJsonObject item_object = item_doc.object();
//    int id = item_object["id"].toInt();
//    QString key = item_object["key"].toString();
//}

void Server::processMessage(const QString &message)
{
    qDebug()<<"from client: "<<message;
//    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
//    for (QWebSocket *pClient : qAsConst(m_clients)) {
//        if (pClient != pSender) //don't echo message back to sender
//            pClient->sendTextMessage(message);
//    }
}

void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QTextStream(stdout) << getIdentifier(pClient) << " disconnected!\n";
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
