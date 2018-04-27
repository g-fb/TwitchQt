#ifndef TWITCHSTREAMREPLY_HPP
#define TWITCHSTREAMREPLY_HPP

#include "twitchreply.hpp"
#include "twitchstream.hpp"

Q_DECLARE_METATYPE(Twitch::Stream);
Q_DECLARE_METATYPE(Twitch::Streams);

namespace Twitch {
class StreamReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;

protected:
    virtual void parseData(const QJsonDocument&) override;
};

class StreamsReply : public JSONReply {
    Q_OBJECT
public:
    using JSONReply::JSONReply;
    qulonglong combinedViewerCount() const;
    const QString& cursor() const;

protected:
    qulonglong m_combinedViewerCount{ 0u };
    QString m_cursor;
    virtual void parseData(const QJsonDocument&) override;
};

#include "twitchstreamreply.inl"
}

#endif // TWITCHSTREAMREPLY_HPP
