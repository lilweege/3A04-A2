#pragma once

#include "FIM.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <chrono>


class VerifyFIM : public FIM
{
public:
    using TimeStamp = std::time_t;
    static const TimeStamp UNSET_TIME = 0;
    struct DateRange
    {
        TimeStamp date1;
        TimeStamp date2;
    };

    VerifyFIM(std::unique_ptr<FIM>);
    FIM::Error ChangePassword(const User&) override;
private:
    std::unique_ptr<FIM> mRealFIM;
    std::unordered_map<UserID, std::pair<Password, std::unordered_map<Password, DateRange>>> mUsedPasswordDB;
};

