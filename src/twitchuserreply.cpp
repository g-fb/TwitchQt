/*
    SPDX-FileCopyrightText: 2018 Kamil Jakubus

    SPDX-License-Identifier: MIT
*/

#include "twitchuserreply.hpp"

#include <QJsonArray>

namespace Twitch {
void UserReply::parseData(const QJsonObject& json)
{
    if (json.find("data") != json.end()) {
        const auto& data = json["data"].toObject();
        if (!data.isEmpty()) {
            const auto& user = data;
            QString broadcasterTypeStr = user["broadcaster_type"].toString();
            User::BroadcasterType broadcasterType = User::BroadcasterType::No;
            if (broadcasterTypeStr == "partner")
                broadcasterType = User::BroadcasterType::Partner;
            else if (broadcasterTypeStr == "affiliate")
                broadcasterType = User::BroadcasterType::Affiliate;

            QString userTypeStr = user["type"].toString();
            User::UserType userType = User::UserType::No;
            if (userTypeStr == "global_mod")
                userType = User::UserType::GlobalMod;
            else if (userTypeStr == "admin")
                userType = User::UserType::Admin;
            else if (userTypeStr == "staff")
                userType = User::UserType::Staff;

            m_data.setValue(User {
                                broadcasterType,
                                user["description"].toString(),
                                user["display_name"].toString(),
                                user["email"].toString(),
                                user["id"].toString("-1"),
                                user["login"].toString(),
                                user["offline_image_url"].toString(),
                                user["profile_image_url"].toString(),
                                userType,
                                user["view_count"].toInt(-1)
                            });
        }
    }
}

void UsersReply::parseData(const QJsonObject &json)
{
    Users users;
    if (json.find("data") != json.end()) {
        const auto& data = json["data"].toArray();
        for (const auto& user : data) {
            auto userObject = user.toObject();
            QString broadcasterTypeStr = userObject["broadcaster_type"].toString();
            User::BroadcasterType broadcasterType = User::BroadcasterType::No;
            if (broadcasterTypeStr == "partner")
                broadcasterType = User::BroadcasterType::Partner;
            else if (broadcasterTypeStr == "affiliate")
                broadcasterType = User::BroadcasterType::Affiliate;

            QString userTypeStr = userObject["type"].toString();
            User::UserType userType = User::UserType::No;
            if (userTypeStr == "global_mod")
                userType = User::UserType::GlobalMod;
            else if (userTypeStr == "admin")
                userType = User::UserType::Admin;
            else if (userTypeStr == "staff")
                userType = User::UserType::Staff;

            users.push_back(User {
                                broadcasterType,
                                userObject["description"].toString(),
                                userObject["display_name"].toString(),
                                userObject["email"].toString(),
                                userObject["id"].toString("-1"),
                                userObject["login"].toString(),
                                userObject["offline_image_url"].toString(),
                                userObject["profile_image_url"].toString(),
                                userType,
                                userObject["view_count"].toInt(-1)
                            });
        }
    }
    m_data.setValue(users);
}

void UserFollowsReply::parseData(const QJsonObject &json)
{
    UserFollows userFollows;
    userFollows.m_total = json["total"].toInt();
    if (json.find("data") != json.end()) {
        const auto& data = json["data"].toArray();
        for (const auto& follow : data) {
            auto followObject = follow.toObject();
            userFollows.m_follows.push_back(Follow {
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
