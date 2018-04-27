#ifndef TWITCHREPLY_HPP
#define TWITCHREPLY_HPP

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QVariant>

namespace Twitch {
enum class ReplyState {
    Pending,
    Success,
    Error
};

class Reply : public QObject {
    Q_OBJECT
public:
    Reply(QNetworkReply*);
    virtual ~Reply();

    const ReplyState& currentState() const;
    operator bool() const;

    QVariant& data();

    const QString& cursor() const;
signals:
    void finished();

protected:
    QNetworkReply* m_reply;
    ReplyState m_currentState;
    QVariant m_data;
    QString m_cursor;
};

class RawReply : public Reply {
    Q_OBJECT
public:
    RawReply(QNetworkReply*);
    virtual ~RawReply();

protected:
    virtual void parseData(const QByteArray&) = 0;
};

class JSONReply : public Reply {
    Q_OBJECT
public:
    JSONReply(QNetworkReply*);
    virtual ~JSONReply();

protected:
    virtual void parseData(const QJsonDocument&) = 0;
};

#include "twitchreply.inl"
}

#endif // TWITCHREPLY_HPP
