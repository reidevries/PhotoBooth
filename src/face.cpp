#include "face.hpp"

using namespace face;

Face::Face(const NamedImg& img, FaceDetector& face_detector) : name(img.name)
{
	auto img_dlib = convert::cv_to_dlib_rgb(img.img);
	auto rect_dlib = face_detector.detect(img_dlib);
	vertices = face_detector.predict(img_dlib, rect_dlib);

	rect = convert::dlib_to_cv(rect_dlib);
	auto fg_edges = FaceDetector::get_fg_edge_points(img.img, rect);
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
