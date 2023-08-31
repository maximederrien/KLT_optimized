#include "obj.hpp"
#include "klt.hpp"


__device__ int FASTalgorithme(int x, int y, image[]){
	int score = 0;
	
	if(get_HKpoint(FASTcalculus(x,y, image))){ // test if the consecutive brighter px condition is respected
		score = feature_score_calculus( x, y, image);
	}
	else {
		score = 0;
	}
	return score;
}


__global__ void kernel_feature_calculus(int image[], params_t block_param, features ftr_final_list){
	int id = threadIdx.x ;
	__shared__ coor_t feature_list[block_param.width * block_param.height]; // list of all the features to find the best one with a reduction after
        //int x = blockIdx.x*blockDim.x + id;
	//int y = blockIdx.y*blockDim.y;
	
	feature_liste[threadIdx.x] = is_feature(x % block_param.x, y % block_param.y , image);
	int max_score = 0;
	int new_score = 0;
	int x_px, y_px;
	//calculus of features
	for(int line = 0; line < block_param.y / L ; line ++) {
		//new score calculus
		x_px = blockIdx.x*block_param.width + threadIdx.x;
		y_px = blockIdx.y * block_param.height + threadIdx.y * (block_param.y / L) + line;
		new_score = FASTalgorithme( x_px, y_px, image);
		__syncthreads(); 
		if (max_score < new_score){
			new_score = max_score
			feature_list[threadIdx.x + threadIdx.y * L ].x = x_px;
			feature_list[threadIdx.x + threadIdx.y * L ].y = y_px;
			feature_list[threadIdx.x + threadIdx.y * L ].score = max_score;
		}
	}

	//reduction to find the best feature within the block 
	int t_id = threadIdx.x + threadIdx.y * L ;
	int nb_threads_alive = L * block_param.x / 2;
	while((nb_threads > 1) && (t_id <= nb_threads_alive) ){
		nb_threads /=2;
		if ( feature_list[t_id + nb_threads_alive].score > feature_list[t_id].score){
			feature_list[t_id].x = feature_list[t_id + nb_threads_alive].x;
			feature_list[t_id].y = feature_list[t_id + nb_threads_alive].y;
			feature_list[t_id].score = feature_list[t_id + nb_threads_alive].score;
		} // if not, we already have the best feature on this position
	}
	if(t_id == 0){ // last t_id should be 0 accordind to the reduction process, it containes the best feature
		ftr_final_list.list[blockId.x ] = feature_list[0]; // dont know how to finish that 
		//dont forget to uptade the lenght with atomic cuda operation
	}
	return;
}
