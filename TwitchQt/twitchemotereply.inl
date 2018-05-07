
inline void TwitchEmotes::EmotesReply::parseData(const QJsonDocument& json)
{
    QVector<TwitchEmotes::Emote> emotes;
    auto&& root = json.object();
    for (const auto& emoteObject : root) {
        auto&& emoteData = emoteObject.toObject();
        QString code = emoteData.value("code").toString();
        QString description = emoteData.value("description").toString();
        int emoticonSet = emoteData.value("emoticon_set").toInt();
        qulonglong id = emoteData.value("id").toString().toULongLong();
        emotes.push_back({
            id,
            code,
            emoticonSet,
            description
        });
    }
    m_data.setValue(emotes);
}

inline void BTTV::EmotesReply::parseData(const QJsonDocument& json)
{
    auto&& root = json.object();
    if(root.contains("status") && root.value("status").toInt() != 200)
        return;

    QVector<BTTV::Emote> emotes;
    auto&& emotesObject = root.value("emotes").toArray();
    for(const auto& emoteValue : emotesObject)
    {
        auto&& emote = emoteValue.toObject();
        QString id = emote.value("id").toString();
        QString code = emote.value("code").toString();
        QString channel = emote.value("channel").toString();
        // TODO restrictions
        Restrictions restrictions;
        QString imageType = emote.value("imageType").toString();

        emotes.push_back(BTTV::Emote{
            id,
            code,
            channel,
            restrictions,
            imageType
        });
    }
}

inline void FFZ::EmotesReply::parseData(const QJsonDocument& json)
{
    auto&& root = json.object();
    QVector<int> defaultSets;
    auto&& defaultSetsObject = root.value("default_sets").toObject();
    for(const auto& defaultSet : defaultSetsObject)
        defaultSets.push_back(defaultSet.toInt());

    FFZ::Emotes emotes;
    auto&& sets = root.value("sets").toObject();
    for(const auto& setValue : defaultSets)
    {
        auto&& set = sets.value(QString::number(setValue)).toObject();
        auto&& emoticons = set.value("emoticons").toArray();
        for(const auto& emoteObject : emoticons)
        {
            auto&& emote = emoteObject.toObject();
            QString css = emote.value("css").toString();
            int height = emote.value("height").toInt();
            bool hidden = emote.value("hidden").toBool();
            QString id = emote.value("id").toString();
            int margins = emote.value("margins").toInt();
            bool modifier = emote.value("modifier").toBool();
            QString name = emote.value("name").toString();
            int offset = emote.value("offset").toInt();
            auto&& ownerObject = emote.value("owner").toObject();
            qulonglong ownerID = ownerObject.value("_id").toString().toULongLong();
            QString ownerDisplayName = ownerObject.value("display_name").toString();
            QString ownerName = ownerObject.value("name").toString();
            Owner owner{
                ownerID,
                ownerDisplayName,
                ownerName
            };
            bool isPublic = emote.value("public").toBool();
            QVector<QString> urls;
            auto&& urlObject = emote.value("urls").toObject();
            for(const auto& url : urlObject)
                urls.push_back(url.toString());
            int width = emote.value("width").toInt();

            emotes.push_back(FFZ::Emote{
                css,
                height,
                hidden,
                id,
                margins,
                modifier,
                name,
                offset,
                owner,
                isPublic,
                urls,
                width
            });
        }
    }

    m_data.setValue(emotes);
}

inline void EmoteImageReply::parseData(const QByteArray &data)
{
    m_data.setValue(QImage::fromData(data));
}

inline Twitch::Emote TwitchEmotes::EmotesReply::toEmote()
{
    return Twitch::Emote::fromEmote(m_data.value<TwitchEmotes::Emote>());
}

inline Twitch::Emote BTTV::EmotesReply::toEmote()
{
    return Twitch::Emote::fromEmote(m_data.value<BTTV::Emote>());
}

inline Twitch::Emote FFZ::EmotesReply::toEmote()
{
    return Twitch::Emote::fromEmote(m_data.value<FFZ::Emote>());
}

