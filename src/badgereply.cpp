/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "badgereply.hpp"

#include <QJsonArray>

namespace Twitch
{
void GlobalBadgesReply::parseData(const QJsonObject &json)
{
    Twitch::Badges badges;
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        for (const auto &badge : data) {
            QVector<Badge::Version> versions;
            const auto &badgeVersions = badge.toObject()[u"versions"_qs].toArray();
            for (const auto &versionObject : badgeVersions) {
                const auto &version = versionObject.toObject();
                versions.append(Badge::Version{
                    version[u"id"_qs].toString(),
                    version[u"image_url_1x"_qs].toString(),
                    version[u"image_url_2x"_qs].toString(),
                    version[u"image_url_4x"_qs].toString(),
                });
            }
            badges.push_back(Twitch::Badge{badge[u"set_id"_qs].toString(), versions});
        }
    }
    m_data.setValue(badges);
}

Twitch::Badges Twitch::GlobalBadgesReply::badges()
{
    return m_data.value<Twitch::Badges>();
}
}

#include "moc_badgereply.cpp"
