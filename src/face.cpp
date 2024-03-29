#include "face.hpp"

using namespace face;

const u8 L_CHEEK_I = 2;
const u8 R_CHEEK_I = 14;
const u8 NOSE_TIP_I = 33;
const u8 NOSE_BASE_I = 27;
const u8 CHIN_I = 8;
const u8 OUTER_LAST_I = 26;

void Face::store_fg_mask(
	const cv::Mat& img,
	FaceDetector& face_detector
)
{
	// calculate a rectangle containing the entire head (not just the face)
	// do this by averaging `rect` and the image border rect
	auto img_w = img.size().width;
	auto img_h = img.size().height;
	auto rect_head = cv::Rect(
		fmax(1, rect.x/2.0),
		fmax(1, rect.y/2.0),
		img_w-1,
		img_h-1
	);
	rect_head.width = fmin(
		rect_head.width,
		(rect.x + rect.width + img_w)/2.0 - rect_head.x
	);
	rect_head.height = fmin(
		rect_head.height,
		(rect.y + rect.height + img_h)/2.0 - rect_head.y
	);
	mask = face_detector.get_fg_mask(img, rect_head, 1, 3);
}

void Face::store_fg_edge_points()
{
	// mask should have been initialized by store_fg_mask!
	CV_Assert(!mask.empty());
	auto num_points = direction_vectors.size();
	auto midpoint = vertices[NOSE_TIP_I];

	auto boundary = cv::Rect(
		1,
		1,
		mask.size().width - 1,
		mask.size().height - 1
	);

	// fill with first 28 vertices, corresponding to face outer points
	auto edge_points = std::vector<cv::Point2f>(
		vertices.begin(),
		vertices.begin()+OUTER_LAST_I+1
	);

	std::cout << "Face::store_fg_edge_points Starting ray tracing"
		<< std::endl;

	for (u8 i = 0; i < num_points; ++i) {
		auto& cur_ray = edge_points[i];
		auto& cur_dir = direction_vectors[i];
		auto next_ray = cur_ray + cur_dir;
		while (
			next_ray.inside(boundary)
			&& mask.at<u8>(next_ray.y, next_ray.x) > 0
		) {
			cur_ray = next_ray;
			next_ray += cur_dir;
		}
		if (utils::distance(cur_ray, midpoint)
			< utils::distance(vertices[i], midpoint)
		) {
			cur_ray = next_ray;
		}
//		std::cout << "Face::store_fg_edge_points "
//			<< "found mask edge at " << cur_ray << std::endl;
	}
	vertices.insert(vertices.end(), edge_points.begin(), edge_points.end());
	std::cout << "Face::store_fg_edge_points Finished ray tracing" << std::endl;
}

void Face::estimate_direction()
{
	const auto& l_cheek = vertices[L_CHEEK_I];
	const auto& r_cheek = vertices[R_CHEEK_I];
	const auto&& midpoint = utils::mean(l_cheek, r_cheek, 0.5);
	const auto& nose_tip = vertices[NOSE_TIP_I];
	const auto& nose_base = vertices[NOSE_BASE_I];
	const auto& chin = vertices[CHIN_I];

	auto x = (nose_tip.x - midpoint.x) / (r_cheek.x - midpoint.x);
	auto y = (nose_tip.y - midpoint.y) / (chin.y - midpoint.y);
	direction.x = fmaxf(-1, fminf(1, 2 * x));
	direction.y = fmaxf(-1, fminf(1, 2 * y));
	std::cout << "Face::estimate_direction: " << direction
		<< std::endl;

	// now find direction vectors
	direction_vectors.clear();
	for (u8 i = 0; i <= OUTER_LAST_I; ++i) {
		auto& v = vertices[i];
		direction_vectors.push_back(utils::normalize_vec(v - midpoint));
	}
}

Face::Face(NamedImg& img, FaceDetector& face_detector) : name(img.name)
{
	img_rect = cv::Rect(0, 0, img.img.size().width, img.img.size().height);
	auto img_dlib = convert::cv_to_dlib_rgb(img.img);
	auto rect_dlib = face_detector.detect(img_dlib);
	vertices = face_detector.predict(img_dlib, rect_dlib);
	estimate_direction();

	if (direction.x < 0) {
		cv::flip(img.img, img.img, 1);
		img_dlib = convert::cv_to_dlib_rgb(img.img);
		rect_dlib = face_detector.detect(img_dlib);
		vertices = face_detector.predict(img_dlib, rect_dlib);
		estimate_direction();
	}

	rect = convert::dlib_to_cv(rect_dlib);
	store_fg_mask(img.img, face_detector);
	store_fg_edge_points();

	calc_delaunay();
}

void Face::set_vertex_at(const cv::Point2f& vertex, const int i)
{
	vertices.at(i) = vertex;
	delaunay_valid = false;
}

void Face::calc_delaunay()
{
	if (delaunay_valid) {
		std::cout << "Face::calc_delaunay did not need to run because delaunay"
			<< " already valid" << std::endl;
	}
	auto subdiv = cv::Subdiv2D();

	subdiv.initDelaunay(img_rect);
	for (auto& point : vertices) {
		try {
			subdiv.insert(point);
		} catch (const cv::Exception& e) {
			std::cerr << "Got this cv exception: " << e.msg
				<< "When adding point" << point
				<< " for face '" << name << "'" << std::endl << std::endl;
		}
	}
	std::vector<cv::Vec6f> tri_list;
	subdiv.getTriangleList(tri_list);
	std::cout << "Face::calc_delaunay stored "
		<< tri_list.size() << " triangles" << std::endl;

	// the triangles need to be in a different format for getAffineTransform
	for (const auto& tri: tri_list) {
		auto index = cv::Point3i(-1,-1,-1);
		auto p1 = cv::Point2f(tri[0], tri[1]);
		auto p2 = cv::Point2f(tri[2], tri[3]);
		auto p3 = cv::Point2f(tri[4], tri[5]);
		for (u32 i = 0; i < vertices.size(); ++i) {
			if (vertices[i] == p1) {
				index.x = i;
			} else if (vertices[i] == p2) {
				index.y = i;
			} else if (vertices[i] == p3) {
				index.z = i;
			}
		}
		if (index.x == -1) {
			std::cerr << "point " << p1 << " not found in face!" << std::endl;
		}
		if (index.y == -1) {
			std::cerr << "point " << p2 << " not found in face!" << std::endl;
		}
		if (index.z == -1) {
			std::cerr << "point " << p3 << " not found in face!" << std::endl;
		}
		delaunay_indices.push_back(index);
	}
	std::cout << "Face::calc_delaunay stored " << delaunay_indices.size()
		<< " delaunay indices" << std::endl;

	delaunay_valid = true;
}

auto Face::get_tri(const cv::Point3i& indices) const -> cv::Mat
{
	auto tri_o = cv::Mat(3,2,CV_32F);
	auto p0 = vertices[indices.x];
	auto p1 = vertices[indices.y];
	auto p2 = vertices[indices.z];
	tri_o.at<float>(0,0) = p0.x;
	tri_o.at<float>(0,1) = p0.y;
	tri_o.at<float>(1,0) = p1.x;
	tri_o.at<float>(1,1) = p1.y;
	tri_o.at<float>(2,0) = p2.x;
	tri_o.at<float>(2,1) = p2.y;
	return tri_o;
}

auto Face::get_delaunay_indices() const -> std::vector<cv::Point3i>
{
	CV_Assert(delaunay_valid);
	return delaunay_indices;
}

void Face::draw_markers(cv::Mat& img) const
{
	img.setTo(cv::Scalar(0,0,0), mask);
	static const cv::Scalar RED(0,0,255);
	static const cv::Scalar DLIBCOLOR(255,100,20);
	static const cv::Scalar YELLOW(0,255,255);
	static const cv::Scalar GREEN(0,255,0);
	for (u8 i = 0; i < vertices.size(); ++i) {
		auto& p = vertices[i];
		auto color = DLIBCOLOR;
		auto marker = cv::MARKER_CROSS;
		if (i < 68) {
			if (i == L_CHEEK_I
				|| i == R_CHEEK_I
				|| i == NOSE_TIP_I
				|| i == NOSE_BASE_I
				|| i == CHIN_I) {
				color = RED;
			}
		} else if (i < 76) {
			color = YELLOW;
			marker = cv::MARKER_TILTED_CROSS;
		} else {
			color = GREEN;
			marker = cv::MARKER_TRIANGLE_DOWN;
		}
		cv::drawMarker(img, p, color, marker, 6);
	}
	cv::rectangle(img, rect, YELLOW);
}

auto Face::serialize() const -> std::stringstream
{
	if (!delaunay_valid) {
		std::cout << "Face::serialize: note: Delaunay triangulation invalid when serializing" << std::endl;
	}
	auto serial = std::stringstream("");

	serial << name << delim;

	serial << direction.x << delim;
	serial << direction.y << delim;

	serial << rect.x << delim;
	serial << rect.y << delim;
	serial << rect.width << delim;
	serial << rect.height << delim;

	serial << img_rect.x << delim;
	serial << img_rect.y << delim;
	serial << img_rect.width << delim;
	serial << img_rect.height << delim;

	serial << delaunay_indices.size() << delim;
	for (const auto& index : delaunay_indices) {
		serial << index.x << delim;
		serial << index.y << delim;
		serial << index.z << delim;
	}
	serial << array_end << delim;

	serial << vertices.size() << delim;
	for (const auto& vertex : vertices) {
		serial << vertex.x << delim;
		serial << vertex.y << delim;
	}
	serial << array_end << delim;

	return serial;
}

Face::Face(std::string serial_str)
{
	auto serial = std::stringstream(serial_str);
	std::string token;
	auto i = 0;
	auto j = 0;
	auto delaunay_indices_end_pos = 0;
	auto cur_delaunay_tri = cv::Point3i(-1,-1,-1);
	auto vertices_end_pos = 0;
	auto cur_vertex = cv::Point2f(-1,-1);
	while (std::getline(serial, token, delim)) {
		if (i == 0) {
			name = token;
		} else if (i == 1) {
			direction.x = std::stof(token);
		} else if (i == 2) {
			direction.y = std::stof(token);
		} else if (i == 3) {
			rect.x = std::stof(token);
		} else if (i == 4) {
			rect.y = std::stof(token);
		} else if (i == 5) {
			rect.width = std::stof(token);
		} else if (i == 6) {
			rect.height = std::stof(token);
		} else if (i == 7) {
			img_rect.x = std::stof(token);
		} else if (i == 8) {
			img_rect.y = std::stof(token);
		} else if (i == 9) {
			img_rect.width = std::stof(token);
		} else if (i == 10) {
			img_rect.height = std::stof(token);
		} else if (i == 11) {
			delaunay_indices_end_pos = 12 + 3*std::stoi(token);
			j = -1; // set to -1 because it will incr at end of loop
		} else if (i < delaunay_indices_end_pos) {
			if (j%3 == 0) {
				cur_delaunay_tri.x = std::stoi(token);
			} else if (j%3 == 1) {
				cur_delaunay_tri.y = std::stoi(token);
			} else if (j%3 == 2) {
				cur_delaunay_tri.z = std::stoi(token);
				if (cur_delaunay_tri.x == -1
					|| cur_delaunay_tri.y == -1
					|| cur_delaunay_tri.z == -1) {
					std::cout << "encountered invalid delaunay tri "
						<< cur_delaunay_tri << " while deserializing for "
						<< name << " at index" << j << std::endl;
				}
				delaunay_indices.push_back(cur_delaunay_tri);
				cur_delaunay_tri = cv::Point3i(-1,-1,-1);
			}
		} else if (i == delaunay_indices_end_pos) {
			if (token[0] != array_end) {
				std::cout << "expected array end token '" << array_end
					<< "', but instead found '" << token << "' while"
					<< " deserializing delaunay_indices array for "
					<< name << std::endl;
			} else {
				delaunay_valid = true;
			}
		} else if (i == delaunay_indices_end_pos+1) {
			vertices_end_pos = delaunay_indices_end_pos + 2 + 2*std::stoi(token);
			j = -1; // set to -1 because it will incr at end of loop
		} else if (i < vertices_end_pos) {
			if (j%2 == 0) {
				cur_vertex.x = std::stof(token);
			} else if (j%2 == 1) {
				cur_vertex.y = std::stof(token);
				if (cur_vertex.x == -1
					|| cur_vertex.y == -1) {
					std::cout << "encountered invalid vertex "
						<< cur_vertex << " while deserializing for "
						<< name << " at index " << j << std::endl;
				}
				vertices.push_back(cur_vertex);
				cur_vertex = cv::Point2f(-1,-1);
			}
		} else if (i == vertices_end_pos) {
			if (token[0] != array_end) {
				std::cout << "expected array end token '" << array_end
					<< "', but instead found '" << token << "' while"
					<< " deserializing vertices array for "
					<< name << std::endl;
			}
		}
		++i;
		++j;
	}
}

auto Face::load(const std::string& filename) -> Face
{
	auto file = std::ifstream(filename.c_str(), std::ifstream::in);
	if (!file) {
		std::cout << "Face::load: file not found, returning empty Face"
			<< std::endl;
		return Face();
	}
	auto ss = std::stringstream();
	ss << file.rdbuf();
	return Face(ss.str());
}

void Face::save(const std::string& filename) const
{
	auto file = std::ofstream(filename.c_str(), std::ios::binary);
	if (!file) {
		CV_Error(cv::Error::StsBadArg, "invalid filename for Face::save");
	}
	file << serialize().str();
	file.close();
}
