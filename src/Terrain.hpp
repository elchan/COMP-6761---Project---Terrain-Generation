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
    
    using VertexVector = std::vector<ofVec3f>;
    using IndexType = typename VertexVector::size_type;
    
    static int const MAX_ITERATIONS = {8};
    
    Terrain() {
        
        mesh.addVertex(ofPoint(-unit/2, -unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(unit/2, -unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(unit/2, unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(-unit/2, unit/2, 0 /* unit*/));
        
        mesh.addIndex(0);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.enableIndices();
        vbo.setMesh(mesh, GL_STATIC_DRAW);
    }
    
    void draw() const {
        

        if (indicesEnabled) {
            vbo.drawElements(GL_QUADS, vbo.getNumIndices());
        }
        else {
            vbo.draw(GL_QUADS, 0, vbo.getNumVertices());
        }
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
        std::cout << "Diamond Square iterations: " << iterations << " resulted in " << mesh.getNumVertices() << " vertices, " << mesh.getNumVertices() / 4 << " quads" << std::endl;
    }
    
    
    void diamondSquareIterationByIdx() {
//        if (iterations >= MAX_ITERATIONS)
//            return;
        indicesEnabled = true;
        auto & previousItVertices = mesh.getVertices();
        auto previousItIndices = mesh.getIndices();
        mesh.clearIndices();
//        mesh.clear();
        iterations++;
        auto previousItIdx = previousItIndices.begin();
        while (previousItIdx != previousItIndices.end()) {
            
            IndexType bottomLeftIdx = *previousItIdx++;
            IndexType topLeftIdx = *previousItIdx++;
            IndexType topRightIdx = *previousItIdx++;
            IndexType bottomRightIdx = *previousItIdx++;
            
            ofVec3f bottomLeft = getVertex(bottomLeftIdx);
            ofVec3f topLeft = getVertex(topLeftIdx);
            ofVec3f topRight = getVertex(topRightIdx);
            ofVec3f bottomRight = getVertex(bottomRightIdx);
            
            
            IndexType middlePointIdx = addVertex(((bottomLeft + bottomRight + topRight + topLeft)/4.f));
            
            disturbMidpoint(getVertex(middlePointIdx), bottomLeft, topLeft, topRight, bottomRight);
            
            IndexType leftMidIdx = addVertex((topLeft + bottomLeft)/2.f);
            IndexType botMidIdx = addVertex((bottomRight + bottomLeft)/2.f);
            IndexType topMidIdx = addVertex((topRight + topLeft)/2.f);
            IndexType rightMidIdx = addVertex((bottomRight + topRight)/2.f);
            
            addQuad(bottomLeftIdx, leftMidIdx, middlePointIdx, botMidIdx);
            addQuad(leftMidIdx, topLeftIdx, topMidIdx, middlePointIdx);
            addQuad(middlePointIdx, topMidIdx, topRightIdx, rightMidIdx);
            addQuad(botMidIdx, middlePointIdx, rightMidIdx, bottomRightIdx);
            
        }
        vbo.clear(); // clear what is in the GPU
        vbo.setMesh(mesh, GL_STATIC_DRAW);
        std::cout << "Diamond Square iterations: " << iterations << " resulted in " << mesh.getNumVertices() << " vertices, " << mesh.getNumIndices() / 4 << " quads" << std::endl;
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
    
    /**
     * Add a quad, using indices, with clockwise front face vertex order
     */
    void addQuad(IndexType const bottomLeft, IndexType const topLeft, IndexType const topRight, IndexType const bottomRight) {
        mesh.addIndex(bottomLeft);
        mesh.addIndex(topLeft);
        mesh.addIndex(topRight);
        mesh.addIndex(bottomRight);
    }
    
    IndexType addVertex(ofVec3f const & vertex) {
        mesh.getVertices().push_back(vertex);
        return mesh.getVertices().size()-1;
    }
    
    ofVec3f & getVertex(IndexType const idx) {
        return mesh.getVertices()[idx];
    }
    
    void disturbMidpoint(ofVec3f & midPoint, ofVec3f const & bottomLeft, ofVec3f const & topLeft, ofVec3f const & topRight, ofVec3f const & bottomRight) const {
        midPoint[2] += (topLeft.distance(bottomRight) + topRight.distance(bottomLeft)) * randomValue();
    }

    bool indicesEnabled = {false};
    
    
    ofMesh mesh;
    ofVbo vbo;
    
};

#endif /* defined(__quaternionArcballExample__ofTerrain__) */
