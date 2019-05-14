//
//  LayerIDManager.h
//  ProjectionApp
//
//  Created by Nicole Messier on 5/14/19.
//
//

#pragma once
#include "ofMain.h"

class LayerIDManager
{
public:
    
    
    static LayerIDManager& one()
    {
        static LayerIDManager instance;
        return instance;
    }
    
    LayerIDManager(){};
    ~LayerIDManager(){};
    
    string playNextObject = "playNextObject";
    string stopPlayingImgSeq = "stopPlayingImgSeq"; 
};
