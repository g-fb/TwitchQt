/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHGAMEREPLY_HPP
#define TWITCHGAMEREPLY_HPP

#include "twitchqt_export.h"

#include "game.hpp"
#include "reply.hpp"

namespace Twitch
{
class TWITCHQT_EXPORT GameReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    void parseData(const QJsonObject &) override;
};

class TWITCHQT_EXPORT GamesReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Games games();

protected:
    void parseData(const QJsonObject &) override;
};

class TWITCHQT_EXPORT BoxArtReply : public RawReply
{
    Q_OBJECT
public:
    using RawReply::RawReply;

protected:
    void parseData(const QByteArray &) override;
};

}

#endif // TWITCHGAMEREPLY_HPP
