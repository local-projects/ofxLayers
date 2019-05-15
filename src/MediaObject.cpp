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
    A_Pos = _pos;
    pos = _pos;
    layer = _layer;
    zoneOrder = _zoneOrder;
    zoneUID = _zoneUID; 
    layer = _layer;
    
    animFloat1.reset(0);
    animFloat1.setRepeatType(PLAY_ONCE);
    animFloat1.setDuration(ofRandom(1.5f, duration1));
    animFloat1.setCurve(LINEAR);
    ofAddListener(animFloat1.animFinished, this, &MediaObject::onAnim1Finish);
    
    animFloat2.reset(0);
    animFloat2.setRepeatType(PLAY_ONCE);
    animFloat2.setDuration(duration2);
    animFloat2.setCurve(LINEAR);
    ofAddListener(animFloat2.animFinished, this, &MediaObject::onAnim2Finish);
    
    pause.reset(0);
    pause.setRepeatType(PLAY_ONCE);
    pause.setDuration(duration2);
    pause.setCurve(LINEAR);
    ofAddListener(pause.animFinished, this, &MediaObject::onPauseFinish);
    
    ofAddListener(animFloat2.animFinished, this, &MediaObject::onAnim2Finish);
    
    
    ofAddListener(ofEvents().keyPressed, this, &MediaObject::onKeyPressed);
    
    
}

void MediaObject::update(float dt)
{
    float time = ofGetElapsedTimef();
    animFloat1.update(dt);
    animFloat2.update(dt);
    pause.update(dt);
    
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE: {break;}
        case LayerData::TRAVERSING_2_POINT:
        {
            pos = A_Pos + (B_Pos - A_Pos)*animFloat1.val();
            break;
        }
        case LayerData::ROTATING: { break;}
        case LayerData::WIGGLING: { break;}
        case LayerData::BOBBING:
        {
            pos.x = A_Pos.x + (bobDrift.x + randomnBobVal.x*ofNoise(time) )*animFloat1.val() + tex.getWidth()/2;
            pos.y = A_Pos.y + (bobDrift.y + randomnBobVal.y*ofNoise(time) )*animFloat1.val() + tex.getHeight()/2;
            break;
        }
        case LayerData::TRAVERSING_3_POINT: {
            
            
            //Three point traversing
            //Duraiton one is point A to B
            //Duraiton two is point C to A
            
            if(animFloat1.isAnimating())
            {
                pos = A_Pos + (B_Pos - A_Pos)*animFloat1.val();
            }
            else if(animFloat2.isAnimating())
            {
                pos = C_Pos + (A_Pos - C_Pos)*animFloat2.val();
            }
            
            break;
        }
        case LayerData::STATIC: { break;}
        default:break;
    }

    
    
}

void MediaObject::draw(ofVec2f offset)
{
    
    if(!animationVisble)
    {
        return;
    }
 
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE: {
            tex.draw(pos.x + offset.x, pos.y + offset.y, tex.getWidth(), tex.getHeight());
            
            
            if(debug)
            {
                ofSetColor(ofColor::yellow);
                
                ofVec2f debugPos = ofVec2f(pos.x, pos.y + tex.getHeight());
                ofDrawBitmapString(UID,
                                   debugPos.x,
                                   debugPos.y);
                
                ofSetColor(255);
            }
            break;
        }
        case LayerData::TRAVERSING_2_POINT:
        {
            tex.draw(pos.x + offset.x, pos.y + offset.y, tex.getWidth(), tex.getHeight());
           
            if(debug)
            {
                ofSetColor(ofColor::yellow);
                
                ofVec2f debugPos = ofVec2f(pos.x, pos.y + tex.getHeight());
                ofDrawBitmapString(UID + "\n" +
                                   "duration1:" + ofToString(duration1) + "\n",
                                   debugPos.x,
                                   debugPos.y);
                
                ofSetColor(255);
            }
            
            
            break;
        }
        case LayerData::ROTATING: {
            ofPushMatrix();
            
            ofTranslate(pos.x + tex.getWidth()/2 + offset.x, pos.y + tex.getHeight()/2 + offset.y);
            
            ofRotateDeg(360.0f*animFloat1.val());
            
            tex.draw(-tex.getWidth()/2, -tex.getHeight()/2);
            
            ofPopMatrix();
            break;
        }
        case LayerData::WIGGLING: { break;}
        case LayerData::BOBBING:
        {
            
            ofPushMatrix();
            ofTranslate(pos.x + offset.x, pos.y + offset.y);
            
            float rotationVal = bobRotation*animFloat1.val();
            float rotationValMapped = ofMap(rotationVal, 0, bobRotation, -bobRotation/2, bobRotation/2);
            ofRotateDeg(rotationValMapped);

            tex.draw(-tex.getWidth()/2, -tex.getHeight()/2);
            ofPopMatrix();
            break;
        }
        case LayerData::TRAVERSING_3_POINT: {
            
            
            //Three point traversing
            //Duraiton one is point A to B
            //Duraiton two is point C to A
            
            tex.draw(pos.x + offset.x, pos.y + offset.y, tex.getWidth(), tex.getHeight());
            
            if(debug)
            {
                ofSetColor(ofColor::yellow);
                
                ofVec2f debugPos = ofVec2f(pos.x, pos.y + tex.getHeight());
                ofDrawBitmapString(UID + "\n" +
                                   "duration1:" + ofToString(duration1) + "\n"
                                   "duration2:" + ofToString(duration2) + "\n",
                                   debugPos.x,
                                   debugPos.y);
                
                ofSetColor(255);
            }
            
            break;
        }
        case LayerData::STATIC: {
            tex.draw(pos.x + offset.x, pos.y + offset.y, tex.getWidth(), tex.getHeight());
            break;
        }
        default:break;
    }
    
    
    
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

void MediaObject::setZone(int _zoneOrder, string _zoneUID)
{
    zoneOrder = _zoneOrder;
    zoneUID = _zoneUID;
}

#pragma mark PLAYING SEQUENTIAL OBJECTS

void MediaObject::setNextObject(string _nextUID)
{
    nextUID =ofToUpper(_nextUID);
    sequentialObj = true;
}

void MediaObject::setPrevObject(string _prevUID)
{
    prevUID = ofToUpper(_prevUID);
    sequentialObj = true;
    animationVisble = false;
}

void MediaObject::sendPlayNextObject()
{
    
    //Send event via notification center
    ofxNotificationCenter::Notification mnd;
    mnd.ID = LayerIDManager::one().playNextObject;
    
    if(nextUID.size())
    {
        mnd.data["next"] = nextUID;
        ofLogNotice("MediaObject") << UID << " send play next object " << nextUID;
    }
    else
    {
        mnd.data["next"] = prevUID;
        ofLogNotice("MediaObject") << UID << " send play next object " << prevUID;
    }


    ofxNotificationCenter::one().postNotification(LayerIDManager::one().playNextObject, mnd);
    makeAnimationInvisible(); 
    
}


bool MediaObject::isFirstSequentialObject()
{
    
     if (nextUID.size())
     {
         return true;
     }
    else
    {
        return false;
    }
}

bool MediaObject::isSecondSequentialObject()
{
    if (prevUID.size())
    {
        return true;
    }
    else
    {
        return false;
    }
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
        case LayerData::TRAVERSING_2_POINT: {
            ofLogNotice("MediaObject") << UID << " ANIMTYPE: TRAVERSING_2_POINT";
            break;
        }
        case LayerData::ROTATING: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: ROTATING"; break;}
        case LayerData::WIGGLING: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: WIGGLING"; break;}
        case LayerData::BOBBING: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: BOBBING"; break;}
        case LayerData::TRAVERSING_3_POINT: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: TRAVERSING_3_POINT"; break;}
        case LayerData::STATIC: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: STATIC"; break;}
        default:break;
    }
}

LayerData::AnimationType MediaObject::getAnimationType()
{
    return animType;
}


#pragma mark DURATIONS

void MediaObject::setDuration1(float _duration1)
{
    duration1 = _duration1;
	animFloat1.setDuration(duration1); 
}

void MediaObject::setDuration2(float _duration2)
{
    duration2 = _duration2;
	animFloat2.setDuration(duration2);
}


#pragma mark POSITIONS
void MediaObject::setB_Pos(ofVec2f _B_Pos)
{
    B_Pos = _B_Pos;
}

void MediaObject::setC_Pos(ofVec2f _C_Pos)
{
    C_Pos = _C_Pos;
}

#pragma mark TEXTURE

void MediaObject::setTexture(ofTexture &_tex)
{
    tex = _tex; 
}

#pragma mark ANIMATIONS

void MediaObject::setAnimationState(AnimationState _animState)
{
    animState  = _animState;
    
    switch(animState)
    {
        case AnimationState::STOPPED: {
            break;
        }
        case AnimationState::PLAYING: {
            makeAnimationVisble();
            break;
        }
        case AnimationState::BRIEF_PAUSE: {
            break;
        }
        default: break;
    }
}

void MediaObject::triggerPlay()
{
    
    ofLogNotice("MediaObject") << "Toggle object-" << UID << " to playing";
    
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:{ break;}
        case LayerData::TRAVERSING_2_POINT:{ animFloat1.animateFromTo(0.0f, 1.0f); break;}
        case LayerData::ROTATING:{ animFloat1.animateFromTo(0.0f, 1.0f); break;}
        case LayerData::WIGGLING:{ animFloat1.animateFromTo(0.0f, 1.0f); break;}
        case LayerData::BOBBING:{ animFloat1.animateFromTo(0.0f, 1.0f); break; }
        case LayerData::TRAVERSING_3_POINT:{ animFloat1.animateFromTo(0.0f, 1.0f); break;}
        case LayerData::STATIC:{break;}
        default:break;
    }
    
    setAnimationState(AnimationState::PLAYING);
    
}

void MediaObject::triggerStop()
{
    setAnimationState(AnimationState::BRIEF_PAUSE);
}

void MediaObject::makeAnimationVisble()
{
    animationVisble = true;
}

void MediaObject::makeAnimationInvisible()
{
    animationVisble = false;
}

#pragma mark BOBBING

void MediaObject::setBobDrift(ofVec2f _bobDrift)
{
    ofLogNotice("MediaObject") << "bobDrift " << UID << " : " << bobDrift;
    bobDrift = _bobDrift;
}

void MediaObject::setBobRotation(float _bobRotation)
{
    bobRotation = _bobRotation; 
}

void MediaObject::setRandomnVariance(ofVec2f _randomnBobVal)
{
    randomnBobVal = _randomnBobVal;
}

#pragma mark ANIMATION CALLBACKS

void MediaObject::onAnim1Finish(ofxAnimatable::AnimationEvent & event)
{
    
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:{ break;}
        case LayerData::TRAVERSING_2_POINT:
        {
            
            pause.animateFromTo(0.0f, 1.0f);
            setAnimationState(AnimationState::BRIEF_PAUSE);
            break;
        }
        case LayerData::ROTATING:
        {
            if(animState == AnimationState::PLAYING)
            {
                animFloat1.animateFromTo(0.0f, 1.0f);
            }
            break;
        }
        case LayerData::WIGGLING:{break;}
        case LayerData::BOBBING:
        {
            //ofLogNotice() << "animFloat1.val(): " << animFloat1.val();
            
            if(animState == AnimationState::PLAYING && animFloat1.val() == 1)
            {
                animFloat1.animateFromTo(1.0f, 0.0f);
            }
            else if (animFloat1.val() == 0.0)
            {
                animFloat1.animateFromTo(0.0f, 1.0f);
            }
            break;
        }
        case LayerData::TRAVERSING_3_POINT:
        {
         
            setAnimationState(AnimationState::BRIEF_PAUSE);
            pause.animateFromTo(0.0f, 1.0f);
            animFloat2.reset(0.0f);
            
            break;
        }
        case LayerData::STATIC:{break;}
        default:break;
    }
}

void MediaObject::onAnim2Finish(ofxAnimatable::AnimationEvent & event)
{
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:{ break;}
        case LayerData::TRAVERSING_2_POINT:{break;}
        case LayerData::ROTATING:{break;}
        case LayerData::WIGGLING:{break;}
        case LayerData::BOBBING:{ break; }
        case LayerData::TRAVERSING_3_POINT:{
            
            
            setAnimationState(AnimationState::BRIEF_PAUSE);
            pause.animateFromTo(0.0f, 1.0f);
            
            break;
        }
        case LayerData::STATIC:{break;}
        default:break;
    }
}

void MediaObject::onPauseFinish(ofxAnimatable::AnimationEvent & event)
{
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:{ break;}
        case LayerData::TRAVERSING_2_POINT:{
            
            if(sequentialObj)
            {
                sendPlayNextObject();
                setAnimationState(MediaObject::STOPPED);
            }
            else
            {
                animFloat1.animateFromTo(0.0f, 1.0f); 
            }
            
            break;
        }
        case LayerData::ROTATING:{break;}
        case LayerData::WIGGLING:{break;}
        case LayerData::BOBBING:{ break; }
        case LayerData::TRAVERSING_3_POINT:{
            
            
            if(animState == AnimationState::BRIEF_PAUSE && !animFloat2.val())
            {
                animFloat2.animateFromTo(0.0f, 1.0f);
                setAnimationState(MediaObject::PLAYING);
            }
            else if(sequentialObj)
            {
                sendPlayNextObject();
                setAnimationState(MediaObject::STOPPED); 
            }
            else
            {
                animFloat1.animateFromTo(0.0f, 1.0f);
                setAnimationState(MediaObject::PLAYING);
            }
            
            break;
        }
        case LayerData::STATIC:{break;}
        default:break;
    }

}


#pragma mark DEBUG

void MediaObject::setDebug(bool _debug)
{
    debug = _debug; 
}

#pragma mark CALLBACKS

void MediaObject::onKeyPressed(ofKeyEventArgs & args)
{
    switch(args.key)
    {
        case 'M': { debug = !debug; break; }
        default: break;
    }
}
