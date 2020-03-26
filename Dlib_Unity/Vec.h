#pragma once



NS_DLIB_UNITY_BEGIN

template<class T>
class Vec2 {
public:
	T x, y;

	Vec2 () : x (), y () {}
	Vec2 (const cv::Vec<T, 2>& a) : x (a[0]), y (a[1]) {}
	Vec2 (const cv::Size_<T>& a) : x (a.width), y (a.height) {}
	Vec2 (const cv::Point_<T>& a) : x (a.x), y (a.y) {}
	Vec2 (const glm::tvec2<T>& a) : x (a.x), y (a.y) {}
	operator cv::Vec<T, 2> () const { return { x, y }; }
	operator cv::Size_<T> () const { return { x, y }; }
	operator cv::Point_<T> () const { return { x, y }; }
	operator glm::tvec2<T> () const { return { x, y }; }
	bool operator== (const Vec2<T>& rhs) { return x == rhs.x && y == rhs.y; }
};



template<class T>
class Vec3 {
public:
	T x, y, z;

	Vec3 () : x (), y (), z () {}
	Vec3 (const cv::Vec<T, 3>& a) : x (a[0]), y (a[1]) {}
	Vec3 (const cv::Point3_<T>& a) : x (a.x), y (a.y), z (a.z) {}
	Vec3 (const glm::tvec3<T>& a) : x (a.x), y (a.y), z (a.z) {}
	operator cv::Vec<T, 3> () const { return { x, y, z }; }
	operator cv::Point3_<T> () const { return { x, y, z }; }
	operator glm::tvec3<T> () const { return { x, y, z }; }
	bool operator== (const Vec3<T>& rhs) { return x == rhs.x && y == rhs.y && z == rhs.z; }

};



template<class T>
class Vec4 {
public:
	T x, y, z, w;

	Vec4 () : x (), y (), z (), w () {}
	Vec4 (const cv::Vec<T, 4>& a) : x (a[0]), y (a[1]) {}
	Vec4 (const glm::tvec4<T>& a) : x (a.x), y (a.y), z (a.z), w (a.w) {}
	Vec4 (const cv::Rect_<T>& a) : x (a.x), y (a.y), z (a.width), w (a.height) {}
	Vec4 (const CvExt::Color& a) {
		auto b = a (CvExt::EColor_RGB);
		if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
			x = (T)(b[0] / 255), y = (T)(b[1] / 255), z = (T)(b[2] / 255), w = (T)(b[3] / 255);
		else
			x = (T)b[0], y = (T)b[1], z = (T)b[2], w = (T)b[3];
	}
	operator cv::Vec<T, 4> () const { return { x, y, z, w }; }
	operator glm::tvec4<T> () const { return { x, y, z, w }; }
	operator cv::Rect_<T> () const { return { x, y, z, w }; }
	operator CvExt::Color () const {
		if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
			return CvExt::Color::RGB ((T)(x * 255), (T)(y * 255), (T)(z * 255), (T)(w * 255));
		return CvExt::Color::RGB (x, y, z, w);
	}
	bool operator== (const Vec4<T>& rhs) { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
};

typedef Vec2<uint8_t> Vec2b;
typedef Vec2<int16_t> Vec2s;
typedef Vec2<int32_t> Vec2i;
typedef Vec2<uint32_t> Vec2u;
typedef Vec2<int64_t> Vec2l;
typedef Vec2<uint64_t> Vec2ul;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec3<uint8_t> Vec3b;
typedef Vec3<int16_t> Vec3s;
typedef Vec3<int32_t> Vec3i;
typedef Vec3<uint32_t> Vec3u;
typedef Vec3<int64_t> Vec3l;
typedef Vec3<uint64_t> Vec3ul;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec4<uint8_t> Vec4b;
typedef Vec4<int16_t> Vec4s;
typedef Vec4<int32_t> Vec4i;
typedef Vec4<uint32_t> Vec4u;
typedef Vec4<int64_t> Vec4l;
typedef Vec4<uint64_t> Vec4ul;
typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;


NS_DLIB_UNITY_END



namespace cv {
	template<typename _Tp>
	class DataType<Dlib::Unity::Vec2<_Tp>> {
	public:
		typedef Dlib::Unity::Vec2<_Tp> value_type;
		typedef Dlib::Unity::Vec2<_Tp> work_type;
		typedef _Tp channel_type;
		// DataDepth is another helper trait class
		enum {
			depth = DataDepth<_Tp>::value, channels = 2,
			fmt = (channels - 1) * 256 + DataDepth<_Tp>::fmt,
			type = CV_MAKETYPE (depth, channels)
		};
	};
	template<typename _Tp>
	class DataType<Dlib::Unity::Vec3<_Tp>> {
	public:
		typedef Dlib::Unity::Vec3<_Tp> value_type;
		typedef Dlib::Unity::Vec3<_Tp> work_type;
		typedef _Tp channel_type;
		// DataDepth is another helper trait class
		enum {
			depth = DataDepth<_Tp>::value, channels = 3,
			fmt = (channels - 1) * 256 + DataDepth<_Tp>::fmt,
			type = CV_MAKETYPE (depth, channels)
		};
	};
	template<typename _Tp>
	class DataType<Dlib::Unity::Vec4<_Tp>> {
	public:
		typedef Dlib::Unity::Vec4<_Tp> value_type;
		typedef Dlib::Unity::Vec4<_Tp> work_type;
		typedef _Tp channel_type;
		// DataDepth is another helper trait class
		enum {
			depth = DataDepth<_Tp>::value, channels = 4,
			fmt = (channels - 1) * 256 + DataDepth<_Tp>::fmt,
			type = CV_MAKETYPE (depth, channels)
		};
	};
};
