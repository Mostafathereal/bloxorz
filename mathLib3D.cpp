#include <cmath>
#include "mathLib3D.h"

Point3D::Point3D() {
	this->mX = 0.0f;
	this->mY = 0.0f;
	this->mZ = 0.0f;
}

Point3D::Point3D(float inX, float inY, float inZ) {
	this->mX = inX;
	this->mY = inY;
	this->mZ = inZ;
}


float Point3D::distanceTo(Point3D other) {
	return sqrt(pow(this->mX - other.mX, 2) + pow(this->mY - other.mY, 2) + pow(this->mZ - other.mZ, 2));
}

float Point3D::fastDistanceTo(Point3D other) {
	return pow(this->mX - other.mX, 2) + pow(this->mY - other.mY, 2) + pow(this->mZ - other.mZ, 2);
}

Vec3D::Vec3D() {
	this->mX = 1.0f;
	this->mY = 1.0f;
	this->mZ = 1.0f;
}

Vec3D::Vec3D(float inX, float inY, float inZ) {
	this->mX = inX;
	this->mY = inY;
	this->mZ = inZ;
}

float Vec3D::length() {
	return sqrt(pow(this->mX, 2) + pow(this->mY, 2) + pow(this->mZ, 2));
}

Vec3D Vec3D::normalize() {
	float length = this->length();
	float normX = this->mX / length;
	float normY = this->mY / length;
	float normZ = this->mZ / length;

	return Vec3D(normX, normY, normZ);
}

Vec3D Vec3D::multiply(float scaler) {
	return Vec3D(scaler * this->mX, scaler * this->mY, scaler * this->mZ);
}

Vec3D Vec3D::add(Vec3D other) {
	return Vec3D(other.mX + this->mX, other.mY + this->mY, other.mZ + this->mZ);
}

Point3D Vec3D::movePoint(Point3D source, float scale) {
	return Point3D(source.mX + (this->mX * scale), source.mY + (this->mY * scale), source.mZ + (this->mZ * scale));
}

Vec3D Vec3D::createVector(Point3D p1, Point3D p2) {
	return Vec3D(p2.mX - p1.mX, p2.mY - p1.mY, p2.mZ - p1.mZ);
}