#include "VerifyFIM.hpp"
#include "RealFIM.hpp"
#include "FIM.hpp"

#include <iostream>
#include <sstream>
#include <ctime>
#include <cstring>

static VerifyFIM::TimeStamp GetCurrentTime()
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

FIM::Error VerifyFIM::ChangePassword(const User& user)
{
    if (user.password.empty())
        return "Cannot use empty string as password";

    auto& [lastUsedPassword, userPasswords] = mUsedPasswordDB[user.uID];
    auto it = userPasswords.find(user.password);
    if (it != userPasswords.end()) {
        const DateRange& range = it->second;

        // Report an error
        std::stringstream ss;
        if (range.date2 == UNSET_TIME) {
            ss << "Cannot set password to current password";
        }
        else {
            const auto t1 = std::ctime(&range.date1);
            ss << "This password has been used between ["
                << std::string_view{t1, strlen(t1)-1};

            const auto t2 = std::ctime(&range.date2);
            ss << "] and ["
                << std::string_view{t2, strlen(t2)-1}
                << "]";
        }
        return ss.str();
    }

    auto now = GetCurrentTime();
    userPasswords.emplace(user.password, DateRange{now, UNSET_TIME});
    if (!lastUsedPassword.empty())
        userPasswords[lastUsedPassword].date2 = now;
    lastUsedPassword = user.password;

    // Forward the call to the wrapped real subject and return the result
    return mRealFIM->ChangePassword(user);
}


VerifyFIM::VerifyFIM(std::unique_ptr<FIM> fim)
    : mRealFIM(std::move(fim))
{
}

// Deserialize
VerifyFIM::VerifyFIM(std::istream& is)
    : mRealFIM(std::make_unique<RealFIM>())
{
    std::unordered_map<UserID, TimeStamp> lastTimes;
    UserID uID;
    Password password;
    DateRange range;
    while (is >> uID >> password >> range.date1 >> range.date2) {
        auto& [lastUsedPassword, userPasswords] = mUsedPasswordDB[uID];
        TimeStamp& lastTime = lastTimes[uID];
        if (lastTime < range.date1) {
            lastTime = range.date1;
            lastUsedPassword = password;
        }
        userPasswords[password] = range;
    }
    
    for (const auto& [id, p] : mUsedPasswordDB) {
        const auto& [lastUsedPassword, _] = p;
        mRealFIM->ChangePassword({id, lastUsedPassword});
    }
}

// Serialize
void VerifyFIM::ToStream(std::ostream& os) const
{
    for (const auto& [uID, p] : mUsedPasswordDB) {
        const auto& [_, userPasswords] = p;
        for (const auto& [password, range] : userPasswords) {
            os << uID << ' ' << password << ' ' << range.date1 << ' ' << range.date2 << '\n';
        }
    }
}
