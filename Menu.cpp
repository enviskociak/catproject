#include "Menu.hpp"
#include <algorithm>
#include "includes.hpp"
#include "Settings.hpp"
#include <connection.hpp>
#include "imgui_animated.h"
#include <Lists.hpp>
#include <d3d9.h>
#include <d3dx9core.h>
#include <d3d11.h>
#include <xor.hpp>
#include <imgui_browser.h>
#include <boost/lexical_cast.hpp>
#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <wininet.h>
#include <fstream>
#include <sstream>
#include "Classes.hpp"
#include "Settings.hpp"
#include <native.hpp>
#include <ShlObj.h>
#include <Fonts.hpp>
#include "xorstr.h"
#include "imgui_notify.h"
#include "hooks.hpp" 

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "wininet.lib")

char car_name[1000];
char weaponname[256] = "";
char vehiclename [1000] ;
bool spawnCarEnabled = false;

std::string nomedopalyerselecionado;
std::string menulua;
std::string fiveguard;
std::string menupriv;
std::string rac;
static int selectedVeh_index = 0;
static int selected_index = 1;

inline ImGui::FileBrowser LoadfileDEV(ImGuiFileBrowserFlags_EnterNewFilename);
inline ImGui::FileBrowser LoadfileUD(ImGuiFileBrowserFlags_EnterNewFilename);
inline ImGui::FileBrowser LoadFile(ImGuiFileBrowserFlags_EnterNewFilename);
inline ImGui::FileBrowser dump_browser1(ImGuiFileBrowserFlags_SelectDirectory);
static char placanome[10] = "";
static char eventname[50] = "";
static char configname[50] = "config_name";
inline bool desativar = false;
inline bool desativarpriv = false;
#define MAX_RGB 255.0
#define HELPMARKER(str)  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.5f); ImGui::SameLine(); ImGui::TextColored(ImColor(255, 255, 0, 255), "(?)"); if (ImGui::IsItemHovered()) ImGui::SetTooltip(str)
#define HELPMARKER2(str)  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.5f); ImGui::SameLine(); ImGui::TextColored(ImColor(255, 255, 0, 255), "(?)"); if (ImGui::IsItemHovered()) ImGui::SetTooltip(str)
INT Static_241, Static_240, Static_242, Static_12;

namespace fs = std::filesystem;

class EventData
{
public:
    char eventName[128];
    char pad_0001[88];
    bool unk_0001;
    char pad_0002[4039];
};
struct EventLogEntry
{
    std::string eventName;
};
std::vector<std::string> nomes;
int selectedItemIndex = -1;
std::string directirionomes = _xor_("C:\\ProgramData\\nm").c_str();

std::vector<std::string> nomesEvents;
int selectedItemIndexEVENTS = -1;
std::string eventnames = _xor_("C:\\ProgramData\\ev").c_str();


const char* path_t = _xor_("C:\\Users\\2342\\AppData\\Roaming\\CitizenFX\\").c_str();
using hex32 = uint32_t;
hex32 account;

hex32 __fastcall_block()
{
    PWSTR pathasd = nullptr;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &pathasd);
    std::wstring roamingPath(pathasd);
    CoTaskMemFree(pathasd);
    std::wstring pathfinal = roamingPath + L"\\CitizenFX\\";

    // Obtendo o tamanho necessário para o buffer de caracteres estreitos
    int size = WideCharToMultiByte(CP_UTF8, 0, roamingPath.c_str(), -1, NULL, 0, NULL, NULL);

    // Convertendo std::wstring para std::string usando WideCharToMultiByte
    std::string path_str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, roamingPath.c_str(), -1, &path_str[0], size, NULL, NULL);

    FILE* stream;
    hex32 accountId = 0;

    std::string path = path_str;
    path.append("ros_id.dat");
    fopen_s(&stream, path.c_str(), "r+");
    fseek(stream, 0, SEEK_SET);

    fwrite(&accountId, 1, sizeof(accountId), stream);

    fclose(stream);

    return true;
}

bool __getaccount(hex32* out)
{
    PWSTR pathasd = nullptr;
    static std::once_flag gotAccountId;
    static hex32 accountId;
 
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &pathasd);
    std::wstring roamingPath(pathasd);
    CoTaskMemFree(pathasd);
    std::wstring pathfinal = roamingPath + L"\\CitizenFX\\";

    // Obtendo o tamanho necessário para o buffer de caracteres estreitos
    int size = WideCharToMultiByte(CP_UTF8, 0, roamingPath.c_str(), -1, NULL, 0, NULL, NULL);

    // Convertendo std::wstring para std::string usando WideCharToMultiByte
    std::string path_str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, roamingPath.c_str(), -1, &path_str[0], size, NULL, NULL);

    FILE* stream;
    std::string path = path_str;
    if (path.find("username") != std::string::npos)
    {
        MessageBoxA(0, "Set your path", "C!@?#", 0);
        exit(0x0);
    }
    path.append("ros_id.dat");
    fopen_s(&stream, path.c_str(), "r+");
    fseek(stream, 0, SEEK_SET);

    fread(&accountId, 1, sizeof(accountId), stream);

    fclose(stream);

    *out = accountId;
    if (accountId != NULL)
    {
        return true;
    }
    else
        return false;
}

bool endsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void ListFilesInFolderWithExtensions(const std::string& folderPath, const std::vector<std::string>& extensions, std::vector<std::string>& files)
{
    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            const std::string& filePath = entry.path().string();
            for (const auto& extension : extensions)
            {
                if (endsWith(filePath, extension))
                {
                    files.push_back(entry.path().filename().string());
                    break;  // Solo se agrega una vez por archivo
                }
            }
        }
    }
}




std::string configName; // Assumindo que você tem uma variável para armazenar o nome da configuração
struct ImColorSave {
    float red, green, blue, alpha;

    // Default constructor
    ImColorSave() : red(0.0f), green(0.0f), blue(0.0f), alpha(1.0f) {}

    // Constructor to initialize the values
    ImColorSave(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) {}
};
void SaveColor(const char* key, const ImColor& color, std::ofstream& file) {
    // Save the color components in the range of 0 to 255
    file << key << " 0 " << color.Value.x << " " <<  color.Value.y << " " <<  color.Value.z << " " << color.Value.w << std::endl;
}

// Função para salvar a configuração atual em um arquivo
void SaveConfig(const char* filename) {

    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        // Visuals
        file << "activevisibleforyou " << (Settings::Visuals::activevisibleforyou ? "true" : "false") << std::endl;
        file << "enable " << (Settings::Visuals::enable ? "true" : "false") << std::endl;
        file << "Box " << (Settings::Visuals::Box ? "true" : "false") << std::endl;
        file << "Filled " << (Settings::Visuals::Filled ? "true" : "false") << std::endl;
        file << "Corner " << (Settings::Visuals::Corner ? "true" : "false") << std::endl;
        file << "Skeleton " << (Settings::Visuals::Skeleton ? "true" : "false") << std::endl;
        file << "Snapline " << (Settings::Visuals::Snapline ? "true" : "false") << std::endl;
        file << "HealthBar " << (Settings::Visuals::HealthBar ? "true" : "false") << std::endl;
        file << "ArmorBar " << (Settings::Visuals::ArmorBar ? "true" : "false") << std::endl;
        file << "Weapon " << (Settings::Visuals::Weapon ? "true" : "false") << std::endl;
        file << "NameEsp " << (Settings::Visuals::NameEsp ? "true" : "false") << std::endl;
        file << "PlayerName " << (Settings::Visuals::PlayerName ? "true" : "false") << std::endl;
        file << "StaticESP " << (Settings::Visuals::StaticESP ? "true" : "false") << std::endl;
        file << "Draw_LocalPlayer " << (Settings::Visuals::Draw_LocalPlayer ? "true" : "false") << std::endl;
        file << "Draw_Dead " << (Settings::Visuals::Draw_Dead ? "true" : "false") << std::endl;
        file << "Draw_Ped " << (Settings::Visuals::Draw_Ped ? "true" : "false") << std::endl;
        file << "MenuStyle " << Settings::Visuals::menu_idx << std::endl;  // 
        file << "ArmorBar_idx " << Settings::Visuals::ArmorBar_idx << std::endl;  // 
        file << "BoxSize " << Settings::Visuals::BoxSize << std::endl;  // 
        file << "Distance " << Settings::Visuals::Distance << std::endl;  // 
        file << "Distancevheicle " << Settings::Visuals::Distancevheicle << std::endl;  // 
        file << "FillBoxAlpha " << Settings::Visuals::FillBoxAlpha << std::endl;  //
        file << "EspNameStyle " << Settings::Visuals::espnames_idx << std::endl;  //


        // cores

        SaveColor("BoxColor", Settings::Visuals::BoxColor, file);
        SaveColor("CornerColor", Settings::Visuals::CornerColor, file);
        SaveColor("SkelColor", Settings::Visuals::SkelColor, file);
        SaveColor("crosshairColor", Settings::Visuals::crosshair, file);
        SaveColor("linesColor", Settings::Visuals::linesColor, file);
        SaveColor("fovColor", Settings::Visuals::fovColor, file);
        SaveColor("linesColornovisibel", Settings::Visuals::linesColornovisibel, file);
        SaveColor("PlayernameColor", Settings::Visuals::PlayernameColor, file);

        // Aimbot
        file << "showaimposicion " << (Settings::Aimbot::showaimposicion ? "true" : "false") << std::endl;
        file << "AimbotFovPosicion " << Settings::Aimbot::AimbotFovPosicion << std::endl;  // 

        file << "Aimbot " << (Settings::Aimbot::Aimbot ? "true" : "false") << std::endl;
        file << "Draw_Fov " << (Settings::Aimbot::Draw_Fov ? "true" : "false") << std::endl;

        file << "crosshair " << (Settings::Aimbot::crosshair ? "true" : "false") << std::endl;
    
        file << "AimbotFov " << Settings::Aimbot::AimbotFov << std::endl;  // 
        file << "AimbotSmooth " << Settings::Aimbot::AimbotSmooth << std::endl;  // 
        file << "TypeOfTarget " << Settings::Aimbot::TypeOfTarget << std::endl;  // 
        file << "AimbotBone " << Settings::Aimbot::AimbotBone << std::endl;  // 
        file << "AimbotTypes " << Settings::Aimbot::AimbotTypes << std::endl;  // 
        file << "DistanceAimbotLimit " << Settings::Aimbot::DistanceAimbotLimit << std::endl;  // 
        file << "Hotkey " << Settings::Aimbot::Hotkey << std::endl;  // 
        file << "AimSensitivity " << Settings::Aimbot::test << std::endl;  // 
        file << "AimSpeed " << Settings::Aimbot::test2 << std::endl;  // 
        // end Aimbot
        // Player

        file << "Godmod " << (Settings::Player::Godmod ? "true" : "false") << std::endl;
        file << "Semi_Godmod " << (Settings::Player::Semi_Godmod ? "true" : "false") << std::endl;
        file << "Infinite_Armor " << (Settings::Player::Infinite_Armor ? "true" : "false") << std::endl;
        file << "SuperJump " << (Settings::Player::SuperJump ? "true" : "false") << std::endl;
        file << "fastrun " << (Settings::Player::fastrun ? "true" : "false") << std::endl;
        file << "Invisible " << (Settings::Player::Invisible ? "true" : "false") << std::endl;
        file << "NoClip " << (Settings::Player::NoClip ? "true" : "false") << std::endl;
      
      
        file << "ArmorToSet " << Settings::Player::ArmorToSet << std::endl;  // 
        file << "HealthToSet " << Settings::Player::HealthToSet << std::endl;  // 
        file << "FovValue " << Settings::Player::FovValue << std::endl;  // 
        file << "NoclipKey " << Settings::misc::NoclipKey << std::endl;  // 
        file << "noclipboost " << Settings::misc::noclipboost << std::endl;  // 
        file << "RunSpeedToSet " << Settings::Player::RunSpeedToSet << std::endl;  // 
        file << "Speed " << Settings::Player::Speed << std::endl;  // 

      
        // end Player

        // Weapon
      

        file << "No_Recoil " << (Settings::Weapon::No_Recoil ? "true" : "false") << std::endl;
        file << "No_Spread " << (Settings::Weapon::No_Spread ? "true" : "false") << std::endl;
        file << "InfiniteAmmo " << (Settings::Weapon::InfiniteAmmo ? "true" : "false") << std::endl;
        file << "NoRangeLimit " << (Settings::Weapon::NoRangeLimit ? "true" : "false") << std::endl;
        file << "NoReload " << (Settings::Weapon::NoReload ? "true" : "false") << std::endl;
        file << "DoubleTap " << (Settings::Weapon::DoubleTap ? "true" : "false") << std::endl;
        file << "WeaponOption " << (Settings::Weapon::enable ? "true" : "false") << std::endl;

        file << "AmmoToSet " << Settings::Weapon::AmmoToSet << std::endl;  // 
        file << "BulletToShot " << Settings::Weapon::BulletToShot << std::endl;  // 


        // end
        // vaiculos

        file << "godmode " << (Settings::vehicle::godmode ? "true" : "false") << std::endl;
        file << "vehInvisible " << (Settings::vehicle::vehInvisible ? "true" : "false") << std::endl;
        file << "shift_boost " << (Settings::vehicle::shift_boost ? "true" : "false") << std::endl;

        //vehicle_speed_value
        file << "vehicle_speed_value " << Settings::vehicle::vehicle_speed_value << std::endl;  // 
        // end
        
        // ... Adicione linhas para outras variáveis ...
        file.close();
    }
}

// Função para carregar a configuração de um arquivo
void LoadConfig(const char* filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        if (file.is_open()) {
            std::string configName;
            std::string configValue;
            ImColorSave colorSave;

            while (file >> configName >> configValue)
            {


                // visuals

                if (configName == "activevisibleforyou") {
                    Settings::Visuals::activevisibleforyou = (configValue == "true");
                }
                else if (configName == "enable") {
                    Settings::Visuals::enable = (configValue == "true");
                }
                else if (configName == "Box") {
                    Settings::Visuals::Box = (configValue == "true");
                }
                else if (configName == "Filled") {
                    Settings::Visuals::Filled = (configValue == "true");
                }
                else if (configName == "Corner") {
                    Settings::Visuals::Corner = (configValue == "true");
                }
                else if (configName == "Skeleton") {
                    Settings::Visuals::Skeleton = (configValue == "true");
                }
                else if (configName == "Snapline") {
                    Settings::Visuals::Snapline = (configValue == "true");
                }
                else if (configName == "HealthBar") {
                    Settings::Visuals::HealthBar = (configValue == "true");
                }
                else if (configName == "ArmorBar") {
                    Settings::Visuals::ArmorBar = (configValue == "true");
                }
                else if (configName == "Weapon") {
                    Settings::Visuals::Weapon = (configValue == "true");
                }
                else if (configName == "NameEsp") {
                    Settings::Visuals::NameEsp = (configValue == "true");
                }
                else if (configName == "PlayerName") {
                    Settings::Visuals::PlayerName = (configValue == "true");
                }
                else if (configName == "StaticESP") {
                    Settings::Visuals::StaticESP = (configValue == "true");
                }
                else if (configName == "Draw_LocalPlayer") {
                    Settings::Visuals::Draw_LocalPlayer = (configValue == "true");
                }
                else if (configName == "Draw_Dead") {
                    Settings::Visuals::Draw_Dead = (configValue == "true");
                }
                else if (configName == "Draw_Ped") {
                    Settings::Visuals::Draw_Ped = (configValue == "true");
                }
                else if (configName == "StaticESP") {
                    Settings::Visuals::StaticESP = (configValue == "true");
                }
                else if (configName == "MenuStyle") {
                    Settings::Visuals::menu_idx = atoi(configValue.c_str());
                }
                else if (configName == "ArmorBar_idx") {
                    Settings::Visuals::ArmorBar_idx = atoi(configValue.c_str());
                }
                else if (configName == "BoxSize") {
                    Settings::Visuals::BoxSize = atoi(configValue.c_str());
                }
                else if (configName == "Distance") {
                    Settings::Visuals::Distance = atoi(configValue.c_str());
                }
                else if (configName == "Distancevheicle") {
                    Settings::Visuals::Distancevheicle = atoi(configValue.c_str());
                }
                else if (configName == "FillBoxAlpha") {
                    Settings::Visuals::FillBoxAlpha = atoi(configValue.c_str());
                }
                else if (configName == "EspNameStyle") {
                    Settings::Visuals::espnames_idx = atoi(configValue.c_str());
                }
                // end visuals


                // Aimbot
                else if (configName == "showaimposicion") {
                    Settings::Aimbot::showaimposicion = (configValue == "true");
                }
                else if (configName == "Aimbot") {
                    Settings::Aimbot::Aimbot = (configValue == "true");
                }
                else if (configName == "Draw_Fov") {
                    Settings::Aimbot::Draw_Fov = (configValue == "true");
                }
                else if (configName == "crosshair") {
                    Settings::Aimbot::crosshair = (configValue == "true");
                }



                else if (configName == "AimbotFov") {
                    Settings::Aimbot::AimbotFov = atoi(configValue.c_str());
                }
                else if (configName == "AimbotSmooth") {
                    Settings::Aimbot::AimbotSmooth = atoi(configValue.c_str());
                }
                else if (configName == "TypeOfTarget") {
                    Settings::Aimbot::TypeOfTarget = atoi(configValue.c_str());
                }
                else if (configName == "AimbotBone") {
                    Settings::Aimbot::AimbotBone = atoi(configValue.c_str());
                }
                else if (configName == "AimbotTypes") {
                    Settings::Aimbot::AimbotTypes = atoi(configValue.c_str());
                    }
                else if (configName == "DistanceAimbotLimit") {
                        Settings::Aimbot::DistanceAimbotLimit = atoi(configValue.c_str());
                        }
                else if (configName == "Hotkey") {
                            Settings::Aimbot::Hotkey = atoi(configValue.c_str());
                            }
                else if (configName == "DistanceAimbotLimit") {
                                Settings::Aimbot::DistanceAimbotLimit = atoi(configValue.c_str());
                                }
                else if (configName == "AimSensitivity") {
                                    Settings::Aimbot::test = atoi(configValue.c_str());
                                    }
                else if (configName == "AimSpeed") {
                                        Settings::Aimbot::test2 = atoi(configValue.c_str());
                                        }

                // end aimbot

               // player
                else if (configName == "Godmod") {
                   Settings::Player::Godmod = (configValue == "true");
                   }
                else if (configName == "Semi_Godmod") {
                       Settings::Player::Semi_Godmod = (configValue == "true");
                       }
                else if (configName == "Infinite_Armor") {
                           Settings::Player::Infinite_Armor = (configValue == "true");
                           }
                else if (configName == "SuperJump") {
                               Settings::Player::SuperJump = (configValue == "true");
                               }
                else if (configName == "fastrun") {
                                   Settings::Player::fastrun = (configValue == "true");
                                   }
                else if (configName == "Invisible") {
                                       Settings::Player::Invisible = (configValue == "true");
                                       }
                else if (configName == "NoClip") {
                                           Settings::Player::NoClip = (configValue == "true");
                                           }         
                else if (configName == "ArmorToSet") {
                                                   Settings::Player::ArmorToSet = atoi(configValue.c_str());
                                                   }
                else if (configName == "HealthToSet") {
                                                       Settings::Player::HealthToSet = atoi(configValue.c_str());
                                                       }
                else if (configName == "FovValue") {
                                                           Settings::Player::FovValue = atoi(configValue.c_str());
                                                           }
                else if (configName == "NoclipKey") {
                                                               Settings::misc::NoclipKey = atoi(configValue.c_str());
                                                               }
                else if (configName == "noclipboost") {
                                                                   Settings::misc::noclipboost = atoi(configValue.c_str());
                                                                   }
                else if (configName == "RunSpeedToSet") {
                                                                       Settings::Player::RunSpeedToSet = atoi(configValue.c_str());
                                                                       }
                else if (configName == "Speed") {
                                                                           Settings::Player::Speed = atoi(configValue.c_str());
                                                                           }


              // end

     
              // weapons

                else if (configName == "No_Recoil") {
                  Settings::Weapon::No_Recoil = (configValue == "true");
                    }
                else if (configName == "InfiniteAmmo") {
                    Settings::Weapon::InfiniteAmmo = (configValue == "true");
                    }
                else if (configName == "NoRangeLimit") {
                        Settings::Weapon::NoRangeLimit = (configValue == "true");
                        }
                else if (configName == "NoReload") {
                            Settings::Weapon::NoReload = (configValue == "true");
                            }
                else if (configName == "DoubleTap") {

                                Settings::Weapon::DoubleTap = (configValue == "true");
                                  }
                else if (configName == "WeaponOption") {
                                    Settings::Weapon::enable = (configValue == "true");
                                      }
                else if (configName == "No_Spread") {
                                          Settings::Weapon::No_Spread = (configValue == "true");
                                          }
                else if (configName == "AmmoToSet") {
                                        Settings::Weapon::AmmoToSet = atoi(configValue.c_str());
                                          }
                                        
                else if (configName == "BulletToShot") {
                                            Settings::Weapon::BulletToShot = atoi(configValue.c_str());
                                            }


              // end

              // vaiculo

                else if (configName == "godmode") {
                  Settings::vehicle::godmode = (configValue == "true");
                  }
                else if (configName == "vehInvisible") {

                      Settings::vehicle::vehInvisible = (configValue == "true");
                      }
                else if (configName == "WeaponOption") {
                          Settings::vehicle::enable = (configValue == "true");
                          }

                else if (configName == "shift_boost") {
                              Settings::vehicle::shift_boost = (configValue == "true");
                              }
                else if (configName == "vehicle_speed_value") {
                              Settings::vehicle::vehicle_speed_value = atoi(configValue.c_str());
                              }

              //end


                // colors
                else if (configName == "BoxColor")
                {
                    float r, g, b, a;
                    if (file >> r >> g >> b >> a) {
                        Settings::Visuals::BoxColor = ImColor(r, g, b, a);
                        Settings::TempColor::Box[0] = r;
                        Settings::TempColor::Box[1] = g;
                        Settings::TempColor::Box[2] = b;
                        Settings::TempColor::Box[3] = a;
                    }
                }
                else if (configName == "CornerColor")
                {
                    float r, g, b, a;
                    if (file >> r >> g >> b >> a) {
                        Settings::Visuals::CornerColor = ImColor(r, g, b, a);
                        Settings::TempColor::Corner[0] = r;
                        Settings::TempColor::Corner[1] = g;
                        Settings::TempColor::Corner[2] = b;
                        Settings::TempColor::Corner[3] = a;
                    }
                    }
                else if (configName == "SkelColor")
                {
                    float r, g, b, a;
                    if (file >> r >> g >> b >> a) {
                        Settings::Visuals::SkelColor = ImColor(r, g, b, a);
                        Settings::TempColor::Skeleton[0] = r;
                        Settings::TempColor::Skeleton[1] = g;
                        Settings::TempColor::Skeleton[2] = b;
                        Settings::TempColor::Skeleton[3] = a;
                    }
                    }
                else if (configName == "crosshairColor")
                {
                    float r, g, b, a;
                    if (file >> r >> g >> b >> a) {
                        Settings::Visuals::crosshair = ImColor(r, g, b, a);
                        Settings::TempColor::cross[0] = r;
                        Settings::TempColor::cross[1] = g;
                        Settings::TempColor::cross[2] = b;
                        Settings::TempColor::cross[3] = a;
                    }
                    }
                else if (configName == "linesColor")
                {
                    float r, g, b, a;
                    if (file >> r >> g >> b >> a) {
                        Settings::Visuals::linesColor = ImColor(r, g, b, a);
                        Settings::TempColor::lines[0] = r;
                        Settings::TempColor::lines[1] = g;
                        Settings::TempColor::lines[2] = b;
                        Settings::TempColor::lines[3] = a;
                    }
                    }
                else if (configName == "fovColor") 
                {
                    float r, g, b, a;
                    if (file >> r >> g >> b >> a) {
                        Settings::Visuals::fovColor = ImColor(r, g, b, a);
                        Settings::TempColor::fov[0] = r;
                        Settings::TempColor::fov[1] = g;
                        Settings::TempColor::fov[2] = b;
                        Settings::TempColor::fov[3] = a;
                    }                  
                } 
                else if (configName == "linesColornovisibel")
                {
                    float r, g, b, a;
                    if (file >> r >> g >> b >> a) {
                        Settings::Visuals::linesColornovisibel = ImColor(r, g, b, a);
                        Settings::TempColor::linesvisivel[0] = r;
                        Settings::TempColor::linesvisivel[1] = g;
                        Settings::TempColor::linesvisivel[2] = b;
                        Settings::TempColor::linesvisivel[3] = a;
                    }
                    }
                else if (configName == "PlayernameColor")
                {
                    float r, g, b, a;
                    if (file >> r >> g >> b >> a) {
                        Settings::Visuals::PlayernameColor = ImColor(r, g, b, a);
                        Settings::TempColor::name[0] = r;
                        Settings::TempColor::name[1] = g;
                        Settings::TempColor::name[2] = b;
                        Settings::TempColor::name[3] = a;
                    }
                    }

                
            }
        }
    }
}

void devtools(LPCSTR Modulo, int valores, int offsets)
{
    uintptr_t moduleBase;

    moduleBase = (uintptr_t)lazy(GetModuleHandleA)(Modulo);

    HANDLE hProcess = GetCurrentProcess();

    // Endereço na memória onde você deseja escrever
    uintptr_t endereco = moduleBase + offsets;

    // Valor que você deseja escrever na posição de memória
  

    // Escreve na memória
    WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(endereco), &valores, sizeof(valores), nullptr);

    // Fecha o handle do processo
    CloseHandle(hProcess);
}


void SalvarNomes() {
    std::ofstream arquivo(directirionomes);
    if (arquivo.is_open()) {
        for (const auto& nome : nomes) {
            arquivo << nome << std::endl;
        }
        arquivo.close();
    }
}

void CarregarNomes() {
    std::ifstream arquivo(directirionomes);
    if (arquivo.is_open()) {
        std::string nome;
        while (std::getline(arquivo, nome)) {
            nomes.push_back(nome);
        }
        arquivo.close();
    }
}


void SalvarNomesEvent() {
    std::ofstream arquivo(eventnames);
    if (arquivo.is_open()) {
        for (const auto& nome : nomesEvents) {
            arquivo << nome << std::endl;
        }
        arquivo.close();
    }
}

void CarregarNomesEvent() {
    std::ifstream arquivo(eventnames);
    if (arquivo.is_open()) {
        std::string nome;
        while (std::getline(arquivo, nome)) {
            nomesEvents.push_back(nome);
        }
        arquivo.close();
    }
}

void ListLuaFilesInFolder(const std::string & folderPath, std::vector<std::string>&luaFiles)
{
    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".lua")
        {
            luaFiles.push_back(entry.path().filename().string()); // Armazenar apenas o nome do arquivo
        }
    }
}
void ListtxtFilesInFolder(const std::string& folderPath, std::vector<std::string>& luaFiles)
{
    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            luaFiles.push_back(entry.path().filename().string()); // Armazenar apenas o nome do arquivo
        }
    }
}
void SendF8KeyPress() {
    Menu::Open = false;
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = MapVirtualKey(VK_F8, MAPVK_VK_TO_VSC);
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = VK_F8;
    input.ki.dwFlags = 0; // 0 for key press

    // Envia a tecla F8 pressionada
    SendInput(1, &input, sizeof(INPUT));

    // Espera um curto período para a tecla ser processada
    Sleep(100);

    // Envia a tecla F8 liberada
    input.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &input, sizeof(INPUT));
}
bool IsTargetWindow(HWND hWnd) {
    constexpr size_t maxWindowTitleLength = 256;
    char windowTitle[maxWindowTitleLength];

    // Obtém o título da janela
    GetWindowTextA(hWnd, windowTitle, maxWindowTitleLength);

    // Verifica se o título corresponde a "fovem"
    return strcmp(windowTitle, "FiveM® by Cfx.re") == 0;
}

std::string tm_to_readable_time(tm ctx) {
    char buffer[80];

    strftime(buffer, sizeof(buffer), "%d/%m/%y", &ctx);

    return std::string(buffer);
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

void ShowMessageBox(const char* title, const char* message) {
    MessageBox(NULL, message, title, MB_OK | MB_ICONINFORMATION);
}

inline void dumpServer()
{
    json dump = sdk::fetchMetadata();

    std::string path = dump_browser1.GetSelected().string();

    if (!path.empty())
    {
        std::string savePath = path.append(_xor_("\\").c_str()).append(sdk::netLibrary->GetCurrentServer().GetAddress());
        std::string streamPath = savePath;
        sdk::dumpAll(savePath.c_str(), dump);
        ImGui::InsertNotification({ ImGuiToastType_Success, 5000, "The server has been successfully dumped! %s", "" }); //me quede aqui
        system(("start " + savePath).c_str());
        lazy(TerminateThread).safe_cached()(lazy(GetCurrentThread).safe_cached()(), NULL);
    }
}

ImVec4 ToVec4(float r, float g, float b, float a)
{
	return ImVec4(r / MAX_RGB, g / MAX_RGB, b / MAX_RGB, a / MAX_RGB);
}

static ImVec4 active = ToVec4(19, 148, 250, 175);
static ImVec4 inactive = ToVec4(125, 125, 125, 85);

void CenterTextEx(const char* text, float width, int lineId, bool separator)
{
	if (text == nullptr)
		return;

	ImGui::Spacing();
	ImGui::SameLine((width / 2) - (ImGui::CalcTextSize(text).x / 2));
	ImGui::Text(text);
	ImGui::Spacing();

}
void CenterText(const char* text, int lineId, bool separator)
{
	if (text == nullptr)
		return;

	ImGui::Spacing();
	ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(text).x / 2));
	ImGui::Text(text);
	ImGui::Spacing();
}

void set_status_text(std::string str, bool isGxtEntry)
{
    HUD::BEGIN_TEXT_COMMAND_THEFEED_POST(const_cast<char*>(isGxtEntry ? &str[0u] : "STRING"));
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&str[0u]);
    HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(FALSE, FALSE); // _DRAW_NOTIFICATION(BOOL blink, BOOL p1)
}

//diseno color 
// color de la ui


void Menu::Stylesaveold(void)
{


    ImGuiStyle* Style = &ImGui::GetStyle();
    ImVec4* Colors = Style->Colors;
    Style->WindowPadding = { 10.f, 10.f };
    Style->PopupRounding = 0.f;
    Style->FramePadding = { 8.f, 4.f };
    Style->ItemSpacing = { 10.f, 8.f };
    Style->ItemInnerSpacing = { 6.f, 6.f };
    Style->TouchExtraPadding = { 0.f, 0.f };
    Style->IndentSpacing = 21.f;
    Style->ScrollbarSize = 15.f;
    Style->GrabMinSize = 8.f;
    Style->WindowBorderSize = 1.f;
    Style->ChildBorderSize = 0.f;
    Style->PopupBorderSize = 1.f;
    Style->FrameBorderSize = 0.f;
    Style->TabBorderSize = 0.f;
    Style->WindowRounding = 6.f;
    Style->ChildRounding = 0.f;
    Style->FrameRounding = 0.f;
    Style->ScrollbarRounding = 0.f;
    Style->GrabRounding = 0.f;
    Style->TabRounding = 0.f;
    Style->WindowTitleAlign = { 0.5f, 0.5f };
    Style->ButtonTextAlign = { 0.5f, 0.5f };
    Style->DisplaySafeAreaPadding = { 3.f, 3.f };

    Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.90f, 0.19f, 1.00f);
    Colors[ImGuiCol_WindowBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_PopupBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
    Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
    Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
    Colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.27f, 0.27f, 0.54f);
    Colors[ImGuiCol_TitleBg] = ImVec4(ImColor(14, 26, 42));
    Colors[ImGuiCol_TitleBgActive] = ImVec4(ImColor(14, 26, 42));
    Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(ImColor(14, 26, 42, 130));
    Colors[ImGuiCol_MenuBarBg] = ImVec4((ImColor(0, 18, 32, 240)));
    Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
    Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.39f, 0.38f, 0.38f, 1.00f);
    Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_ButtonHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.78f);
    Colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 0.87f);
    Colors[ImGuiCol_Header] = ImVec4(0.37f, 0.37f, 0.37f, 0.31f);
    Colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.38f, 0.37f);
    Colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.37f, 0.37f, 0.51f);
    Colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
    Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.50f);
    Colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.64f);
    Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    Colors[ImGuiCol_Tab] = ImVec4(ImColor(11, 26, 39));
    Colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
    Colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.86f);
    Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
    Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

#define MAX_RGB 255.0
ImVec4 ToVvec422(float r, float g, float b, float a)
{
    return ImVec4(r / MAX_RGB, g / MAX_RGB, b / MAX_RGB, a / MAX_RGB);
}

void Menu::Style2(void)
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    // colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
     //colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 15;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 4;

}


void Menu::Style(void)
{
    ImGuiStyle* Style = &ImGui::GetStyle();
    ImVec4* Colors = Style->Colors;

    Style->WindowBorderSize = 0;
    Style->ChildRounding = 3;
    Style->FrameRounding = 0;
    // Style->ScrollbarRounding = 0;
    Style->GrabRounding = 0;
    Style->PopupRounding = 3;
    Style->WindowRounding = 3;
    Style->ItemSpacing = ImVec2(-30, -30);


    Colors[ImGuiCol_Border] = ImVec4(ImColor(255, 255, 255, 0));
    Colors[ImGuiCol_WindowBg] = ImVec4(ImColor(10, 10, 10, 255));
    Colors[ImGuiCol_ChildBg] = ImVec4(ImColor(20, 20, 20, 175));
    Colors[ImGuiCol_Text] = ImVec4(ImColor(255, 255, 255, 255));
    Colors[ImGuiCol_Separator] = ImVec4(ImColor(45, 45, 45, 135));
    Colors[ImGuiCol_CheckMark] = ImVec4(ImColor(2, 106, 191, 175));
    Colors[ImGuiCol_PopupBg] = ImVec4(ImColor(20, 20, 20, 175));
    Colors[ImGuiCol_Button] = ImVec4(ImColor(33, 33, 33, 255));
    Colors[ImGuiCol_ButtonActive] = ImVec4(ImColor(48, 48, 48, 255));
    Colors[ImGuiCol_ButtonHovered] = ImVec4(ImColor(43, 43, 43, 255));
    //Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
    //Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    //Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

    Colors[ImGuiCol_FrameBg] = ImVec4(ImColor(20, 20, 20, 255));
    Colors[ImGuiCol_FrameBgHovered] = ImVec4(ImColor(33, 32, 32, 255));
    Colors[ImGuiCol_FrameBgActive] = ImVec4(ImColor(28, 28, 28, 255));


    Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

    Colors[ImGuiCol_Separator] = Colors[ImGuiCol_Border];
    Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 0.00f);
    Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0);
    Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0);
    Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0);
    Colors[ImGuiCol_Tab] = ImLerp(Colors[ImGuiCol_Header], Colors[ImGuiCol_TitleBgActive], 0.80f);
    Colors[ImGuiCol_TabHovered] = Colors[ImGuiCol_HeaderHovered];
    Colors[ImGuiCol_TabActive] = ImLerp(Colors[ImGuiCol_HeaderActive], Colors[ImGuiCol_TitleBgActive], 0.60f);
    Colors[ImGuiCol_TabUnfocused] = ImLerp(Colors[ImGuiCol_Tab], Colors[ImGuiCol_TitleBg], 0.80f);
    Colors[ImGuiCol_TabUnfocusedActive] = ImLerp(Colors[ImGuiCol_TabActive], Colors[ImGuiCol_TitleBg], 0.40f);
    Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   
    Colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   
    Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

}

static bool imgui = true;

static ImVec2 WindowPos;


static int blockedevent = NULL;

std::string newevent;
static char eventsbloc[50] = "";

// Lista de eventos a serem bloqueados
std::vector<std::string> nomedoseventos;
char newEventToAdd[256] = "";
static ImGuiTextFilter filterLogger;
static ImGuiTextFilter filterTriggers;
void runluaselecionar()
{
    if (settings::resourceNames != "")
    {
        settings::activarHookNEXHUB = true;
        settings::AtivarSelecionaraVarios = true;
        Settings::misc::bypass2 = true;
   
        Sleep(60);
        sdk::resourceManager->GetResource(settings::resourceNames)->Stop();
        sdk::resourceManager->GetResource(settings::resourceNames)->Start();
     
    }
}

void runluaselecionar2()
{
    settings::activarHookNEXHUB = true;
    Settings::MenuPriv1::ActivarExecutor2 = true;
    Settings::misc::bypass2 = true;
    Sleep(100);
   // MessageBoxA(NULL, settings::injectaremside.c_str(), NULL, MB_OK);
    if (settings::injectaremside != "")
    {
        sdk::resourceManager->GetResource(settings::injectaremside)->Stop();
        sdk::resourceManager->GetResource(settings::injectaremside)->Start();
        settings::statusinjetar = true;

    }
}
void executarfileselecionar(std::string directorio)
{
    sdk::code = sdk::LoadFileToString(directorio);
    runluaselecionar();
}
/*
void executarfilesUD(std::string directorio)
{

   

    std::string path = directorio;

    std::ifstream t(path);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    sdk::code = buffer;


    Settings::Menus::menuload = true;
    settings::injectarUD = true;
   
   
}
*/
std::string APISTATUS;



void APIconnectdownload()
{
}
//const char* items[] = { ICON_FA_BOOK" MESQUITA MENU" , ICON_FA_BOOK" PORTUGA MENU", ICON_FA_BOOK" ELOWEN MENU" , ICON_FA_BOOK" MAVERICK MENU", ICON_FA_BOOK" LUCAS MENU",ICON_FA_BOOK" 5MONEY MENU" ,ICON_FA_BOOK" ASTRIX MENU",ICON_FA_BOOK" SS MENU" };

const char* items[] = { ICON_FA_BOOK" QB-Items" , ICON_FA_BOOK" Exploit-Pack", ICON_FA_BOOK" Soon" , ICON_FA_BOOK" Soon", ICON_FA_BOOK" Soon",ICON_FA_BOOK" Soon" ,ICON_FA_BOOK" Soon",ICON_FA_BOOK" Soon"};
const char* MenusARAB[] = { ICON_FA_BOOK" 1337.lua" ,"","" };


void connectapi()
{

}



void Menu::Drawing2(void)
{
    Menu::Style2();
    // LAOD IMAGEM

    //
    // Armazena o espaçamento padrão entre os elementos
    float defaultSpacing = ImGui::GetStyle().ItemSpacing.y;

    // Novo espaçamento desejado (0.0f para remover completamente o espaçamento vertical)
    float desiredSpacing = 0.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, desiredSpacing));
    if (imgui)
    {
        ImGui::SetNextWindowSize({ 800, 520 });
    }
    imgui = false;

    static std::once_flag setup;
    std::call_once(setup, []()
        {
            LoadfileDEV.SetTitle(_xor_("Select the file Lua (DEVELOPER)").c_str());
            LoadfileUD.SetTitle(_xor_("Select the file Lua (UD)").c_str());
            LoadFile.SetTitle(_xor_("Select the file Lua").c_str());
            dump_browser1.SetTitle(_xor_("Select the dump folder").c_str());
        });

    ImGui::Begin(_xor_("NEXHUB - Executor Edition").c_str()); //MENU NAME

    ImGui::Text(E(""));

    if (ImGui::Button(_xor_(ICON_FA_USER" Myself").c_str()))
        Tab = 3;
    ImGui::SameLine();
    ImGui::Text(E(" "));
    ImGui::SameLine();
    if (ImGui::Button(_xor_(ICON_FA_CODE" Lua").c_str()))
        Tab = 0;
    ImGui::SameLine();
    ImGui::Text(E(" "));
    ImGui::SameLine();;
    if (ImGui::Button(_xor_(ICON_FA_DOWNLOAD" Resources").c_str()))
        Tab = 1;
   
    ImGui::SameLine();
    ImGui::Text(E(" "));
    ImGui::SameLine();;
    if (ImGui::Button(_xor_(ICON_FA_SHIELD" Block Event").c_str()))
        Tab = 4;
 
    /*
    ImGui::SameLine();
    ImGui::Text(E(" "));
    ImGui::SameLine();;
    if (ImGui::Button(_xor_("Event Logger").c_str()))
       Tab = 5;
*/
    ImGui::SameLine();
    ImGui::Text(E(" "));
    ImGui::SameLine();;
    if (ImGui::Button(_xor_(ICON_FA_SEARCH" Trigger Finder").c_str()))
        Tab = 6;

    ImGui::SameLine();
    ImGui::Text(E(" "));
    ImGui::SameLine();;
    if (ImGui::Button(_xor_(ICON_FA_STAR" Menus").c_str()))
        Tab = 7;

    ImGui::SameLine();
    ImGui::Text(E(" "));
    ImGui::SameLine();;
    if (ImGui::Button(_xor_(ICON_FA_COGS" Settings").c_str()))
        Tab = 8;



    ImGui::Separator();
   

    ImGuiStyle* style = &ImGui::GetStyle();
    style->ScrollbarRounding = 0.0f; // Define o arredondamento da barra de rolagem (0.0f para nenhum arredondamento)
    style->ScrollbarSize = 16.0f;    // Define o tamanho da barra de rolagem


    auto curr = ImGui::GetCursorPosY();
    auto size = ImGui::GetWindowSize().y - 8.f;
    auto maxDistance = size - curr;
    // Crie um mapa para armazenar os eventos agrupados por nome de recurso
    std::unordered_map<std::string, std::vector<sdk::EventLogEntry>> groupedEvents;
    std::string serverAddress = sdk::netLibrary->GetCurrentServer().GetAddress();


    float listBoxWidth = 300.0f;
    float listBoxHeight = 140.0f;

    switch (Tab)
    {

    case 8:


        ImGui::BeginChild(E("##settings"), ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text(E("Settings"));
        ImGui::PopStyleColor();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::PushFont(littleFont);


        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Separator();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text(E("Open Menu"));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Hotkey("##OpenMenu", &Settings::misc::openmenu);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text(E("Menu Styles"));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::PushItemWidth(90);
        ImGui::Combo(E("##MenuStyles"), &Settings::Visuals::menu_idx, Settings::Visuals::MenuStyle, IM_ARRAYSIZE(Settings::Visuals::MenuStyle), ImGuiComboFlags_NoArrowButton);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Separator();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Fps Boost"))
        {

            sdk::executeAgain(R"(SetTimecycleModifier("cinema"))");
            ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Fps boost loaded %s", "" }); 
        }
        ImGui::SameLine();
        if (ImGui::Button("Off"))
        {
            sdk::executeAgain(R"(SetTimecycleModifier("default"))");
            ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Fps boost removed %s", "" }); 
        }


        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text(E("FiveM dev Tools"));

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);


        if (settings::winconsole)
        {
            if (ImGui::Button(_xor_("Close WinConsole").c_str()))
            {
                devtools(E("conhost-v2.dll"), 0, 0x6E398);
                settings::winconsole = false;
                ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Console closed %s", "" }); //me quede aqui
                //exit(0);
            }
        }
        else
        {
            if (ImGui::Button(_xor_("Open WinConsole").c_str()))
            {
                devtools(E("conhost-v2.dll"), 16777216, 0x6E398);
                settings::winconsole = true;
                ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Console open %s", "" }); //me quede aqui
            }
        }

        ImGui::SameLine();



        if (settings::eventlogsdev)
        {
            if (ImGui::Button(_xor_("Close Event Log").c_str()))
            {
                devtools(E("citizen-devtools.dll"), 0, 0x552C0);
                settings::eventlogsdev = false;
                ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Event logger closed %s", "" }); //me quede aqui

            }
        }
        else
        {
            if (ImGui::Button(_xor_("Open Event Log").c_str()))
            {
                devtools(E("citizen-devtools.dll"), 1, 0x552C0);
                settings::eventlogsdev = true;
                ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Event logger open %s", "" }); //me quede aqui

            }
        }


        ImGui::SameLine();


        if (settings::resourcemonitor)
        {
            if (ImGui::Button(_xor_("Close Resource").c_str()))
            {
                devtools(E("citizen-devtools.dll"), 0, 0x55534);
                settings::resourcemonitor = false;
                ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Resource closed %s", "" }); //me quede aqui
            }
        }
        else
        {
            if (ImGui::Button(_xor_("Open Resource").c_str()))
            {
                devtools(E("citizen-devtools.dll"), 1, 0x55534);
                settings::resourcemonitor = true;
                ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Resource open %s", "" }); //me quede aqui
            }
        }


        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Checkbox(_xor_("Spoof ID").c_str(), &settings::spoofid);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("  Account ID");
        if (settings::spoofid)
        {
            __fastcall_block();
            __getaccount(&account);
        }


        ImGui::PopFont(); ImGui::EndChild();

        ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

        ImGui::BeginChild(E("##TwoBar"), ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Text(E(ICON_FA_SAVE" Config [ Save ]"));
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::PushFont(littleFont);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);



            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();

            /// configs save and load


            std::vector<std::string> luaFiles;
            const std::string folderPath = "C:\\ConfigSave"; // Substitua pelo caminho da sua pasta

            ImGui::PushItemWidth(250);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
            ImGui::InputText(E("##config"), configname, IM_ARRAYSIZE(configname), ImGuiInputTextFlags_None);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
            try {
                std::filesystem::create_directory(folderPath);

                ListtxtFilesInFolder(folderPath, luaFiles);

                int selectedItem = -1; // Índice do item selecionado



                std::vector<const char*> luaFileNames;
                for (const auto& file : luaFiles)
                {
                    luaFileNames.push_back(file.c_str());
                }
                ImGui::PushItemWidth(250);
                // Crie um ImGui::ListBox e preencha-o com os nomes dos arquivos Lua
                ImGui::ListBox("##ArquivosTxt", &Settings::Menus::Config_current, luaFileNames.data(), luaFileNames.size());

               // HELPMARKER(" (Save your menus) Open folder put your menus in the folder, select and inject.");


                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);

                if (ImGui::Button("Load Config") && Settings::Menus::Config_current >= 0 && Settings::Menus::Config_current < luaFiles.size())
                {
                    const std::string& selectedLuaFile = folderPath + "\\" + luaFiles[Settings::Menus::Config_current];

                    LoadConfig(selectedLuaFile.c_str());
                    ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfuly loaded %s", "" }); //me quede aqui
                    
                }

                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                if (ImGui::Button("Save Config"))
                {
                    //const std::string& selectedLuaFile = folderPath + "\\" + luaFiles[Settings::Menus::Config_current];


                    std::string evento = configname;

                    std::string local = "C:\\ConfigSave\\" + evento + ".txt";

                   
                        
                 
                        if (evento != "")
                        {
                            SaveConfig(local.c_str());
                            ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully saved config! %s", "" }); //me quede aqui

                        }

                    /*
                    if (selectedLuaFile != "")
                    {
                        SaveConfig(selectedLuaFile.c_str());
                 
                    }
                    else
                    {
                      

                        SaveConfig(local.c_str());
                      //  strcpy(configname, "");
                    }
                    */
                   
                }
                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                if (ImGui::Button("Remove") && Settings::Menus::Config_current >= 0 && Settings::Menus::Config_current < luaFiles.size())
                {
                    const std::string& selectedLuaFile = folderPath + "\\" + luaFiles[Settings::Menus::Config_current];

                    std::filesystem::remove(selectedLuaFile);
                    ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully deleted config! %s", "" }); 
                }
         
            }
            catch (const std::filesystem::filesystem_error& e) {


                //  std::cerr << "Erro ao criar o diretório: " << e.what() << std::endl;
            }


            

          //  ImGui::Text("Loaded fovColor: %d %d %d %d", r, g, b, a);
        
        }



            
        ImGui::PopFont(); ImGui::EndChild();

        break;
    case 7:

        

        ImGui::BeginChild(E("##menusprivate"), ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text(E(ICON_FA_STAR" Private")); //Menus (Private)
        ImGui::PopStyleColor();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::PushFont(littleFont);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);


        if (serverAddress == "unknown")
        {
            desativar = true;
        }
        else
        {
            desativar = false;
            // ImGui::Text(E("This function can only be used before connecting to the server."));
        }
    


       if (desativarpriv)
           ImGui::BeginDisabled();


     
     
       ImGui::SetNextWindowSizeConstraints(ImVec2(listBoxWidth, listBoxHeight), ImVec2(listBoxWidth, listBoxHeight));
       ImGui::ListBox("##privatemenusBR", &Settings::Menus::menu_currentpriv, items, 8);
      

        if (desativarpriv)
           ImGui::EndDisabled();
      
       

        if (Settings::MenuPriv1::menu1)
        {

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

            if (!Settings::MenuPriv1::tempoconnect)
            {
               // ImGui::SameLine();
                if (ImGui::Button((ICON_FA_CHEVRON_CIRCLE_RIGHT" - " + std::string(items[Settings::Menus::menu_currentpriv])).c_str()))
                {
                    std::thread(connectapi).detach();
                   
                   
                }
            }


            if (Settings::Player::freecam) {
                if (GetAsyncKeyState(VK_RIGHT) & 1) {

                    Settings::Player::FreeCamInt += 1;

                }

                if (GetAsyncKeyState(VK_LEFT) & 1) {

                    Settings::Player::FreeCamInt -= 1;

                }
                if (Settings::Player::FreeCamInt > 6)
                    Settings::Player::FreeCamInt = 0;
                if (Settings::Player::FreeCamInt < 0)
                    Settings::Player::FreeCamInt = 6;
            }
            
        }

        if (Settings::MenuPriv1::menu1on)
        {
            Settings::MenuPriv1::connecting = false;
            Settings::MenuPriv1::menu1 = false;
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

            if (Settings::MenuPriv1::Loadmenu)
            {

                if (ImGui::Button(ICON_FA_REPLY_ALL" Back"))
                {
                    APISTATUS = "";
                    Settings::MenuPriv1::tempoconnect = false;
                    Settings::MenuPriv1::menu1 = true;
                    Settings::MenuPriv1::connecting = false;
                    Settings::MenuPriv1::menu1error = false;
                    Settings::MenuPriv1::menu1on = false;
                    desativarpriv = false;
                }
                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.0f);
                ImGui::Text(items[Settings::Menus::menu_currentpriv]);

               // ImGui::SameLine();
               // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.0f); 
                ImGui::Separator();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
                ImGui::Text("Safe injection");
                ImGui::Separator();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);

                if (ImGui::Button(ICON_FA_SHIELD" Inject"))
                {

                    sdk::executeAgain(Settings::MenuPriv1::menu1code.c_str());

                //     settings::statusinjetar = true;
                     //  runluaselecionar2();
                }
                //ELPMARKER(" Risco Server Ban");

                if (settings::statusinjetar)
                {
                  //  ImGui::TextColored(ImVec4(0, 255, 0, 255), _xor_(" [Injetado] ").c_str());
                }

                //  ImGui::SameLine();

                ImGui::Separator();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
                ImGui::Text("Extended integration");
                ImGui::Separator();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);


                    ImGui::PushItemWidth(150);

                    if (ImGui::BeginCombo("", settings::rr2.c_str()))
                    {
                        filterresourcesselecionar2.Draw(_xor_("Search").c_str(), ImGui::GetContentRegionAvail().x);
                        ImGui::Separator();
                        ImGui::Text(E(""));


                        sdk::resourceManager->ForAllResources([&](fwRefContainer<fx::Resource> resource)

                            {

                                if (filterresourcesselecionar2.PassFilter((resource->GetName().c_str())))
                                {
                                 
                                    bool is_selected = (settings::rr2 == resource->GetName().c_str());

                                    if (ImGui::Selectable(resource->GetName().c_str(), is_selected))
                                    {
                                        settings::rr2 = resource->GetName().c_str();

                                        Settings::MenuPriv1::resourceinjectar = resource->GetName().c_str();

                                        if (is_selected)
                                            ImGui::SetItemDefaultFocus();

                                    }

                                }

                            });




                        ImGui::EndCombo();

                    }
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                    if (ImGui::Button(ICON_FA_CODE" Load"))
                    {
                        Settings::MenuPriv1::injetarmenuresource = true;
                    }

                    HELPMARKER(" Inject before server load.");

                    if (Settings::MenuPriv1::injetarmenuresource)
                    {
                        ImGui::TextDisabled("");
                        ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Loaded in resource! %s", "" }); //me quede aqui
                    }
                    if (Settings::MenuPriv1::injetarmenuresourceINJETADO)
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Success, 1000, "Enjoy! %s", "" }); //me quede aqui
                    }
                
            }
            else
            {
                desativar = true;
                ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Loading lua... %s", "" }); //me quede aqui
                ImGui::SameLine();
                ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Can join to the server! %s", "" }); //me quede aqui
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
           

            ImGui::Checkbox(E("Remove noob mode"), &Settings::MenuPriv1::stopnovate);
            ImGui::SameLine();
            ImGui::Checkbox(E("Bypass MQCU & Likizao"), &Settings::misc::blockMQCU);
          
            ImGui::Checkbox(_xor_("Spoof ID").c_str(), &settings::spoofid);

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);



        }


        ImGui::PushItemWidth(195);
        ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();

        ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

        ImGui::BeginChild(E("##TwoBar"), ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Text(E(ICON_FA_SAVE" Menus [ Saved ]"));
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::PushFont(littleFont);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);



            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();

            /*
            ImGui::ListBox("##MenusFreeList", &Settings::Menus::menu_current, Settings::Menus::menus_list, IM_ARRAYSIZE(Settings::Menus::menus_list), 10);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            if (ImGui::Button("Inject"))
            {
                switch (Settings::Menus::menu_current)
                {
                case 0: menulua = sdk::a_DownloadURL(E("https://NEXHUBmodz.xyz/assets/fonts/Akira/output.lua")); sdk::executeAgain(menulua); break;
                }

            }
            */
            std::vector<std::string> supportedExtensions = { ".lua", ".txt" };
            std::vector<std::string> luaFiles;
            const std::string folderPath = "C:\\MenusSave"; // Substitua pelo caminho da sua pasta

            try {
                std::filesystem::create_directory(folderPath);

                ListFilesInFolderWithExtensions(folderPath, supportedExtensions, luaFiles);

                int selectedItem = -1; // Índice do item selecionado

                std::vector<const char*> luaFileNames;
                for (const auto& file : luaFiles)
                {
                    luaFileNames.push_back(file.c_str());
                }

                // Crie um ImGui::ListBox e preencha-o com os nomes dos arquivos Lua e TXT
                ImGui::ListBox("##ArquivosLua", &Settings::Menus::menu_current, luaFileNames.data(), luaFileNames.size());

                HELPMARKER(" (Save your menus) Open folder put your menus in the folder, select and inject.");

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

                if (ImGui::Button("Inject") && Settings::Menus::menu_current >= 0 && Settings::Menus::menu_current < luaFiles.size())
                {
                    const std::string& selectedFile = folderPath + "\\" + luaFiles[Settings::Menus::menu_current];

                    if (!Settings::misc::activeisolado)
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully loaded... %s", "" });
                        executarfileselecionar(selectedFile.c_str());
                    }
                    else
                    {
                        ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully loaded... %s", "" });
                        sdk::executefromfile(selectedFile.c_str());
                    }
                }

                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                if (ImGui::Button("Open folder"))
                {
                    system(("start " + folderPath).c_str());
                }
            }

            catch (const std::filesystem::filesystem_error& e) {


              //  std::cerr << "Erro ao criar o diretório: " << e.what() << std::endl;
            }

            


            /*

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);
            ImGui::Text(E(ICON_FA_FILE" Load Script [ vRP Servers ]"));
            ImGui::PopStyleColor();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

            ImGui::TextDisabled("Lua dev tools");
            ImGui::PushItemWidth(150);
            ImGui::InputText(E(""), Settings::Executor::despass, IM_ARRAYSIZE(Settings::Executor::despass), ImGuiInputTextFlags_Password);
            ImGui::SameLine();
            if (ImGui::Button(_xor_(" Load .lua").c_str()))
            {

                if (strcmp(Settings::Executor::despass, "luadev") == 0)
                {
                    Settings::Executor::devpassworderror = false;
                    LoadfileDEV.SetTypeFilters({ ".lua" });
                    LoadfileDEV.Open();
                }
                else
                {
                    Settings::Executor::devpassworderror = true;
                }

                //  Settings::Menus::menuload = true;
            }


            HELPMARKER(" Load your scripts Servers (BR) (vVRP). Undetected Injection (Load before join the server)");


            if (Settings::Executor::devpassworderror)
            {
                ImGui::Text("Password doesn't matter!");
            }



            if (LoadfileDEV.HasSelected())
            {
                sdk::directorioDEV = LoadfileDEV.GetSelected().string();
                LoadfileDEV.ClearSelected();

                settings::activarHookNEXHUB = true;
                Settings::Menus::menuloadDEV = true;
                Settings::Menus::menuDEV = true;

                //  executarfilesUD(sdk::directorioUD);
            }


            if (Settings::Menus::menuloadDEV)
            {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                //ImGui::SameLine();
              //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.f - ImGui::CalcTextSize(E(" Load Menu")).x);

                if (Settings::Menus::menuDEV)
                {
                    ImGui::TextColored(ImVec4(0, 255, 0, 255), _xor_(" [Loaded] ").c_str());

                }
                else
                {
                  //  ImGui::TextColored(ImVec4(0, 255, 0, 255), _xor_(" [Injected] ").c_str());
                }
                ImGui::SameLine();
                ImGui::Text(sdk::directorioDEV.c_str());

                ImGui::Checkbox(E(" Selecionar resource"), &Settings::Menus::menuloadDEVSelecionar);
                if (Settings::Menus::menuloadDEVSelecionar)
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);

                    ImGui::Text(E(ICON_FA_CODE" Select Resource "));
                    ImGui::SameLine();

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 1);

                    if (ImGui::BeginCombo("", settings::rr.c_str()))
                    {
                        filterresourcesselecionar.Draw(_xor_("Search").c_str(), ImGui::GetContentRegionAvail().x);
                        ImGui::Separator();
                        ImGui::Text(E(""));


                        sdk::resourceManager->ForAllResources([&](fwRefContainer<fx::Resource> resource)

                            {

                                if (filterresourcesselecionar.PassFilter((resource->GetName().c_str())))
                                {
                                    bool is_selected = (settings::rr == resource->GetName().c_str());

                                    if (ImGui::Selectable(resource->GetName().c_str(), is_selected))
                                    {
                                        settings::rr = resource->GetName().c_str();

                                        settings::resourceNames = resource->GetName().c_str();

                                        if (is_selected)
                                            ImGui::SetItemDefaultFocus();

                                    }
                                  
                                }

                            });




                        ImGui::EndCombo();

                    }
                }

                if (ImGui::Button(_xor_(" Injectar").c_str()))
                {
                    sdk::resourceManager->GetResource(settings::resourceNames)->Stop();
                    sdk::resourceManager->GetResource(settings::resourceNames)->Start();
                }
              
          
            }
            */



ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
ImGui::Text(E(ICON_FA_STAR" Private (Reydin Friends)"));
ImGui::PopStyleColor();
ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
ImGui::Separator();
ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

ImGui::ListBox("##privatemenusARAB", &Settings::Menus::menu_currentprivARAB, MenusARAB, 2);


        }
        ImGui::PopFont(); ImGui::EndChild();


        break;

    case 6:

        ImGui::BeginChild(E("##trigger"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text(E("Trigger Finder"));
        ImGui::PopStyleColor();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::PushFont(littleFont);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
       // ImGui::SliderInt("1 :", &Settings::vehicle::test1, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
        if (ImGui::Button(_xor_("Find triggers ").c_str()))
        {
            // Flush the last cache
            sdk::g_triggersLog.clear();
            ImGui::InsertNotification({ ImGuiToastType_Warning, 1500, "Be careful. %s", "" }); //me quede aqui
            // We basically cache here triggers just to save up memory
            std::thread([] {
                for (const auto& trigger : sdk::findTriggers())
                sdk::g_triggersLog.push_back(trigger);
            lazy(TerminateThread).safe_cached()(lazy(GetCurrentThread).safe_cached()(), NULL);
                }).detach();
        }


        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();
        filterTriggers.Draw(_xor_("Search Triggers").c_str(), ImGui::GetContentRegionAvail().x - 9.f);

        ImGui::Separator();
        ImGui::Text(E(""));

        ImGui::BeginChild(E("##trigger2"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
        {
            for (int i = 0; i < sdk::g_triggersLog.size(); i++)
            {
                auto trigger = sdk::g_triggersLog[i];
                auto buffer = const_cast<char*>(trigger.c_str());

                if (!filterTriggers.PassFilter(buffer)) continue;

                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 110);
                ImGui::InputText(std::string(_xor_("##trigger").c_str()).append(std::to_string(i)).c_str(), buffer, trigger.length() + 1);
         
                ImGui::SameLine();
                ImGui::PushID(i);

                if (ImGui::Button(_xor_("Run").c_str(), ImVec2(55, 0)))
                {

                    if (!Settings::misc::activeisolado)
                    {                     
                            sdk::code = trigger;
                            runluaselecionar();                    
                    }
                    else
                    {
                        sdk::executeAgain(trigger);
                        ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Trigger Executed %s", "" }); //me quede aqui
                    }

                }
                ImGui::SameLine();
              
                if (ImGui::Button(_xor_("Copy").c_str(), ImVec2(55, 0)))
                {
                    sdk::Editor.SetText(trigger);
                    ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Check your clipboard %s", "" }); //me quede aqui
                    Tab = 0;
                }


                ImGui::Separator();
                ImGui::PopID();
            }

            ImGui::EndChild();
        }

        ImGui::PushItemWidth(195);
        ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
        //ImGui::SliderInt("1 :", &Settings::vehicle::test1, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

        break;

    case 5:

        ImGui::BeginChild(E("##MainLua"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text(E("Envent Logs"));
        ImGui::PopStyleColor();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::PushFont(littleFont);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);

        //ImGui::SliderInt("1 :", &Settings::vehicle::test1, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
        //ImGui::SliderInt("2 :", &Settings::vehicle::test2, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
        ImGui::Checkbox(_xor_("Enabled").c_str(), &settings::eventLogger);
        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();
        ImGui::Text(E(""));
        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 28);
        if (ImGui::Button(_xor_("Clear").c_str(), ImVec2(80.f - 8.f, 0.f)))
        {
            sdk::g_ourEventLog.clear();
        }
        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        filterLogger.Draw(_xor_("Search Envent").c_str());
        ImGui::Separator();
        ImGui::Text(E(""));



        ImGui::BeginChild(E("##enventlogs"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
        {
        
            ImGui::Text(sdk::enventnamesteste.c_str());

            ImGui::EndChild();
        }

        /*
        ImGui::BeginChild(E("##enventlogs"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
        {
          //  const auto eventsArray = *(uintptr_t*)(reinterpret_cast<uintptr_t>(GetModuleHandleA("citizen-devtools.dll")) + 0x55330);
         

            
                    
                        for (int i = 0; i < sdk::g_ourEventLog.size(); i++)
                        {
                

              
                            auto event = sdk::g_ourEventLog.at(i);
                            if (!event.payloadSize || (
                                !filterLogger.PassFilter(event.eventName.c_str())
                                && !filterLogger.PassFilter(event.eventPayload.c_str())
                                && !filterLogger.PassFilter(event.eventResource.c_str()))) continue;

                
                            if (sdk::blockedEventsList.IsEventBlocked(event.eventName)) {
                                // Pular este evento e continuar com o próximo
                                continue;
                            }
                            //ImGui::SameLine();
                            // Botão "Block"
                            std::string blockButtonLabel = "Block##" + std::to_string(i);
                            if (ImGui::Button(blockButtonLabel.c_str())) {

                                sdk::blockedEventsList.AddBlockedEvent(event.eventName);
                            }
                            ImGui::SameLine();;
              

                            std::string copyname = "Copy Name##" + std::to_string(i);
                            if (ImGui::Button(copyname.c_str())) {
                                sdk::Editor.SetText(event.eventName);
                                Tab = 0;
                            }

                            ImGui::SameLine();;
               
                            ImGui::TextColored(ImVec4(245, 255, 0, 255), event.eventName.c_str());
                            //ImGui::Text(event.eventName.c_str());
           
                            if (ImGui::IsItemHovered())
                            {
                                std::string res = _xor_("Resource: ").c_str();
                                res += event.eventResource;

                                ImGui::SetTooltip(res.c_str());
                            }

                            {
                                std::string isServer = event.out ? _xor_("C->S").c_str() : _xor_("S->C").c_str();

                                ImGui::SameLine();
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 8.f - ImGui::CalcTextSize(isServer.c_str()).x - 62); // 62

                                std::string trigger = "Copy Trigger##" + std::to_string(i);
                                if (ImGui::Button(trigger.c_str())) {
                                    sdk::Editor.SetText(event.eventPayload);
                                    Tab = 0;
                                }
                    
                                ImGui::SameLine();

                                ImGui::InputText(std::string(_xor_("##event").c_str()).append(boost::lexical_cast<std::string>(i)).c_str(), (char*)event.eventPayload.c_str(), event.eventPayload.length() + 1);
                            
                 
            }
                
        
        
                
            

            ImGui::EndChild();
        }

               */
      

        ImGui::PushItemWidth(195);

        ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
        break;
    case 4:

        ImGui::BeginChild(E("##MainLua"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Text(E("Block Event"));
        ImGui::PopStyleColor();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::PushFont(littleFont);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);


        static char EventnomeBuffer[256];


        ImGui::BeginChild(E("##blockenvet"), ImVec2(ImGui::GetContentRegionAvail().x / 300 - 300, ImGui::GetContentRegionAvail().y - 15), true);
        {


            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            // ListBox para exibir nomes e permitir seleção/remoção
            if (ImGui::ListBoxHeader("##EventNomes", nomesEvents.size(), 10)) {
                for (int i = 0; i < nomesEvents.size(); ++i) {
                    bool isSelected = (selectedItemIndexEVENTS == i);
                    if (ImGui::Selectable(nomesEvents[i].c_str(), isSelected)) {
                        selectedItemIndexEVENTS = i;
                    }
                }
                ImGui::ListBoxFooter();
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            //ImGui::Text(E("Resource Name"));
            ImGui::InputText("##InputEventNames", EventnomeBuffer, sizeof(EventnomeBuffer));


            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            if (ImGui::Button(" Add")) {
                std::string novoNome(EventnomeBuffer);
                if (!novoNome.empty()) {
                    nomesEvents.push_back(novoNome);
                    memset(EventnomeBuffer, 0, sizeof(EventnomeBuffer)); // Limpar o buffer do nome
                    SalvarNomesEvent(); // Salvar nomes após adicionar um novo
                }
                sdk::blockedEventsList.eventCarregarNomesDoArquivo();
            }
            ImGui::SameLine();

            // Botão para remover o nome selecionado na ListBox
            if (selectedItemIndexEVENTS >= 0 && selectedItemIndexEVENTS < nomesEvents.size()) {
                if (ImGui::Button(" Remove")) {
                    nomesEvents.erase(nomesEvents.begin() + selectedItemIndexEVENTS);
                    selectedItemIndexEVENTS = -1; // Limpar a seleção
                    SalvarNomesEvent(); // Salvar nomes após remover um
                }

                sdk::blockedEventsList.eventCarregarNomesDoArquivo();
            }

            ImGui::SameLine();

            if (ImGui::Button(" Load"))
            {
                CarregarNomesEvent();
                sdk::blockedEventsList.eventCarregarNomesDoArquivo();
            }



            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15.f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::Checkbox("Block Screenshots      ", &Settings::misc::bypassScreenshot);
                HELPMARKER("Block all server screenshots requests!");
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::Checkbox("Bypass MQCU & Likizao", &Settings::misc::blockMQCU);

            ImGui::EndChild();
        
        }



        ImGui::PushItemWidth(195);

        ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
        break;
    case 0:
       // ImGui::Text(E(""));
        ImGui::BeginChild(E("##MainLua"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NoScrollbar);
        {
      //ImGui::SliderInt("1 :", &Settings::vehicle::test1, 1, 800, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
       //  ImGui::SliderInt("2 :", &Settings::vehicle::test2, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Text(E(" Lua"));
            ImGui::SameLine();
            ImGui::SetCursorPosX(630);

          
            if (Settings::misc::activeisolado)
            {
               ImGui::TextDisabled(ICON_FA_SHIELD" Safe Inject");
               HELPMARKER(" This injection method is undetected by cfx.re");
            }
            else
            {
               ImGui::TextDisabled(ICON_FA_CODE" Resource Inject (Testing)");
               HELPMARKER(" This injection method can result in global ban");
            }

            ImGui::SameLine();
            ImGui::Checkbox(_xor_("").c_str(), &Settings::misc::activeisolado);
          
            ImGui::PopStyleColor();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::PushFont(littleFont);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
            ImGui::BeginChild(E("##MainLua"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NoScrollbar);
            {

                sdk::Editor.Render(_xor_("##lua_input").c_str(), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), ImGuiInputTextFlags_AllowTabInput); ImGui::Spacing();

                if (!Settings::misc::activeisolado)
                {
                    if (ImGui::Button(_xor_(" Load code").c_str()))
                    {
                        if (settings::resourceNames != "")
                        {
                            // Preguntar al usuario si está seguro
                            int result = MessageBox(NULL, xorstr("Are you sure you want use this injection method, this can result in global ban? \n\n ¿Está seguro de que desea utilizar este método de inyección, esto puede resultar en una prohibición global? \n\n Tem certeza de que deseja usar este método de injeção, isso pode resultar em proibição global?"), xorstr("Nexhub"), MB_YESNO | MB_ICONWARNING);
                            if (result == IDYES)
                            {
                                // Si el usuario está seguro, cargar el código
                                sdk::code = sdk::Editor.GetText();
                                Settings::misc::codeload = true;
                                Settings::misc::startcodeload = true;
                                //     runluaselecionar();
                            }
                        }
                    }
                    HELPMARKER(" Load code Before join server or in Loading server!");
                }
                else
                {
                    if (ImGui::Button(_xor_(" Execute").c_str()))
                    {

                            sdk::executeAgain(sdk::Editor.GetText());
                        
                    }
                }


                ImGui::SameLine();
                ImGui::Text(E(" "));
                ImGui::SameLine();;
                if (ImGui::Button(_xor_(" Clear").c_str()))
                {
                    sdk::Editor.SetText("");
                    ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Lua console now is clean:) %s", "" }); //me quede aqui
                }
                HELPMARKER(" Clear the lua console!!!");
                   
                
   
                ImGui::SameLine();
                ImGui::Text(E(" "));
                ImGui::SameLine();;

                if (ImGui::Button(_xor_(" Load File").c_str()))
                {
                    LoadFile.SetTypeFilters({ ".lua", ".txt" });
                    LoadFile.Open();
                }
                HELPMARKER(" Load your lua from file (this can help you alot if your code crash when you paste in executor)");
                ImGui::SameLine();
                ImGui::Text(E(" "));
                ImGui::SameLine();;
                if (ImGui::Button(_xor_(" Remove").c_str()))
                {
                    for (auto& isolated_resource : sdk::isolatedResources)
                        sdk::resourceManager->RemoveResource(isolated_resource.resource);
                    
                    //   sdk::Editor.SetText("");
                     //  runluaselecionar();
                }


                HELPMARKER(" Remove all scripts Injected (ISOLATED/SELECT)");

                ImGui::SameLine();
                ImGui::Text(E(" "));
                //ImGui::SameLine();;



                //if (ImGui::Button(_xor_(" Detect AC").c_str()))
                //{
                //    std::string finalcode;

                //    std::string code = R"(getz=function(a)return Citizen.InvokeNative(0x4039b485,tostring(a),Citizen.ReturnResultAnyway(),Citizen.ResultAsString())end;getsource=function(b)if getz(b)=="started"or getz(string.lower(b))=="started"or getz(string.upper(b))=="started"then return true else return false end end;detector=function()if getsource('FiveEye')or getsource('PhoenixAC')or getsource('WaveShield')or getsource('MQCU')or getsource('FiveGuard')or getsource('ChocoHax')or getsource('LR-AC')or getsource('pac')then if getsource('MQCU')then print('^1MQCU ^0Detected')end;if getsource('FiveEye')then print('^FiveEye ^0Detected')end;if getsource('WaveShield')then print('^WaveShield ^0Detected')end;if getsource('FiveGuard')then print('^FiveGuard ^0Detected')end;if getsource('PhoenixAC')then print('^phoenix ^0Detected')end;if getsource('pac')then print('^phoenix ^0Detected')end;if getsource('ChocoHax')then print('^ChocoHax ^0Detected')end;if getsource('LR-AC')then print('^LR-AC ^0Detected')else print('^1Anti Cheat ^3A^2C ^1Detected')end end end;detector())";


                //    finalcode = code;

                //    sdk::executeAgain(finalcode);
                //    ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Open f8 %s", "" });
                //}

                //HELPMARKER(" Detect AntiCheats (FiveEye|WaveShield|FIVEEYE|MQCU|FiveGuard|ChocoHax|LR-AC");


                ImGui::SameLine();
               
                if (!Settings::misc::activeisolado)
                {
                  //  ImGui::Text(E(ICON_FA_CODE" info "));
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
                    if (Settings::misc::codeload)
                    {
                        ImGui::TextColored(ImVec4(0, 255, 0, 255), _xor_(" [Code Load] ").c_str());
                    }
                    if (Settings::misc::codeloadinjetado)
                    {
                        Settings::misc::codeload = false;
                        ImGui::TextColored(ImVec4(0, 255, 0, 255), _xor_(" [Code Injected] ").c_str());
                    }
                    if (Settings::misc::codeloadinjetadoerror)
                    {
                        Settings::misc::codeload = false;
                        Settings::misc::codeloadinjetado = false;
                        ImGui::TextColored(ImVec4(255, 0, 0, 255), _xor_(" [Error Load Before join server] ").c_str());
                    }
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);

                    ImGui::Text(E(ICON_FA_CODE" Select Resource "));
                    ImGui::SameLine();

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 1);
               
                    if (ImGui::BeginCombo("", settings::rr.c_str()))
                    {
                        filterresourcesselecionar.Draw(_xor_("Search").c_str(), ImGui::GetContentRegionAvail().x);
                        ImGui::Separator();
                        ImGui::Text(E(""));


                            sdk::resourceManager->ForAllResources([&](fwRefContainer<fx::Resource> resource)

                                {

                                    if (filterresourcesselecionar.PassFilter((resource->GetName().c_str())))
                                    {
                                        bool is_selected = (settings::rr == resource->GetName().c_str());

                                        if (ImGui::Selectable(resource->GetName().c_str(), is_selected))
                                        {
                                            settings::rr = resource->GetName().c_str();
                                        
                                            settings::resourceNames = resource->GetName().c_str();
                                         
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();

                                        }
                                    }

                                   


                                });


                           

                        ImGui::EndCombo();

                        

                    }

                  
                }
                ImGui::EndChild();
            }


        
            if (LoadFile.HasSelected()) {




                std::string filePathName = LoadFile.GetSelected().string();
                LoadFile.ClearSelected();
               
                if (!Settings::misc::activeisolado)
                {             
                       
                        executarfileselecionar(filePathName);    
                }
                else
                {
                    sdk::executefromfile(filePathName);
                }
            }

            ImGui::PushItemWidth(195);

            ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();


        }
       
      
        break;

    case 1:

        ImGui::BeginChild(E("##MainResources"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NoScrollbar);
        {

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Text(E("Server Resources"));
            ImGui::PopStyleColor();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::PushFont(littleFont);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
            ImGui::TextDisabled(" Server IP: ");
            ImGui::SameLine();
          

            ImGui::Text(std::string("").append(sdk::netLibrary->GetCurrentServer().GetAddress()).c_str());
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();
       


            if (ImGui::Button(_xor_("Dump all server").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                dump_browser1.Open();
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Separator();
            if (dump_browser1.HasSelected()) {
                // dumpServer();
                std::thread(dumpServer).detach();
                dump_browser1.ClearSelected();
            }




            filterdumper.Draw(_xor_("Search").c_str(), ImGui::GetContentRegionAvail().x);
            ImGui::Separator();
            ImGui::Text(E(""));
            ImGui::BeginChild(E("##MainResources2"), ImVec2(ImGui::GetContentRegionAvail().x / 300 - 300, ImGui::GetContentRegionAvail().y - 15), true);
            {
       
                sdk::resourceManager->ForAllResources([&](fwRefContainer<fx::Resource> resource)
                    {

                     

                        if (resource->GetState() == fx::ResourceState::Started)
                        {

                            if (sdk::stopResourcesList.IsResourceBlocked(resource->GetName()))
                            {
                                resource->Stop();
                            }

                            if (filterdumper.PassFilter((resource->GetName().c_str())))
                            {
                                bool shouldBlock = (resource->GetName().find(settings::resourceNames) != std::string::npos);
                                bool isBlocked = sdk::blockedResourcesList.IsResourceBlocked(resource->GetName());
                                ImGui::Text(_xor_("  ").c_str());
                                ImGui::SameLine();
                                ImGui::Text(resource->GetName().c_str());
                                ImGui::SameLine();
                                ImGui::TextColored(ImVec4(0, 255, 0, 255), _xor_(" [Starting] ").c_str());
                                ImGui::SameLine();

                                if (ImGui::Button((_xor_("Stop##").c_str() + std::string(resource->GetName().c_str())).c_str()))
                                {
                                    Sleep(60);


                                    resource->Stop();


                                }
                              
                                if (isBlocked) {
                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(255, 0, 0, 255), _xor_(" [Blocked] ").c_str());
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                                    if (ImGui::Button(("Unblock##" + std::string(resource->GetName())).c_str())) {
                                        Sleep(60);
                                        sdk::blockedResourcesList.ClearBlockedResources();
                                    }
                                   
                                }
                                else {
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                                    if (ImGui::Button(("Block##" + std::string(resource->GetName())).c_str())) {
                                        Sleep(60);
                                        sdk::blockedResourcesList.AddBlockedResource(resource->GetName());
                                
                                    }
                                   
                                }


                                ImGui::SameLine();
                                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                ImGui::SameLine();
                                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                                if (ImGui::Button(("Add##" + std::string(resource->GetName())).c_str())) {
                                    std::string novoNome(resource->GetName().c_str());
                                    if (!novoNome.empty()) {
                                        nomes.push_back(novoNome);

                                        SalvarNomes();
                                    }

                                }
                            }
                        }
                        else if (resource->GetState() == fx::ResourceState::Stopped)
                        {
                            if (filterdumper.PassFilter((resource->GetName().c_str())))
                            {
                                bool shouldBlock = (resource->GetName().find(settings::resourceNames) != std::string::npos);
                                bool isBlocked = sdk::blockedResourcesList.IsResourceBlocked(resource->GetName());
                                ImGui::Text(_xor_("  ").c_str());
                                ImGui::SameLine();
                                ImGui::Text(resource->GetName().c_str());
                                ImGui::SameLine();
                                ImGui::TextColored(ImVec4(255, 0, 0, 255), _xor_(" [Stopped] ").c_str());
                                ImGui::SameLine();

                                if (ImGui::Button((_xor_("Start##").c_str() + std::string(resource->GetName().c_str())).c_str()))
                                {
                                    Sleep(60);


                                    resource->Start();

                                }
                               
                                if (isBlocked) {

                                    ImGui::SameLine();
                                    ImGui::TextColored(ImVec4(255, 0, 0, 255), _xor_(" [Blocked] ").c_str());
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                                    if (ImGui::Button(("Unblock##" + std::string(resource->GetName())).c_str())) {
                                        Sleep(60);
                                        sdk::blockedResourcesList.ClearBlockedResources();
                                    }
                                   
                                }
                                else {
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                                    if (ImGui::Button(("Block##" + std::string(resource->GetName())).c_str())) {
                                        Sleep(60);
                                        sdk::blockedResourcesList.AddBlockedResource(resource->GetName());

                                    }
                                   
                                }

                                ImGui::SameLine();
                                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                                ImGui::SameLine();
                                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                                if (ImGui::Button(("Add##" + std::string(resource->GetName())).c_str())) {
                                    std::string novoNome(resource->GetName().c_str());
                                    if (!novoNome.empty()) {
                                        nomes.push_back(novoNome);
                                       
                                        SalvarNomes(); // Salvar nomes após adicionar um novo
                                    }

                                }


                            }
                        }
                    });

              

              ImGui::EndChild();
            }

            ImGui::SameLine();


            ImGui::BeginChild(E("##resourceblock"), ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 5), true, ImGuiWindowFlags_NoScrollbar);
            {
               

                ImGui::Checkbox("Stop", &Settings::Executor::autostop);
              
                
                
               if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable Auto Stop");
               
          

                ImGui::SameLine();

                ImGui::Checkbox("Block", &Settings::Executor::autoblock);
              
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable Auto Block");
                //ImGui::SetCursorPosX(Settings::vehicle::test1); ImGui::SameLine(); ImGui::TextColored(ImColor(255, 255, 0, 255), "(?)"); if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable Auto Block");

                if(Settings::Executor::autostop)
                {
                    sdk::stopResourcesList.CarregarNomesDoArquivo();
                }
                if (Settings::Executor::autoblock)
                {
                    sdk::blockedResourcesList.CarregarNomesDoArquivo();
                }
                //CarregarNomes();


                static char nomeBuffer[256];
         
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     // ListBox para exibir nomes e permitir seleção/remoção
                    if (ImGui::ListBoxHeader("##Nomes", nomes.size(), 5)) {
                        for (int i = 0; i < nomes.size(); ++i) {
                            bool isSelected = (selectedItemIndex == i);
                            if (ImGui::Selectable(nomes[i].c_str(), isSelected)) {
                                selectedItemIndex = i;
                            }
                        }
                        ImGui::ListBoxFooter();
                    }

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                    //ImGui::Text(E("Resource Name"));
                    ImGui::InputText("##ResourceName", nomeBuffer, sizeof(nomeBuffer));

               
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                    if (ImGui::Button(" Add")) {
                        std::string novoNome(nomeBuffer);
                        if (!novoNome.empty()) {
                            nomes.push_back(novoNome);
                            memset(nomeBuffer, 0, sizeof(nomeBuffer)); // Limpar o buffer do nome
                            SalvarNomes(); // Salvar nomes após adicionar um novo
                        }
                    }
                    ImGui::SameLine();

                    // Botão para remover o nome selecionado na ListBox
                    if (selectedItemIndex >= 0 && selectedItemIndex < nomes.size()) {
                        if (ImGui::Button(" Remove")) {
                            nomes.erase(nomes.begin() + selectedItemIndex);
                            selectedItemIndex = -1; // Limpar a seleção
                            SalvarNomes(); // Salvar nomes após remover um
                        }
                    }

                    ImGui::SameLine();

                    if (ImGui::Button(" Load"))
                    {
                        CarregarNomes();
                    }

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                  //  ImGui::Text(E("Event Name"));
                    ImGui::PushItemWidth(150);
//                    ImGui::InputText(E("##rg"), eventname, IM_ARRAYSIZE(eventname), ImGuiInputTextFlags_None);
//                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
//                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
//                    if (ImGui::Button(_xor_(" Bypass FiveGuard").c_str()))
//                    {
//                        std::string finalcode;
//                        std::string evento = eventname;
//
//                        std::string local = "local wefsdfwedsda = '" + evento + "'";
//
//                        std::string code = R"(Citizen.CreateThread(function()
//    while true do
//        TriggerServerEvent(wefsdfwedsda, "e14a2b19546a0", 6541089)
//        Citizen.Wait(5000)
//    end
//end) )";
//
//
//                        finalcode = local + '\n' + code;
//
//                        //  sdk::Editor.SetText(finalcode);
//                        sdk::executeAgain(finalcode);
//                        Settings::Executor::bypassinjected = true;
//
//                    }
                    //if (Settings::Executor::bypassinjected)
                    //{
                    //    ImGui::SameLine();
                    //    ImGui::TextColored(ImVec4(0, 255, 0, 255), _xor_(" [Actived] ").c_str());
                    //}

                    // ImGui::SameLine();  bypassinjected
              //ImGui::TextColored(ImVec4(0, 255, 0, 255), _xor_(" [Menu Loaded] ").c_str());


                    //HELPMARKER(" Put event log NAME and press bypass fiveguard");


                ImGui::EndChild();
            }


            ImGui::PushItemWidth(195);

            ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
        }
       

        break;
    case 3:
      
        ImGui::Text(E(""));

        if (ImGui::Button(_xor_("Player").c_str()))
            TabEsp = 3;
        ImGui::SameLine();
        ImGui::Text(E(" "));
        ImGui::SameLine();
        if (ImGui::Button(_xor_("AimBot").c_str()))
            TabEsp = 0;
        ImGui::SameLine();
        ImGui::Text(E(" "));
        ImGui::SameLine();
        if (ImGui::Button(_xor_("Esp").c_str()))
            TabEsp = 1;
        ImGui::SameLine();
        ImGui::Text(E(" "));
        ImGui::SameLine();
        if (ImGui::Button(_xor_("Weapon").c_str()))
            TabEsp = 2;
       
        ImGui::SameLine();
        ImGui::Text(E(" "));
        ImGui::SameLine();
        if (ImGui::Button(_xor_("Vehicle").c_str()))
            TabEsp = 4;

        ImGui::SameLine();
        ImGui::Text(E(" "));
        ImGui::SameLine();
        if (ImGui::Button(_xor_("Online").c_str()))
            TabEsp = 5;

        ImGui::SameLine();
        ImGui::Text(E(" "));
        ImGui::SameLine();
        if (ImGui::Button(_xor_("Misc").c_str()))
            TabEsp = 6;
          
        ImGui::Separator();


        switch (TabEsp)
        {
        case 0:

            ImGui::BeginChild(E("##MainBar"), ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::Text(E("Aim assistance"));
                ImGui::PopStyleColor();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::Separator();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::PushFont(littleFont);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
                ImGui::Checkbox(E(" Enable Aimbot"), &Settings::Aimbot::Aimbot);

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Draw Fov"), &Settings::Aimbot::Draw_Fov);
                if (Settings::Aimbot::Draw_Fov)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 195.f - ImGui::CalcTextSize(E(" Draw Fov")).x);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                    ImGui::ColorEdit4(E("Fov Color"), Settings::TempColor::fov, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                    ImGui::PopStyleVar();
                }
                Settings::Visuals::fovColor.Value.x = Settings::TempColor::fov[0];
                Settings::Visuals::fovColor.Value.y = Settings::TempColor::fov[1];
                Settings::Visuals::fovColor.Value.z = Settings::TempColor::fov[2];
                Settings::Visuals::fovColor.Value.w = Settings::TempColor::fov[3];
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Draw Crosshair"), &Settings::Aimbot::crosshair);
                if (Settings::Aimbot::crosshair)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 195.f - ImGui::CalcTextSize(E(" Draw Crosshair")).x);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                    ImGui::ColorEdit4(E("Fov Crosshair"), Settings::TempColor::cross, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                    ImGui::PopStyleVar();
                }
                Settings::Visuals::crosshair.Value.x = Settings::TempColor::cross[0];
                Settings::Visuals::crosshair.Value.y = Settings::TempColor::cross[1];
                Settings::Visuals::crosshair.Value.z = Settings::TempColor::cross[2];
                Settings::Visuals::crosshair.Value.w = Settings::TempColor::cross[3];
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Draw Aim Position"), &Settings::Aimbot::showaimposicion);

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Combo(E("Aimbot Type"), &Settings::Aimbot::AimbotTypes, Settings::Aimbot::AimbotType, IM_ARRAYSIZE(Settings::Aimbot::AimbotType), ImGuiComboFlags_NoArrowButton);
       
                if (Settings::Aimbot::AimbotTypes == 1)
                {
                    ImGui::SliderInt(E("Damage"), &Settings::Aimbot::damagesilent, 1, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                    ImGui::SliderInt(E("precisao"), &Settings::Aimbot::precisaosilent, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                    ImGui::SliderInt(E("velocidade"), &Settings::Aimbot::velociadaesilent, 1, 500, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);


                }

                ImGui::PushItemWidth(195);

                ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
            }

            ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

            ImGui::BeginChild(E("##TwoBar"), ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::Text(E("Aim settings"));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::PopStyleColor();
                ImGui::Separator();
                ImGui::PushFont(littleFont);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::PushItemWidth(210);
                ImGui::SliderInt(E("Fov"), &Settings::Aimbot::AimbotFov, 1, 600, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
               // ImGui::SliderFloat(E("Smooth"), &Settings::Aimbot::AimbotSmooth, 1.00, 100.00, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                ImGui::SliderInt(E("AimSensitivity"), &Settings::Aimbot::test, 1, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::SliderInt(E("AimSpeed"), &Settings::Aimbot::test2, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
               // ImGui::SliderInt(E("Aimtest3"), &Settings::Aimbot::test3, 1, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::SliderInt(E("Distance"), &Settings::Aimbot::DistanceAimbotLimit, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::Hotkey(E("##aimbot_key"), &Settings::Aimbot::Hotkey);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::Combo(E("Aimbot Target"), &Settings::Aimbot::TypeOfTarget, Settings::Aimbot::AimbotTarget, IM_ARRAYSIZE(Settings::Aimbot::AimbotTarget), ImGuiComboFlags_NoArrowButton);
         
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::Combo(E("Aim Bone"), &Settings::Aimbot::AimbotBone, Settings::Aimbot::AimbotBones, IM_ARRAYSIZE(Settings::Aimbot::AimbotBones), ImGuiComboFlags_NoArrowButton);

                

            }
            ImGui::PopFont(); ImGui::EndChild();

            break;

        case 1:

            ImGui::BeginChild(E("##MainBar"), ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::Text(E("Visuals assistance"));
                ImGui::PopStyleColor();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::Separator();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::PushFont(littleFont);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);

                ImGui::Checkbox(E(" Enable ESP"), &Settings::Visuals::enable);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Enable Box"), &Settings::Visuals::Box);
                if (Settings::Visuals::Box)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 175.f - ImGui::CalcTextSize(E(" Enable Box")).x);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                    ImGui::ColorEdit4(E("Box Color"), Settings::TempColor::Box, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                    ImGui::PopStyleVar();
                }
                Settings::Visuals::BoxColor.Value.x = Settings::TempColor::Box[0];
                Settings::Visuals::BoxColor.Value.y = Settings::TempColor::Box[1];
                Settings::Visuals::BoxColor.Value.z = Settings::TempColor::Box[2];
                Settings::Visuals::BoxColor.Value.w = Settings::TempColor::Box[3];
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Enable Corner Box"), &Settings::Visuals::Corner);
                if (Settings::Visuals::Corner)
                {


                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 175.f - ImGui::CalcTextSize(E(" Enable Corner Box")).x);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                    ImGui::ColorEdit4(E("Corner Color"), Settings::TempColor::Corner, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                    ImGui::PopStyleVar();
                }
                Settings::Visuals::CornerColor.Value.x = Settings::TempColor::Corner[0];
                Settings::Visuals::CornerColor.Value.y = Settings::TempColor::Corner[1];
                Settings::Visuals::CornerColor.Value.z = Settings::TempColor::Corner[2];
                Settings::Visuals::CornerColor.Value.w = Settings::TempColor::Corner[3];

                if (Settings::Visuals::Box || Settings::Visuals::Corner)
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                    ImGui::Checkbox(E(" Fill Box"), &Settings::Visuals::Filled);


                }
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Enable Skeleton"), &Settings::Visuals::Skeleton);
                if (Settings::Visuals::Skeleton)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 175.f - ImGui::CalcTextSize(E(" Enable Skeleton")).x);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                    ImGui::ColorEdit4(E("Skeleton Color"), Settings::TempColor::Skeleton, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                    ImGui::PopStyleVar();
                }
                Settings::Visuals::SkelColor.Value.x = Settings::TempColor::Skeleton[0];
                Settings::Visuals::SkelColor.Value.y = Settings::TempColor::Skeleton[1];
                Settings::Visuals::SkelColor.Value.z = Settings::TempColor::Skeleton[2];
                Settings::Visuals::SkelColor.Value.w = Settings::TempColor::Skeleton[3];
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Enable HealthBar"), &Settings::Visuals::HealthBar);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Enable ArmorBar"), &Settings::Visuals::ArmorBar);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Enable Lines"), &Settings::Visuals::Snapline);
                if (Settings::Visuals::Snapline)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 175.f - ImGui::CalcTextSize(E(" Enable Lines")).x);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                    ImGui::ColorEdit4(E("Color Lines "), Settings::TempColor::lines, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                    ImGui::PopStyleVar();
                }
           
                Settings::Visuals::linesColor.Value.x = Settings::TempColor::lines[0];
                Settings::Visuals::linesColor.Value.y = Settings::TempColor::lines[1];
                Settings::Visuals::linesColor.Value.z = Settings::TempColor::lines[2];
                Settings::Visuals::linesColor.Value.w = Settings::TempColor::lines[3];


                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Enable Weapon Name"), &Settings::Visuals::Weapon);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Enable Player Name"), &Settings::Visuals::PlayerName);
                if (Settings::Visuals::PlayerName)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 175.f - ImGui::CalcTextSize(E(" Enable Player Name")).x);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                    ImGui::ColorEdit4(E("Names Color"), Settings::TempColor::name, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                    ImGui::PopStyleVar();
                   
                    
                }
                Settings::Visuals::PlayernameColor.Value.x = Settings::TempColor::name[0];
                Settings::Visuals::PlayernameColor.Value.y = Settings::TempColor::name[1];
                Settings::Visuals::PlayernameColor.Value.z = Settings::TempColor::name[2];
                Settings::Visuals::PlayernameColor.Value.w = Settings::TempColor::name[3];



                ImGui::PushItemWidth(195);

                ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
            }
            ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

            ImGui::BeginChild(E("##TwoBar"), ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::Text(E("Visuals settings"));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::PopStyleColor();
                ImGui::Separator();
                ImGui::PushFont(littleFont);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::PushItemWidth(210);
                ImGui::SliderInt(E("Distance"), &Settings::Visuals::Distance, 1, 1000, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::SliderInt(E("FillBox Alpha"), &Settings::Visuals::FillBoxAlpha, 1, 255, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::SliderInt("Lines size :", &Settings::Visuals::BoxSize, 0, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Draw dead player"), &Settings::Visuals::Draw_Dead);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Draw localplayer"), &Settings::Visuals::Draw_LocalPlayer);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                ImGui::Checkbox(E(" Draw ped"), &Settings::Visuals::Draw_Ped);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                /*
                ImGui::Checkbox(E(" Preview ESP"), &Settings::Visuals::PreviewEsp);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                */
                ImGui::Checkbox(E(" Static ESP"), &Settings::Visuals::StaticESP);

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);

                ImGui::Checkbox(E(" Diff Players"), &Settings::Visuals::activevisibleforyou);
                if (Settings::Visuals::activevisibleforyou)
                {


                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 175.f - ImGui::CalcTextSize(E(" Enable visivel Players")).x);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                    ImGui::ColorEdit4(E("Color visivel Players"), Settings::TempColor::linesvisivel, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                    ImGui::PopStyleVar();

                    Settings::Visuals::linesColornovisibel.Value.x = Settings::TempColor::linesvisivel[0];
                    Settings::Visuals::linesColornovisibel.Value.y = Settings::TempColor::linesvisivel[1];
                    Settings::Visuals::linesColornovisibel.Value.z = Settings::TempColor::linesvisivel[2];
                    Settings::Visuals::linesColornovisibel.Value.w = Settings::TempColor::linesvisivel[3];
                }

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
               // ImGui::Combo(E("HealthBar Style"), &Settings::Visuals::HealthBar_idx, Settings::Visuals::HealthbarStyle, IM_ARRAYSIZE(Settings::Visuals::HealthbarStyle), ImGuiComboFlags_NoArrowButton);

                if (Settings::Visuals::PlayerName)
                {
                    ImGui::PushItemWidth(80);
                    ImGui::Combo(E("Names Style"), &Settings::Visuals::espnames_idx, Settings::Visuals::NamesStyle, IM_ARRAYSIZE(Settings::Visuals::NamesStyle), ImGuiComboFlags_NoArrowButton);
                }
           
                //ImGui::SameLine();

                //ImGui::Combo(E("Names Style"), &Settings::Visuals::espnamespusicao_idx, Settings::Visuals::espnamespusicao, IM_ARRAYSIZE(Settings::Visuals::espnamespusicao), ImGuiComboFlags_NoArrowButton);
                /*
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                ImGui::Combo(E("ArmorBar Style"), &Settings::Visuals::ArmorBar_idx, Settings::Visuals::ArmorBarrStyle, IM_ARRAYSIZE(Settings::Visuals::ArmorBarrStyle), ImGuiComboFlags_NoArrowButton);
                */
                
              //  ImGui::SliderInt("Box size Name :", &Settings::Visuals::BoxSizename, 0, 200, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
              //  ImGui::SliderInt("Box size Name1 :", &Settings::Visuals::BoxSizename1, 0, 30, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
               // ImGui::SliderInt("Box size Name2 :", &Settings::Visuals::BoxSizename2, 0, 50, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
               // ImGui::SliderInt("Box size Name3 :", &Settings::Visuals::BoxSizename3, 0, 50, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
               // ImGui::SliderInt("skeleto 2 :", &Settings::Visuals::Skeleton_idx2, 0, 15, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                 // ImGui::SliderInt("skeleto 1 :", &Settings::Visuals::Skeleton_idx, 0, 15, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
               


                ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();

            }

            break;

             case 2:


                 ImGui::BeginChild("##MainWeapon", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                 {
                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Text("Weapon assistance");
                     ImGui::PopStyleColor();
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Separator();
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::PushFont(littleFont);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
                     ImGui::Checkbox(" Enable Force gun", &Settings::Weapon::force_gun);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox(" Enable No Recoil", &Settings::Weapon::No_Recoil);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox(" Enable No Spread", &Settings::Weapon::No_Spread);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox(" Enable Infinite Ammo", &Settings::Weapon::InfiniteAmmo);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox(" Enable No Reload", &Settings::Weapon::NoReload);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox(" Enable No Range limit", &Settings::Weapon::NoRangeLimit);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox(" Enable DoubleTab", &Settings::Weapon::DoubleTap);
                     //   ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.5f);
                      //  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                      //  ImGui::Checkbox(" Enable Explosive Ammo", &Settings::Weapon::ExplosiveAmmo);

                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     if (ImGui::Button("Set Ammo", ImVec2(250, 30)))
                     {

                         if (!Settings::Weapon::enable)
                         {
                           //  ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Please Enable Weapon Option :)" });

                         }
                         else
                         {
                          

                         }

                         Settings::Weapon::SetAmmo = true;
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Ammo edited %s", "" }); //me quede aqui
                     }



                     ImGui::PushItemWidth(195);

                     ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
                 }

                 ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

                 ImGui::BeginChild("##TwoBar", ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                 {
                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Text("Weapon settings");
                     ImGui::PopStyleColor();
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Separator();
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SliderInt("Ammo to set :", &Settings::Weapon::AmmoToSet, 1, 999, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SliderInt("Bullet To Shoot :", &Settings::Weapon::BulletToShot, 1, 50, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     ImGui::Separator();
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Text("Spawn weapon");
                 
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Separator();

                     if (ImGui::Button("Weapon names")) {
                         system(xorstr("start https://wiki.rage.mp/index.php?title=Weapons"));
                     }


                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);  
                     ImGui::InputText("", weaponname, sizeof(weaponname));
                     ImGui::SameLine();
                     if (ImGui::Button("Give Weapon")) {
                         std::string spawnweapon = std::format(R"(
        function spawnweapon()
            local weaponname = "{}"
            local vehicle = GiveWeaponToPed(GetPlayerPed(-1), GetHashKey(weaponname), 250, true, true)
        end
        spawnweapon()
    )", weaponname);

                         sdk::executeAgain(spawnweapon);
                     }



                     /*
                     if (ImGui::Button(" 1 Give Weapon knife"))
                     {
                         Ped playerPed = -1;
                        
                        // WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::GET_PLAYER_PED(-1), MISC::GET_HASH_KEY("weapon_knife"), true);
                       //  WEAPON::GET_CURRENT_PED_WEAPON();
                             WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), MISC::GET_HASH_KEY("weapon_knife"), 9999, 1);
                        // WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(PLAYER::PLAYER_PED_ID());
                     }

                     if (ImGui::Button(" 2 Give Weapon knife"))
                     {
                         Ped playerPed = -1;
                     
                         PVector3 c = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(playerPed), 1);
                         OBJECT::CREATE_AMBIENT_PICKUP(MISC::GET_HASH_KEY("PICKUP_WEAPON_KNIFE"), c.x + 3, c.y + 3, c.z + 3, 0, 100, MISC::GET_HASH_KEY("prop_knife"), 0, 1);
                     }
                     if (ImGui::Button(" 3 Give Weapon knife"))
                     {
                         Ped playerPed = -1;

                         WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::GET_PLAYER_PED(-1), MISC::GET_HASH_KEY("weapon_knife"), true);
                     }
                     */
                 }
                 ImGui::EndChild();
        

                 break;
             case 3:



                 ImGui::BeginChild("##MainBar", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                 {
                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Text("Player assistance");
                     ImGui::PopStyleColor();
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Separator();
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::PushFont(littleFont);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
                     ImGui::Checkbox("Revive native", &Settings::Player::revive);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Godmode", &Settings::Player::Godmod);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Semi-Godmod", &Settings::Player::Semi_Godmod);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Auto Armor", &Settings::Player::Infinite_Armor);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Super Jump", &Settings::Player::SuperJump);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Fast Run", &Settings::Player::fastrun);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Invisible", &Settings::Player::Invisible);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("NoClip", &Settings::Player::NoClip);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Never wanted", &Settings::Player::never_wanted);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("No-Ragdoll", &Settings::Player::NoRagDoll);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("No-Headshot", &Settings::Player::no_hs);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Infinite stamina", &Settings::Player::infinite_stamina);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox("Desync simulation", &Settings::Player::desync);


                   
                     

                     ImGui::PushItemWidth(195);

                     ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
                 }

                 ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

                 ImGui::BeginChild("##TwoBar", ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                 {
                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Text("Player settings");
                     ImGui::PopStyleColor();
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Separator();
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                   
                     ImGui::SliderInt("Armor to set :", &Settings::Player::ArmorToSet, 1, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     if (ImGui::Button("Set Armor", ImVec2(ImGui::GetContentRegionAvail().x - 20.0f, 23)))
                     {
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Armor added %s", "" }); //me quede aqui
                         Settings::Player::SetArmor = true;
                     }
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                     ImGui::SliderInt("Health to set :", &Settings::Player::HealthToSet, 1, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     if (ImGui::Button("Set Health", ImVec2(ImGui::GetContentRegionAvail().x - 20.0f, 23)))
                     {
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Health added %s", "" }); //me quede aqui
                         Settings::Player::SetHealth = true;
                     }
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                     ImGui::SliderFloat(("Camera Fov :"), &Settings::Player::FovValue, 0, 150, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     if (ImGui::Button("Set FOV", ImVec2(ImGui::GetContentRegionAvail().x - 20.0f, 23)))
                     {
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Fov Changed %s", "" }); //me quede aqui
                         Settings::Player::EnableFovChange = true;
                     }
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     if (ImGui::Button("Teleport To Waypoint", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                     {
                         sdk::executeAgain(R"(
local ped = PlayerPedId()
local blip = GetFirstBlipInfoId(8)

if DoesBlipExist(blip) then
    local blipCoords = GetBlipCoords(blip)

    for height = 1, 1000 do
        SetEntityCoordsNoOffset(ped, blipCoords.x, blipCoords.y, height + 0.0, true, true, true)
        local foundGround, zPos = GetGroundZFor_3dCoord(blipCoords.x, blipCoords.y, height + 0.0)
        if foundGround then
            print("[NEXHUB]^2 Player teleported to waypoint:^0", GetEntityCoords(ped, false))
            break
        end
        Wait(0)
    end
else
    print("[NEXHUB]^1 No waypoint found.^0")
end
)");
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                     }
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     if (ImGui::Button("Random outfit", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                     {
                         std::string randomoutfit = std::format(R"(	SetPedRandomComponentVariation(PlayerPedId(), true))");
                         sdk::executeAgain(randomoutfit);
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                     }
                     if (Settings::Player::fastrun)
                     {
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                         ImGui::SliderFloat("Fast Run Speed :", &Settings::Player::RunSpeedToSet, 1, 10, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     }

                 
                     ImGui::Separator();


                     if (Settings::Player::NoClip)
                     {
                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                         ImGui::Text("NoClip settings");
                         ImGui::PopStyleColor();
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                         ImGui::Separator();
 
                                            
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                         ImGui::Hotkey("Key Active", &Settings::misc::NoclipKey);
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                         ImGui::Hotkey("Key Boost", &Settings::misc::noclipboost);                 
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                         ImGui::SliderInt("NoClip Speed", &Settings::Player::Speed, 0, 40, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                       
                     } 
                 }

                 ImGui::EndChild();


                 break;


             case 4:


                 ImGui::BeginChild("##MainVehicle", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                 {
                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Text("Vehicle assistance");
                     ImGui::PopStyleColor();

                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Separator();
                 //    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::PushFont(littleFont);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);
                     //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     //  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     ImGui::Checkbox(" Enable Godmode", &Settings::vehicle::godmode);
                     ImGui::Checkbox(" Enable vehicle Invisible", &Settings::vehicle::vehInvisible);
                     if (!Settings::vehicle::vehInvisible)
                     {
                         auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
                         ENTITY::SET_ENTITY_VISIBLE(current_vehicle, true, 0);
                     }
                     else
                     {
                         auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
                         ENTITY::SET_ENTITY_VISIBLE(current_vehicle, false, 0);
                     }
                     ImGui::Checkbox(" Enable vehicle Boost", &Settings::vehicle::shift_boost);
                     if (Settings::vehicle::shift_boost)
                     {
                         ImGui::SameLine();
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 175.f - ImGui::CalcTextSize(E(" Enable vehicle Boost")).x);
                         ImGui::Text("Press Key SHIFT");                   
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                         ImGui::SliderFloat(("Boost multiplier"), &Settings::vehicle::vehicle_speed_value, 0.00f, 0.50f, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     }

                     ImGui::Checkbox("Skip Animation", &Settings::vehicle::skipani);
                     if (!Settings::vehicle::skipani)
                     {

                             Settings::vehicle::skip_animation = true;
                     }
                     else
                     {
                         Settings::vehicle::skip_animation = false;
                     }

                     ImGui::Checkbox("Auto open car", &Settings::vehicle::autoopen);
                     if (!Settings::vehicle::autoopen)
                     {

                         Settings::vehicle::auto_open_car = true;
                     }
                     else
                     {
                         Settings::vehicle::auto_open_car = false;
                     }
                   
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     if (ImGui::Button("Flip Car"))
                     {
                         sdk::executeAgain(R"(local function flipVehicle()
    local playerPed = PlayerPedId()
    local vehicle = GetVehiclePedIsIn(playerPed, false)

    if DoesEntityExist(vehicle) and IsEntityAVehicle(vehicle) then
        local roll, _, _ = GetEntityRoll(vehicle)

        if math.abs(roll) > 75.0 then
            SetVehicleOnGroundProperly(vehicle)
        else
        end
    else
    end
end
flipVehicle()
)");
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" });
                     }
                     ImGui::SameLine();

                     if (ImGui::Button("Repair"))
                     {
                         sdk::executeAgain(R"(local function a()local b=GetVehiclePedIsIn(PlayerPedId(),false)if DoesEntityExist(b)and IsEntityAVehicle(b)then SetVehicleFixed(b)SetVehicleDeformationFixed(b)print("[NEXHUB]^2 Vehicle repaired:^0",b)else print("[NEXHUB]^1 No vehicle found or not in a vehicle.^0")end end;a())");
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui

                     }
                     ImGui::SameLine();

                     if (ImGui::Button("Delete"))
                     {
                         sdk::executeAgain(R"(Citizen.CreateThread(function()Citizen.Wait(0)local a=GetPlayerPed(-1)local b=GetVehiclePedIsIn(a,false)if DoesEntityExist(b)then NetworkRequestControlOfEntity(b)local c=2000;local d=GetGameTimer()while not NetworkHasControlOfEntity(b)and GetGameTimer()-d<c do Wait(100)end;if NetworkHasControlOfEntity(b)then DeleteEntity(b)end end end))");
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui

                     }

                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                     if (ImGui::Button("Update Colors"))
                     {
                         // Extrair os valores de cor RGBA da estrutura
                         float r = Settings::TempColor::vehicle[0];
                         float g = Settings::TempColor::vehicle[1];
                         float b = Settings::TempColor::vehicle[2];
                         float a = Settings::TempColor::vehicle[3];

                         float r2 = Settings::TempColor::vehicle2[0];
                         float g2 = Settings::TempColor::vehicle2[1];
                         float b2 = Settings::TempColor::vehicle2[2];
                         float a2 = Settings::TempColor::vehicle2[3];

                         // Certifique-se de que os valores de cor estejam no intervalo [0, 1]
                         r = ImClamp(r, 0.0f, 1.0f);
                         g = ImClamp(g, 0.0f, 1.0f);
                         b = ImClamp(b, 0.0f, 1.0f);
                         a = ImClamp(a, 0.0f, 1.0f);

                         r2 = ImClamp(r2, 0.0f, 1.0f);
                         g2 = ImClamp(g2, 0.0f, 1.0f);
                         b2 = ImClamp(b2, 0.0f, 1.0f);
                         a2 = ImClamp(a2, 0.0f, 1.0f);

                         // Converta os valores de cor para o intervalo [0, 255]
                         int intR = static_cast<int>(r * 255);
                         int intG = static_cast<int>(g * 255);
                         int intB = static_cast<int>(b * 255);

                         int intR2 = static_cast<int>(r2 * 255);
                         int intG2 = static_cast<int>(g2 * 255);
                         int intB2 = static_cast<int>(b2 * 255);

                          auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
                         VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(current_vehicle, intR, intG, intB);
                         VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(current_vehicle, intR2, intG2, intB2);
                         ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully changed color %s", "" }); //me quede aqui
                     }     
                   
                         ImGui::SameLine();
                         //ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 175.f - ImGui::CalcTextSize(E("uu")).x);
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
                         ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);
                         ImGui::ColorEdit4(E("Primary "), Settings::TempColor::vehicle, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                         ImGui::SameLine();
                         ImGui::ColorEdit4(E("Secundary Color"), Settings::TempColor::vehicle2, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip);
                         ImGui::PopStyleVar();
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                         if (ImGui::Button("Random Colors"))
                         {
                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Random color loaded %s", "" }); //me quede aqui
                             auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
                             int pearl;
                             int wcolor;

                             VEHICLE::GET_VEHICLE_EXTRA_COLOURS(current_vehicle, &pearl, &wcolor);
                             int mcolor = rand() % 160;

                             VEHICLE::SET_VEHICLE_COLOURS(current_vehicle, mcolor, mcolor); //primary & secondary
                             VEHICLE::SET_VEHICLE_EXTRA_COLOURS(current_vehicle, rand() % 75, wcolor); //pearl
                         }
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                         if (ImGui::Button("Unlock Car"))
                         {
                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Unlocked car %s", "" }); //me quede aqui
                             PVector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);
                             Vehicle VehID = VEHICLE::GET_CLOSEST_VEHICLE(coords.x, coords.y, coords.z, 100.0, 0, 71);
                             auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
                                 VEHICLE::SET_VEHICLE_DOORS_LOCKED(VehID, 1);
                                 VEHICLE::SET_VEHICLE_DOORS_LOCKED(current_vehicle, 1);
                           
                         }
                         ImGui::SameLine();
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.5);
                         if (ImGui::Button("Lock Car"))
                         {
                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Now the car is locked:) %s", "" }); //me quede aqui
                             PVector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);
                             Vehicle VehID = VEHICLE::GET_CLOSEST_VEHICLE(coords.x, coords.y, coords.z, 100.0, 0, 71);
                             auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
                                 VEHICLE::SET_VEHICLE_DOORS_LOCKED(VehID, 4);
                                 VEHICLE::SET_VEHICLE_DOORS_LOCKED(current_vehicle, 4);
                         }

                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);

                         ImGui::SameLine();
                         if (ImGui::Button("Tp to Car"))
                         {
                             PVector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);

                             Vehicle VehID = VEHICLE::GET_CLOSEST_VEHICLE(coords.x, coords.y, coords.z, 500.0, 0, 71);


                             if (ENTITY::DOES_ENTITY_EXIST(VehID))
                             {
                                 PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), VehID, -1);

                             }

                         }
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                         if (ImGui::Button("Max Upgrades"))
                         {
                             sdk::executeAgain(R"(function MaxOut(a)SetVehicleModKit(GetVehiclePedIsIn(GetPlayerPed(-1),false),0)SetVehicleWheelType(GetVehiclePedIsIn(GetPlayerPed(-1),false),7)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),0,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),0)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),1,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),1)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),2,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),2)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),3,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),3)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),4,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),4)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),5,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),5)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),6,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),6)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),7,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),7)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),8,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),8)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),9,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),9)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),10,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),10)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),11,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),11)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),12,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),12)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),13,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),13)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),14,16,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),15,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),15)-2,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),16,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),16)-1,false)ToggleVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),17,true)ToggleVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),18,true)ToggleVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),19,true)ToggleVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),20,true)ToggleVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),21,true)ToggleVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),22,true)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),23,1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),24,1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),25,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),25)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),27,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),27)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),28,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),28)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),30,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),30)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),33,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),33)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),34,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),34)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),35,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),35)-1,false)SetVehicleMod(GetVehiclePedIsIn(GetPlayerPed(-1),false),38,GetNumVehicleMods(GetVehiclePedIsIn(GetPlayerPed(-1),false),38)-1,true)SetVehicleWindowTint(GetVehiclePedIsIn(GetPlayerPed(-1),false),1)SetVehicleTyresCanBurst(GetVehiclePedIsIn(GetPlayerPed(-1),false),false)SetVehicleNumberPlateTextIndex(GetVehiclePedIsIn(GetPlayerPed(-1),false),5)SetVehicleNeonLightEnabled(GetVehiclePedIsIn(GetPlayerPed(-1)),0,true)SetVehicleNeonLightEnabled(GetVehiclePedIsIn(GetPlayerPed(-1)),1,true)SetVehicleNeonLightEnabled(GetVehiclePedIsIn(GetPlayerPed(-1)),2,true)SetVehicleNeonLightEnabled(GetVehiclePedIsIn(GetPlayerPed(-1)),3,true)SetVehicleNeonLightsColour(GetVehiclePedIsIn(GetPlayerPed(-1)),222,222,255)end;MaxOut(GetVehiclePedIsUsing(PlayerPedId(-1))))" );
                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Full tunning loaded %s", "" }); 
                         }
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                         if (ImGui::Button("Drift mode (SHIFT)"))
                         {
                             sdk::executeAgain(R"(local a=true;Citizen.CreateThread(function()while true do Citizen.Wait(0)if IsControlJustReleased(0,56)then a=not a;if a then print("Drift mode enabled.")else print("Drift mode disabled.")end end;if not a then goto b end;local c=GetPlayerPed(-1)if IsPedInAnyVehicle(c,false)then local d=GetVehiclePedIsIn(c,false)if IsControlPressed(0,21)then SetVehicleReduceGrip(d,true)else SetVehicleReduceGrip(d,false)end end::b::end end))");
                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Full tunning loaded %s", "" }); 
                         }

                         HELPMARKER(" Press f9 if you want disable it");
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                         ImGui::SetNextItemWidth(100);
                         ImGui::InputText("", car_name, sizeof(car_name));
                         ImGui::SameLine();
                         if (ImGui::Button("Spawn car"))
                         {



                             std::string caradder = std::format(R"(
function spawnVehicle()
    local vehicleModel = "{}"

    RequestModel(vehicleModel)

    while not HasModelLoaded(vehicleModel) do
        Wait(500)
    end

    local playerPed = GetPlayerPed(-1)
    local pos = GetEntityCoords(playerPed)

    local vehicle = CreateVehicle(vehicleModel, pos.x, pos.y, pos.z, GetEntityHeading(playerPed), true, false)

    if DoesEntityExist(vehicle) then
    else
    end

    TaskWarpPedIntoVehicle(playerPed, vehicle, -1)
    SetEntityAsNoLongerNeeded(vehicle)
end
spawnVehicle()
)", car_name);
                             sdk::executeAgain(caradder);

                         }

                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22.5f);

                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);

                         ImGui::InputText(E("##rg"), placanome, IM_ARRAYSIZE(placanome), ImGuiInputTextFlags_None);
                         ImGui::SameLine();
                         if (ImGui::Button("Set Plate"))
                         {
                             auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
                             VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(current_vehicle, placanome);
                         }
                    //  ImGui::SliderInt("1 :", &Settings::vehicle::test1, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     //      ImGui::SliderInt("2 :", &Settings::vehicle::test2, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                     // Settings::vehicle::Acceleration

                     ImGui::PushItemWidth(195);

                     ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
                 }

                 ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

                 ImGui::BeginChild("##TwoBar", ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                 {
                     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                     ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Text("Vehicle List");
                     ImGui::PopStyleColor();
                     ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                     ImGui::Separator();
                    // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                   
                         hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                         if (!World)
                             return;
                         hk_Ped* LocalPlayer = World->LocalPlayer();

                         if (LocalPlayer)
                         {
                             hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
                             if (!Nav)
                                 return;

                             hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                             if (!ReplayInterface)
                                 return;

                             hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface();
                             if (!VehicleInterface)
                                 return;

                        

                             static ImGuiTextFilter filter;

                             ImGui::SetNextItemWidth(231);//332
                             filter.Draw("Search");

                             if (ImGui::ListBoxHeader("", ImVec2(231, 231)))
                             {

                                 for (int i = 0; i < VehicleInterface->VehicleMaximum(); i++) {



                                     hk_Vehicle* Peds = VehicleInterface->VehicleList()->Vehicle(i);
                                     if (!Peds) continue;
                                     auto playerName = Peds->VehicleModelInfo()->GetCharName();
                                     const char* c = playerName;
                                     const char* items[] = { c };

                                   
                                     int healthtoshow = Peds->GetHealth();
                                     std::string hp = " - [HP " + std::to_string(healthtoshow) + "]";



                                     Vector3 DistanceCalculationvaiculo = (LocalPlayer->GetCoordinate() - Peds->GetCoordinate());

                                     int Distance = sqrtf(DistanceCalculationvaiculo.x * DistanceCalculationvaiculo.x + DistanceCalculationvaiculo.y * DistanceCalculationvaiculo.y + DistanceCalculationvaiculo.z * DistanceCalculationvaiculo.z);

                                     if (Distance < Settings::Visuals::Distancevheicle)
                                     {
                                         std::string strr = " [ " + std::to_string(Distance) + "m]" + hp;


                                         std::string VehicleName = playerName;
                                         VehicleName.append(" ##");
                                         VehicleName.append(std::to_string(i));
                                         bool is_selected = (selectedVeh_index == i);






                                         if (filter.PassFilter(VehicleName.c_str()))
                                         {
                                             if (ImGui::Selectable(VehicleName.c_str(), is_selected))
                                             {

                                                 selectedVeh_index = i;
                                                 
                                             }
                                             ImGui::SameLine();
                                             ImGui::Text(strr.c_str());
                                         }
                                     }


                                 }




                                 ImGui::ListBoxFooter();
                             }


                             // ImGui::SliderInt("1 :", &Settings::vehicle::test1, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                            //  ImGui::SliderInt("2 :", &Settings::vehicle::test2, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                             ImGui::SliderInt("Distance Vehicle ", &Settings::Visuals::Distancevheicle, 1, 250, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                             ImGui::Checkbox("Vehicle Name ESP", &Settings::vehicle::NameESP);


                             /*
                             if (ImGui::Button("test spawn de carros"))
                             {
                                 PVector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), FALSE);
         
                                 std::string VehBuffer = "akuma";
                        
                                 char* c = const_cast<char*>(VehBuffer.c_str());
                                 Hash customveh = MISC::GET_HASH_KEY(c);
                                 DWORD model = ENTITY::GET_ENTITY_MODEL(customveh);
                                 STREAMING::REQUEST_MODEL(model);
                                // SYSTEM::WAIT(100);
                                 if (STREAMING::HAS_MODEL_LOADED(model))
                                 {
                                     Vehicle Veh = VEHICLE::CREATE_VEHICLE1(model, playerOnlineCoords.x, playerOnlineCoords.y, playerOnlineCoords.z + 2.0f, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
                                     int newnetid = NETWORK::VEH_TO_NET(Veh);
                                     NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(newnetid);

                                 }
                              
                             }
                             */


                             hk_Vehicle* SelectedPed = VehicleInterface->VehicleList()->Vehicle(selectedVeh_index);
                             if (SelectedPed->GetCoordinate().x != 0)
                             {
                                 ImGui::SetCursorPosY(35);
                                 ImGui::SetCursorPosX(238);
                                 ImGui::BeginChild("Settings");
                                 {


                                     if (ImGui::TreeNode("Troll"))
                                     {
                                         if (ImGui::Button("Destroy vehicle"))
                                         {
                                             /*
                                             WEAPON::REQUEST_WEAPON_ASSET((0xB1CA77B1), 31, 26);

                                             auto player = PLAYER::PLAYER_PED_ID();
                                             PVector3 AirstrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), false);
                                             PVector3 TargetCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), MISC::GET_RANDOM_FLOAT_IN_RANGE(-40, 40), MISC::GET_RANDOM_FLOAT_IN_RANGE(-40, 40), MISC::GET_RANDOM_FLOAT_IN_RANGE(30, 50));
                                             Ped randomped = PED::GET_RANDOM_PED_AT_COORD(AirstrikeCoords.x, AirstrikeCoords.y, AirstrikeCoords.z, 100, 100, 100, -1);
                                             NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(randomped);

                                             MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)1000, false, 0xB1CA77B1, randomped, true, false, 3.f);
                                             */
                                             auto player = PLAYER::PLAYER_PED_ID();
                                             auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
                                             auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xB1CA77B1, NULL);
                                             MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0xB1CA77B1, player, true, false, 3.f);


                                         }
                                         if (ImGui::Button("Lock vehicle"))
                                         {
                                             VEHICLE::SET_VEHICLE_DOORS_LOCKED(FiveM::pointer_to_handle((DWORD64)SelectedPed), 4);
                                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully locked %s", "" }); //me quede aqui

                                         }

                                         if (ImGui::Button("Unlock vehicle"))
                                         {
                                             VEHICLE::SET_VEHICLE_DOORS_LOCKED(FiveM::pointer_to_handle((DWORD64)SelectedPed), 1);
                                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully unlocker %s", "" }); //me quede aqui

                                         }


                                         ImGui::TreePop();
                                     }
                                     if (ImGui::TreeNode("Utils"))
                                     {
                                         if (ImGui::Button("TP To vehicle"))
                                         {

                                             Nav->SetRotation1({ 0, 0, 0, 0 });
                                             LocalPlayer->SetVelocity();
                                             LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate());
                                             Nav->SetCoordinate(SelectedPed->GetCoordinate());
                                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui

                                         }
                                         if (ImGui::Button("Repair"))
                                         {
                                             sdk::executeAgain(R"(local function a()local b=GetVehiclePedIsIn(PlayerPedId(),false)if DoesEntityExist(b)and IsEntityAVehicle(b)then SetVehicleFixed(b)SetVehicleDeformationFixed(b)else end end;a())");

                                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                                         }
                                         if (ImGui::Button("Enter vehicle"))
                                         {
                                             auto player = PLAYER::PLAYER_PED_ID();
                                             auto vehicle = FiveM::pointer_to_handle((DWORD64)SelectedPed);

                                             if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), -1, false)) {

                                                 PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), -1);

                                             }
                                             else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 0, false)) {


                                                 PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 0);

                                             }
                                             else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 1, false)) {


                                                 PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 1);

                                             }
                                             else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 2, false)) {


                                                 PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 2);

                                             }
                                         }
                                         if (ImGui::Button("Tp vehicle to me"))
                                         {
                                             PVector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), FALSE);
                                             
                                            // PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), FiveM::pointer_to_handle((DWORD64)SelectedPed), -1);
                                           //  TASK::TASK_LEAVE_VEHICLE(PLAYER::GET_PLAYERS_LAST_VEHICLE(), FiveM::pointer_to_handle((DWORD64)SelectedPed), 1);
                                             NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(FiveM::pointer_to_handle((DWORD64)SelectedPed));
                                             ENTITY::SET_ENTITY_COORDS(FiveM::pointer_to_handle((DWORD64)SelectedPed), playerOnlineCoords.x, playerOnlineCoords.y, playerOnlineCoords.z, 0, 0, 0, 1);
                                             ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                                         }

                                         ImGui::TreePop();
                                     }



                                     ImGui::EndChild();

                                 }

                             }



                         }
                         else
                         {
                             ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                             ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                             ImGui::Text("Waiting to Join server ... ");
                             ImGui::PopStyleColor();
                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                             ImGui::Separator();



                         }

                    
                 }
                 ImGui::EndChild();

                 break;

                 case 5:
                     ImGui::BeginChild("##MainOnline", ImVec2(ImGui::GetContentRegionAvail().x + 22, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                     {
                         ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                         ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                         ImGui::Text("Online Players");
                         ImGui::PopStyleColor();

                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                         ImGui::Separator();
                         //    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                         ImGui::PushFont(littleFont);
                         ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.5f);


                         hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                         if (!World)
                             return;

                         hk_Ped* LocalPlayer = World->LocalPlayer();
                      
                       if (LocalPlayer)
                       {

                        
                         hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
                         if (!Nav)
                             return;

                         hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                         if (!ReplayInterface)
                             return;

                         hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
                         if (!PedInterface)
                             return;




                                 static ImGuiTextFilter filter;

                                 ImGui::SetNextItemWidth(239);//332
                                 filter.Draw("Search");
                                 if (ImGui::ListBoxHeader(" ", ImVec2(238, 283)))
                                 {
                                     for (int i = 0; i < PedInterface->PedMaximum(); i++) {

                                         hk_Ped* Peds = PedInterface->PedList()->Ped(i);
                                         auto ped_type = Peds->GetPedType();

                                         bool IsPed = false;
                                         if (!Peds) continue;

                                         if (!ped_type)
                                         {
                                             continue;
                                         }

                                         ped_type = ped_type << 11 >> 25;

                                         if (ped_type != 2) {
                                             IsPed = true;
                                         }

                                         if (!Settings::misc::PedList) {

                                             if (IsPed)
                                             {
                                                 continue;
                                             }

                                         }
                                

                                         int PlayerID = FiveM::pointer_to_handle((DWORD64)Peds);

                                         void* addr = (void*)(Peds->PlayerInfo() + 0x20);

                                         auto momes = FiveM::Playername(addr);

                                 
                                         std::string playerName = momes;

                                         const char* c = playerName.c_str();
                                         const char* items[] = { c };
                               

                                         if (Peds != LocalPlayer && IsPed)
                                         {
                                             momes = "Ped";
                                         }
                                         Vector3 DistanceCalculation = (LocalPlayer->GetCoordinate() - Peds->GetCoordinate());
                                         int Ddistance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

                                         std::string strr = " - [" + std::to_string(Ddistance) + "m]";

                                         bool is_selected = (selected_index == i);
                                         if (filter.PassFilter(playerName.c_str()))
                                         {
                                             if (ImGui::Selectable(playerName.c_str(), is_selected))
                                             {

                                                 selected_index = i;
                                                 nomedopalyerselecionado = playerName.c_str();
                                             }
                                             ImGui::SameLine();
                                             ImGui::Text(strr.c_str());
                                         }
                                        
                                     }


                                     ImGui::ListBoxFooter();
                                 }

                       

                        // ImGui::SliderInt("1 :", &Settings::vehicle::test1, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                       //  ImGui::SliderInt("2 :", &Settings::vehicle::test2, 1, 900, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                      //   ImGui::SliderInt("3 :", &Settings::vehicle::test3, 1, 300, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);

                        // std::string SelectedString = "Selected one is : " + std::to_string(selected_index);
                        // ImGui::Text(SelectedString.c_str());
                     //    ImGui::Checkbox(("Show Ped "), &Settings::misc::PedList);


                                 
                                 hk_Ped* SelectedPed = PedInterface->PedList()->Ped(selected_index);
                                 if (SelectedPed->GetCoordinate().x != 0)
                                 {
    
                              

                                        // ImGui::Text("Player:");

                                         //ImGui::SameLine();
                                         ImGui::SetCursorPosY(50);
                                         ImGui::SetCursorPosX(249);
                                         if (ImGui::TreeNode(nomedopalyerselecionado.c_str()))
                                         {

                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("Destroy car"))
                                             {
                                                 auto player = PLAYER::PLAYER_PED_ID();
                                                 auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xBFEFFF6D, NULL);
                                                 MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)10000, false, (Hash)0xBFEFFF6D, player, true, false, 3.f);

                                             }
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("Lock vehicle"))
                                             {
                                                 auto vehicle = PED::GET_VEHICLE_PED_IS_IN(FiveM::pointer_to_handle((DWORD64)SelectedPed), false);
                                                 VEHICLE::SET_VEHICLE_DOORS_LOCKED(vehicle, 4);

                                             }
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("HandCuff"))
                                             {
                                                 PED::SET_ENABLE_HANDCUFFS(FiveM::pointer_to_handle((DWORD64)SelectedPed), true);

                                             }
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("Taze player"))
                                             {
                                                 auto player = PLAYER::PLAYER_PED_ID();
                                                 auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
                                                 auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0x8BB05FD7, NULL);
                                                 MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0x8BB05FD7, player, true, false, 3.f);
                                             }
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("Kill player"))
                                             {
                                                 auto player = PLAYER::PLAYER_PED_ID();
                                                 auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
                                                 auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xB1CA77B1, NULL);
                                                 MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0xB1CA77B1, player, true, false, 3.f);
                                             }

                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("Copy Outfit"))
                                             {
                                                 auto player = PLAYER::PLAYER_PED_ID();
                                                 PED::CLONE_PED_TO_TARGET(FiveM::pointer_to_handle((DWORD64)SelectedPed), player);
                                             }
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("TP To Player"))
                                             {
                                                 if (SelectedPed != LocalPlayer)
                                                 {
                                                     Nav->SetRotation1({ 0, 0, 0, 0 });
                                                     LocalPlayer->SetVelocity();
                                                     LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate());
                                                     Nav->SetCoordinate(SelectedPed->GetCoordinate());
                                                 }

                                             }
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (Settings::misc::spectate)
                                             {

                                                 if (ImGui::Button("Spectate"))
                                                 {
                                                     auto player = PLAYER::PLAYER_PED_ID();
                                                     NETWORK::NETWORK_SET_IN_SPECTATOR_MODE((bool)true, FiveM::pointer_to_handle((DWORD64)SelectedPed));
                                                     Settings::misc::spectate = false;
                                                 }
                                             }
                                             else
                                             {
                                                 if (ImGui::Button("Off Spectate"))
                                                 {
                                                     auto player = PLAYER::PLAYER_PED_ID();
                                                     NETWORK::NETWORK_SET_IN_SPECTATOR_MODE((bool)false, FiveM::pointer_to_handle((DWORD64)SelectedPed));
                                                     Settings::misc::spectate = true;
                                                 }
                                             }
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("Enter Vehicle"))
                                             {
                                                 auto player = PLAYER::PLAYER_PED_ID();
                                                 auto vehicle = PED::GET_VEHICLE_PED_IS_IN(FiveM::pointer_to_handle((DWORD64)SelectedPed), false);

                                                 if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), -1, false)) {

                                                     PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), -1);

                                                 }
                                                 else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 0, false)) {


                                                     PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 0);

                                                 }
                                                 else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 1, false)) {


                                                     PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 1);

                                                 }
                                                 else if (VEHICLE::IS_VEHICLE_SEAT_FREE(FiveM::pointer_to_handle((DWORD64)SelectedPed), 2, false)) {


                                                     PED::SET_PED_INTO_VEHICLE(player, FiveM::pointer_to_handle((DWORD64)SelectedPed), 2);

                                                 }


                                             }
                                             /*
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("Kick Vehicle"))
                                             {
                                                 auto player = PLAYER::PLAYER_PED_ID();
                                                 auto vehicle = PED::GET_VEHICLE_PED_IS_IN(FiveM::pointer_to_handle((DWORD64)SelectedPed), false);
                                                 TASK::TASK_LEAVE_VEHICLE(player, vehicle, 1);
                                                
                                             }
                                             */
                                             // TASK::TASK_LEAVE_VEHICLE(PLAYER::GET_PLAYERS_LAST_VEHICLE(), FiveM::pointer_to_handle((DWORD64)SelectedPed), 1);
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.5f);
                                             ImGui::SetCursorPosX(249);
                                             if (ImGui::Button("Shoot bullet"))
                                             {
                                                 auto player = PLAYER::PLAYER_PED_ID();
                                                 auto WeaponHash = WEAPON::GET_SELECTED_PED_WEAPON(player);
                                                 auto WeaponDamage = WEAPON::GET_WEAPON_DAMAGE(0xBFEFFF6D, NULL);
                                                 MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z + 2, SelectedPed->GetCoordinate().x, SelectedPed->GetCoordinate().y, SelectedPed->GetCoordinate().z - 1, (int)WeaponDamage, false, 0xBFEFFF6D, player, true, false, 3.f);
                                             }

                                             // palyer info
                                             ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                                             ImGui::SetCursorPosY(9);
                                             ImGui::SetCursorPosX(411);
                                             ImGui::Text("Info Player");
                                             ImGui::PopStyleColor();
                                             ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                                             ImGui::Separator();



                                             ImGui::SetCursorPosY(69);
                                             //int playerHealthMax = SelectedPed->GetMaxHealth();
                                          
                                             
                                             int hp = SelectedPed->GetHealth();
                                             int armadura = SelectedPed->GetArmor();
                                             std::string healthtoshow = "HP : " + std::to_string(hp) + "  Armor : " + std::to_string(armadura);
                                             ImGui::SetCursorPosX(411);
                                             ImGui::Text(healthtoshow.c_str());

                                             std::string coordstoshow = "X : " + std::to_string(SelectedPed->GetCoordinate().x) + "  Y : " + std::to_string(SelectedPed->GetCoordinate().y) + "  Z : " + std::to_string(SelectedPed->GetCoordinate().z);
                                             ImGui::SetCursorPosX(411);
                                             ImGui::Text(coordstoshow.c_str());


                                             std::string ID = "Player ID : " + std::to_string(NETWORK::NETWORK_GET_PLAYER_INDEX_FROM_PED(FiveM::pointer_to_handle((DWORD64)SelectedPed)));
                                             ImGui::SetCursorPosX(411);
                                             ImGui::Text(ID.c_str());

                                             ImGui::TreePop();
                                         }

                           
                                
                                 }
                             
                                     

                       }
                       else
                       {
                           ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                           ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                           ImGui::Text("Waiting to Join server ... ");
                           ImGui::PopStyleColor();
                           ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                           ImGui::Separator();
                           }

                   

                         
                       ImGui::PushItemWidth(195);

                       ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
                    
                     }
                   


                   

                     break;
                      case 6:


                          ImGui::BeginChild("##MainBar", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                          {
                              ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              ImGui::Text("Troll servers tab<3");
                              ImGui::PopStyleColor();
                              ImGui::Separator();
                              ImGui::PushFont(littleFont);
       
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              if (ImGui::Button("Delete all vehicles"))
                              {
                                  sdk::executeAgain(R"(Citizen.CreateThread(function()while true do Citizen.Wait(0)for a,b in ipairs(GetGamePool("CVehicle"))do if DoesEntityExist(b)then NetworkRequestControlOfEntity(b)if NetworkHasControlOfEntity(b)then SetEntityAsMissionEntity(b,true,true)DeleteVehicle(b)DeleteEntity(b)end end end end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" });
                              }

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              if (ImGui::Button("Become a nexhub"))
                              {
                                  sdk::executeAgain(R"(local a=false;local b=false;local c=nil;local d=nil;Citizen.CreateThread(function()while true do Citizen.Wait(0)if a and c then local e=PlayerPedId()local f=GetPedBoneCoords(e,0x796e,0.0,0.0,0.0)DrawText3Ds(f.x,f.y,f.z+0.5,"(NEXHUB) [Y] Drop Prop")if b and not IsEntityPlayingAnim(e,'anim@mp_rollarcoaster','hands_up_idle_a_player_one',3)then RequestAnimDict('anim@mp_rollarcoaster')while not HasAnimDictLoaded('anim@mp_rollarcoaster')do Citizen.Wait(100)end;TaskPlayAnim(e,'anim@mp_rollarcoaster','hands_up_idle_a_player_one',8.0,-8.0,-1,50,0,false,false,false)elseif not IsEntityPlayingAnim(e,"anim@heists@box_carry@","idle",3)and not b then RequestAnimDict("anim@heists@box_carry@")while not HasAnimDictLoaded("anim@heists@box_carry@")do Citizen.Wait(100)end;TaskPlayAnim(e,"anim@heists@box_carry@","idle",8.0,-8.0,-1,50,0,false,false,false)end;if not IsEntityAttached(c)then a=false;b=false;c=nil end end end end)Citizen.CreateThread(function()while true do Citizen.Wait(0)local e=PlayerPedId()local g=GetGameplayCamCoord()local h=GetGameplayCamRot(2)local i=RotationToDirection(h)local j=vec3(g.x+i.x*10.0,g.y+i.y*10.0,g.z+i.z*10.0)local k=StartShapeTestRay(g.x,g.y,g.z,j.x,j.y,j.z,-1,e,0)local l,m,l,l,n=GetShapeTestResult(k)local o=false;if m==1 then d=GetEntityType(n)if d==3 or d==2 then o=true;local p=d==3 and"Object"or(d==2 and"Car"or"")local q=GetEntityModel(n)local r="(NEXHUB) Entity Type: "..p..", Entity: "..n..", Model: "..q;local f=GetPedBoneCoords(e,0x796e,0.0,0.0,0.0)DrawText3Ds(f.x,f.y,f.z+0.5,r)end end;if IsControlJustReleased(0,246)then if o then if not a and n and d==3 then local q=GetEntityModel(n)DeleteEntity(n)RequestModel(q)while not HasModelLoaded(q)do Citizen.Wait(100)end;local s=CreateObject(q,g.x,g.y,g.z,true,true,true)SetModelAsNoLongerNeeded(q)a=true;c=s;RequestAnimDict("anim@heists@box_carry@")while not HasAnimDictLoaded("anim@heists@box_carry@")do Citizen.Wait(100)end;TaskPlayAnim(e,"anim@heists@box_carry@","idle",8.0,-8.0,-1,50,0,false,false,false)AttachEntityToEntity(s,e,GetPedBoneIndex(e,60309),0.0,0.2,0.0,0.0,0.0,0.0,true,true,false,true,1,true)elseif not a and n and d==2 then a=true;b=true;c=n;RequestAnimDict('anim@mp_rollarcoaster')while not HasAnimDictLoaded('anim@mp_rollarcoaster')do Citizen.Wait(100)end;TaskPlayAnim(e,'anim@mp_rollarcoaster','hands_up_idle_a_player_one',8.0,-8.0,-1,50,0,false,false,false)AttachEntityToEntity(c,e,GetPedBoneIndex(e,60309),1.0,0.5,0.0,0.0,0.0,0.0,true,true,false,false,1,true)end else if a and b then a=false;b=false;ClearPedTasks(e)DetachEntity(c,true,true)ApplyForceToEntity(c,1,i.x*20,i.y*20,i.z*20,0.0,0.0,0.0,0,false,true,true,false,true)elseif a then a=false;ClearPedTasks(e)DetachEntity(c,true,true)local t=GetEntityCoords(PlayerPedId())SetEntityCoords(c,t.x,t.y,t.z-1,false,false,false,false)SetEntityHeading(c,GetEntityHeading(PlayerPedId()))end end end end end)function RotationToDirection(u)local v=vec3(math.pi/180*u.x,math.pi/180*u.y,math.pi/180*u.z)local i=vec3(-math.sin(v.z)*math.abs(math.cos(v.x)),math.cos(v.z)*math.abs(math.cos(v.x)),math.sin(v.x))return i end;function DrawText3Ds(w,x,y,z)local A,B,C=World3dToScreen2d(w,x,y)local D,E,F=table.unpack(GetGameplayCamCoords())local G=1/GetDistanceBetweenCoords(D,E,F,w,x,y,1)*2;local H=1/GetGameplayCamFov()*100;G=G*H;if A then SetTextScale(0.0*G,0.35*G)SetTextFont(0)SetTextProportional(1)SetTextColour(255,255,255,215)SetTextDropshadow(0,0,0,0,155)SetTextEdge(2,0,0,0,150)SetTextDropShadow()SetTextOutline()SetTextEntry("STRING")SetTextCentre(1)AddTextComponentString(z)DrawText(B,C)end end)");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" });
                              }

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              if (ImGui::Button("Spawn vehicle on all players"))
                              {
                                  sdk::executeAgain(R"(function spawnVehicleForPlayer(a)local b="adder"local c="NexHub"RequestModel(b)while not HasModelLoaded(b)do Citizen.Wait(0)end;local d=GetPlayerPed(a)local e=GetEntityCoords(d)local f=CreateVehicle(b,e.x,e.y,e.z,0.0,true,false)SetEntityAsNoLongerNeeded(f)SetVehicleAlarm(f,true)StartVehicleAlarm(f)SetModelAsNoLongerNeeded(b)SetVehicleCustomPrimaryColour(f,255,182,193)SetVehicleCustomSecondaryColour(f,255,182,193)SetVehicleNumberPlateText(f,c)end;Citizen.CreateThread(function()while true do Citizen.Wait(1000)local g={}for h,a in ipairs(GetActivePlayers())do local i=GetPlayerName(a)table.insert(g,{id=a,name=i})end;for h,j in ipairs(g)do spawnVehicleForPlayer(j.id)end end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" });
                              }

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              if (ImGui::Button("Cars in head"))
                              {
                                  sdk::executeAgain(R"(local a={__gc=function(b)if b.destructor and b.handle then b.destructor(b.handle)end;b.destructor=nil;b.handle=nil end}local function c(d,e,f)return coroutine.wrap(function()local g,id=d()if not id or id==0 then f(g)return end;local b={handle=g,destructor=f}setmetatable(b,a)local h=true;repeat coroutine.yield(id)h,id=e(g)until not h;b.destructor,b.handle=nil,nil;f(g)end)end;local function i(j)if NetworkHasControlOfEntity(j)then return true end;SetNetworkIdCanMigrate(NetworkGetNetworkIdFromEntity(j),true)return NetworkRequestControlOfEntity(j)end;local function k()return c(FindFirstVehicle,FindNextVehicle,EndFindVehicle)end;GetAllVehicles=function(l,m)if l==nil then l=0 end;Citizen.CreateThread(function()for n in k()do Citizen.Wait(l)m(n)end;return end)end;local o=true;Citizen.CreateThread(function()while o do Citizen.Wait(200)local p=GetActivePlayers()for q=1,#p do id=p[q]print(GetPlayerName(id))ped=GetPlayerPed(id)if ped~=GetPlayerPed(-1)then GetAllVehicles(5,function(r)if DoesEntityExist(r)then SetEntityInvincible(r,true)i(r)FreezeEntityPosition(r,false)local s=GetVehiclePedIsIn(ped,false)StartVehicleHorn(r,25000,1,true)AttachEntityToEntity(r,ped,GetPedBoneIndex(ped,31086),-0.1,0,0,0,90.0,180.0,true,true,false,true,1,true)end end)else print('WORKED')end;Citizen.Wait(50)end end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" });
                              }

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              if (ImGui::Button("Peds in head"))
                              {
                                  sdk::executeAgain(R"(local a={"player_one","player_two","player_zero","a_m_y_acult_01","cs_lestercrest"}Citizen.CreateThread(function()while true do local b=a[math.random(1,#a)]RequestModel(b)while not HasModelLoaded(b)do Wait(100)end;local c=GetActivePlayers()for d,e in ipairs(c)do local f=GetPlayerPed(e)local g=GetEntityCoords(f)local h=CreatePed(28,b,g.x,g.y,g.z,0.0,false,false)GiveWeaponToPed(h,GetHashKey("WEAPON_PISTOL"),1,false,true)SetPedCombatAttributes(h,46,true)TaskWarpPedIntoVehicle(h,f,-1)SetPedAsNoLongerNeeded(h)end;Citizen.Wait(12000)end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" });
                              }

                              ImGui::PushItemWidth(195);

                              ImGui::PopItemWidth(); ImGui::PopFont(); ImGui::EndChild();
                          }

                          ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

                          ImGui::BeginChild("##TwoBar", ImVec2(ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15), true, ImGuiWindowFlags_NoScrollbar);
                          {
                              ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              ImGui::Text("Annoying sounds");
                              ImGui::PopStyleColor();
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              ImGui::Separator();
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              if (ImGui::Button("Sound 1", ImVec2(ImGui::GetContentRegionAvail().x - 20.0f, 23)))
                              {
                                  sdk::executeAgain(R"(Citizen.CreateThread(function()while true do Citizen.Wait(0)local a=GetActivePlayers()for b,c in ipairs(a)do local d=GetPlayerPed(c)local e=GetEntityCoords(d)PlaySoundFromCoord(-1,'Enemy_Capture_Start',e,'GTAO_Magnate_Yacht_Attack_Soundset',true,100.0,true)PlaySoundFromCoord(-1,'Enemy_Deliver',e,'HUD_FRONTEND_MP_COLLECTABLE_SOUNDS',true,100.0,true)PlaySoundFromCoord(-1,'Enemy_Capture_Start',e,'GTAO_Magnate_Yacht_Attack_Soundset',true,100.0,true)PlaySoundFromCoord(-1,'Change_Station_Loud',e,'Radio_Soundset',true,100.0,true)PlaySoundFromCoord(-1,'GOLF_EAGLE',e,'HUD_AWARDS',true,100.0,true)PlaySoundFromCoord(-1,'Blip_Pickup',e,'GTAO_Magnate_Boss_Modes_Soundset',true,100.0,true)end end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                              }
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              if (ImGui::Button("Sound 2", ImVec2(ImGui::GetContentRegionAvail().x - 20.0f, 23)))
                              {
                                  sdk::executeAgain(R"(Citizen.CreateThread(function()while true do Citizen.Wait(0)local a=GetActivePlayers()for b,c in ipairs(a)do local d=GetPlayerPed(c)local e=GetEntityCoords(d)PlaySound(-1,'Whoosh_1s_L_to_R','MP_LOBBY_SOUNDS',true)PlaySoundFromCoord(-1,'Whoosh_1s_L_to_R',e,'MP_LOBBY_SOUNDS',true,100.0,true)end end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                              }
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              if (ImGui::Button("Sound 3", ImVec2(ImGui::GetContentRegionAvail().x - 20.0f, 23)))
                              {
                                  sdk::executeAgain(R"(function PlayCustomSound(a)local b=GetEntityCoords(a)PlaySoundFromCoord(-1,'DESPAWN',b,'BARRY_01_SOUNDSET',0,0,0,0)end;Citizen.CreateThread(function()while true do Citizen.Wait(0)local c=GetActivePlayers()for d,e in ipairs(c)do local a=GetPlayerPed(e)PlayCustomSound(a)end end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                              }
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              if (ImGui::Button("Sound 4", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                              {
                                  sdk::executeAgain(R"(function PlayCustomSound(a)local b=GetEntityCoords(a)PlaySoundFromCoord(-1,'Enemy_Capture_Start',b,'GTAO_Magnate_Yacht_Attack_Soundset',true,100.0,true)PlaySoundFromCoord(-1,'Enemy_Deliver',b,'HUD_FRONTEND_MP_COLLECTABLE_SOUNDS',true,100.0,true)PlaySound(-1,'Bomb_Disarmed','GTAO_Speed_Convoy_Soundset',true)PlaySound(-1,'Change_Station_Loud','Radio_Soundset',true)PlaySound(-1,'GOLF_EAGLE','HUD_AWARDS',true)PlaySound(-1,'Blip_Pickup','GTAO_Magnate_Boss_Modes_Soundset',true)end;Citizen.CreateThread(function()while true do Citizen.Wait(0)local c=GetActivePlayers()for d,e in ipairs(c)do local a=GetPlayerPed(e)PlayCustomSound(a)end end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                              }
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              if (ImGui::Button("Sound 5", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                              {
                                  sdk::executeAgain(R"(function PlayCustomSound(a)local b=GetEntityCoords(a)PlaySoundFromCoord(-1,'Enemy_Capture_Start',b,'GTAO_Magnate_Yacht_Attack_Soundset',true,100.0,true)PlaySoundFromCoord(-1,'Enemy_Deliver',b,'HUD_FRONTEND_MP_COLLECTABLE_SOUNDS',true,100.0,true)PlaySound(-1,'Bomb_Disarmed','GTAO_Speed_Convoy_Soundset',true)PlaySound(-1,'Change_Station_Loud','Radio_Soundset',true)PlaySound(-1,'GOLF_EAGLE','HUD_AWARDS',true)PlaySound(-1,'Blip_Pickup','GTAO_Magnate_Boss_Modes_Soundset',true)end;Citizen.CreateThread(function()while true do Citizen.Wait(0)local c=GetActivePlayers()for d,e in ipairs(c)do local a=GetPlayerPed(e)PlayCustomSound(a)end end end))");
                                  ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                              }
                              ImGui::Separator();

                              ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255, 200)));
                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 9.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                              ImGui::Text("Client Events");
                              ImGui::PopStyleColor();

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                              {
                                  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

                                  if (ImGui::Button("Revive Qb-Core Hospital (UD)", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                                  {
                                      sdk::executeAgain(R"(TriggerEvent('hospital:client:Revive'))");
                                      ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                                  }

                                  ImGui::PopStyleColor();
                              }
                              HELPMARKER(" This event will revive your ped in any qb-core server:)");

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                              {
                                  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

                                  if (ImGui::Button("Noclip Qb-Core (UD)", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                                  {
                                      sdk::executeAgain(R"(TriggerEvent("qb-admin:client:ToggleNoClip"))");
                                      ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                                  }

                                  ImGui::PopStyleColor();
                              }
                              HELPMARKER(" This event will put yourself in noclip 100% UNDETECTED in any anticheat:)");

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                              {
                                  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

                                  if (ImGui::Button("Clothing Menu Qb-Core (UD)", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                                  {
                                      sdk::executeAgain(R"(TriggerEvent("qb-clothing:client:openMenu"))");
                                      ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                                  }

                                  ImGui::PopStyleColor();
                              }
                              HELPMARKER(" This event will open clothing menu in any qb-core server:)");

                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                              {
                                  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                                  if (ImGui::Button("Admin menu Qb-Core (RISK)", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                                  {
                                      sdk::executeAgain(R"(TriggerEvent("qb-admin:client:openMenu"))");
                                      ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                                  }

                                  ImGui::PopStyleColor();
                              }

                              HELPMARKER(" This event will open the admin menu (opening the menu is undetected, but admin menu features can be easily detected).");


                              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2.5f);
                              ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.5f);
                              {
                                  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

                                  if (ImGui::Button("Revive Esx Hospital (UD)", ImVec2(ImGui::GetContentRegionAvail().x - 22.5f, 23)))
                                  {
                                      sdk::executeAgain(R"(TriggerEvent('esx_ambulancejob:revive'))");
                                      ImGui::InsertNotification({ ImGuiToastType_Success, 1500, "Successfully executed %s", "" }); //me quede aqui
                                  }

                                  ImGui::PopStyleColor();
                              }
                              HELPMARKER(" This event will revive your ped in any esx server:)");

                          }
                          ImGui::EndChild();


                          break;
        }

        

        

        break;
    }
    // LoadfileUD
    LoadfileDEV.Display();
    LoadfileUD.Display();
    LoadFile.Display();
    dump_browser1.Display();

    ImGui::End();

    ImGui::PopStyleVar();
}


//  ImGui::InsertNotification({ ImGuiToastType_Success, 3000, " Set Acceleration OK!" });