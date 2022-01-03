#include "face.hpp"

using namespace face;

const u8 L_CHEEK_I = 2;
const u8 R_CHEEK_I = 14;
const u8 NOSE_TIP_I = 33;
const u8 NOSE_BASE_I = 27;
const u8 CHIN_I = 8;
const u8 OUTER_LAST_I = 26;


auto Face::get_fg_mask(
	const cv::Mat& img,
	const int threshold,
	const int iter_count
) const -> cv::Mat
{
	auto rect_head = cv::Rect(
		fmax(0, rect.x - rect.width/2.0),
		fmax(0, rect.y - rect.height/2.0),
		fmin(img.size().width, rect.width*2),
		fmin(img.size().height, rect.height*2)
	);
	cv::Mat bg_model;
	cv::Mat fg_model;
	cv::Mat mask;
	cv::grabCut(
		img,
		mask,
		rect_head,
		bg_model,
		fg_model,
		iter_count,
		cv::GC_INIT_WITH_RECT
	);
	auto output_mask = cv::Mat(mask.size(), CV_8UC1);
	mask.convertTo(output_mask, CV_8UC1);
	if (threshold == 0) { // basically just includes the whole face
		output_mask = mask;
	} else if (threshold == 1) { // GC_PR_FGD and GC_FGD included
		output_mask = mask & cv::GC_FGD;
	} else if (threshold == 2) { // only GC_FGD included
		for (u16 row_i = 0; row_i < mask.rows; ++row_i) {
			uchar* p = output_mask.ptr(row_i);
			for (u16 col_i = 0; col_i < mask.cols; ++col_i) {
				auto p_i = *p;
				*p = p_i & cv::GC_FGD & ! ((p_i & cv::GC_PR_BGD) >> 1 );
				++p;
			}
		}
	}
	return output_mask*255;
}

auto Face::get_fg_edge_points(
	const cv::Mat& img
) const -> std::vector<cv::Point2f>
{
	auto mask = get_fg_mask(img);
	auto num_points = direction_vectors.size();
	auto midpoint = utils::mean(vertices[L_CHEEK_I], vertices[R_CHEEK_I], 0.5);

	auto boundary = cv::Rect(
		1,
		1,
		mask.size().width - 1,
		mask.size().height - 1
	);

	// fill with enough copies of midpoint
	auto edge_points = std::vector<cv::Point2f>(
		num_points,
		midpoint
	);

	std::cout << "Face::get_fg_edge_points Starting ray tracing"
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
		std::cout << "Face::get_fg_edge_points "
			<< "found mask edge at " << cur_ray << std::endl;
	}

	return edge_points;
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
	std::cout << "face is probably pointing in direction " << direction
		<< std::endl;

	// now find direction vectors
	direction_vectors.clear();
	for (u8 i = 0; i <= OUTER_LAST_I; ++i) {
		auto& v = vertices[i];
		direction_vectors.push_back(utils::normalize_vec(v - midpoint));
	}
}

Face::Face(const NamedImg& img, FaceDetector& face_detector) : name(img.name)
{
	auto img_dlib = convert::cv_to_dlib_rgb(img.img);
	auto rect_dlib = face_detector.detect(img_dlib);
	vertices = face_detector.predict(img_dlib, rect_dlib);
	estimate_direction();

	rect = convert::dlib_to_cv(rect_dlib);
	auto fg_edges = get_fg_edge_points(img.img);
	vertices.insert(vertices.end(), fg_edges.begin(), fg_edges.end());

	img_rect = cv::Rect(0, 0, img.img.size().width, img.img.size().height);
	calc_delaunay();
}

void Face::set_vertex_at(const cv::Point2f& vertex, const int i)
{
	vertices.at(i) = vertex;
	delaunay_valid = false;
}

void Face::calc_delaunay()
{
	auto subdiv = cv::Subdiv2D();

	subdiv.initDelaunay(img_rect);
	for (auto& point : vertices) {
		try {
			subdiv.insert(point);
		} catch (const cv::Exception& e) {
			std::cerr << "Got this cv exception: " << e.msg << std::endl
				<< "When adding point" << point
				<< " for face '" << name << "'" << std::endl;
		}
	}
	std::vector<cv::Vec6f> tri_list;
	subdiv.getTriangleList(tri_list);
	std::cout << "stored " << tri_list.size() << " triangles" << std::endl;

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
	std::cout << "stored " << delaunay_indices.size()
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
	img.setTo(cv::Scalar(0,0,0), get_fg_mask(img));
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
