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
        void applyMod();

    private:
		string pathToGame;
		string pathToMod;
		string preferredResolution;
    bool assignedPathToGame;
    bool assignedPathToMod;

    static vector<string> resolutionList;
};