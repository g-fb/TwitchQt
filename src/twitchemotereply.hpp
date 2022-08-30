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
    virtual void parseData(const JSON&) override = 0;
};

namespace TwitchEmotes {
    class TWITCHQT_EXPORT GlobalEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class TWITCHQT_EXPORT SubscriberEmotesReply : public JSONReply {
        Q_OBJECT
    public:
        using JSONReply::JSONReply;
        Twitch::EmotesMap emotes();

    protected:
        virtual void parseData(const JSON&) override;
    };
}

namespace BTTV {
    class TWITCHQT_EXPORT GlobalEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class TWITCHQT_EXPORT SubscriberEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

namespace FFZ {
    class TWITCHQT_EXPORT GlobalEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };

    class TWITCHQT_EXPORT SubscriberEmotesReply : public EmotesReply {
        Q_OBJECT
    public:
        using EmotesReply::EmotesReply;

    protected:
        virtual void parseData(const JSON&) override;
    };
}

class TWITCHQT_EXPORT EmoteSetsReply : public EmotesReply {
    Q_OBJECT
public:
    using EmotesReply::EmotesReply;

protected:
    virtual void parseData(const JSON&) override;
};

}

#endif // TWITCHEMOTEREPLY_HPP
