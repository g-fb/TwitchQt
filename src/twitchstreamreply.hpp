/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHSTREAMREPLY_HPP
#define TWITCHSTREAMREPLY_HPP

#include "twitchqt_export.h"

#include "twitchreply.hpp"
#include "twitchstream.hpp"

namespace Twitch {
class TWITCHQT_EXPORT StreamReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Stream stream();

protected:
    virtual void parseData(const JSON&) override;
};

class TWITCHQT_EXPORT StreamsReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;
    int combinedViewerCount() const;

    Twitch::Streams streams();

protected:
    int m_combinedViewerCount{ 0 };
    virtual void parseData(const JSON&) override;
};

}

#endif // TWITCHSTREAMREPLY_HPP
