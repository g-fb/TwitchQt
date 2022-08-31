/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "twitchbadgereply.hpp"

#include <QJsonArray>

namespace Twitch {
void GlobalBadgesReply::parseData(const QJsonObject &json)
{
    Twitch::Badges badges;
    if (json.find("badge_sets") != json.end()) {
        const auto& badgeSets = json["badge_sets"].toArray();
        for (const auto& badge : badgeSets) {
            QMap<QString, Badge::Version> versions;
            const auto& badgeVersions = badge.toObject()["versions"].toArray();
            for (const auto& versionObject : badgeVersions) {
                const auto& version = versionObject.toObject();
                versions[version.begin().key()] = Badge::Version {
                        version["image_url_1x"].toString(),
                        version["description"].toString(),
                        version["title"].toString(),
                };
            }
            badges.push_back(Twitch::Badge {
                badge.toObject().begin().key(),
                versions });
        }
    }
    m_data.setValue(badges);
}

Twitch::Badges Twitch::GlobalBadgesReply::badges()
{
    return m_data.value<Twitch::Badges>();
}
}
