#include "headers/Group.hpp"
#include <GL/glew.h>

using namespace std;

class Group::GroupBuilder {
	private:
		std::vector<std::string> file;
		std::string texture_file;
		std::vector<Transformation> transformation;
		std::vector<Group> subgroups;
		std::vector<std::vector<float>> total_figures;
		std::vector<float> light_color;
		GLuint teximage;

	public:

		GroupBuilder() = default;
		
		GroupBuilder(std::vector<std::string> file, std::string texture_file, std::vector<Transformation> transf, std::vector<Group> subgroup, std::vector<std::vector<float>> total_figures, std::vector<float> light_color, GLuint teximage) {
			this->file = file;
			this->texture_file = texture_file;
			this->transformation = transf;
			this->subgroups = subgroup;
			this->total_figures = total_figures;
			this->light_color = light_color;
			this->teximage = teximage;
		}

		std::vector<std::string> getFile() {
			return file;
		}

		std::string getTextureFile() {
			return texture_file;
		}

		std::vector<Transformation> getTransf() {
			return transformation;
		}

		std::vector<Group> getSubGroup() {
			return this->subgroups;
		}

		std::vector<std::vector<float>> getTotalFigures(){
			return this->total_figures;
		}

		GLuint getTexImage() {
			return this->teximage;
		}

		void setFile(std::vector<std::string> file) {
			this->file = file;
		}

		void setTextureFile(std::string texture_file) {
			this->texture_file = texture_file;
		}

		void setTransf(std::vector<Transformation> transf) {
			this->transformation = transf;
		}

		void setSubGroup(std::vector<Group> subgroups) {
			this->subgroups = subgroups;
		}

		void setLightColor(std::vector<float> light_color) {
			this->light_color = light_color;
		}

		void setTexImage(GLuint teximage) {
			this->teximage = teximage;
		}

		vector<float> getLightColor() {
			return this->light_color;
		}

		void addFile(std::string file) {
			this->file.push_back(file);
		}

		void addGroup(Group g) {
			this->subgroups.push_back(g);
		}

		void addTransformation(Transformation t) {
			this->transformation.push_back(t);
		}

		void addTotalFigures(std::vector<float> figure){
			this->total_figures.push_back(figure);
		}
};

Group::Group() : groupBuilder { new class GroupBuilder() } {}

Group::Group(std::vector<std::string> file, std::string texture_file, std::vector<Transformation> transformation, std::vector<Group> subgroup, std::vector<std::vector<float>> total_figures, std::vector<float> light_color, GLuint teximage) : groupBuilder { new GroupBuilder(file, texture_file, transformation, subgroup, total_figures, light_color, teximage) } {}

std::vector<std::string> Group::getFile() {
	return groupBuilder->getFile();
}

std::string Group::getTextureFile() {
	return groupBuilder->getTextureFile();
}

std::vector<Transformation> Group::getTransf() {
	return groupBuilder->getTransf();
}

std::vector<Group> Group::getSubGroup() {
	return groupBuilder->getSubGroup();
}

std::vector<std::vector<float>> Group::getTotalFigures(){
	return groupBuilder->getTotalFigures();
}

GLuint Group::getTexImage() {
	return groupBuilder->getTexImage();
}

void Group::setFile(std::vector<std::string> file) {
	groupBuilder->setFile(file);
}

void Group::setTextureFile(std::string texture_file) {
	groupBuilder->setTextureFile(texture_file);
}

void Group::setTransf(std::vector<Transformation> transf) {
	groupBuilder->setTransf(transf);
}

void Group::setSubGroup(std::vector<Group> subgroups) {
	groupBuilder->setSubGroup(subgroups);
}

void Group::setLightColor(std::vector<float> color_light) {
	groupBuilder->setLightColor(color_light);
}

void Group::setTexImage(GLuint teximage) {
	groupBuilder->setTexImage(teximage);
}

std::vector<float> Group::getLightColor() {
	return groupBuilder->getLightColor();
}
void Group::addFile(std::string file) {
	groupBuilder->addFile(file);
}

void Group::addGroup(Group g) {
	groupBuilder->addGroup(g);
}

void Group::addTransformation(Transformation t) {
	groupBuilder->addTransformation(t);
}

void Group::addTotalFigures(std::vector<float> figure){
	groupBuilder->addTotalFigures(figure);
}