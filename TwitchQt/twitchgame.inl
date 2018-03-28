
inline TopGamesReply::TopGamesReply(QNetworkReply* reply) :
    Reply(reply)
{

}

inline void TopGamesReply::parseData(const QJsonDocument& json)
{
    Games topGames;
    auto&& root = json.object();
    if(root.contains("data"))
    {
        auto&& data = root.value("data").toArray();
        for(const auto& gameElement : data)
        {
            auto&& game = gameElement.toObject();
            int id = game.value("id").toInt();
            QString name = game.value("name").toString();
            QString boxArtUrl = game.value("box_art_url").toString();
            topGames.push_back({
                id,
                name,
                boxArtUrl
            });
        }
    }
    m_data.setValue(topGames);
}


inline void Twitch::GameReply::parseData(const QJsonDocument &json)
{
    auto&& root = json.object();
    if(root.contains("data"))
    {
        auto&& data = root.value("data").toArray();
        for(const auto& gameElement : data)
        {
            auto&& game = gameElement.toObject();
            int id = game.value("id").toInt();
            QString name = game.value("name").toString();
            QString boxArtUrl = game.value("box_art_url").toString();
            m_data.setValue(Game{
                id,
                name,
                boxArtUrl
            });
        }
    }
}
