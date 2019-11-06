//
//  Layer.cpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "Layer.h"

Layer::Layer()
{
    
}

Layer::~Layer()
{
    
}

void Layer::setup()
{
    
}

void Layer::update(float dt)
{
    for(auto & obj : mediaObjects)
    {
        obj.second->update(dt);
    }
}

void Layer::draw(ofVec2f offset, unordered_map<string, bool> & drawToggles){
    
    if(!isVisible)
    {
        return;
    }
    
    if(hasBgImage)
    {
        //Draw BackgroundImage
        bgImage->draw(0.0f, 0.0f);
    }

	if(drawToggles.size()){ //selectively draw
		for(auto &obj : mediaObjects){ // Draw media Objects
			auto it = drawToggles.find(obj.first);
			if(it!= drawToggles.end() && it->second){
				obj.second->draw(offset);
			}
		}
	}else{
		for(auto &obj : mediaObjects){ // Draw media Objects
			obj.second->draw(offset);
		}
	}
}

void Layer::drawDebug(ofVec2f offset, unordered_map<string, bool> & drawToggles){
	if(!isVisible) return;
	if(drawToggles.size()){ //selectively draw
		for(auto &obj : mediaObjects){ // Draw media Objects
			auto it = drawToggles.find(obj.first);
			if(it!= drawToggles.end() && it->second){
				obj.second->drawDebug();
			}
		}
	}else{
		for(auto &obj : mediaObjects){ // Draw media Objects
			obj.second->drawDebug();
		}
	}
}

#pragma mark VISIBILITY
void Layer::setIsVisible(bool visible)
{
    isVisible = visible;
}

#pragma mark MEDIA OBJECTS

void Layer::addBgImage(string imgPath)
{
    if(hasBgImage) return;
    
    ofLogNotice("Layer::addBgImage") << "Adding static bg image " << imgPath;
    
    bgImage = new ofImage();
    bgImage->load(imgPath);
    hasBgImage = true;
}

void Layer::addMediaObject(string UID,
                           ofVec2f pos,
                           int zoneOrder,
                           string zoneUID,
                           int layer,
                           LayerData::AnimationType animType)
{
    
    ofLogNotice("Layer::addMediaObject") << "Adding MediaObject-" << UID << " layer: " << layer;
    
    MediaObject * temp = new MediaObject();
    temp->setup(ofToUpper(UID), pos, zoneOrder, zoneUID, layer);
    temp->setAnimationType(animType); 
    
    mediaObjects.insert(pair<string, MediaObject*>(ofToUpper(UID), temp));
    
    mediaObjectIds.push_back(ofToUpper(UID));
    
}

MediaObject * Layer::getMediaObject( string UID)
{
    return mediaObjects.find(UID)->second; 
}

int Layer::getNumMediaObjects()
{
    return mediaObjects.size();
}

string Layer::getMediaObjectUID(int index)
{
    if(index >= mediaObjectIds.size())
        return "";
    
    return mediaObjectIds[index];
    
}
