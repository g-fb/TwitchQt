/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2023 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "channelreply.hpp"

#include <QJsonArray>

Twitch::Channel channelFromJson(const QJsonObject &channel)
{
    return Twitch::Channel{
        channel[QStringLiteral("broadcaster_id")].toString(),
        channel[QStringLiteral("broadcaster_login")].toString(),
        channel[QStringLiteral("broadcaster_name")].toString(),
        channel[QStringLiteral("followed_at")].toString(),
    };
}

namespace Twitch
{
void ChannelReply::parseData(const QJsonObject &json)
{
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
        if (!data.isEmpty()) {
            m_data.setValue(channelFromJson(data.first().toObject()));
        }
    }
}

void ChannelsReply::parseData(const QJsonObject &json)
{
    Channels channels;
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
        for (const auto &user : data) {
            channels.push_back(channelFromJson(user.toObject()));
        }
    }
    m_data.setValue(channels);
}

Twitch::Channel Twitch::ChannelReply::channel()
{
    return m_data.value<Twitch::Channel>();
}

Twitch::Channels Twitch::ChannelsReply::channels()
{
    return m_data.value<Twitch::Channels>();
}
}

#include "moc_channelreply.cpp"
