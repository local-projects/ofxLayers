//
//  ofxLayerManager.cpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "ofxLayerManager.h"

ofxLayerManager::ofxLayerManager()
{
    
}

ofxLayerManager::~ofxLayerManager()
{
    
}

void ofxLayerManager::setup(string jsonLayersFile)
{
    /*
     Parse Json
     */

	if(jsonLayersFile.size()) jsonFilePath = jsonLayersFile;
    bool parsingJson = jsonRef.open(jsonFilePath);
    
    if(parsingJson)
    {
        ofLogNotice("ofxLayerManager::setup") << "Succesfully initial parse of " << jsonFilePath << " with data: " << jsonRef.getRawString();
        
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
            const ofxJSONElement & zoneUID = zones[i]["zone-uid"];
            const ofxJSONElement & animations = zones[i]["animations"];
            
            zoneNames.push_back(zoneUID.asString());
            
            for(int j = 0; j < animations.size(); j++)
            {
                const ofxJSONElement & newMediaObj = animations[j];
                const ofxJSONElement & layer = newMediaObj["layer"];
                
                if(layer.asInt() < layers.size())
                {
                    const ofxJSONElement & name = newMediaObj["name"];
                    const ofxJSONElement & fps = newMediaObj["fps"];
                    const ofxJSONElement & loadType = newMediaObj["load-type"];
                    const ofxJSONElement & _animType = newMediaObj["animation-type"];
                    
                     const ofxJSONElement & pos = newMediaObj["a_position"];
                    
                    /*
                   addMediaObject (string UID,
                     ofVec2f pos,
                     int zoneOrder,
                     string zoneUID,
                     int layer,
                     LayerData::AnimationType animType)
                    */
                    LayerData::AnimationType animType = static_cast<LayerData::AnimationType>(_animType.asInt());
                    string uid = ofToUpper(name.asString());
                    
                    layers[layer.asInt()]->addMediaObject(uid,
                                                          ofVec2f(pos[0].asFloat(), pos[1].asFloat()),
                                                          zoneNum.asInt(),
                                                          zoneUID.asString(),
                                                          layer.asInt(),
                                                          animType); //zone
                    
                    
                    const ofxJSONElement & next = newMediaObj["next"];
                    if(next.asString().size())
                    {
                        ofLogNotice("ofxLayerManager") << next.asString() << " next object for " << name.asString();
                        
                        layers[layer.asInt()]->getMediaObject(uid)->setNextObject(next.asString());
                    }
                    
                    const ofxJSONElement & prev = newMediaObj["prev"];
                    
                    if(prev.asString().size())
                    {
                        ofLogNotice("ofxLayerManager") << prev.asString() << " prev object for " << name.asString();
                        layers[layer.asInt()]->getMediaObject(uid)->setPrevObject(prev.asString());
                    }
                    
                    switch(animType)
                    {
                        case LayerData::IMAGE_SEQUENCE: {
							break;
						}
                        case LayerData::TRAVERSING_2_POINT: {
                            
                            const ofxJSONElement & duration1 = newMediaObj["duration1"];
                            layers[layer.asInt()]->getMediaObject(uid)->setDuration1(duration1.asFloat());
                            
                            const ofxJSONElement & _B_Pos = newMediaObj["b_position"];
                            ofVec2f B_Pos = ofVec2f(_B_Pos[0].asFloat(), _B_Pos[1].asFloat());
                            layers[layer.asInt()]->getMediaObject(uid)->setB_Pos(B_Pos);
                                                                                                        
                            break;
                        }
                        case LayerData::ROTATING: { break; }
                        case LayerData::STEM: {
                            
                            
                            
                            break;
                        }
                        case LayerData::BOBBING: { break; }
                        case LayerData::TRAVERSING_3_POINT: {
                            
                            const ofxJSONElement & duration1 = newMediaObj["duration1"];
                            layers[layer.asInt()]->getMediaObject(uid)->setDuration1(duration1.asFloat());
                            
                            const ofxJSONElement & duration2 = newMediaObj["duration2"];
                            layers[layer.asInt()]->getMediaObject(uid)->setDuration2(duration2.asFloat());
                            
                            
                            const ofxJSONElement & _B_Pos = newMediaObj["b_position"];
                            ofVec2f B_Pos = ofVec2f(_B_Pos[0].asFloat(), _B_Pos[1].asFloat());
                            layers[layer.asInt()]->getMediaObject(uid)->setB_Pos(B_Pos);
                            
                            const ofxJSONElement & _C_Pos = newMediaObj["c_position"];
                            ofVec2f C_Pos = ofVec2f(_C_Pos[0].asFloat(), _C_Pos[1].asFloat());
                            layers[layer.asInt()]->getMediaObject(uid)->setC_Pos(C_Pos);
                            
                            break;
                        }
                        case LayerData::STATIC: { break; }
                        case LayerData::TWO_PT_SWAY:{break;}
                        default: break;
                    }
                                                                                              
                    
					if(newMediaObj.isMember("pauseDuration")){
						//pause duration is to be set in spreadsheet as a range, "float1-float2"
						//so we can randomize pause durations
						string pauseDurationInterval = newMediaObj["pauseDuration"].asString();
						if(pauseDurationInterval.size()){
							if(ofIsStringInString(pauseDurationInterval, "-")){
								auto elements = ofSplitString(pauseDurationInterval, "-");
								if(elements.size() == 2){
									float low = ofToFloat(elements[0]);
									float high = ofToFloat(elements[1]);
									layers[layer.asInt()]->getMediaObject(uid)->setPauseDuration(low, high);
								}else{
									ofLogError("ofxLayerManager") << "bad syntax parsing pause duration! \"" << pauseDurationInterval << "\"";
								}
							}else{
								ofLogError("ofxLayerManager") << "bad syntax parsing pause duration! \"" << pauseDurationInterval << "\"";
							}
						}
					}
					
                    animationsByZone.insert(pair<string, string>(ofToUpper(name.asString()), ofToUpper(zoneUID.asString()) ));
                }
                else
                {
                    ofLogError("ofxLayerManager::setup") << "Object layer, " << layer.asInt() << ", is out of bounds!";
                }
            }
        }
        
        
        /*
         3.  Parse Background
         */
        
        const ofxJSONElement & background = jsonRef["background-layer"];
        layers[0]->addBgImage(background.asString());
        
        for(int j = 0; j < layers.size(); j++)
        {
            for(int i = 0; i < layers[j]->getNumMediaObjects(); i++)
            {
                animationsByLayer.insert(pair<string, int>(ofToUpper(layers[j]->getMediaObjectUID(i)), j ));
                
                
            }
        }
    }
    else
    {
        ofLogError("ofxLayerManager::setup") << "Unable to parse json";
    }
}

void ofxLayerManager::update(float dt)
{
    for( auto & layer : layers)
    {
        layer->update(dt);
    }
}

void ofxLayerManager::draw(ofVec2f offset)
{
	unordered_map<string, bool> toggles;
    for( auto & layer : layers)
    {
        layer->draw(offset, toggles);
    }
}

#pragma mark LAYERS
int ofxLayerManager::getNumLayers()
{
    return layers.size();
}

Layer* ofxLayerManager::getLayer(int index)
{
    if(index >= layers.size())
        return nullptr;
    
    return layers[index];
}

#pragma mark GET MEDIA OBJECT

MediaObject *ofxLayerManager::getMediaObject(string UID)
{

	auto layerInfo = animationsByLayer.find(UID); 
	map<string, int>::iterator it = animationsByLayer.find(UID);
	
	if (it == animationsByLayer.end())
	{
		return nullptr;
	}

	int layerIndex = animationsByLayer.find(UID)->second;
	auto mediaObject = layers[layerIndex]->getMediaObject(UID);

    if(layerIndex < layers.size() && mediaObject)
    {
        return layers[layerIndex]->getMediaObject(UID);
    }
    else
    {
        return nullptr;
    }
    
}

