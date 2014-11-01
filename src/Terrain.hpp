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
#include <limits>
#include <glm/glm.hpp>
#include <cmath>

struct Terrain {
    
    using VertexVector = std::vector<ofVec3f>;
    using IndexType = typename VertexVector::size_type;
    
    static int const MAX_ITERATIONS = {8};
    
    Terrain() {
        
        reset();


    }
    
    void reset() {
        iterations = 0;
        mesh.clearVertices();
        mesh.clearIndices();
        
        mesh.addVertex(ofPoint(-unit/2, -unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(-unit/2, unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(unit/2, unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(unit/2, -unit/2, 0 /* unit*/));
        
        
        
        mesh.addIndex(0);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.addIndex(0);
        mesh.enableIndices();

        vbo.clear(); // clear what is in the GPU
        vbo.setMesh(mesh, GL_STATIC_DRAW);
    }
    
    void draw() {
        
        if (indicesEnabled) {
            vbo.drawElements(GL_TRIANGLES, vbo.getNumIndices());
        }
        else {
            vbo.draw(GL_QUADS, 0, vbo.getNumVertices());
        }


    }
    
//    void diamondSquareIteration() {
//        if (iterations >= MAX_ITERATIONS)
//            return;
//        vector<ofVec3f> previousIteration = mesh.getVertices();
//        mesh.clear();
//        iterations++;
//        auto originalVertexIt = previousIteration.begin();
//        while (originalVertexIt != previousIteration.end()) {
//
//            ofVec3f bottomLeft = *originalVertexIt++;
//            ofVec3f topLeft = *originalVertexIt++;
//            ofVec3f topRight = *originalVertexIt++;
//            ofVec3f bottomRight = *originalVertexIt++;
//
//            ofVec3f middlePoint = (bottomLeft + bottomRight + topRight + topLeft)/4.f;
//            
//            disturbMidpoint(middlePoint, bottomLeft, topLeft, topRight, bottomRight);
//
//            ofVec3f leftMid = (topLeft + bottomLeft)/2.f;
//            ofVec3f botMid = (bottomRight + bottomLeft)/2.f;
//            ofVec3f topMid = (topRight + topLeft)/2.f;
//            ofVec3f rightMid = (bottomRight + topRight)/2.f;
//            
//            addQuad(bottomLeft, leftMid, middlePoint, botMid);
//            addQuad(leftMid, topLeft, topMid, middlePoint);
//            addQuad(middlePoint, topMid, topRight, rightMid);
//            addQuad(botMid, middlePoint, rightMid, bottomRight);
//            
//        }
//        vbo.clear(); // clear what is in the GPU
//        vbo.setMesh(mesh, GL_STATIC_DRAW);
//        std::cout << "Diamond Square iterations: " << iterations << " resulted in " << mesh.getNumVertices() << " vertices, " << mesh.getNumVertices() / 4 << " quads" << std::endl;
//    }
    
    
    void diamondSquareIterationByIdx() {
        if (iterations >= MAX_ITERATIONS)
            return;
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
                                    previousItIdx++; // skip top right
            IndexType bottomRightIdx = *previousItIdx++;
                                    previousItIdx++; // skip bottom left
            
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
        

      
        
        
//        int red = 0, green = 0, blue = 0;

//        glm::vec3 red(1.f, 0.f, 0.f);
//        glm::vec3 green(0.f, 1.f, 0.f);
//        glm::vec3 blue(0.f, 0.f, 1.f);
//        colors.clear();
//        for (auto & vec : mesh.getVertices()) {
////            applyElevationFunction(vec);
//            float offset = vec[2] - minHeight;
//            float heightDelta = maxHeight - minHeight;
//            float normalizedOffset = offset / heightDelta;
////            std::cout << "Z value: " << vec[2] << " delta " << heightDelta << " normalizedOffSet " << normalizedOffset << std::endl;
//            if (normalizedOffset < 0.333) {
//                colors.push_back(ofVec4f(0, 0, 1, 1));
////                colors.push_back(0);
////                colors.push_back(1);
//            }
//            else {
//                if (normalizedOffset < 0.7) {
//                colors.push_back(ofVec4f(0, 1, 0, 1));
//                } else {
//                colors.push_back(ofVec4f(1, 0, 0, 1));
//                }
//            }
//        }
//        std::cout << "Red: " << red << " Green: " << green << " Blue: " << blue << std::endl;
        vbo.clear(); // clear what is in the GPU
        vbo.setMesh(mesh, GL_STATIC_DRAW);
        std::cout << "Diamond Square iterations: " << iterations << " resulted in " << mesh.getNumVertices() << " vertices, " << mesh.getNumIndices() / 4 << " quads" << std::endl;
        std::cout << "MinHeight: " << minHeight << " MaxHeight: " << maxHeight << std::endl;
        
    }
    
    void save() {
        mesh.save("Terrain.ply");
    }
    
    void load() {
         mesh.load("Terrain.ply");
        minHeight = 0;
        maxHeight = 0;
        for (auto & vec : mesh.getVertices()) {
            if (vec.z > maxHeight) {
                maxHeight = vec.z;
            }
            else if (vec.z < minHeight) {
                minHeight = vec.z;
            }
        }
        vbo.clear(); // clear what is in the GPU
        vbo.setMesh(mesh, GL_STATIC_DRAW);
    }
    
    void iterateTo(unsigned int iteration) {
        if (iteration > iterations) {
            diamondSquareIterationByIdx();
        }
    }
    
    float randomValue() const {
        return std::rand() % 2 == 0 ? unit * kModifier : - unit * kModifier;
//        return iterations % 2 == 0 ? unit * kModifier  : - unit * kModifier;
    }
    
    unsigned int iterations = {0};
    float const unit = {550.f};
    float const kModifier = 0.0002;
//    float minHeight = std::numeric_limits<float>::max();
//    float maxHeight = std::numeric_limits<float>::min();
    float minHeight = {0};
    float maxHeight = {0};
    GLuint colorsAttributeId = {0};
    std::vector<ofVec4f> colors;
    
    
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
        mesh.addIndex(topRight);
        mesh.addIndex(bottomRight);
        mesh.addIndex(bottomLeft);
    }
    
    IndexType addVertex(ofVec3f const & vertex) {
        mesh.getVertices().push_back(vertex);
        return mesh.getVertices().size()-1;
    }
    
    ofVec3f & getVertex(IndexType const idx) {
        return mesh.getVertices()[idx];
    }
    
    
    void applyElevationFunction(ofVec3f & point) {
//        point.z = (std::sin(point.x * ( 2 * PI / unit)) +  std::sin(point.y * (PI / unit)))  * 50;
        auto x = point.x * ( 2 * PI / unit);
        auto y = point.y * ( 2 * PI / unit);
        point.z = std::sqrt(std::sin(x * x +  y * y)) * 50;
    }
    
    void disturbMidpoint(ofVec3f & midPoint, ofVec3f const & bottomLeft, ofVec3f const & topLeft, ofVec3f const & topRight, ofVec3f const & bottomRight) {
        midPoint[2] += (topLeft.distance(bottomRight) + topRight.distance(bottomLeft)) * randomValue();
        
        if (midPoint[2] < minHeight) {
            minHeight = midPoint[2];
        }
        if (midPoint[2] > maxHeight) {
            maxHeight = midPoint[2];
        }

    }

    bool indicesEnabled = {true};
    

    ofMesh mesh;
    ofVbo vbo;
    
};

#endif /* defined(__quaternionArcballExample__ofTerrain__) */
