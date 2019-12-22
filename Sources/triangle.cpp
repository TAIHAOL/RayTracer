#include "triangle.h"

//constructor given  center, radius, and material
triangle::triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float tx0, float tx1, float tx2, float ty0, float ty1, float ty2, int m, scene* s) : rtObject(s)
{
	point0 = p0;
	point1 = p1;
	point2 = p2;

	texX0 = tx0;
	texX1 = tx1;
	texX2 = tx2;
	texY0 = ty0;
	texY1 = ty1;
	texY2 = ty2;
	matIndex = m;
	myScene = s;
}

float triangle::testIntersection(glm::vec3 eye, glm::vec3 dir)
{
	//see the book/slides for a description of how to use Cramer's rule to solve
	//for the intersection(s) of a line and a plane, implement it here and
	//return the minimum distance (if barycentric coordinates indicate it hit
	//the triangle) otherwise 9999999

	float x01= point0.x - point1.x;
	float y01 = point0.y - point1.y;
	float z01 = point0.z - point1.z;

	float x02 = point0.x - point2.x;
	float y02 = point0.y - point2.y;
	float z02 = point0.z - point2.z;

	float x = dir.x;
	float y = dir.y;
	float z = dir.z;

	float a = point0.x - eye.x;
	float b = point0.y - eye.y;
	float c = point0.z - eye.z;

	float M = (x01 * (y02 * z - y * z02)) + (y01 * (x * z02 - x02 * z)) + (z01 * (x02 * y - y02 * x));

	float beta = (a * (y02 * z - y * z02) + b * (x * z02 - x02 * z) + c * (x02 * y - y02 * x)) / M;

	float gamma = (z * (x01 * b - a * y01) + y * (a * z01 - x01 * c) + x * (y01 * c - b * z01)) / M;

	float t = -(z02 * (x01 * b - a * y01) + y02 * (a * z01 - x01 * c) + x02 * (y01 * c - b * z01)) / M;

	if (t < 0.001f || t>9999999)
		return 9999999;
	if (gamma < 0 || gamma>1)
		return 9999999;
	if (beta < 0 || (beta + gamma) > 1)
		return 9999999;

	glm::vec3 pointA = point0 - eye;
	glm::vec3 b1 = point1 - point0;
	glm::vec3 b2 = point2 - point0;

	float dist = glm::length(pointA + beta * b1 + gamma * b2);

	return dist;
}

glm::vec3 triangle::getNormal(glm::vec3 eye, glm::vec3 dir)
{
	//construct the barycentric coordinates for the plane
	glm::vec3 bary1 = point1 - point0;
	glm::vec3 bary2 = point2 - point0;

	//cross them to get the normal to the plane
	//note that the normal points in the direction given by right-hand rule
	//(this can be important for refraction to know whether you are entering or leaving a material)
	glm::vec3 normal = glm::normalize(glm::cross(bary1, bary2));
	return normal;
}

glm::vec2 triangle::getTextureCoords(glm::vec3 eye, glm::vec3 dir)
{
	//find alpha and beta (parametric distance along barycentric coordinates)
	//use these in combination with the known texture surface location of the vertices
	//to find the texture surface location of the point you are seeing

	glm::mat3 A, a, b, c;
	float t, beta, gamma, alpha;

	A = { point0.x - point2.x, point0.x - point1.x, dir.x,
		  point0.y - point2.y, point0.y - point1.y, dir.y,
		  point0.z - point2.z, point0.z - point1.z, dir.z };

	a = { point0.x - eye.x, point0.x - point1.x, dir.x,
		  point0.y - eye.y, point0.y - point1.y, dir.y,
		  point0.z - eye.z, point0.z - point1.z, dir.z };

	b = { point0.x - point2.x, point0.x - eye.x, dir.x,
		  point0.y - point2.y, point0.y - eye.y, dir.y,
		  point0.z - point2.z, point0.z - eye.z, dir.z };



	beta = glm::determinant(a) / glm::determinant(A);
	gamma = glm::determinant(b) / glm::determinant(A);
	alpha = 1 - beta - gamma;

	glm::vec2 coords = (glm::vec2(texX0, texY0) * alpha) + (glm::vec2(texX1, texY1) * gamma) + (glm::vec2(texX2, texY2) * beta);

	return coords;
}