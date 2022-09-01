/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#ifndef TWITCHVIDEOREPLY_HPP
#define TWITCHVIDEOREPLY_HPP

#include "twitchqt_export.h"

#include "twitchreply.hpp"
#include "twitchvideo.hpp"

namespace Twitch
{
class TWITCHQT_EXPORT VideoReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;

    Twitch::Video video();

protected:
    void parseData(const QJsonObject &) override;
};

class TWITCHQT_EXPORT VideosReply : public JSONReply
{
    Q_OBJECT
public:
    using JSONReply::JSONReply;
    int combinedViewerCount() const;

    Twitch::Videos videos();

protected:
    int m_combinedViewerCount{0};
    void parseData(const QJsonObject &) override;
};

}

#endif // TWITCHVIDEOREPLY_HPP
