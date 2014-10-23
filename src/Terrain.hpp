//
//  ofTerrain.h
//  quaternionArcballExample
//
//  Created by Alexandre Pretyman on 10/18/14.
//
//

#ifndef __quaternionArcballExample__ofTerrain__
#define __quaternionArcballExample__ofTerrain__

#include <iostream>
#include "ofMain.h"
#include <cstdlib>
struct Terrain {
    
    static int const MAX_ITERATIONS = {8};
    
    Terrain() {
        
        mesh.addVertex(ofPoint(-unit/2, -unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(unit/2, -unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(unit/2, unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(-unit/2, unit/2, 0 /* unit*/));
        
        vbo.setMesh(mesh, GL_STATIC_DRAW);
    }
    
    void draw() const {
        vbo.draw(GL_QUADS, 0, mesh.getNumVertices());
    }
    
    void diamondSquareIteration() {
        if (iterations >= MAX_ITERATIONS)
            return;
        vector<ofVec3f> previousIteration = mesh.getVertices();
        mesh.clear();
        iterations++;
        auto originalVertexIt = previousIteration.begin();
        while (originalVertexIt != previousIteration.end()) {

            ofVec3f bottomLeft = *originalVertexIt++;
            ofVec3f topLeft = *originalVertexIt++;
            ofVec3f topRight = *originalVertexIt++;
            ofVec3f bottomRight = *originalVertexIt++;

            ofVec3f middlePoint = (bottomLeft + bottomRight + topRight + topLeft)/4.f;
            
            disturbMidpoint(middlePoint, bottomLeft, topLeft, topRight, bottomRight);

            ofVec3f leftMid = (topLeft + bottomLeft)/2.f;
            ofVec3f botMid = (bottomRight + bottomLeft)/2.f;
            ofVec3f topMid = (topRight + topLeft)/2.f;
            ofVec3f rightMid = (bottomRight + topRight)/2.f;
            
            addQuad(bottomLeft, leftMid, middlePoint, botMid);
            addQuad(leftMid, topLeft, topMid, middlePoint);
            addQuad(middlePoint, topMid, topRight, rightMid);
            addQuad(botMid, middlePoint, rightMid, bottomRight);
            
        }
        vbo.clear(); // clear what is in the GPU
        vbo.setMesh(mesh, GL_STATIC_DRAW);
        std::cout << "Diamond square iterations: " << iterations << " resulted in " << mesh.getNumVertices() << " vertices" << std::endl;
    }
    
    float randomValue() const {
        return std::rand() % 2 == 0 ? unit * kModifier : - unit * kModifier;
//        return iterations % 2 == 0 ? unit * kModifier  : - unit * kModifier;
    }
    
    unsigned int iterations = {0};
    float const unit = {550.f};
    float const kModifier = 0.0002;
    
private:
    /**
     * Add a quad to the mesh with clockwise front face vertex order
     */
    void addQuad(ofVec3f const & bottomLeft, ofVec3f const & topLeft, ofVec3f const & topRight, ofVec3f const & bottomRight) {
        mesh.addVertex(bottomLeft);
        mesh.addVertex(topLeft);
        mesh.addVertex(topRight);
        mesh.addVertex(bottomRight);
    }
    
    void disturbMidpoint(ofVec3f & midPoint, ofVec3f const & bottomLeft, ofVec3f const & topLeft, ofVec3f const & topRight, ofVec3f const & bottomRight) const {
        midPoint[2] += (topLeft.distance(bottomRight) + topRight.distance(bottomLeft)) * randomValue();
    }

    ofMesh mesh;
    ofVbo vbo;
    
};

#endif /* defined(__quaternionArcballExample__ofTerrain__) */
