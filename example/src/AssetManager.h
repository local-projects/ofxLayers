//
//  AssetManager.hpp
//  LayerSystemIDs
//
//  Created by Nicole Messier on 4/5/19.
//
//

#pragma once
#include "ofMain.h"

class AssetManager
{
public:
    
    AssetManager();
    ~AssetManager();
    
    void setup();
   ofImage* getImage(string uid);
    
    
private:
    
    unordered_map<string, ofImage*> images; //only static images
    
    string pandas = "Assets/pandas/";
    string assets = "Assets/";
};
