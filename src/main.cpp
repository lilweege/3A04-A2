#include "FIM.hpp"
#include "RealFIM.hpp"
#include "VerifyFIM.hpp"

#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>

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

int main(int argc, char** argv)
{
    // Optional command line arguments to specify paths of files to be read from and written to.
    const char* in_filepath = argc >= 2 ? argv[1] : "data_before.txt";
    const char* out_filepath = argc >= 3 ? argv[2] : "data_after.txt";

    // Example usage of proxy pattern:

    // Create a proxy (VerifyFIM) for the real subject (RealFIM).
    // The client interfaces with the abstract FIM class only.
    // This is fine since the interface is the same for both concrete classes.
    std::ifstream f{in_filepath};
    std::unique_ptr<FIM> fim = std::make_unique<VerifyFIM>(f);

    // Shorthand for changing password and checking for an error.
    auto ChangePassword = [&](FIM::UserID uID, const char* s) {
        std::cout << "Changing password to \"" << s << "\"\n";
        auto err = fim->ChangePassword({uID, s});
        if (err) {
            std::cout << "ERROR: " << *err << '\n';
        }
    };

    PrintDate();
    ChangePassword(1, "cat");
    Sleep(2000ms);
    PrintDate();
    ChangePassword(1, "dog");
    Sleep(2000ms);
    PrintDate();
    ChangePassword(1, "cat"); // Will fail (old password)
    ChangePassword(1, "dog"); // Will fail (current password)
    ChangePassword(0, "password"); // Will fail (old password from file)

    // Write modified database to new file.
    std::ofstream fs{out_filepath};
    dynamic_cast<VerifyFIM*>(fim.get())->ToStream(fs);
}
