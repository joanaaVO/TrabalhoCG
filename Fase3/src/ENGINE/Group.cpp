#include "headers/Group.hpp"

using namespace std;

class Group::GroupBuilder {
	private:
		std::vector<std::string> file;
		std::vector<Transformation> transformation;
		std::vector<Group> subgroups;
		std::vector<std::vector<float>> total_figures;

	public:

		GroupBuilder() = default;
		
		GroupBuilder(std::vector<std::string> file, std::vector<Transformation> transf, std::vector<Group> subgroup, std::vector<std::vector<float>> total_figures) {
			this->file = file;
			this->transformation = transf;
			this->subgroups = subgroup;
			this->total_figures = total_figures;
		}

		std::vector<std::string> getFile() {
			return file;
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

		void setFile(std::vector<std::string> file) {
			this->file = file;
		}

		void setTransf(std::vector<Transformation> transf) {
			this->transformation = transf;
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

		void addTransformation(Transformation t) {
			this->transformation.push_back(t);
		}

		void addTotalFigures(std::vector<float> figure){
			this->total_figures.push_back(figure);
		}
};

Group::Group() : groupBuilder { new class GroupBuilder() } {}

Group::Group(std::vector<std::string> file, std::vector<Transformation> transformation, std::vector<Group> subgroup, std::vector<std::vector<float>> total_figures) : groupBuilder { new GroupBuilder(file, transformation, subgroup, total_figures) } {}

std::vector<std::string> Group::getFile() {
	return groupBuilder->getFile();
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

void Group::setFile(std::vector<std::string> file) {
	groupBuilder->setFile(file);
}

void Group::setTransf(std::vector<Transformation> transf) {
	groupBuilder->setTransf(transf);
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

void Group::addTransformation(Transformation t) {
	groupBuilder->addTransformation(t);
}

void Group::addTotalFigures(std::vector<float> figure){
	groupBuilder->addTotalFigures(figure);
}