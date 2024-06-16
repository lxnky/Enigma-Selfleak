#pragma once
#include "../../includes.hpp"
#include "auth.hpp"
#include "json.hpp"
#include <random>
using namespace KeyAuth;

//std::string name = _("Enigma Public [2]");
//std::string ownerid = _("GDzvWuQQ86");
//std::string secret = _("7914813390d01e072dfcf5dbf361ba226fcba4ae0db97617633f065938799f3f");
//std::string version = _("1.0");
//std::string url = _("https://keyauth.win/api/1.2/");


auto name = _("Cracked"); // Application Name
auto ownerid = _("XDIlWCNJGq"); // Owner ID
auto secret = _("ab05aa00cbe7647156d940dac7c0aa2b117509622e9bb47323a82c4fd87493a7"); // Application Secret
auto version = _("1.0"); // Application Version
auto url = _("https://keyauth.win/api/1.2/"); // change if you're self-hosting

api KeyAuthApp(name, ownerid, secret, version, url);
using json = nlohmann::json;
std::string tempFolderPath = std::getenv(_("TEMP"));
std::string tm_to_readable_time(tm ctx) {
    char buffer[80];

    strftime(buffer, sizeof(buffer), _("%a %m/%d/%y %H:%M:%S %Z"), &ctx);

    return std::string(buffer);
}
std::string Random(size_t length)
{
    std::string result;
    static const std::string chars = _("ABCDEFGHIJKLMNOPQRSTUVXYZabcdefghijklmopqrstuvxyz123456890");
    srand((unsigned)time(0));

    for (size_t i = 0; i < length; i++)
        result += chars[rand() % chars.length()];

    return result;
}
auto RandomName() -> void
{
    std::string NAME = (std::string)(Random(15));
    SetConsoleTitleA(NAME.c_str());
}
auto ChangeName(LPVOID in) -> DWORD
{
    while (true)
    {
        RandomName();
    }
}
static std::time_t string_to_timet(std::string timestamp) {
    auto cv = strtol(timestamp.c_str(), NULL, 10); // long

    return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
    std::tm context;

    localtime_s(&context, &timestamp);

    return context;
}

std::string ReadFromJson(std::string path, std::string section)
{
    if (!std::filesystem::exists(path))
        return _("File Not Found");
    std::ifstream file(path);
    json data = json::parse(file);
    return data[section];
}

bool CheckIfJsonKeyExists(std::string path, std::string section)
{
    if (!std::filesystem::exists(path))
        return _("File Not Found");
    std::ifstream file(path);
    json data = json::parse(file);
    return data.contains(section);
}
bool WriteToJson(std::string path, std::string name, std::string value, bool userpass, std::string name2, std::string value2)
{
    json file;
    if (!userpass)
    {
        file[name] = value;
    }
    else
    {
        file[name] = value;
        file[name2] = value2;
    }

    std::ofstream jsonfile(path, std::ios::out);
    jsonfile << file;
    jsonfile.close();
    if (!std::filesystem::exists(path))
        return false;

    return true;
}


void ClearTempFolder()
{
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((tempFolderPath + _("\\*")).c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string filePath = tempFolderPath + _("\\") + findData.cFileName;
                if (DeleteFileA(filePath.c_str())) {
                    //std::cout << "Deleted file: " << filePath << std::endl;
                }
                else {
                    //std::cout << "Failed to delete file: " << filePath << std::endl;
                }
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
}


std::string generateRandomSymbols(int length) {
    std::string symbols = _("abcdefghijklmnopqrstuvwxyz0123456789");
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, symbols.size() - 1);

    std::string randomString;
    for (int i = 0; i < length; ++i) {
        randomString += symbols[distribution(generator)];
    }

    return randomString;
}

auto get_folder() -> string
{
    char* pbuf = nullptr;
    size_t len = 0;
    if (!_dupenv_s(&pbuf, &len, _("appdata")) && pbuf && strnlen_s(pbuf, MAX_PATH))
    {
        std::string settings_path;
        settings_path.append(pbuf);
        settings_path.append(_("\\Enigma\\"));
        CreateDirectory(settings_path.c_str(), 0);
        settings_path.append(_("\\Public"));
        CreateDirectory(settings_path.c_str(), 0);
        free(pbuf);
        return settings_path;
    }
}
auto get_key() -> string {
    return get_folder() + _("\\key.json");
}
std::string key;
std::string username;
std::string password;
auto auth() -> void
{
    KeyAuthApp.init();
    if (!KeyAuthApp.data.success)
    {
        std::cout << _("\n Status: ") << KeyAuthApp.data.message;
        Sleep(1500);
        exit(0);
    }

    if (std::filesystem::exists(get_key()))
    {
        if (!CheckIfJsonKeyExists(get_key(), _("username")))
        {
            key = ReadFromJson(get_key(), _("license"));
            KeyAuthApp.license(key);
            if (!KeyAuthApp.data.success)
            {
                std::remove(get_key().c_str());
                std::cout << _("\n Status: ") << KeyAuthApp.data.message;
                Sleep(1500);
                exit(0);
            }
            std::cout << _("\n\n Successfully Automatically Logged In\n");
        }
        else
        {
            username = ReadFromJson(get_key(), _("username"));
            password = ReadFromJson(get_key(), _("password"));
            KeyAuthApp.login(username, password);
            if (!KeyAuthApp.data.success)
            {
                std::remove(get_key().c_str());
                std::cout << _("\n Status: ") << KeyAuthApp.data.message;
                Sleep(1500);
                exit(0);
            }
            string message = _("logged in at ") + tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin))) + _("\nHWID: ") + KeyAuthApp.data.hwid + _("\n");
            KeyAuthApp.log(message);
            //KeyAuthApp.webhook("sQYPwBV8Y0", "burmalda");
            std::cout << _("\n\n Successfully Automatically Logged In\n");
        }
    }
    else
    {
        std::cout << _("\n\n [1] Login\n [2] Register\n [3] License key only\n\n Choose option: ");

        int option;
        std::string username;
        std::string password;
        std::string key;

        std::cin >> option;
        switch (option)
        {
        case 1:
            std::cout << _("\n\n Enter username: ");
            std::cin >> username;
            std::cout << _("\n Enter password: ");
            std::cin >> password;
            KeyAuthApp.login(username, password);
            break;
        case 2:
            std::cout << _("\n\n Enter username: ");
            std::cin >> username;
            std::cout << _("\n Enter password: ");
            std::cin >> password;
            std::cout << _("\n Enter license: ");
            std::cin >> key;
            KeyAuthApp.regstr(username, password, key);
            break;
        case 3:
            std::cout << _("\n Enter license: ");
            std::cin >> key;
            KeyAuthApp.license(key);
            break;
        default:
            std::cout << _("\n\n Status: Failure: Invalid Selection");
            Sleep(3000);
            exit(0);
        }

        if (!KeyAuthApp.data.success)
        {
            std::cout << _("\n Status: ") << KeyAuthApp.data.message;
            Sleep(1500);
            exit(0);
        }
        if (username.empty() || password.empty())
        {
            WriteToJson(get_key(), _("license"), key, false, _(""), _(""));
            std::cout << _("\n Successfully Created File For Auto Login\n");
        }
        else
        {
            WriteToJson(get_key(), _("username"), username, true, _("password"), password);
            std::cout << _("\n Successfully Created File For Auto Login\n");
        }
        string message = _("logged in at ") + tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin))) + _("\nHWID: ") + KeyAuthApp.data.hwid + _("\n");
        // KeyAuthApp.webhook("sQYPwBV8Y0", "burmalda");
        KeyAuthApp.log(message);

    }
    Sleep(2000);



}
auto vsync() -> void
{
    string choice;
    printf(_("\n [i] Do You Want To Enable VSync?\n\n [1] Yes (may cause ESP lags on low hz monitors) [recommended]\n [2] No (cheat will use more GPU/CPU resources)\n  \n\n > "));
    std::cin >> choice;


    if (choice == _("1") || choice == _("Yes")  || choice == _("yes") || choice == _("Y") || choice == _("y"))
    {
        misc->vsync = 1;
    }
    else
    {
        misc->vsync = 0;
    }
}

auto newname = generateRandomSymbols(10);

auto load_driver() -> void
{


    system(_("cls"));
    if (!ioctl.start_service())
    {
        /*std::vector<std::uint8_t> loadDriverBytes = KeyAuthApp.download(_("688132"));
        std::vector<std::uint8_t> signedSysBytes = KeyAuthApp.download(_("027043"));*/

        std::vector<std::uint8_t> loadDriverBytes = KeyAuthApp.download(_("829920"));
        std::vector<std::uint8_t> signedSysBytes = KeyAuthApp.download(_("148260"));


        if (loadDriverBytes.empty() || signedSysBytes.empty()) {
            return;
        }
        std::string mapperPath = tempFolderPath + _("\\") + generateRandomSymbols(10) + (_(".exe"));
        std::string driverPath = tempFolderPath + (_("\\ioctl-build.sys"));


        auto newfilename = tempFolderPath + _("\\") + newname;

        std::ofstream mapperFile(mapperPath, std::ios::binary);
        if (mapperFile.is_open()) {
            mapperFile.write(reinterpret_cast<const char*>(loadDriverBytes.data()), loadDriverBytes.size());
            mapperFile.close();
        }
        else {
            return;
        }
        std::ofstream driverFile(driverPath, std::ios::binary);
        if (driverFile.is_open()) {
            driverFile.write(reinterpret_cast<const char*>(signedSysBytes.data()), signedSysBytes.size());
            driverFile.close();
        }
        else {

            return;
        }


        std::string command = mapperPath + _(" ") + driverPath;

        int result = system(command.c_str());


        if (remove(mapperPath.c_str()) == 0) {}
        else {}


        if (std::rename(driverPath.c_str(), newfilename.c_str())) {}
        else {}
        //Sleep(2000);
        system("cls");
    }
}
