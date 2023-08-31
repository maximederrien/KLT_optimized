#include "klt.hpp"
#include <iostream>



KLT::KLT(float threshold, params_t image_param, int max_feature){
	this->max_feature = max_feature;
	this->image_param = image_param;
	this->threshold = threshold;
	fit_block(max_feature, image_param, &(this->block_param));
}

features get_features(float image[]){	
	features ftr();
	//kernel call
	return ftr;
}

params_t KLT::get_block_param(){
	return this->block_param;
}

float KLT::get_threshold(){
	return this->threshold;	
}

params_t KLT::get_image_param(){
	return this->image_param;
}

params_t KLT::get_block_param(){
	return this->block_param;
}

/*
int main() {
	params_t jsp = {1, 1};
	KLT klter(3.0, jsp, 5);
	std::cout << klter.get_threshold() << std::endl;
	std::cout << klter.get_image_param().height << std::endl;
	return 1;
}*/
