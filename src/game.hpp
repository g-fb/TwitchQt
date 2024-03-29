/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHGAME_HPP
#define TWITCHGAME_HPP

#include <QObject>
#include <QString>
#include <QVector>

namespace Twitch
{
struct Game {
    QString m_id;
    QString m_name;
    QString m_boxArtUrl;
};

using Games = QVector<Twitch::Game>;
}

Q_DECLARE_METATYPE(Twitch::Game);
Q_DECLARE_METATYPE(Twitch::Games);

#endif // TWITCHGAME_HPP
