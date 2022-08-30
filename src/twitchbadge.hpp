/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHBADGE_HPP
#define TWITCHBADGE_HPP

#include "twitchqt_export.h"

#include <QString>
#include <QMap>

namespace Twitch {
struct Badge {
    struct Version {
        QString m_imageUrl;
        QString m_description;
        QString m_title;
    };
    QString m_name;
    QMap<QString, Version> m_versions;
};
using Badges = QVector<Badge>;
}

Q_DECLARE_METATYPE(Twitch::Badge);

#endif // TWITCHBADGE_HPP
