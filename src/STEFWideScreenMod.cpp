#include "STEFWideScreenMod.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

vector<string> STEFWideScreenMod::resolutionList({"1024x600", "1024x640", "1280x720", "1280x768",
                                                  "1280x800", "1360x768", "1366x768", "1440x900",
                                                  "1600x900", "1680x945", "1680x1050", "1920x1080",
                                                  "1920x1200", "2048x1152", "2560x1080", "2560x1440",
                                                  "2560x1600", "3072x768", "3440x1440", "3840x1024",
                                                  "3840x1600", "3840x2160", "4096x2160", "4800x900",
                                                  "5040x1050", "5760x1080", "5760x1200", "7680x1440"});

STEFWideScreenMod::STEFWideScreenMod() : pathToGame("C:\\Program Files (x86)\\GOG Galaxy\\Games\\Star Trek Elite Force"),
                                         pathToMod(""),
                                         preferredResolution(""),
                                         assignedPathToGame(false),
                                         assignedPathToMod(false),
                                         detectedOldFiles(false){};

bool STEFWideScreenMod::detectConfigFile()
{
    if (fs::exists("dirs.ini"))
    {
        ifstream dirsFile("dirs.ini");
        string line("");
        string dir("");
        string dirVal("");
        unsigned int posOfColon(0);

        while (getline(dirsFile, line))
        {
            posOfColon = line.find_first_of(':');
            dir = line.substr(0, posOfColon);
            dirVal = line.substr(posOfColon + 1, line.length() - posOfColon + 1);

            if (dir == "GameDir")
            {
                if (fs::exists(dirVal + "\\stvoy.exe"))
                {
                    pathToGame = dirVal;
                    assignedPathToGame = true;
                }
            }
            else if (dir == "ModDir")
            {
                if (fs::exists(dirVal))
                {
                    pathToMod = dirVal;
                    assignedPathToMod = true;
                }
            }
            else
            {
                continue;
            }
        }

        dirsFile.close();

        if (assignedPathToGame && assignedPathToMod)
        {
            return true;
        }
    }

    return false;
}

void STEFWideScreenMod::getGameDirectory()
{
    if (!assignedPathToGame)
    {
        if (!fs::exists(pathToGame + "\\stvoy.exe"))
        {
            cout << "Could not find the path to the game directory." << endl;

            while (!fs::exists(pathToGame + "\\stvoy.exe"))
            {
                cout << "Please provide a valid path to the game directory:" << endl;
                getline(cin, pathToGame);
                cout << endl;
            }
        }
        else
        {
            assignedPathToGame = true;

            ofstream dirsFile("dirs.ini", ios_base::app);
            dirsFile << "GameDir:" << pathToGame << endl;
            dirsFile.close();
        }

        cout << "Game directory found!" << endl
             << endl;
    }
}

void STEFWideScreenMod::getModDirectory()
{
    if (!assignedPathToMod)
    {
        bool pathIsValid(false);

        cout << "Please download the widescreen fix from this website:" << endl
             << "https://www.wsgf.org/dr/star-trek-voyager-elite-force" << endl
             << endl
             << "Once you have downloaded the archive, extract it anywhere into its own folder" << endl;

        do
        {
            cout << "Please provide the path to the extracted mod folder:" << endl;

            getline(cin, pathToMod);
            cout << endl;

            // Check if the directory exists.
            while (!fs::exists(pathToMod))
            {
                cout << "The path you provided does not exist. Please provide a valid path:" << endl;
                getline(cin, pathToMod);
                cout << endl;
            }

            pathIsValid = true;

            // Check if the directory contains all of the folders.
            for (int i(0); i < resolutionList.size(); ++i)
            {
                if (!fs::exists(pathToMod + "//" + resolutionList.at(i)))
                {
                    pathIsValid = false;
                    cout << "Error! The path does not contain all the mod files. Please extract the mod archive and DO NOT TOUCH ANYTHING IN IT." << endl;
                    break;
                }
            }
        } while (!pathIsValid);

        assignedPathToMod = true;

        ofstream dirsFile("dirs.ini", ios_base::app);
        dirsFile << "ModDir:" << pathToMod << endl;
        dirsFile.close();
    }
}

void STEFWideScreenMod::getPreferredResolution()
{
    if (assignedPathToGame && assignedPathToMod)
    {
        string input("");

        cout << "Which resolution would you like your game to have?" << endl;

        for (int i(0); i < resolutionList.size(); ++i)
        {
            cout << "[" + to_string(i + 1) + "] - " + resolutionList.at(i) << endl;
        }

        int resolutionNum(0);

        do
        {
            cout << endl
                 << "Type the number of the resolution you want: ";
            cin >> input;
            resolutionNum = stoi(input);

        } while (resolutionNum < 1 || resolutionNum > resolutionList.size());

        preferredResolution = resolutionList.at(resolutionNum - 1);

        cout << endl
             << "Your resolution: " << preferredResolution << endl
             << endl;
    }
}

bool STEFWideScreenMod::detectOldFiles()
{
    if (assignedPathToGame && assignedPathToMod)
    {
        if (fs::exists(pathToGame + "//stvoy.exe.old") ||
            fs::exists(pathToGame + "//efgamex86.dll.old") ||
            fs::exists(pathToGame + "//BaseEF//autoexec.cfg.old"))
        {
            detectedOldFiles = true;
            return true;
        }
    }

    return false;
}

void STEFWideScreenMod::applyMod(const bool &backupFiles)
{
    if (assignedPathToGame && assignedPathToMod)
    {
        vector<string> rootFiles({
            "stvoy.exe",
            "efgamex86.dll",
        });
        string baseEfFile("autoexec.cfg");

        for (int i(0); i < rootFiles.size(); ++i)
        {
            if (backupFiles)
            {
                fs::rename(pathToGame + "//" + rootFiles.at(i),
                           pathToGame + "//" + rootFiles.at(i) + ".old");
            }

            fs::copy(pathToMod + "//" + preferredResolution + "//" + rootFiles.at(i),
                     pathToGame + "//" + rootFiles.at(i),
                     fs::copy_options::overwrite_existing);
        }

        if (backupFiles && fs::exists(pathToGame + "//BaseEF//" + baseEfFile))
        {
            fs::rename(pathToGame + "//BaseEF//" + baseEfFile,
                       pathToGame + "//BaseEF//" + baseEfFile + ".old");
        }

        fs::copy(pathToMod + "//" + preferredResolution + "//BaseEF//" + baseEfFile,
                 pathToGame + "//BaseEF//" + baseEfFile,
                 fs::copy_options::overwrite_existing);

        cout << "Successfully installed the mod." << endl
             << endl;
    }
}

void STEFWideScreenMod::revertMod()
{
    if (detectedOldFiles && assignedPathToGame && assignedPathToMod)
    {
        vector<string> rootFiles({
            "stvoy.exe",
            "efgamex86.dll",
        });
        string baseEfFile("autoexec.cfg");

        for (int i(0); i < rootFiles.size(); ++i)
        {
            if (fs::exists(pathToGame + "//" + rootFiles.at(i) + ".old"))
            {
                fs::remove(pathToGame + "//" + rootFiles.at(i));
                fs::rename(pathToGame + "//" + rootFiles.at(i) + ".old",
                           pathToGame + "//" + rootFiles.at(i));
            }
        }

        if (fs::exists(pathToGame + "//BaseEF//" + baseEfFile + ".old"))
        {
            fs::remove(pathToGame + "//BaseEF//" + baseEfFile);
            fs::rename(pathToGame + "//BaseEF//" + baseEfFile + ".old",
                       pathToGame + "//BaseEF//" + baseEfFile);
        }
        else if (fs::exists(pathToGame + "//BaseEF//" + baseEfFile))
        {
            fs::remove(pathToGame + "//BaseEF//" + baseEfFile);
        }

        cout << endl
             << "Successfully uninstalled the mod." << endl
             << endl;
    }
}