#ifndef Transformation_hpp
#define Transformation_hpp

#include <string>
#include <vector>

class Transformation{
    class TransformationBuilder;
    TransformationBuilder* transformationBuilder;

public:

    Transformation();

    Transformation(std::string name, std::vector<float> coordinates, float angle, float time, bool align);

    std::string getName();

    std::vector<float> getCoordinates();

    float getAngle();

    float getTime();

    bool getAlign();

    void setName(std::string name);

    void addCoordinates(std::vector<float> coordinates);

    void setAngle(float angle);

    void setTime(float time);

    void setAlign(bool align);

};

#endif