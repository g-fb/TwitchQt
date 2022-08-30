/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHBADGEREPLY_HPP
#define TWITCHBADGEREPLY_HPP

#include "twitchqt_export.h"

#include "twitchreply.hpp"
#include "twitchbadge.hpp"

namespace Twitch {
class TWITCHQT_EXPORT GlobalBadgesReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Badges badges();

protected:
    virtual void parseData(const JSON&) override;
};

// Pretty much the same algorithm for parsing the json document
using ChannelBadgesReply = GlobalBadgesReply;

}

#endif // TWITCHBADGEREPLY_HPP
