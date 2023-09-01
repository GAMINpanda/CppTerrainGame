#include <string>
#include <vector>

class map{
    private:
        /*
        std::string water = "\u007e"; //~
        std::string forest = "\u2592"; //▒
        std::string land = "\u2591"; //░
        std::string mountains = "\u2593"; //▓
        std::string capital = "\uc2a9"; //©
        std::string city = "\uc2a4"; //¤
        std::string town = "\uc2ba"; //°
        */

        std::string water = " "; //simpler character set whilst I figure out the problem
        std::string forest = "#";
        std::string land = "=";
        std::string beach = "-";
        std::string deep = "~";
        std::string mountains = "^";
        std::string capital = "C";
        std::string city = "c";
        std::string town = ".";

        std::string mapTerrainAsVisual[30][100];
        double mapTerrainAsWeight[30][100];

        std::vector<double> genDisplace(int targetX, int targetY, int originX, int originY);
        double genDot(std::vector<double> gradient, std::vector<double> offset);
        double interpolate(double val1, double val2, double d);
        double overlayCircle(int x, int y, int a, int b, int xr, int yr);

    public:
        int mapY = 30;
        int mapX = 100;
        void genNoise(double (&inputArr)[30][100], int seed);
        void genTerrain(int seed);
        void outVisualMap();
        void outWeightMap();
        void outCircle(int a, int b, int xr, int yr);
};