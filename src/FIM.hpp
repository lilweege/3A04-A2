#pragma once

#include <cstdint>
#include <optional>
#include <string>

class FIM
{
public:
    using UserID = uint64_t;
    using Password = std::string;
    struct User
    {
        UserID uID;
        Password password;
    };

    using Error = std::optional<std::string>;

    virtual Error ChangePassword(const User&) = 0;
    virtual ~FIM() = default;
};
