# 1. A simple gstreamer pipeline and run using gst-launch command line tool to read the mp4 video file, scale down the input to 640x640 resolution and write JPEG encoded frames.

### 1:
1) cd into 1_stream_to_frames directory.
2) Run the shell script and pass two command line arguments:\
	argv[1]: Path to the mp4 file (I used ```./sample_video/sample_1.mp4```)\
	argv[2]: Name of the output folder (I used ```out_1``` for sample_1.mp4 and ```out_1_2``` for sample_3.mp4)
	
	So it looks something like this:\
	```$ ./stream_to_frames.sh ./sample_video/sample_1.mp4 out_1```
	
	The output will be in the same directory with the name you gave when running the command.
#

# 2: A C++ application to read each JPEG frame one by one. Uses OpenCV’s pre-trained XML classifier for detecting faces from the image and crop it. Creates a frame wise folder structure of cropped faces.

### 2:
1) cd into 2_CaptureFaces directory.
2) Run the command:
	```$ cmake .```
	To build the VS Solution.
	
	*note*: If you want to change the directory from which the C++ application takes input of the frames from, let's say you want to change the folder from ```1_stream_to_frames/out_1_2``` to ```1_stream_to_frames/out_1"```":
	1) Open CMakeLists.txt in a file editor.
	2) Locate the ```set_target_properties(CaptureFaces PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "../1_stream_to_frames/out_1_2/")``` command and change the command line argument:\
		```set_target_properties(CaptureFaces PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "//Enter changed directory here.//")```
		
		Let's say if you want to take input from out_1 in the 1_stream_to_frames directory, then it will look something like this:
		```set_target_properties(CaptureFaces PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "../1_stream_to_frames/out_1/")```
		
		save and exit and start from step 1.
3) Open the 'CaptureFaces.sln' file in Visual Studio and there you have the C++ Application project.