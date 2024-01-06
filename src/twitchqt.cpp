/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#include "twitchqt.hpp"

namespace Twitch
{
Api::Api(QObject *parent)
    : QObject(parent)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID()
{
    resetRateLimit();
    auto diskCache = new QNetworkDiskCache(m_http);
    diskCache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + u"/TCache"_qs);
    m_http->setCache(diskCache);
}

Api::Api(QNetworkAccessManager *http, QObject *parent)
    : Api(parent)
{
    m_http = http;
}

Api::Api(const QString &clientID, QObject *parent)
    : Api(parent)
{
    m_clientID = clientID;
}

Api::Api(const QString &clientID, QNetworkAccessManager *http, QObject *parent)
    : Api(parent)
{
    m_http = http;
    m_clientID = clientID;
}

Api::~Api() = default;

QString Api::api() const
{
    return u"https://api.twitch.tv/helix"_qs;
}

QString Api::emotesApi() const
{
    return u"https://twitchemotes.com/api_cache/v3"_qs;
}

QString Api::ffzApi() const
{
    return u"https://api.frankerfacez.com/v1"_qs;
}

QString Api::bttvApi() const
{
    return u"https://api.betterttv.net/2"_qs;
}

const QString &Api::clientID() const
{
    return m_clientID;
}

void Api::setClientID(const QString &id)
{
    m_clientID = id;
}

const QString &Api::bearerToken() const
{
    return m_bearerToken;
}

void Api::setBearerToken(const QString &bearerToken)
{
    m_bearerToken = bearerToken;
}

int Api::rateLimit() const
{
    return m_rateLimit;
}

int Api::remainingRequests() const
{
    return m_rateRemaining;
}

const QDateTime &Api::resetDate() const
{
    return m_rateResetDate;
}

QString Api::repeatDelimeter(const QString &parameter, const QChar &delimeter) const
{
    return QString(QString(delimeter) + u"{parameter}="_qs).replace(u"{parameter}"_qs, parameter);
}

QNetworkRequest Api::buildRequest(QUrl url, bool includeID, const CacheFlag cacheFlag)
{
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    if (includeID)
        request.setRawHeader("Client-ID", m_clientID.toUtf8());

    request.setRawHeader("Authorization", u"Bearer %1"_qs.arg(bearerToken()).toUtf8());

    switch (cacheFlag) {
    case CacheFlag::UseNetworkDoNotCache:
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
        request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);
        break;
    case CacheFlag::PreferCache:
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
        request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, true);
        break;
    }

    request.setUrl(url);
    return request;
}

#include <type_traits>

template<class T>
T *Api::createReply(const QNetworkRequest &request, bool shouldUpdate)
{
    static_assert(std::is_base_of<Reply, T>::value, " must derive from Twitch::Reply");

    QNetworkReply *requestReply = m_http->get(QNetworkRequest(request));
    T *reply = new T(requestReply);
    reply->setParent(this);

    // Check if it should update rate rate limiting
    if (shouldUpdate) {
        connect(requestReply, &QNetworkReply::metaDataChanged, [this, requestReply]() {
            updateLimits(requestReply);
        });
    }

    return reply;
}

void Api::updateLimits(QNetworkReply *reply)
{
    if (reply->hasRawHeader("RateLimit-Limit"))
        m_rateLimit = reply->rawHeader("RateLimit-Limit").toInt();
    if (reply->hasRawHeader("RateLimit-Remaining"))
        m_rateRemaining = reply->rawHeader("RateLimit-Remaining").toInt();
    if (reply->hasRawHeader("RateLimit-Reset")) {
        auto timestamp = reply->rawHeader("RateLimit-Reset").toUInt();

        m_rateResetDate.setSecsSinceEpoch(timestamp);
    }
}

void Twitch::Api::resetRateLimit()
{
    m_rateLimit = 30;
    m_rateRemaining = 30;
    m_rateResetDate = QDateTime::currentDateTime();
}

// Games
GamesReply *Api::getTopGames(int first, const QString &after)
{
    QString url = api() + u"/games/top?first="_qs + QString::number(first);
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

GameReply *Api::getGameById(const QString &id)
{
    const QUrl url{api() + u"/games?id="_qs + id};
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

GamesReply *Api::getGameByIds(const QStringList &ids)
{
    const QUrl url{api() + u"/games?id="_qs + ids.join(repeatDelimeter(u"id"_qs))};
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

GameReply *Api::getGameByName(const QString &name)
{
    const QUrl url{api() + u"/games?name="_qs + name};
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

BoxArtReply *Api::getBoxArtByUrl(const QString &url, int width, int height)
{
    QString targetUrl = url;
    targetUrl = targetUrl.replace(u"{width}x{height}"_qs, QString::number(width) + u"x"_qs + QString::number(height));
    auto request = buildRequest(QUrl(targetUrl), false, CacheFlag::PreferCache);
    return createReply<BoxArtReply>(request);
}

GamesReply *Api::getGameByNames(const QStringList &names)
{
    const QUrl url{api() + u"/games?name="_qs + names.join(repeatDelimeter(u"name"_qs))};
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

// Streams

StreamReply *Api::getStreamByUserId(const QString &userId)
{
    const QUrl url{api() + u"/streams?user_id="_qs + userId};

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

StreamReply *Api::getStreamByName(const QString &userName)
{
    const QUrl url{api() + u"/streams?user_login="_qs + userName};

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

StreamsReply *Api::getStreamsByNames(const QStringList &names, int first, const QString &after)
{
    QString url = api() + u"/streams?first="_qs + QString::number(first) + u"&user_login="_qs + names.join(u"&user_login="_qs);
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByUserIds(const QStringList &ids, int first, const QString &after)
{
    QString url = api() + u"/streams?first="_qs + QString::number(first) + u"&user_id="_qs + ids.join(u"&user_id="_qs);
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByGameId(const QString &gameId, int first, const QString &after)
{
    QString url = api() + u"/streams?first="_qs + QString::number(first) + u"&game_id="_qs + gameId;
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByGameIds(const QStringList &ids, int first, const QString &after)
{
    QString url = api() + u"/streams?first="_qs + QString::number(first) + u"&game_id="_qs + ids.join(u"&game_id="_qs);
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByLanguage(const QString &language, int first, const QString &after)
{
    QString url = api() + u"/streams?first="_qs + QString::number(first) + u"?language="_qs + language;
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByLanguages(const QStringList &languages, int first, const QString &after)
{
    QString url = api() + u"/streams?first="_qs + QString::number(first) + u"?language="_qs + languages.join(repeatDelimeter(u"language"_qs));
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

// Videos
VideoReply *Api::getVideoById(const QString &id)
{
    const QUrl url{api() + u"/videos?id="_qs + id};

    auto request = buildRequest(QUrl(url));
    return createReply<VideoReply>(request);
}

VideosReply *Api::getVideosByUserId(const QString &id, int first, const QString &after)
{
    QString url = api() + u"/videos?first="_qs + QString::number(first) + u"&user_id="_qs + id;
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<VideosReply>(request);
}

ChannelsReply *Api::getFollowedChannel(const QString &user_id, const QString &broadcaster_id, int first, const QString &after)
{
    QString url = api() + u"/channels/followed?user_id="_qs + user_id;
    if (!broadcaster_id.isEmpty()) {
        url += u"&broadcaster_id="_qs + broadcaster_id;
    }
    url += u"&first="_qs + QString::number(first);
    if (!after.isEmpty()) {
        url += u"&after="_qs + after;
    }

    auto request = buildRequest(QUrl(url));
    return createReply<ChannelsReply>(request);
}

// Users Follows
UserFollowsReply *Api::getUserFollowsFromId(const QString &userId)
{
    const QUrl url{api() + u"/users/follows?from_id="_qs + userId};

    auto request = buildRequest(QUrl(url));
    return createReply<UserFollowsReply>(request);
}

UserFollowsReply *Api::getUserFollowsToId(const QString &userId)
{
    const QUrl url{api() + u"/users/follows?to_id="_qs + userId};

    auto request = buildRequest(QUrl(url));
    return createReply<UserFollowsReply>(request);
}

// User
UserReply *Api::getUserById(const QString &userId)
{
    const QUrl url{api() + u"/users?id="_qs + userId};

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

UserReply *Api::getUserByName(const QString &name)
{
    const QUrl url{api() + u"/users?login="_qs + name};

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

UsersReply *Api::getUserByIds(const QStringList &ids, const QString &after)
{
    QString url = api() + u"/users?id="_qs + ids.join(repeatDelimeter(u"id"_qs));
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

UsersReply *Api::getUserByNames(const QStringList &names, const QString &after)
{
    QString url = api() + u"/users?login="_qs + names.join(repeatDelimeter(u"login"_qs));
    if (!after.isEmpty())
        url += u"&after="_qs + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

// Emotes
EmotesReply *Api::getGlobalEmotes()
{
    const QUrl url{api() + u"/chat/emotes/global"_qs};
    auto request = buildRequest(QUrl(url));
    return createReply<EmotesReply>(request);
}

EmotesReply *Api::getChannelEmotes(const QString &id)
{
    const QUrl url{api() + u"/chat/emotes?broadcaster_id="_qs + id};
    auto request = buildRequest(QUrl(url));
    return createReply<EmotesReply>(request);
}

EmotesReply *Api::getEmoteSets(const QStringList &ids)
{
    const QUrl url{api() + u"/chat/emotes/set?emote_set_id="_qs + ids.join(repeatDelimeter(u"emote_set_id"_qs))};
    auto request = buildRequest(QUrl(url));
    return createReply<EmotesReply>(request);
}

Twitch::GlobalBadgesReply *Twitch::Api::getGlobalBadges()
{
    const QUrl url{api() + u"/chat/badges/global"_qs};
    auto request = buildRequest(QUrl(url));
    return createReply<GlobalBadgesReply>(request);
}

Twitch::ChannelBadgesReply *Twitch::Api::getChannelBadges(const QString &id)
{
    const QUrl url{api() + u"/chat/badges?broadcaster_id="_qs + id};
    auto request = buildRequest(QUrl(url));
    return createReply<GlobalBadgesReply>(request);
}
}

#include "moc_twitchqt.cpp"
