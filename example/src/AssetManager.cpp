//
//  AssetManager.cpp
//  LayerSystemIDs
//
//  Created by Nicole Messier on 4/5/19.
//
//

#include "AssetManager.h"

AssetManager::AssetManager()
{
    
}

AssetManager::~AssetManager()
{
    
}

void AssetManager::setup()
{
    
    //Set up assets
    ofDirectory dir;
    dir.allowExt("jpeg");
    dir.allowExt("png");
    dir.listDir(assets);
    
    for(int i = 0; i<dir.size(); i++)
    {
        string path = dir.getPath(i);
        ofFile file = dir.getFile(i);
        string baseName = file.getBaseName();
        
        ofLogNotice("AssetManager") << "Adding " << ofToUpper(baseName) << " at path " << path;
        
        
        ofImage * temp = new ofImage();
        temp->load(path);
        images.insert(pair<string, ofImage*>(ofToUpper(baseName), temp));
    }
    
    ofDirectory pandaDir;
    pandaDir.allowExt("jpeg");
    pandaDir.allowExt("png");
    pandaDir.listDir(pandas);
    
    for(int i = 0; i<pandaDir.size(); i++)
    {
        string path = pandaDir.getPath(i);
        ofFile file = pandaDir.getFile(i);
        string baseName = file.getBaseName();
        
        ofLogNotice("AssetManager") << "Adding " << ofToUpper(baseName) << " at path " << path;
        
        
        ofImage * temp = new ofImage();
        temp->load(path);
        
        
        images.insert(pair<string, ofImage*>(ofToUpper(baseName), temp));
    }
    
}

ofImage* AssetManager::getImage(string uid)
{
    //ofLogNotice("AssetManager") << "get image: " << ofToUpper(uid);
    return images.find(ofToUpper(uid))->second;
}
