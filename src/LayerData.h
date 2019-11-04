//
//  LayerData.h
//  ProjectionApp
//
//  Created by Nicole Messier on 4/24/19.
//
//

#include "ofMain.h"

namespace LayerData {
   
    //#SEQUENCE = 0, TRAVERSING_2_POINT = 1, ROTATING = 2, STEM = 3, BOBBING = 4, TRAVERSING_3_POINT = 5, STATIC = 6, TWO_PT_SWAY = 7

    enum AnimationType
    {
        IMAGE_SEQUENCE, // 0 & default
        TRAVERSING_2_POINT, // 1
        ROTATING, // 2
        STEM, // 3,
        BOBBING, //4
        TRAVERSING_3_POINT, //5
        STATIC, //6,
        TWO_PT_SWAY, //7
		//SPLIT_ANIMATION, //8 (single animation split in 2 img sequences, when one plays the other stops and disappears, back and forth) - using SequentialObject
       //CURVE,
        NUM_ANIMATION_TYPE //8
    };
}
