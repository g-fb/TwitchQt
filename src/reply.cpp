/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "reply.hpp"

#include <QJsonDocument>
#include <QString>

namespace Twitch
{
template<class T>
T *Twitch::Reply::fromData(QObject *parent, const QVariant &data)
{
    T *reply = new T;
    reply->setParent(parent);
    reply->m_data = data;
    return reply;
}

Reply::Reply()
    : QObject(nullptr)
    , m_reply(nullptr)
    , m_currentState(ReplyState::Success)
    , m_cursor(QString())
{
}

Reply::Reply(QNetworkReply *reply)
    : QObject(reply->manager())
    , m_reply(reply)
    , m_currentState(ReplyState::Pending)
    , m_cursor(QString())
{
    connect(m_reply, &QNetworkReply::finished, this, &Reply::onFinished, Qt::DirectConnection);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &Reply::downloadProgress, Qt::UniqueConnection);
    connect(m_reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError) {
        m_currentState = ReplyState::Error;
    });
}

Reply::~Reply()
{
}

QVariant &Reply::data()
{
    return m_data;
}

const ReplyState &Reply::currentState() const
{
    return m_currentState;
}

Reply::operator bool() const
{
    return currentState() == ReplyState::Success;
}

const QString &Reply::cursor() const
{
    return m_cursor;
}

RawReply::RawReply(QNetworkReply *reply)
    : Reply(reply)
{
}

RawReply::~RawReply() = default;

JSONReply::JSONReply(QNetworkReply *reply)
    : Reply(reply)
{
}

JSONReply::~JSONReply()
{
}

void ImageReply::parseData(const QByteArray &data)
{
    m_data.setValue(QImage::fromData(data));
}

void RawReply::onFinished()
{
    if (m_currentState != ReplyState::Error) {
        auto data = m_reply->readAll();
        // Check errors
        if (data.isEmpty() || data.isNull()) {
            m_currentState = ReplyState::Error;
        } else {
            m_currentState = ReplyState::Success;
            parseData(data);
        }
    }
    Q_EMIT finished();
    m_reply->setParent(nullptr);
    m_reply->deleteLater();
}

void JSONReply::onFinished()
{
    if (m_currentState != ReplyState::Error) {
        auto reply = m_reply->readAll();
        m_json = QJsonDocument::fromJson(reply.data()).object();
        {
            if (m_json.isEmpty()) {
                m_currentState = ReplyState::Error;
            } else {
                m_currentState = ReplyState::Success;
                parseData(m_json);

                if (m_json.find(u"pagination"_qs) != m_json.end()) {
                    auto pagination = m_json[u"pagination"_qs].toObject();
                    if (pagination.find(u"cursor"_qs) != pagination.end()) {
                        // Save the pagination
                        m_cursor = pagination[u"cursor"_qs].toString();
                    }
                }
            }
        }
    }

    Q_EMIT finished();
    m_reply->setParent(nullptr);
    m_reply->deleteLater();
}

const QJsonObject &Twitch::JSONReply::json() const
{
    return m_json;
}
}

#include "moc_reply.cpp"
