#include "VerifyFIM.hpp"
#include "FIM.hpp"

#include <sstream>
#include <ctime>
#include <cstring>

VerifyFIM::VerifyFIM(std::unique_ptr<FIM> fim)
    : mRealFIM(std::move(fim))
{
}

static VerifyFIM::TimeStamp GetCurrentTime()
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

FIM::Error VerifyFIM::ChangePassword(const User& user)
{
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
    userPasswords[lastUsedPassword].date2 = now;
    lastUsedPassword = user.password;

    // Forward the call to the wrapped real subject and return the result
    return mRealFIM->ChangePassword(user);
}
