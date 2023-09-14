/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2023 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHCHANNELREPLY_HPP
#define TWITCHCHANNELREPLY_HPP

#include "twitchqt_export.h"

#include "reply.hpp"
#include <iostream>

namespace Twitch
{
struct Channel {
    QString broadcasterId;
    QString broadcasterLogin;
    QString broadcasterName;
    QString followedAt;
};
using Channels = QVector<Channel>;

class TWITCHQT_EXPORT ChannelReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Channel channel();

protected:
    virtual void parseData(const QJsonObject &) override;
};

class TWITCHQT_EXPORT ChannelsReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Channels channels();

protected:
    void parseData(const QJsonObject &) override;
};

}

Q_DECLARE_METATYPE(Twitch::Channel)
Q_DECLARE_METATYPE(Twitch::Channels)

#endif // TWITCHCHANNELREPLY_HPP
