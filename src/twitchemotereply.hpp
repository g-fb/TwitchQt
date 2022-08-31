/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHEMOTEREPLY_HPP
#define TWITCHEMOTEREPLY_HPP

#include "twitchqt_export.h"

#include "twitchreply.hpp"
#include "twitchemote.hpp"

namespace Twitch {
class TWITCHQT_EXPORT EmotesReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;
    virtual ~EmotesReply() = default;
    Twitch::Emotes emotes();

protected:
    virtual void parseData(const QJsonObject&) override;
};
}

#endif // TWITCHEMOTEREPLY_HPP
