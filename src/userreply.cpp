/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "userreply.hpp"

#include <QJsonArray>

Twitch::User userFromJson(const QJsonObject &user)
{
    QString broadcasterTypeStr = user[u"broadcaster_type"_qs].toString();
    Twitch::User::BroadcasterType broadcasterType = Twitch::User::BroadcasterType::No;
    if (broadcasterTypeStr == u"partner"_qs) {
        broadcasterType = Twitch::User::BroadcasterType::Partner;
    } else {
        broadcasterType = Twitch::User::BroadcasterType::Affiliate;
    }

    QString userTypeStr = user[u"type"_qs].toString();
    Twitch::User::UserType userType = Twitch::User::UserType::No;
    if (userTypeStr == u"global_mod"_qs) {
        userType = Twitch::User::UserType::GlobalMod;
    } else if (userTypeStr == u"admin"_qs) {
        userType = Twitch::User::UserType::Admin;
    } else {
        userType = Twitch::User::UserType::Staff;
    }

    return Twitch::User{broadcasterType,
                        user[u"description"_qs].toString(),
                        user[u"display_name"_qs].toString(),
                        user[u"email"_qs].toString(),
                        user[u"id"_qs].toString(u"-1"_qs),
                        user[u"login"_qs].toString(),
                        user[u"offline_image_url"_qs].toString(),
                        user[u"profile_image_url"_qs].toString(),
                        userType,
                        user[u"view_count"_qs].toInt(-1)};
}

namespace Twitch
{
void UserReply::parseData(const QJsonObject &json)
{
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        if (!data.isEmpty()) {
            m_data.setValue(userFromJson(data.first().toObject()));
        }
    }
}

void UsersReply::parseData(const QJsonObject &json)
{
    Users users;
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        for (const auto &user : data) {
            users.push_back(userFromJson(user.toObject()));
        }
    }
    m_data.setValue(users);
}

void UserFollowsReply::parseData(const QJsonObject &json)
{
    UserFollows userFollows;
    userFollows.m_total = json[u"total"_qs].toInt();
    if (json.find(u"data"_qs) != json.end()) {
        const auto &data = json[u"data"_qs].toArray();
        for (const auto &follow : data) {
            auto followObject = follow.toObject();
            userFollows.m_follows.push_back(Follow{
                followObject[u"from_id"_qs].toString(),
                followObject[u"from_name"_qs].toString(),
                followObject[u"to_id"_qs].toString(),
                followObject[u"to_name"_qs].toString(),
                followObject[u"followed_at"_qs].toString(),
            });
        }
    }
    m_data.setValue(userFollows);
}

Twitch::User Twitch::UserReply::user()
{
    return m_data.value<Twitch::User>();
}

Twitch::Users Twitch::UsersReply::users()
{
    return m_data.value<Twitch::Users>();
}

Twitch::UserFollows Twitch::UserFollowsReply::userFollows()
{
    return m_data.value<Twitch::UserFollows>();
}
}

#include "moc_userreply.cpp"
