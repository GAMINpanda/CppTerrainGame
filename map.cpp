#include "map.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <random>

using namespace std;

vector<double> map::genDisplace(int targetX, int targetY, int originX, int originY){
    return { (double)(targetX - originX) / sqrt(50), (double)(targetY - originY) / sqrt(50)};
}

double map::genDot(vector<double> gradient, vector<double> offset){
    return (gradient[0] * offset[0]) + (gradient[1] * offset[1]);
}

double map::interpolate(double val1, double val2, double d){ //interpolate values
    return (val2 - val1) * (3.0 - d * 2.0) * d * d + val1;
}

void map::genNoise(double (&arrInput)[30][100], int seed){ //Generate Perlin Noise

    double wholeX, wholeY, fractionalX, fractionalY, randomX, randomY, dotProduct[4];
    double inter1, inter2, final, fracPartX, fracPartY;
    vector<double> offsetVectors[4], gradientVectors[4];
    int topCornerX, topCornerY;

    vector<double> vectorGrid[6][20];


    for(int y = 0; y < 6; y++){
        for(int x = 0; x < 20; x++){

            randomX = atof(to_string(seed).substr(0, y % 10).c_str()) / ((x + 1)); //some sequence to generate a random number using seed
            wholeX = floor(randomX);
            fractionalX = ((randomX - wholeX) * 2) - 1; //wrap around -1 ==> 1

            randomY = atof(to_string(seed).substr(0, x % 10).c_str()) / ((y + 1));
            wholeY = floor(randomY);
            fractionalY = ((randomY - wholeY) * 2) - 1;

            vectorGrid[y][x] = {
                fractionalX,
                fractionalY
                }; //give grid vectors
        }
    }

    for (int y = 0; y < mapY; y++){
        for (int x = 0; x < mapX; x++){

            topCornerX = min(div(x, 5).quot, 18); //Find top right corner vector
            topCornerY = min(div(y, 5).quot, 4);

            fracPartX = div(x, 5).rem;
            fracPartY = div(y, 5).rem;
            
            gradientVectors[0] = vectorGrid[topCornerY][topCornerX]; //identify gradient vectors
            gradientVectors[1] = vectorGrid[topCornerY][topCornerX + 1];
            gradientVectors[2] = vectorGrid[topCornerY + 1][topCornerX];
            gradientVectors[3] = vectorGrid[topCornerY + 1][topCornerX + 1];

            offsetVectors[0] = genDisplace(topCornerX * 5, topCornerY * 5, x, y); //calculate offset/displacement vector between coord and each corner
            offsetVectors[1] = genDisplace((topCornerX + 1) * 5, topCornerY * 5, x, y);
            offsetVectors[2] = genDisplace(topCornerX * 5, (topCornerY + 1) * 5, x, y);
            offsetVectors[3] = genDisplace((topCornerX + 1) * 5, (topCornerY + 1) * 5, x, y);

            for(int i = 0; i < 4; i++){
                dotProduct[i] = genDot(gradientVectors[i], offsetVectors[i]); //dot product between gradient and offset
            }

            //interpolate the dot products from each
            inter1 = interpolate(dotProduct[0], dotProduct[1], fracPartX / 2.5);
            inter2 = interpolate(dotProduct[2], dotProduct[3], fracPartX / 2.5);
            final = interpolate(inter1, inter2, fracPartY / 5);

            arrInput[y][x] = final;
        }
    }
}

double map::overlayCircle(int x, int y, int a , int b, int xr, int yr){ //overlays circle over weights to create central bias
    double xWeighting = sqrt((x - xr) * (x - xr) / (a * a));
    double yWeighting = sqrt((y - yr) * (y - yr) / (b * b));
    double maxDist = sqrt((xr * xr) / (a * a)) + sqrt((yr * yr) / (b * b));
    return 1 - ((2 * (xWeighting + yWeighting)) / maxDist); // inverse eq of ellipse
}

void map::outCircle(int a, int b, int xr, int yr){
    double circleWeight;
    for (int y = 0; y < mapY; y++){
        for (int x = 0; x < mapX; x++){
            circleWeight = overlayCircle(x, y, a, b, xr, yr);
            if (circleWeight > 0){
                cout << "#";
            }
            else{
                cout << " ";
            }
        }
        cout << "|" << endl;
    }
}

void map::genTerrain(int seed){
    double workingWeight, circleOneWeight, circleTwoWeight;
    genNoise(mapTerrainAsWeight, seed);

    int variableX = seed % 100;
    int variableY = seed % 30;

    for (int y = 0; y < mapY; y++){
        for (int x = 0; x < mapX; x++){
            circleOneWeight = overlayCircle(x, y, 18, 4.5, 50, 15);
            circleTwoWeight = overlayCircle(x, y, 16, 4.5, variableX, variableY);
            workingWeight = (mapTerrainAsWeight[y][x] + (circleOneWeight * 2.5) + circleTwoWeight) / 4.5;

            if (workingWeight > 0.6){
                mapTerrainAsVisual[y][x] = mountains;
            }
            else if (workingWeight > 0.4){
                mapTerrainAsVisual[y][x] = forest;
            }

            else if (workingWeight > 0.2){
                mapTerrainAsVisual[y][x] = land;
            }
            else if (workingWeight > 0){
                mapTerrainAsVisual[y][x] = beach;
            }
            else{
                mapTerrainAsVisual[y][x] = water;
            }
        }
    }
}

void map::outVisualMap(){
    for (int y = 0; y < mapY; y++){
        cout << "|";
        for (int x = 0; x < mapX; x++){
            cout << mapTerrainAsVisual[y][x];
        }
        cout << "|" << endl;
    }
}

void map::outWeightMap(){
    for (int y = 0; y < mapY; y++){
        cout << "|";
        for (int x = 0; x < mapX; x++){
            cout << mapTerrainAsWeight[y][x];
            cout << ", ";
        }
        cout << "|" << endl;
    }
}