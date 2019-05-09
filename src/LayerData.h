//
//  LayerData.h
//  ProjectionApp
//
//  Created by Nicole Messier on 4/24/19.
//
//

#include "ofMain.h"

namespace LayerData {
   
    //#SEQUENCE = 0, TRAVERSING_2_POINT = 1, ROTATING = 2, WIGGLING = 3, BOBBING = 4, TRAVERSING_3_POINT = 5, STATIC = 6

    enum AnimationType
    {
        IMAGE_SEQUENCE, // 0 & default
        TRAVERSING_2_POINT, // 1
        ROTATING, // 2
        WIGGLING, // 3,
        BOBBING, //4
        TRAVERSING_3_POINT, //5
        STATIC, //6
       //CURVE,
        NUM_ANIMATION_TYPE //7
    };
}
