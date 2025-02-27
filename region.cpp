//
// Created by harshil on 19/8/16.
//

#include "region.h"
#include <iostream>

using namespace std;

Region::Region(int level_, int color_) {
	this->level_ = level_;
	this->color_ = color_;
}

void Region::setChildParent() {
	for (int i=0; i<children_.size();i++) {
		children_[i]->setParent(this);
	}
}

void Region::merge(Region *h) {
	max_x_ = max(h->max_x_, max_x_);
	max_y_ = max(h->max_y_, max_y_);
	min_x_ = min(h->min_x_, min_x_);
	min_y_ = min(h->min_y_, min_y_);

	this->area_ = (this->max_y_ - this->min_y_ + 1)*(this->max_x_ - this->min_x_ +1 );
}

double Region::getAspectRatio() {
	return (max_x_ - min_x_+1) / (max_y_ - min_y_+1);
}

Region* Region::checkOverlap() {
	int number_of_overlap = 0;
	Region* parent = this->parent_;
	Region* max_element = this;

    //cerr << "======================\n";
	while (parent && (this->area_ > 0.7* parent->area_)) {
		number_of_overlap ++;
        //cerr <<"(" <<number_of_overlap << "," << parent->min_x_ << "," << parent->min_y_ << "," << parent->max_x_ << "," << parent->max_y_ << "," << parent->getStability() << ")" << endl;
		if (max_element->getStability() < parent->getStability()) {
			max_element = parent;
		}
		parent = parent->parent_;
	}
    //cerr << "=====================\n";

	if (number_of_overlap>=3) return max_element;
	return nullptr;
}

int Region::findNumberOfOverlap(vector<int> levels) {
	return 0;
}

void Region::addRegionUnder(Region *region) {
	pair<set<int>::iterator, bool> a = this->colors_under_.insert(region->color_);
	if (a.second) this->children_.push_back(region);
	region->setParent(this);

	this->merge(region);
}

void Region::addPixel(int x, int y) {
	this->min_x_ = min(this->min_x_, x);
	this->max_x_ = max(this->max_x_, x);
	this->max_y_ = max(this->max_y_, y);
	this->min_y_ = min(this->min_y_, y);

	this->area_ = (this->max_y_ - this->min_y_ + 1)*(this->max_x_ - this->min_x_ +1 );
}

void Region::print() {
	int k=0;
	if (parent_ != nullptr) k = parent_->color_;

	cout << "\n------------------\nLEVEL : " << level_ << " : " << color_ << " : " << area_ << " : " << k << endl << "Colors under : ";
	for (set<int>::iterator it=colors_under_.begin(); it!=colors_under_.end(); ++it)
		std::cout << *it << " ";

	cout << "\n------------------\n";
}

double Region::getStability() {
	int t = 80;
	Region* parent = this;
	while (parent->parent_ != nullptr && (parent->level_ - this->level_) <= t) {
		// cout << "parent->level_ " << parent->level_ << " this->level_ " << this->level_ << " " << this->parent_->level_ - this->level_<< endl;
		parent = parent->parent_;
	}

	double parent_area = (double) parent->area_;
	double mine_area = (double) this->area_;

	return (parent_area - mine_area)/mine_area;
}
