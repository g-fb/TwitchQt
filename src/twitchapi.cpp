/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#include "twitchapi.hpp"

namespace Twitch
{
Api::Api(QObject *parent)
    : QObject(parent)
    , m_http(new QNetworkAccessManager(this))
    , m_clientID()
{
    resetRateLimit();
    auto diskCache = new QNetworkDiskCache(m_http);
    diskCache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/TCache");
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
    return QString("https://api.twitch.tv/helix");
}

QString Api::emotesApi() const
{
    return QString("https://twitchemotes.com/api_cache/v3");
}

QString Api::ffzApi() const
{
    return QString("https://api.frankerfacez.com/v1");
}

QString Api::bttvApi() const
{
    return QString("https://api.betterttv.net/2");
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
    return QString(QString(delimeter) + "{parameter}=").replace("{parameter}", parameter);
}

QNetworkRequest Api::buildRequest(QUrl url, bool includeID, const CacheFlag cacheFlag)
{
    QNetworkRequest request;
    request.setRawHeader("User-Agent", "Twitch.Qt");
    if (includeID)
        request.setRawHeader("Client-ID", m_clientID.toUtf8());

    request.setRawHeader("Authorization", QString("Bearer %1").arg(bearerToken()).toUtf8());

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_rateResetDate.setTime_t(timestamp);
#else
        m_rateResetDate.setSecsSinceEpoch(timestamp);
#endif
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
    QString url = api() + QString("/games/top") + QString("?first=") + QString::number(first);
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(url);
    return createReply<GamesReply>(request);
}

GameReply *Api::getGameById(const QString &id)
{
    const QUrl url = api() + QString("/games") + QString("?id=") + id;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

GamesReply *Api::getGameByIds(const QStringList &ids)
{
    const QUrl url = api() + QString("/games") + QString("?id=") + ids.join(repeatDelimeter("id"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

GameReply *Api::getGameByName(const QString &name)
{
    const QUrl url = api() + QString("/games") + QString("?name=") + name;
    auto request = buildRequest(QUrl(url));
    return createReply<GameReply>(request);
}

BoxArtReply *Api::getBoxArtByUrl(const QString &url, int width, int height)
{
    QString targetUrl = url;
    targetUrl = targetUrl.replace("{width}x{height}", QString::number(width) + "x" + QString::number(height));
    auto request = buildRequest(QUrl(targetUrl), false, CacheFlag::PreferCache);
    return createReply<BoxArtReply>(request);
}

GamesReply *Api::getGameByNames(const QStringList &names)
{
    const QUrl url = api() + QString("/games") + QString("?name=") + names.join(repeatDelimeter("name"));
    auto request = buildRequest(QUrl(url));
    return createReply<GamesReply>(request);
}

// Streams

StreamReply *Api::getStreamByUserId(const QString &userId)
{
    const QUrl url = api() + QString("/streams") + QString("?user_id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

StreamReply *Api::getStreamByName(const QString &userName)
{
    const QUrl url = api() + QString("/streams") + QString("?user_login=") + userName;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamReply>(request);
}

StreamsReply *Api::getStreamsByNames(const QStringList &names, int first, const QString &after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&user_login=") + names.join("&user_login=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByUserIds(const QStringList &ids, int first, const QString &after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&user_id=") + ids.join("&user_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByGameId(const QString &gameId, int first, const QString &after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&game_id=") + gameId;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByGameIds(const QStringList &ids, int first, const QString &after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("&game_id=") + ids.join("&game_id=");
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByLanguage(const QString &language, int first, const QString &after)
{
    QString url = api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("?language=") + language;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

StreamsReply *Api::getStreamsByLanguages(const QStringList &languages, int first, const QString &after)
{
    QString url =
        api() + QString("/streams") + QString("?first=") + QString::number(first) + QString("?language=") + languages.join(repeatDelimeter("language"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<StreamsReply>(request);
}

// Videos
VideoReply *Api::getVideoById(const QString &id)
{
    const QUrl url = api() + QString("/videos") + QString("?id=") + id;

    auto request = buildRequest(QUrl(url));
    return createReply<VideoReply>(request);
}

VideosReply *Api::getVideosByUserId(const QString &id, int first, const QString &after)
{
    QString url = api() + QString("/videos") + QString("?first=") + QString::number(first) + QString("&user_id=") + id;
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<VideosReply>(request);
}

// Users Follows
UserFollowsReply *Api::getUserFollowsFromId(const QString &userId)
{
    const QUrl url = api() + QString("/users/follows") + QString("?from_id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<UserFollowsReply>(request);
}

UserFollowsReply *Api::getUserFollowsToId(const QString &userId)
{
    const QUrl url = api() + QString("/users/follows") + QString("?to_id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<UserFollowsReply>(request);
}

// User
UserReply *Api::getUserById(const QString &userId)
{
    const QUrl url = api() + QString("/users") + QString("?id=") + userId;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

UserReply *Api::getUserByName(const QString &name)
{
    const QUrl url = api() + QString("/users") + QString("?login=") + name;

    auto request = buildRequest(QUrl(url));
    return createReply<UserReply>(request);
}

UsersReply *Api::getUserByIds(const QStringList &ids, const QString &after)
{
    QString url = api() + QString("/users") + QString("?id=") + ids.join(repeatDelimeter("id"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

UsersReply *Api::getUserByNames(const QStringList &names, const QString &after)
{
    QString url = api() + QString("/users") + QString("?login=") + names.join(repeatDelimeter("login"));
    if (!after.isEmpty())
        url += QString("&after=") + after;

    auto request = buildRequest(QUrl(url));
    return createReply<UsersReply>(request);
}

// Emotes
EmotesReply *Api::getGlobalEmotes()
{
    const QUrl url = api() + QString("/chat/emotes/global");
    auto request = buildRequest(QUrl(url));
    return createReply<EmotesReply>(request);
}

EmotesReply *Api::getChannelEmotes(const QString &id)
{
    const QUrl url = api() + QString("/chat/emotes") + QString("?broadcaster_id=") + id;
    auto request = buildRequest(QUrl(url));
    return createReply<EmotesReply>(request);
}

EmotesReply *Api::getEmoteSets(const QStringList &ids)
{
    const QUrl url = api() + QString("/chat/emotes/set") + QString("?emote_set_id=") + ids.join(repeatDelimeter("emote_set_id"));
    auto request = buildRequest(QUrl(url));
    return createReply<EmotesReply>(request);
}

Twitch::GlobalBadgesReply *Twitch::Api::getGlobalBadges()
{
    const QUrl url = api() + QString("/chat/badges/global");
    auto request = buildRequest(QUrl(url));
    return createReply<GlobalBadgesReply>(request);
}

Twitch::ChannelBadgesReply *Twitch::Api::getChannelBadges(const QString &id)
{
    const QUrl url = api() + QString("/chat/badges") + QString("?broadcaster_id=") + id;
    auto request = buildRequest(QUrl(url));
    return createReply<GlobalBadgesReply>(request);
}
}
