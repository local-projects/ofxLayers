//
//  LayerData.h
//  ProjectionApp
//
//  Created by Nicole Messier on 4/24/19.
//
//

#include "ofMain.h"

namespace LayerData {
   
    enum AnimationType
    {
        IMAGE_SEQUENCE, // 0 & default
        TRAVERSING, // 1
        ROTATING, // 2
        WIGGLING, // 3,
        BOBBING, //4
        NUM_ANIMATION_TYPE //5
    };
}
