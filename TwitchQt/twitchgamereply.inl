
inline void GameReply::parseData(const QJsonDocument& json)
{
    auto&& root = json.object();
    if (root.contains("data")) {
        auto&& data = root.value("data").toArray();
        if (!data.isEmpty()) {
            auto&& game = data.first().toObject();
            QString id = game.value("id").toString();
            QString name = game.value("name").toString();
            QString boxArtUrl = game.value("box_art_url").toString();
            m_data.setValue(Game{
                id.toULongLong(),
                name,
                boxArtUrl });
        } else {
            // ??
        }
    }
}

inline void GamesReply::parseData(const QJsonDocument& json)
{
    Games games;
    auto&& root = json.object();
    if (root.contains("data")) {
        auto&& data = root.value("data").toArray();
        for (const auto& gameElement : data) {
            auto&& game = gameElement.toObject();
            QString id = game.value("id").toString();
            QString name = game.value("name").toString();
            QString boxArtUrl = game.value("box_art_url").toString();
            games.push_back({ id.toULongLong(),
                name,
                boxArtUrl });
        }
    }
    m_data.setValue(games);
}

inline void BoxArtReply::parseData(const QByteArray& data)
{
    m_data.setValue(QImage::fromData(data));
}