#version 330
uniform float minHeight;
uniform float maxHeight;
uniform mat4 modelViewProjectionMatrix;
layout(location = 0) in vec4 position;
layout(location = 10) in vec4 color;

out vec4 FS_in_vertexColor;

void main(){
    
    float offset = position.z - minHeight;
    float heightDelta = maxHeight - minHeight;
    float normalizedOffset = offset / heightDelta;
    
    FS_in_vertexColor = color;
//    FS_in_vertexColor = vec4(0,1,0,1);
//    
//    if (normalizedOffset < float(0.333)) {
//        FS_in_vertexColor = vec4(0,0,1,1);
//    }
//    
//    if (normalizedOffset > float(0.666)) {
//            FS_in_vertexColor = vec4(1,0,0,1);
//    }
    
    FS_in_vertexColor = vec4(0,1,0,1);
    if (position.z < 0) {
        FS_in_vertexColor = vec4(0,0,1,1);
    }
    else {
        float normalizedHeight = position.z / maxHeight;
        if (normalizedHeight > 0.8) {
            FS_in_vertexColor = vec4(0.7,0.7,0.7,1);
        }
        else if (normalizedHeight > 0.35) {
            FS_in_vertexColor = vec4(0.55,0.27,0.07,1);
        }
    }

    
    
    gl_Position = modelViewProjectionMatrix * position;
}