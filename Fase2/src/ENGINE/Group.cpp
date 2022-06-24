#include <string>
#include <vector>
#include "Group.hpp"

using namespace std;

class Group::GroupBuilder {
	private:
		vector<string> file;
		vector<float> color;
		vector<float> rotate;
		vector<float> translate;
		vector<float> scale;
		vector<Group> subgroups;

	public:

		GroupBuilder() = default;
		
		GroupBuilder(std::vector<std::string> file, vector<float> color, vector<float> rotate, vector<float> translate, vector<float> scale, std::vector<Group> subgroup) {
			this->file = file;
			for (int i = 0; i < 3; i++)
				this->color.push_back(color[i]);
			for (int i = 0; i < 3; i++)
				this->rotate.push_back(rotate[i]);
			for (int i = 0; i < 3; i++)
				this->translate.push_back(translate[i]);
			for (int i = 0; i < 3; i++)
				this->scale.push_back(scale[i]);
			this->subgroups = subgroup;
		}

		std::vector<std::string> getFile() {
			return file;
		}

		vector<float> getColor() {
			return color;
		}

		vector<float> getRotate() {
			return rotate;
		}

		vector<float> getTranslate() {
			return translate;
		}

		vector<float> getScale() {
			return scale;
		}

		std::vector<Group> getSubGroup() {
			return this->subgroups;
		}

		void setFile(std::vector<std::string> file) {
			this->file = file;
		}

		void setColor(vector<float> color) {
			for (int i = 0; i < 3; i++)
				this->color.push_back(color[i]);
		}

		void setRotate(vector<float> rotate) {
			for (int i = 0; i < 4; i++)
				this->rotate.push_back(rotate[i]);
		}

		void setTranslate(vector<float> translate) {
			for (int i = 0; i < 3; i++)
				this->translate.push_back(translate[i]);
		}

		void setScale(vector<float> scale) {
			for (int i = 0; i < 3; i++)
				this->scale.push_back(scale[i]);
		}

		void setSubGroup(std::vector<Group> subgroups) {
			this->subgroups = subgroups;
		}

		void addFile(std::string file) {
			this->file.push_back(file);
		}

		void addGroup(Group g) {
			this->subgroups.push_back(g);
		}
};

Group::Group() : groupBuilder {
	new class GroupBuilder()
} {}

Group::Group(std::vector<std::string> file, vector<float> color, vector<float> rotate, vector<float> translate, vector<float> scale, std::vector<Group> subgroup) : groupBuilder { new GroupBuilder(file, color, rotate, translate, scale, subgroup)} {}

std::vector<std::string> Group::getFile() {
	return groupBuilder->getFile();
}

vector<float> Group::getColor() {
	return groupBuilder->getColor();
}

vector<float> Group::getRotate() {
	return groupBuilder->getRotate();
}

vector<float> Group::getTranslate() {
	return groupBuilder->getTranslate();
}

vector<float> Group::getScale() {
	return groupBuilder->getScale();
}

std::vector<Group> Group::getSubGroup() {
	return groupBuilder->getSubGroup();
}

void Group::setFile(std::vector<std::string> file) {
	groupBuilder->setFile(file);
}

void Group::setColor(vector<float> color) {
	groupBuilder->setColor(color);
}

void Group::setRotate(vector<float> rotate) {
	groupBuilder->setRotate(rotate);	
}

void Group::setTranslate(vector<float> translate) {
	groupBuilder->setTranslate(translate);
}

void Group::setScale(vector<float> scale) {
	groupBuilder->setScale(scale);
}

void Group::setSubGroup(std::vector<Group> subgroups) {
	groupBuilder->setSubGroup(subgroups);
}

void Group::addFile(std::string file) {
	groupBuilder->addFile(file);
}

void Group::addGroup(Group g) {
	groupBuilder->addGroup(g);
}