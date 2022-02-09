#include "app_live.hpp"

using namespace app;

// copied from gphoto2 `context.c` example
void Live::create_context()
{
	context = gp_context_new();
}

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

void Live::init()
{
	std::cout << "starting app::Live::init()..." << std::endl;
	// copied from gphoto2 `sample-capture.c` example
	gp_log_add_func(GP_LOG_ERROR, error_dumper, NULL); //dunno what this does
	gp_camera_new(&camera);

	std::cout << "Camera initializing, may take 10 seconds" << std::endl;
	gp_cam_return_val = gp_camera_init(camera, context);
}
