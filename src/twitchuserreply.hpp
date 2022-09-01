/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHUSERREPLY_HPP
#define TWITCHUSERREPLY_HPP

#include "twitchqt_export.h"

#include "twitchreply.hpp"
#include "twitchuser.hpp"
#include <iostream>

namespace Twitch
{
class TWITCHQT_EXPORT UserReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::User user();

protected:
    virtual void parseData(const QJsonObject &) override;
};

class TWITCHQT_EXPORT UsersReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Users users();

protected:
    void parseData(const QJsonObject &) override;
};

class TWITCHQT_EXPORT UserFollowsReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::UserFollows userFollows();

protected:
    void parseData(const QJsonObject &) override;
};

}

#endif // TWITCHUSERREPLY_HPP
