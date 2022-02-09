#include "app_live.hpp"

using namespace app;

// copied from gphoto2 `context.c` example
void Live::context_error(
	GPContext* context,
	const char* str,
	void* data
)
{
	std::cerr << "app::Live::context_error: " << std::endl;
	std::cerr << str << std::endl;
}

// copied from gphoto2 `context.c` example
void Live::context_status(
	GPContext* context,
	const char* str,
	void* data
)
{
	std::cerr << "app::Live::context_status: " << std::endl;
	std::cerr << str << std::endl;
}

// copied from gphoto2 `sample-capture.c` example
void Live::error_dumper(
	GPLogLevel level,
	const char* domain,
	const char* str,
	void* data
)
{
	std::cout << "app::Live::error_dumper: " << str << std::endl;
}

// copied from gphoto2 `context.c` example
void Live::create_context()
{
	context = gp_context_new();
	gp_context_set_error_func(context, context_error, NULL);
	gp_context_set_status_func(context, context_status, NULL);
}

// copied from gphoto2 `sample-capture.c` example
void Live::capture()
{
	CameraFile* file;
	CameraFilePath camera_file_path;

	std::cout << "app::Live::capture: Capturing" << std::endl;
	strncpy(camera_file_path.folder, "/", 1);
	strncpy(camera_file_path.name, "capture.jpg", 7);

	auto return_val = gp_camera_capture(
		camera,
		GP_CAPTURE_IMAGE,
		&camera_file_path,
		context
	);
	std::cout << "app::Live::capture: capture returned " << return_val
		<< std::endl;
	std::cout << "app::Live::capture: Pathname of the camera: "
		<< camera_file_path.folder << "/" << camera_file_path.name
		<< std::endl;

	return_val = gp_file_new(&file);
	std::cout << "app::Live::capture: gp_file_new returned " << return_val
		<< std::endl;
	return_val = gp_camera_file_get(
		camera,
		camera_file_path.folder,
		camera_file_path.name,
		GP_FILE_TYPE_NORMAL,
		file,
		context
	);
	std::cout << "app::Live::capture: gp_camera_file_get returned "
		<< return_val << std::endl;

	// i think it might be better to read the file using OpenCV2, rather
	// than reading it with gphoto2
	const char **ptr;
	long unsigned int *size;
	gp_file_get_data_and_size(file, ptr, size);

	std::cout << "app::Live::capture: deleting temporary file" << std::endl;
	return_val = gp_camera_file_delete(
		camera,
		camera_file_path.folder,
		camera_file_path.name,
		context
	);
	std::cout << "app::Live::capture: gp_camera_file_delete returned "
		<< return_val << std::endl;
	gp_file_free(file);
}

void Live::init_cam()
{
	std::cout << "starting app::Live::init()..." << std::endl;
	// copied from gphoto2 `sample-capture.c` example
	gp_log_add_func(GP_LOG_ERROR, error_dumper, NULL); //dunno what this does
	gp_camera_new(&camera);

	std::cout << "Camera initializing, may take 10 seconds" << std::endl;
	auto return_val = gp_camera_init(camera, context);
	if (return_val != GP_OK) {
		std::cout << "app::Live::init: camera init failed: " << return_val
			<< std::endl;
		exit(1);
	}
}
