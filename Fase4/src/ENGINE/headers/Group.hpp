#ifndef Group_h
#define Group_h

#include <string>
#include <vector>
#include "Transformation.hpp"
#include <GL/glew.h>

class Group {
	class GroupBuilder;
	GroupBuilder* groupBuilder;

public:

	Group();

	Group(std::vector<std::string> file, std::string texture_file, std::vector<Transformation> transf, std::vector<Group> subgroup, std::vector<std::vector<float>> total_figures, std::vector<float> color_light, GLuint teximage);

	std::vector<std::string> getFile();

	std::string getTextureFile();

	std::vector<Transformation> getTransf();

	std::vector<Group> getSubGroup();

	std::vector<std::vector<float>> getTotalFigures();

	GLuint getTexImage();

	void setFile(std::vector<std::string> file);

	void setTextureFile(std::string texture_file);

	void setTransf(std::vector<Transformation> transf);

	void setSubGroup(std::vector<Group> subgroups);

	void setLightColor(std::vector<float> color_light);

	void setTexImage(GLuint teximage);

	std::vector<float> getLightColor();

	void addFile(std::string file);

	void addGroup(Group g);

	void addTransformation(Transformation t);

	void addTotalFigures(std::vector<float> figure);

};

#endif