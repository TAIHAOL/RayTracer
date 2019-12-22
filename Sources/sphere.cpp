#include "sphere.h"

//constructor given  center, radius, and material
sphere::sphere(glm::vec3 c, float r, int m, scene* s) : rtObject(s)
{
	center = c;
	radius = r;
	matIndex = m;
	myScene = s;
}

float sphere::testIntersection(glm::vec3 eye, glm::vec3 dir)
{
	//see the book for a description of how to use the quadratic rule to solve
	//for the intersection(s) of a line and a sphere, implement it here and
	//return the minimum positive distance or 9999999 if none
	float t1, t2;
	float check;
	float result;
	//see the book for a description of how to use the quadratic rule to solve
	//for the intersection(s) of a line and a sphere, implement it here and
	//return the minimum positive distance or 9999999 if none

	check = glm::dot(dir, (eye - center)) * glm::dot(dir, (eye - center)) - glm::dot(dir, dir) * (glm::dot((eye - center), (eye - center)) - radius * radius);
	t1 = (glm::dot((-dir), (eye - center)) + sqrt(check)) / glm::dot(dir, dir);
	t2 = (glm::dot((-dir), (eye - center)) - sqrt(check)) / glm::dot(dir, dir);

	if (check < 0) {
		return 9999999;
	}
	else {
		t1 = (glm::dot((-dir), (eye - center)) + sqrt(check)) / glm::dot(dir, dir);
		t2 = (glm::dot((-dir), (eye - center)) - sqrt(check)) / glm::dot(dir, dir);
		if (t1 < 0 && t2 < 0) {
			return 9999999;
		}
		else if (t1 > 0 && t2 < 0) {
			result = t1;
			return result;
		}
		else if (t1 < 0 && t2 > 0) {
			result = t2;
			return result;
		}
		else if (t1 > 0 && t2 > 0) {
			result = fmin(t1, t2);
			return result;
		}

	}

}

glm::vec3 sphere::getNormal(glm::vec3 eye, glm::vec3 dir)
{
	//once you can test for intersection,
	//simply add (distance * view direction) to the eye location to get surface location,
	//then subtract the center location of the sphere to get the normal out from the sphere
	glm::vec3 normal;
	//dont forget to normalize

	float distance = testIntersection(eye, dir);
	//if (distance < 9999999) {
		normal = eye + distance * dir - center;

		normal = glm::normalize(normal);

	//}
		return normal;
}

glm::vec2 sphere::getTextureCoords(glm::vec3 eye, glm::vec3 dir)
{
	//find the normal as in getNormal

	//use these to find spherical coordinates
	float pi = 3.1415926;
	glm::vec3 normal = getNormal(eye, dir);
	//use these to find spherical coordinates
	glm::vec3 x(1, 0, 0);
	glm::vec3 z(0, 0, 1);

	float phi, theta;

	//phi is the angle down from z
	//theta is the angle from x curving toward y
	//hint: angle between two vectors is the acos() of the dot product
	//find phi using normal and z
	phi = acos(glm::dot(normal, z));

	glm::vec3 projection(normal.x, normal.y, 0);

	theta = acos(glm::dot(projection, x) / glm::length(projection));

	if (normal.y < 0) {
		theta = 2 * pi - theta;
	}

	glm::vec2 coords(theta / (2 * pi), phi / pi);
	return coords;
}