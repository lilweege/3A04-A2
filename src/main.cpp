#include "FIM.hpp"
#include "RealFIM.hpp"
#include "VerifyFIM.hpp"

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

// Timing functions to demonstrate date ranges (unimportant).
using namespace std::chrono_literals;
static void Sleep(auto duration)
{
    std::cout << "Sleeping for " << duration << '\n';
    std::this_thread::sleep_for(duration);
}

static void PrintDate()
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "The current date is " << std::ctime(&now);
}

int main()
{
    // Example usage of proxy pattern:

    // Create a proxy (VerifyFIM) for the real subject (RealFIM).
    // The client interfaces with the abstract FIM class only.
    // This is fine since the interface is the same for both concrete classes.
    std::unique_ptr<FIM> fim = std::make_unique<VerifyFIM>(std::make_unique<RealFIM>());

    // For simplicity only test with one user.
    // The FIM works with multiple users as well.
    FIM::UserID myuid = 1;
    // Shorthand for changing password and checking for an error.
    auto ChangePassword = [&](const char* s) {
        std::cout << "Changing password to \"" << s << "\"\n";
        auto err = fim->ChangePassword({myuid, s});
        if (err) {
            std::cout << "ERROR: " << *err << '\n';
        }
    };


    PrintDate();
    ChangePassword("cat");
    Sleep(2000ms);
    PrintDate();
    ChangePassword("dog");
    Sleep(2000ms);
    PrintDate();
    ChangePassword("cat"); // Will fail (old password)
    ChangePassword("dog"); // Will fail (current password)
}
