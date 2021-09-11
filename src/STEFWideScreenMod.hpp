#include <string>
#include <vector>

using namespace std;

class STEFWideScreenMod {
	public:
        STEFWideScreenMod();

        void detectGameDirectory();
        void detectModDirectory();
        void getPreferredResolution();
        void applyMod();

    private:
		string pathToGame;
		string pathToMod;
		string preferredResolution;

		static vector<string> resolutionList;
};