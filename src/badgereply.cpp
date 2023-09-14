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
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
        for (const auto &badge : data) {
            QVector<Badge::Version> versions;
            const auto &badgeVersions = badge.toObject()["versions"].toArray();
            for (const auto &versionObject : badgeVersions) {
                const auto &version = versionObject.toObject();
                versions.append(Badge::Version{
                    version["id"].toString(),
                    version["image_url_1x"].toString(),
                    version["image_url_2x"].toString(),
                    version["image_url_4x"].toString(),
                });
            }
            badges.push_back(Twitch::Badge{badge["set_id"].toString(), versions});
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
