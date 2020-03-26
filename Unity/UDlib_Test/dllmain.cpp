#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "../../Dlib_Unity/All.h"
#include "../../Dlib_CvExt/All.h"

using namespace Dlib::Basic;
using namespace Dlib::Unity;
using namespace Dlib::CvExt;



enum ETest {
	A, B, C
};



class Struct {
public:
	enum ETest { TA, TB, TC };

	ETest a;
	Vec2i b;
	float c[2];

	Struct (ETest a, cv::Point b, float c1, float c2) {
		this->a = a;
		this->b = b;
		this->c[0] = c1;
		this->c[1] = c2;
	}
};



struct Struct2 {
public:
	float a, b;

	//Struct2 (float a, float b) : a (a), b (b) {}
};



class Struct3 {
public:
	bool a;
	Vec2i b;
	Vec2f c;

	Struct3 (bool a, Vec2i b, Vec2f c) : a (a), b (b), c (c) {}
};



extern "C" {

/* 此处C#对应的声明必须以引用传递参数a和b（cv::Size和cv::Point均为64位结构体的特例） */
__declspec(dllexport)
bool cvStructureTest (cv::Size a, cv::Point b, cv::Point& o_c) {

	o_c = { a.width + b.x, a.height + b.y };
	return false;
}



/* 此处C#对应的声明无需以引用传递参数a和b（正常传参模式） */
__declspec(dllexport)
bool cvStructureTest2 (cv::Vec3f a, cv::Vec3i b, cv::Vec3f& o_c) {

	o_c = cv::Vec3f (a[0] + b[0], a[1] + b[1], a[2] + b[2]);
	return true;

}



/* glm结构体可以正常传参 */
__declspec(dllexport)
void glmStructureTest (glm::ivec2 a, glm::ivec3 b, glm::vec2 c, glm::vec3& o_d) {

	o_d.x = a.x + b.x + c.x;
	o_d.y = a.y + b.y + c.y;
	o_d.z = (float)b.z;

}



/* 自定义结构体只需满足无自定义复制构造函数的要求即可正常传参 */
__declspec(dllexport)
void structureTest (Struct a, Struct& o_b) {
	
	o_b.a = Struct::ETest ((a.a + 1) % 3);
	o_b.b = (cv::Point)a.b + cv::Point (100, 200);
	o_b.c[0] = a.c[1];
	o_b.c[1] = a.c[0];

}



/* 错误：小尺寸C++类/结构体无法获得正确的返回结果 */
__declspec(dllexport)
Vec2f structureTest2 () {

	return cv::Point2f ( 1.0f, 2.0f );

}



/* 正确。对于大尺寸结构体返回值C#/C++采用相同的约定 */
__declspec(dllexport)
Struct3 structureTest3 () {

	return { false, cv::Point (1, 2), cv::Size2f (3.0f, 4.0f) };

}


/* 枚举变量测试 */
__declspec(dllexport)
ETest enumTest (ETest a) {
	
	return ETest ((a + 1) % 3);

}


/* 普通字符串测试 */
__declspec(dllexport)
const char* stringTest (const char* str, char* o_str, char** o_str2) {
	
	strcpy (o_str, str);
	*o_str2 = Str::ToReturnStr ("Return by parameter");
	return NULL;// Str::ToReturnStr ("Test OK");

}


/* 宽字符集测试 */
__declspec(dllexport)
const wchar_t* stringTest2 (const wchar_t* str, wchar_t* o_str, wchar_t** o_str2) {

	lstrcpyW (o_str, str);
	return Str::ToReturnStr (L"一切正常！");

}


/* 直接抛出异常会导致程序崩溃 */
__declspec(dllexport)
void exceptionTest2 () {
	
	throw std::exception ("Exception thrown from C++");

}



/* 使用UDLIB_INTERFACE包装后，函数体内抛出的异常会被传递给C#（C#需要首先调用UDlib.Core.Framework.Initialize()） */
UDLIB_INTERFACE (void exceptionTest ()) {

	throw std::exception ("Exception thrown from C++");

} END_UDLIB_INTERFACE



UDLIB_INTERFACE (void arrayAndMatrixTest (
	ArrayRef<float> arr1, ArrayRef<float> arr2, ArrayRef<float> arr3,
	UMatrix<float> mat1, UMatrix<float> mat2, UMatrix<float> mat3)) {

	// 修改arr1的内容
	for (int i = 0; i < arr1.SIZE (); ++i)
		arr1[i] = arr1[i] * 2.0f;

	auto newArr = new float[3];
	newArr[0] = 1.0f; newArr[1] = 2.0f; newArr[2] = 3.0f;
	// 修改arr2的指向
	arr2 = ArrayRef<float> (newArr, 3);
	// 修改arr3的指向
	arr3 = ArrayRef<float> (newArr, 3);

	cv::Mat_<float> mat1_ = mat1;
	for (int r = 0; r < mat1_.rows; ++r)
		for (int c = 0; c < mat1_.cols; ++c)
			mat1_ (r, c) *= 2.0f;

	auto newMat = new cv::Mat_<float> (2, 2);
	(*newMat) << 1.0f, 2.0f, 3.0f, 4.0f;

	// 修改mat2的指向
	mat2 = *newMat;
	// 修改mat3的指向
	mat3 = *newMat;

} END_UDLIB_INTERFACE
}