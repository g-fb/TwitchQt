/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHREPLY_HPP
#define TWITCHREPLY_HPP

#include "twitchqt_export.h"

#include "json.hpp"
#include <QNetworkReply>
#include <QVariant>
#include <QImage>

namespace Twitch {
enum class ReplyState {
    Pending,
    Success,
    Error
};

class TWITCHQT_EXPORT Reply : public QObject {
    Q_OBJECT
public:
    template <class T>
    static T* fromData(QObject*, const QVariant&);

    Reply();
    Reply(QNetworkReply*);
    virtual ~Reply();

    const ReplyState& currentState() const;
    operator bool() const;

    QVariant& data();

    const QString& cursor() const;
signals:
    void downloadProgress(qint64, qint64);
    void finished();

protected:
    virtual void onFinished() = 0;

    QNetworkReply* m_reply;
    ReplyState m_currentState;
    QVariant m_data;
    QString m_cursor;
};

class TWITCHQT_EXPORT RawReply : public Reply {
    Q_OBJECT
public:
    using Reply::Reply;
    RawReply(QNetworkReply*);
    virtual ~RawReply() override;

protected:
    virtual void onFinished() override;

    virtual void parseData(const QByteArray&) = 0;
};

class TWITCHQT_EXPORT JSONReply : public Reply {
    Q_OBJECT
public:
    using Reply::Reply;
    JSONReply() = default;
    JSONReply(QNetworkReply*);
    virtual ~JSONReply() override;

    const JSON& json() const;

protected:
    virtual void onFinished() override;

    virtual void parseData(const JSON&);

private:
    JSON m_json;
};

class TWITCHQT_EXPORT ImageReply : public RawReply {
    Q_OBJECT
public:
    using RawReply::RawReply;

protected:
    virtual void parseData(const QByteArray&) override;
};

}

#endif // TWITCHREPLY_HPP
