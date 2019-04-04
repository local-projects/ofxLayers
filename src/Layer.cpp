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
    
}

void Layer::draw(){
    
    if(hasBgImage)
    {
        //Draw BackgroundImage
        bgImage->draw(0.0f, 0.0f);
    }

    
    // Draw media Objects
    for(auto &obj : mediaObjects)
    {
        obj->draw();
        obj->drawDebug();
    }
}

#pragma mark VISIBILITY
void Layer::setIsVisble(bool visible)
{
    
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

void Layer::addMediaObject(string path,
                           ofVec2f pos,
                           ofVec2f size,
                           int layer,
                           float fps,
                           string name,
                           App::LoadType loadType,
                           App::AnimationType animType,
                           string next,
                           int zone)
{
    
    ofLogNotice("Layer::addMediaObject") << "Adding media object, " << name << ", with path, " << path;
    ofLogNotice("Layer::addMediaObject") << "Object-" << name << " size: " << size;
    
    MediaObject * temp = new MediaObject();
    temp->setup(path, pos, size, layer);
    temp->setFPS(fps);
    temp->setName(name);
    temp->setTypes(loadType, animType);
    temp->setNextOjbect(next);
    temp->setZone(zone);
    mediaObjects.push_back(temp);
    
}

