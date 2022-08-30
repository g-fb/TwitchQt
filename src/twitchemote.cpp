#include "twitchemote.hpp"

namespace Twitch {
template <typename... Args>
Emote Emote::createEmoteImpl(EmoteTag<TwitchEmotes::Emote>, Args&&... args)
{
    Emote emote;
    auto emoteData = QSharedPointer<TwitchEmotes::Emote>::create(std::forward<Args>(args)...);
    emote.m_data = emoteData;
    emote.m_type = EmoteType::TwitchEmotes;
    emote.m_id = QString::number(emoteData->m_id);
    emote.m_code = emoteData->m_code;
    emote.m_url = TwitchEmotes::Emote::urlTemplate().replace("{{id}}", emote.m_id);
    emote.m_imageType = ImageType::PNG;
    return emote;
}

template <typename... Args>
Emote Emote::createEmoteImpl(EmoteTag<BTTV::Emote>, Args&&... args)
{
    Emote emote;
    auto emoteData = QSharedPointer<BTTV::Emote>::create(std::forward<Args>(args)...);
    emote.m_data = emoteData;
    emote.m_type = EmoteType::BTTV;
    emote.m_id = emoteData->m_id;
    emote.m_code = emoteData->m_code;
    emote.m_url = BTTV::Emote::urlTemplate().replace("{{id}}", emote.m_id);
    emote.m_imageType = emoteData->m_imageType.toUpper() == "PNG" ? ImageType::PNG : ImageType::GIF;
    return emote;
}

template <typename... Args>
Emote Emote::createEmoteImpl(EmoteTag<FFZ::Emote>, Args&&... args)
{
    Emote emote;
    auto emoteData = QSharedPointer<FFZ::Emote>::create(std::forward<Args>(args)...);
    emote.m_data = emoteData;
    emote.m_type = EmoteType::FFZ;
    emote.m_id = QString::number(emoteData->m_id);
    emote.m_code = emoteData->m_name;
    emote.m_url = FFZ::Emote::urlTemplate().replace("{{id}}", emote.m_id);
    emote.m_imageType = ImageType::PNG;
    return emote;
}

template <class EmoteClass, typename... Args>
Emote Emote::createEmote(Args&&... args)
{
    return createEmoteImpl(EmoteTag<EmoteClass>{}, std::forward<Args>(args)...);
}

Emote::Emote()
    : m_data(nullptr)
{
}

Emote::Emote(EmoteData* data)
    : m_data(data)
{
}

Emote::Emote(const Emote& other)
    : m_type(other.m_type)
    , m_data(other.m_data)
    , m_id(other.m_id)
    , m_code(other.m_code)
    , m_url(other.m_url)
    , m_imageType(other.m_imageType)
{
}

Emote::Emote(Emote&& other)
    : m_type(other.m_type)
    , m_data(std::move(other.m_data))
    , m_id(other.m_id)
    , m_code(other.m_code)
    , m_url(other.m_url)
    , m_imageType(other.m_imageType)
{
}

Emote& Emote::operator=(const Emote& other)
{
    if (&other == this)
        return *this;

    m_type = other.m_type;
    m_data = other.m_data;
    m_id = other.m_id;
    m_code = other.m_code;
    m_url = other.m_url;
    m_imageType = other.m_imageType;

    return *this;
}

Emote& Emote::operator=(Emote&& other)
{
    if (&other == this)
        return *this;

    m_type = other.m_type;
    m_id = other.m_id;
    m_code = other.m_code;
    m_url = other.m_url;
    m_imageType = other.m_imageType;
    m_data = std::move(other.m_data);

    return *this;
}

Emote::~Emote() = default;

const EmoteType& Emote::emoteType() const
{
    return m_type;
}

const QString& Emote::id() const
{
    return m_id;
}

const QString& Emote::code() const
{
    return m_code;
}

const QString& Emote::url() const
{
    return m_url;
}

const Emote::ImageType& Emote::imageType() const
{
    return m_imageType;
}

template <class EmoteClass>
const EmoteClass& Emote::toEmote() const
{
    // Maybe some kind of const cast?
    return *static_cast<EmoteClass*>(m_data.data());
}

Twitch::Emotes Twitch::Emotes::fromTwitchEmotes(const JSON& json)
{
    Twitch::Emotes emotes;
    for (const auto& emote : json)
        emotes.push_back(Twitch::Emote::createEmote<TwitchEmotes::Emote>(emote));
    return emotes;
}

Twitch::Emotes Twitch::Emotes::fromBTTV(const JSON& json)
{
    Twitch::Emotes emotes;
    for (const auto& emote : json)
        emotes.push_back(Twitch::Emote::createEmote<Twitch::BTTV::Emote>(emote));
    return emotes;
}

Twitch::Emotes Twitch::Emotes::fromFFZ(const JSON& json)
{
    Twitch::Emotes emotes;
    for (const auto& emote : json)
        emotes.push_back(Twitch::Emote::createEmote<FFZ::Emote>(emote));
    return emotes;
}
}
