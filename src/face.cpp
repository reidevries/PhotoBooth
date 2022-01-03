#include "face.hpp"

using namespace face;

const u8 L_CHEEK_I = 2;
const u8 R_CHEEK_I = 14;
const u8 NOSE_TIP_I = 33;
const u8 NOSE_BASE_I = 27;
const u8 CHIN_I = 8;

void Face::estimate_direction()
{
	const auto& l_cheek = vertices[L_CHEEK_I];
	const auto& r_cheek = vertices[R_CHEEK_I];
	const auto& nose_tip = vertices[NOSE_TIP_I];
	const auto& nose_base = vertices[NOSE_BASE_I];
	const auto& chin = vertices[CHIN_I];
	auto upper_cheek = fminf(l_cheek.y, r_cheek.y);
	auto x = (nose_tip.x - l_cheek.x) / (r_cheek.x - l_cheek.x);
	auto y = (nose_tip.y - upper_cheek) / (chin.y - upper_cheek);
	direction.x = fmaxf(-1, fminf(1, 2 * x - 1));
	direction.y = fmaxf(-1, fminf(1, 2 * y));
	std::cout << "face is probably pointing in direction " << direction
		<< std::endl;
}

Face::Face(const NamedImg& img, FaceDetector& face_detector) : name(img.name)
{
	auto img_dlib = convert::cv_to_dlib_rgb(img.img);
	auto rect_dlib = face_detector.detect(img_dlib);
	vertices = face_detector.predict(img_dlib, rect_dlib);
	estimate_direction();

	rect = convert::dlib_to_cv(rect_dlib);
	auto fg_edges = FaceDetector::get_fg_edge_points(img.img, rect, direction);
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
