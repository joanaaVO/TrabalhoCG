#ifndef Group_h
#define Group_h

#include <string>
#include <vector>

class Group {
	class GroupBuilder;
	GroupBuilder* groupBuilder;

public:

	Group();
	
	Group(std::vector<std::string> file, std::vector<float> color, std::vector<float> rotate, std::vector<float> translate, std::vector<float> scale, std::vector<Group> subgroup);

	std::vector<std::string> getFile();

	std::vector<float> getColor();

	std::vector<float> getRotate();

	std::vector<float> getTranslate();

	std::vector<float> getScale();

	std::vector<Group> getSubGroup();

	void setFile(std::vector<std::string> file);

	void setColor(std::vector<float> color);

	void setRotate(std::vector<float> rotate);

	void setTranslate(std::vector<float> translate);

	void setScale(std::vector<float> scale);

	void setSubGroup(std::vector<Group> subgroups);

	void addFile(std::string file);

	void addGroup(Group g);

};

#endif