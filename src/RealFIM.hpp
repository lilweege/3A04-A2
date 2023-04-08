#pragma once

#include "FIM.hpp"
#include <unordered_map>

class RealFIM : public FIM
{
public:
    FIM::Error ChangePassword(const User&) override;
private:
    std::unordered_map<UserID, Password> mActiveDB;
};
