//
//  MediaObject.cpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "MediaObject.h"

MediaObject::MediaObject()
{
    
}

MediaObject::~MediaObject()
{
    
}


void MediaObject::setup(string _UID, ofVec2f _pos, int _zone, int _layer)
{

    pos = _pos;
    layer = _layer;
    zone = _zone;
    layer = _layer;
    
    
}

void MediaObject::update(float dt)
{
    
}

void MediaObject::draw()
{

}

void MediaObject::drawDebug()
{
    ofSetColor(ofColor::yellow);
    
    ofDrawBitmapString("UID: " + UID + "\n" +
                       "layer: " + ofToString(layer) + "\n" +
                       "zone: " + ofToString(zone), pos.x, pos.y);
    
    ofSetColor(255);
}

#pragma mark ZONES

void MediaObject::setUID(string _UID)
{
    UID = _UID;
}

void MediaObject::setPosition(ofVec2f _pos)
{
    pos = _pos;
}


void MediaObject::setLayer(int _layer)
{
    layer = _layer;
}

void MediaObject::setNextOjbect(string _nextUID)
{
    nextUID = _nextUID;
}

void MediaObject::setZone(int _zone)
{
    zone = _zone;
}

