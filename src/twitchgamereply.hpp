#ifndef TWITCHGAMEREPLY_HPP
#define TWITCHGAMEREPLY_HPP

#include "twitchqt_export.h"

#include "twitchreply.hpp"
#include "twitchgame.hpp"

namespace Twitch {
class TWITCHQT_EXPORT GameReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    virtual void parseData(const JSON&) override;
};

class TWITCHQT_EXPORT GamesReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Games games();

protected:
    virtual void parseData(const JSON&) override;
};

class TWITCHQT_EXPORT BoxArtReply : public RawReply {
    Q_OBJECT
public:
    using RawReply::RawReply;

protected:
    virtual void parseData(const QByteArray&) override;
};

}

#endif // TWITCHGAMEREPLY_HPP
