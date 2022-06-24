#include <string>
#include <vector>

#include "headers/Transformation.hpp"
using namespace std;

class Transformation::TransformationBuilder {

private:
    string name;
    std::vector<float> coordinates;
    float angle = -1;
    float time = -1;
    bool align;


public:
    TransformationBuilder() = default;

    TransformationBuilder(string name, std::vector<float> coordinates, float angle, float time, bool align) {
        this->name = name;
        this->coordinates = coordinates;
        this->angle = angle;
        this->time = time; 
        this->align = align;
    }

    std::string getName() {
        return name;
    }

    std::vector<float> getCoordinates(){
        return coordinates;
    }

    float getAngle() {
        return angle;
    }

    float getTime() {
        return time;
    }

    bool getAlign(){
        return align;
    }

    void setName(std::string name){
        this->name = name;
    }

    void addCoordinates(std::vector<float> coordinates){
        for(int i = 0; i < coordinates.size(); i++)
            this->coordinates.push_back(coordinates[i]);
    }

    void setAngle(float angle){
        this->angle = angle;
    }

    void setTime(float time){
        this->time = time;
    }

    void setAlign(bool align){
        this->align = align;
    }

};

Transformation::Transformation() : transformationBuilder{ new class TransformationBuilder() } {}

Transformation::Transformation(string name, std::vector<float> coordinates, float angle, float time, bool align) : transformationBuilder{ new TransformationBuilder(name, coordinates, angle, time, align) } {}

std::string Transformation::getName() {
    return transformationBuilder->getName();
}

std::vector<float> Transformation::getCoordinates(){
    return transformationBuilder->getCoordinates();
}

float Transformation::getAngle() {
    return transformationBuilder->getAngle();
}

float Transformation::getTime() {
    return transformationBuilder->getTime();
}

bool Transformation::getAlign(){
        return transformationBuilder->getAlign();
}

void Transformation::setName(std::string name){
    return transformationBuilder->setName(name);
}    

void Transformation::addCoordinates(std::vector<float> coords){
    return transformationBuilder->addCoordinates(coords);
}

void Transformation::setAngle(float angle){
    return transformationBuilder->setAngle(angle);
}

void Transformation::setTime(float time){
    return transformationBuilder->setTime(time);
}

void Transformation::setAlign(bool align){
    return transformationBuilder->setAlign(align);
}