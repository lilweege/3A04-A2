#include "RealFIM.hpp"

#include <iostream>

FIM::Error RealFIM::ChangePassword(const User& user)
{
    mActiveDB[user.uID] = user.password;
    return std::nullopt;
}
