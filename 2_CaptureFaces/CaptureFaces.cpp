#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <iostream>
#include <vector>
#include <format>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

//global
cv::CascadeClassifier face_cascade;
std::string face_cascade_name = "C:\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_alt.xml";

int main(int argc, char** argv) {
	std::string all_im_path = (std::string)argv[1] + "*.jpeg";

	cv::String path(all_im_path);
	std::vector<cv::String> res;
	std::vector<cv::Mat> data;
	
	cv::glob(path, res, false);


	for (unsigned i = 0; i < res.size(); i++)
	{
		std::string frame_path = (std::string)argv[1];
		cv::Mat frame = cv::imread(std::format("{}{}.jpeg", frame_path, i));
		if (frame.empty()) 
		{
			continue;
		}
		data.push_back(frame);
	}

	if (!face_cascade.load(face_cascade_name)) {
		printf("Error loading cascade classifier.\n");
		return (-1);
	}

	for (unsigned i = 0; i < data.size(); i++) // for each frame
	{
		cv::Mat curr_frame = data[i];
		if (curr_frame.empty()) 
		{
			printf(" --(!) No captured frame -- Break!");
			continue;
			//skip;
		}

		// all the faces in a frame
		std::vector<cv::Rect> faces;

		// detect faces
		face_cascade.detectMultiScale(curr_frame, faces, 1.1, 2, 0, cv::Size(20, 20));

		// biggest and curr roi
		cv::Rect big_roi;
		cv::Rect cur_roi;

		// biggest and curr element and area
		size_t cur_el = 0, big_el = 0;
		int cur_ar = 0, big_ar = 0;

		// int faceNum = 0; //number of faces detected
		fs::path og_path = fs::current_path(); //working dir

		for (cur_el = 0; cur_el < faces.size(); cur_el++) { // for each face in a single frame
			cur_roi.x = faces[cur_el].x;
			cur_roi.y = faces[cur_el].y;
			cur_roi.width = (faces[cur_el].width);
			cur_roi.height = (faces[cur_el].height);

			cur_ar = cur_roi.width * cur_roi.height; // get current info

			big_roi.x = faces[big_el].x;
			big_roi.y = faces[big_el].y;
			big_roi.width = (faces[big_el].width);
			big_roi.height = (faces[big_el].height);

			big_ar = big_roi.width * big_roi.height; // get biggest info

			if (cur_ar > big_ar) { // update biggest ele
				big_el = cur_el; //big_el now points to the biggest element again
				
				//update values
				big_roi.x = faces[big_el].x;
				big_roi.y = faces[big_el].y;
				big_roi.width = (faces[big_el].width);
				big_roi.height = (faces[big_el].height);
			}

			cv::Mat res;
			cv::Mat crop = curr_frame(big_roi);
			cv::resize(crop, res, cv::Size(128,128), 0, 0, cv::INTER_LINEAR);


			if (cur_el == 0)
			{
				std::string dir = "";
				dir.append(og_path.string());
				dir.append("\\OUTPUT\\");
				dir.append(std::to_string(i));
				//dir << og_path << "/OUTPUT/" << std::to_string(i);
				fs::path face_op = dir;
				fs::create_directories(face_op);
				fs::current_path(face_op); // change workdir
			}

			std::string filename = "";
			std::stringstream ssfn;
			ssfn << cur_el << ".jpeg";
			filename = ssfn.str();

			cv::imwrite(filename, res);
		}

		fs::current_path(og_path);
	}
	
	return 0;
}
			/*std::stringstream dir;
			dir << og_path << "\\OUTPUT\\" << std::to_string(i);
			fs::path face_op = dir.str();
			fs::create_directories(face_op);
			fs::path face_op = std::format("{}{}{}", og_path.c_str(), "\\OUTPUT\\frame_", std::to_string(cur_el));*/