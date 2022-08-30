#include "twitchemotereply.hpp"

namespace Twitch {
void TwitchEmotes::GlobalEmotesReply::parseData(const JSON& json)
{
    m_data.setValue(Emotes::fromTwitchEmotes(json));
}

void TwitchEmotes::SubscriberEmotesReply::parseData(const JSON& json)
{
    Twitch::EmotesMap emotes;
    for (const auto& user : json) {
        const QString id = user.value("channel_id", QString(""));
        if (user.find("emotes") != user.end() && !id.isEmpty()) {
            Twitch::Emotes userEmotes = Emotes::fromTwitchEmotes(user["emotes"]);
            for (const auto& emote : userEmotes) {
                emotes.insert(id.toStdString(), emote);
            }
        }
    }
    m_data.setValue(emotes);
}

void BTTV::GlobalEmotesReply::parseData(const JSON& json)
{
    if (json.find("status") != json.end() && json["status"].get<int>() != 200)
        return;

    auto&& emotesArray = json["emotes"];
    m_data.setValue(Emotes::fromBTTV(emotesArray));
}

void BTTV::SubscriberEmotesReply::parseData(const JSON& json)
{
    Twitch::Emotes emotes;

    if (json.find("status") != json.end() && json["status"].get<int>() != 200) {
        m_currentState = ReplyState::Error;
        return;
    }

    auto&& emotesArray = json["emotes"];
    m_data.setValue(Emotes::fromBTTV(emotesArray));
}

void FFZ::GlobalEmotesReply::parseData(const JSON& json)
{
    Twitch::Emotes emotes;

    QVector<int> defaultSets;
    auto&& setsArray = json["default_sets"];
    for (const auto& set : setsArray)
        defaultSets.push_back(set.get<int>());

    auto&& sets = json["sets"];
    for (const auto& set : sets) {
        auto&& emotesArray = set["emoticons"];
        emotes << Emotes::fromFFZ(emotesArray);
    }

    m_data.setValue(emotes);
}

void FFZ::SubscriberEmotesReply::parseData(const JSON& json)
{
    if (json.find("error") != json.end()) {
        m_currentState = ReplyState::Error;
        return;
    }

    Twitch::Emotes emotes;
    auto&& sets = json["sets"];
    for (const auto& set : sets) {
        auto&& emotesArray = set["emoticons"];
        emotes << Emotes::fromFFZ(emotesArray);
    }

    m_data.setValue(emotes);
}

Twitch::Emotes EmotesReply::emotes()
{
    return m_data.value<Twitch::Emotes>();
}

Twitch::EmotesMap TwitchEmotes::SubscriberEmotesReply::emotes()
{
    return m_data.value<Twitch::EmotesMap>();
}

void Twitch::EmoteSetsReply::parseData(const JSON& json)
{
    QMap<QString, Emotes> sets;
    for (const auto& set : json["emoticon_sets"].get<JSON::object_t>())
        sets[QString::fromStdString(set.first)] = Emotes::fromTwitchEmotes(set.second);
    m_data.setValue(sets);
}
}
