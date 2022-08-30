#ifndef TWITCHUSERREPLY_HPP
#define TWITCHUSERREPLY_HPP

#include "twitchqt_export.h"

#include "twitchreply.hpp"
#include "twitchuser.hpp"
#include <iostream>

namespace Twitch {
class TWITCHQT_EXPORT UserReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::User user();

protected:
    virtual void parseData(const JSON&) override;
};

class TWITCHQT_EXPORT UsersReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Users users();

protected:
    virtual void parseData(const JSON&) override;
};

class TWITCHQT_EXPORT UserFollowsReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::UserFollows userFollows();

protected:
    virtual void parseData(const JSON&) override;
};

}

#endif // TWITCHUSERREPLY_HPP
