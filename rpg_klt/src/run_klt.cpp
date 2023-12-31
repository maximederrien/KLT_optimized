
//ros2 include
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "klt_msg/msg/ftr.hpp"
#include "klt_msg/msg/feature_list.hpp"

//just for test, to erase later
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.hpp"

//c++ include
#include "klt.hpp"
#include "obj.hpp"

class KltNode : public rclcpp::Node 
{
	public:
	KltNode(float threshold, params_t image_param, int max_feature) : Node("klt_node"), klt(threshold, image_param, max_feature) 
	{
		pub = this->create_publisher<klt_msg::msg::FeatureList>("klt/feature", 10);
		sub = this->create_subscription<sensor_msgs::msg::Image>("klt/image", 999, std::bind(&KltNode::img_callback, this, std::placeholders::_1));	
		RCLCPP_INFO(this->get_logger(), "node created");
	}
	private:
	rclcpp::Publisher<klt_msg::msg::FeatureList>::SharedPtr pub;
	rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub;
	KLT klt;
		
	void img_callback(const sensor_msgs::msg::Image & image_msg){
		RCLCPP_INFO(this->get_logger(), "image received");
		int img[image_msg.height*image_msg.width];
		
		for (int j = 0; j < (image_msg.height*image_msg.width); j++ ){
			img[j] = image_msg.data[j];
		}

		features ftr;
	       	klt.get_features(img, &ftr);
		
		cv_bridge::CvImagePtr image = cv_bridge::toCvCopy(image_msg , "rgb8");
		int x = 0;
		int y = 0;
		int scr = 0;
		cv::Point pt;
		for(int i=0; i < N_MAX_FEATURE ; i++){
			x = ftr.list[i].x;
			y = ftr.list[i].y;
			scr = ftr.list[i].score;
			RCLCPP_INFO(this->get_logger(), "ftr coordinate: (%i,%i) score: %i iter: %i\n", x, y, scr, i);
			pt.x = ftr.list[i].x;
			pt.y = ftr.list[i].y;
			cv::circle((*image).image, pt, 5, cv::Scalar(255,0,0), cv::FILLED);
		}
		cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
		cv::imshow("test", (*image).image);
		cv::waitKey(0);
	}
};

int main(int argc, char* argv[]){
	rclcpp::init(argc, argv);
	params_t image_param = {640,600};
	rclcpp::spin(std::make_shared<KltNode>(1.0,image_param ,12000));
	rclcpp::shutdown();
	return 0;
}
