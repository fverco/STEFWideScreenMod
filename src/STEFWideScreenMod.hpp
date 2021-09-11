#include <string>
#include <vector>

using namespace std;

class STEFWideScreenMod {
	public:
    STEFWideScreenMod();

    bool detectConfigFile();
    void getGameDirectory();
    void getModDirectory();
    void getPreferredResolution();
    bool detectOldFiles();
    void applyMod(const bool &backupFiles = true);
    void revertMod();

  private:
		string pathToGame;
		string pathToMod;
		string preferredResolution;
    bool assignedPathToGame;
    bool assignedPathToMod;
    bool detectedOldFiles;

    static vector<string> resolutionList;
};