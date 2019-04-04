//
//  ofxLayaerManager.cpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "ofxLayaerManager.h"

ofxLayaerManager::ofxLayaerManager()
{
    
}

ofxLayaerManager::~ofxLayaerManager()
{
    
}

void ofxLayaerManager::setup()
{
    /*
     Parse Json
     */
    
    bool parsingJson = jsonRef.open(jsonFilePath);
    
    if(parsingJson)
    {
        ofLogNotice("ofxLayaerManager::setup") << "Succesfully initial parse of " << jsonFilePath << " with data: " << jsonRef.getRawString();
        
        /*
         1. Set up initial layers
         */
        
        const ofxJSONElement & totalLayers = jsonRef["totalLayers"];
        int numLayers = totalLayers.asInt();
        
        for(int i = 0; i < numLayers; i++)
        {
            Layer* temp = new Layer();
            temp->setup();
            layers.push_back(temp);
        }
        
        /*
         2. Parse zones and individual objects
         */
        const ofxJSONElement & zones = jsonRef["zones"];
        
        for(int i = 0; i < zones.size(); i++)
        {
            const ofxJSONElement & zoneNum = zones[i]["zone-number"];
            const ofxJSONElement & animations = zones[i]["animations"];
            
            for(int j = 0; j < animations.size(); j++)
            {
                const ofxJSONElement & newMediaObj = animations[j];
                const ofxJSONElement & layer = newMediaObj["layer"];
                
                if(layer.asInt() < layers.size())
                {
                    const ofxJSONElement & name = newMediaObj["name"];
                    const ofxJSONElement & fps = newMediaObj["fps"];
                    const ofxJSONElement & loadType = newMediaObj["load-type"];
                    const ofxJSONElement & animType = newMediaObj["animation-type"];
                    
                    //App::LoadType loadType,
                    //App::AnimationType animType,
                    
                    const ofxJSONElement & path = newMediaObj["path"];
                    const ofxJSONElement & pos = newMediaObj["position"];
                    const ofxJSONElement & width = newMediaObj["width"];
                    const ofxJSONElement & height = newMediaObj["height"];
                    const ofxJSONElement & next = newMediaObj["next"];
                    
                    /*
                     
                     string path,
                     ofVec2f pos,
                     ofVec2f size,
                     int layer,
                     float fps,
                     string name,
                     App::LoadType loadType,
                     App::AnimationType animType,
                     string next,
                     int zone
                     
                     */
                    
                    layers[layer.asInt()]->addMediaObject(path.asString(), // path
                                                          ofVec2f(pos[0].asFloat(), pos[1].asFloat()), //pos
                                                          ofVec2f(width.asFloat(),height.asFloat()), //size
                                                          layer.asInt(), //layer
                                                          fps.asFloat(), //fps
                                                          name.asString(), //name
                                                          static_cast<App::LoadType>(loadType.asInt()), //load type
                                                          static_cast<App::AnimationType>(animType.asInt()), //animation type
                                                          next.asString(), // next
                                                          i); //zone
                }
                else
                {
                    ofLogError("ofxLayaerManager::setup") << "Object layer, " << layer.asInt() << ", is out of bounds!";
                }
            }
        }
        
        
        /*
         3.  Parse Image layers
         */
        
        const ofxJSONElement & imageLayers = jsonRef["image-layers"];
        
        for(int i = 0; i < imageLayers.size(); i++)
        {
            const ofxJSONElement & newImage = imageLayers[i];
            const ofxJSONElement & name = newImage["name"];
            const ofxJSONElement & path = newImage["path"];
            const ofxJSONElement & layer = newImage["layer"];
            
            if(layer.asInt() < layers.size())
            {
                layers[layer.asInt()]->addBgImage(path.asString());
            }
            else
            {
                ofLogError("ofxLayaerManager::setup") << "Object layer, " << layer.asInt() << ", is out of bounds!";
            }
            
        }
    }
    else
    {
        ofLogError("ofxLayaerManager::setup") << "Unable to parse json";
    }
}

void ofxLayaerManager::update(float dt)
{
    for( auto & layer : layers)
    {
        layer->update(dt);
    }
}

void ofxLayaerManager::draw()
{
    for( auto & layer : layers)
    {
        layer->draw();
    }
}

