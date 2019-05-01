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


void MediaObject::setup(string _UID, ofVec2f _pos, int _zoneOrder, string _zoneUID, int _layer)
{
    UID = _UID; 
    startPos = _pos;
    pos = _pos;
    layer = _layer;
    zoneOrder = _zoneOrder;
    zoneUID = _zoneUID; 
    layer = _layer;
    
    animFloat.reset(0);
    animFloat.setRepeatType(PLAY_ONCE);
    animFloat.setDuration(animTiming);
    animFloat.setCurve(TANH);
    //animFloat.animateFromTo(0.0f, 1.0f);
    ofAddListener(animFloat.animFinished, this, &MediaObject::onAnimFinish);
    
    
}

void MediaObject::update(float dt)
{
    animFloat.update(dt);
}

void MediaObject::draw()
{
 
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE: {
            tex.draw(pos.x, pos.y, tex.getWidth(), tex.getHeight());
            break;
        }
        case LayerData::TRAVERSING:
        {
            pos = startPos + (endPos - startPos)*animFloat.val();
            
            break;
        }
        case LayerData::ROTATING: { break;}
        case LayerData::WIGGLING: { break;}
        case LayerData::BOBBING:
        {
            
            
            pos.x = startPos.x + bobDrift.x*animFloat.val();
            pos.y = startPos.y + bobDrift.y*animFloat.val();

            break;
        }
        default:break;
    }
    
     tex.draw(pos.x, pos.y, tex.getWidth(), tex.getHeight());
    
}

void MediaObject::drawDebug()
{
    ofSetColor(ofColor::yellow);
    
    ofDrawBitmapString("UID: " + UID + "\n" +
                       "layer: " + ofToString(layer) + "\n" +
                       "zoneUID: " + zoneUID, pos.x, pos.y);
    
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

void MediaObject::setZone(int _zoneOrder, string _zoneUID)
{
    zoneOrder = _zoneOrder;
    zoneUID = _zoneUID;
}

#pragma mark GET

ofVec2f MediaObject::getPos()
{
    return pos; 
}

#pragma mark ANIMATION TYPES

void MediaObject::setAnimationType( LayerData::AnimationType _animType)
{
    animType = _animType;
    
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: IMAGE_SEQUENCE"; break;}
        case LayerData::TRAVERSING: {
            ofLogNotice("MediaObject") << UID << " ANIMTYPE: TRAVERSING";
            break;
        }
        case LayerData::ROTATING: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: ROTATING"; break;}
        case LayerData::WIGGLING: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: WIGGLING"; break;}
        case LayerData::BOBBING: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: BOBBING"; break;}
        default:break;
    }
}

LayerData::AnimationType MediaObject::getAnimationType()
{
    return animType;
}

void MediaObject::setEndPos(ofVec2f _endPos)
{
    endPos = _endPos; 
}

#pragma mark TEXTURE

void MediaObject::setTexture(ofTexture &_tex)
{
    tex = _tex; 
}

#pragma mark ANIMATIONS

void MediaObject::setPlaying(bool _playing)
{
    playing = _playing;
    
    if(playing && animType != LayerData::IMAGE_SEQUENCE)
    {
        ofLogNotice("MediaObject") << UID;
        animFloat.animateFromTo(0.0f, 1.0f);
    }
}

void MediaObject::onAnimFinish(ofxAnimatable::AnimationEvent & event)
{
    
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:{ break;}
        case LayerData::TRAVERSING:
        {
            if(playing)
            {
                animFloat.animateFromTo(0.0f, 1.0f);
            }
            break;
        }
        case LayerData::ROTATING:
        {
            
            break;
        }
        case LayerData::WIGGLING:
        {
            
            break;
        }
        case LayerData::BOBBING:
        {
            ofLogNotice() << "animFloat.val(): " << animFloat.val();
            if(playing && animFloat.val() == 1)
            {
                animFloat.animateFromTo(1.0f, 0.0f);
            }
            else if (animFloat.val() == 0.0)
            {
                animFloat.animateFromTo(0.0f, 1.0f);
            }
            break;
        }
        default:break;
    }
}

void MediaObject::setBobDrift(ofVec2f _bobDrift)
{
    bobDrift = _bobDrift;
}
