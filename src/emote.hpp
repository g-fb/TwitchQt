/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHEMOTE_HPP
#define TWITCHEMOTE_HPP

#include <QMap>
#include <QObject>
#include <QSharedPointer>

namespace Twitch
{
struct Emote {
    QString m_id;
    QString m_name;
    QMap<QString, QVariant> m_images{};
    QList<QVariant> m_format{};
    QList<QVariant> m_scale{};
    QList<QVariant> m_themeMode{};
    QString m_template;
};

using Emotes = QMap<QString, Emote>;
}

Q_DECLARE_METATYPE(Twitch::Emote)
Q_DECLARE_METATYPE(Twitch::Emotes)

#endif // TWITCHEMOTE_HPP
