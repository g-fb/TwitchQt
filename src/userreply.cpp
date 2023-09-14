/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus
    SPDX-FileCopyrightText: 2022 George Florea Bănuș <georgefb899@gmail.com>

    SPDX-License-Identifier: MIT
*/

#include "userreply.hpp"

#include <QJsonArray>

Twitch::User userFromJson(const QJsonObject &user)
{
    QString broadcasterTypeStr = user["broadcaster_type"].toString();
    Twitch::User::BroadcasterType broadcasterType = Twitch::User::BroadcasterType::No;
    if (broadcasterTypeStr == "partner") {
        broadcasterType = Twitch::User::BroadcasterType::Partner;
    } else {
        broadcasterType = Twitch::User::BroadcasterType::Affiliate;
    }

    QString userTypeStr = user["type"].toString();
    Twitch::User::UserType userType = Twitch::User::UserType::No;
    if (userTypeStr == "global_mod") {
        userType = Twitch::User::UserType::GlobalMod;
    } else if (userTypeStr == "admin") {
        userType = Twitch::User::UserType::Admin;
    } else {
        userType = Twitch::User::UserType::Staff;
    }

    return Twitch::User{broadcasterType,
                        user["description"].toString(),
                        user["display_name"].toString(),
                        user["email"].toString(),
                        user["id"].toString("-1"),
                        user["login"].toString(),
                        user["offline_image_url"].toString(),
                        user["profile_image_url"].toString(),
                        userType,
                        user["view_count"].toInt(-1)};
}

namespace Twitch
{
void UserReply::parseData(const QJsonObject &json)
{
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
        if (!data.isEmpty()) {
            m_data.setValue(userFromJson(data.first().toObject()));
        }
    }
}

void UsersReply::parseData(const QJsonObject &json)
{
    Users users;
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
        for (const auto &user : data) {
            users.push_back(userFromJson(user.toObject()));
        }
    }
    m_data.setValue(users);
}

void UserFollowsReply::parseData(const QJsonObject &json)
{
    UserFollows userFollows;
    userFollows.m_total = json["total"].toInt();
    if (json.find("data") != json.end()) {
        const auto &data = json["data"].toArray();
        for (const auto &follow : data) {
            auto followObject = follow.toObject();
            userFollows.m_follows.push_back(Follow{
                followObject["from_id"].toString(),
                followObject["from_name"].toString(),
                followObject["to_id"].toString(),
                followObject["to_name"].toString(),
                followObject["followed_at"].toString(),
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
