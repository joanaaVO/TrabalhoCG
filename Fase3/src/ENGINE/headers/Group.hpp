#ifndef Group_h
#define Group_h

#include <string>
#include <vector>
#include "Transformation.hpp"

class Group {
	class GroupBuilder;
	GroupBuilder* groupBuilder;

public:

	Group();

	Group(std::vector<std::string> file, std::vector<Transformation> transf, std::vector<Group> subgroup, std::vector<std::vector<float>> total_figures);

	std::vector<std::string> getFile();

	std::vector<Transformation> getTransf();

	std::vector<Group> getSubGroup();

	std::vector<std::vector<float>> getTotalFigures();

	void setFile(std::vector<std::string> file);

	void setTransf(std::vector<Transformation> transf);

	void setSubGroup(std::vector<Group> subgroups);

	void addFile(std::string file);

	void addGroup(Group g);

	void addTransformation(Transformation t);

	void addTotalFigures(std::vector<float> figure);

};

#endif