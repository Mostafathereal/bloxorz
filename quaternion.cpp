#include "quaternion.h"
#include <cmath>

float PI = acos(-1);

Quaternion::Quaternion(){
    this->w = 1.0f;
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
}

Quaternion::Quaternion(float angle, float x, float y, float z){
    float halfAngleRad = angle * (PI / 360);
    float halfSin = sin(halfAngleRad);

    this->w = cos(halfAngleRad);
    this->x = halfSin * x;
    this->y = halfSin * y;
    this->z = halfSin * z;

    this->normalize();
}

Quaternion::Quaternion(float rotX, float rotY, float rotZ)
{

    //Set the values, which came in degrees, to radians for C++ trig functions
    float halfRotXRad = rotX * PI / 360;
    float halfRotYRad = rotY * PI / 360;
    float halfRotZRad = rotZ * PI / 360;

    //Components
    float C1 = cos(halfRotXRad);
    float C2 = cos(halfRotYRad);
    float C3 = cos(halfRotZRad);
    float S1 = sin(halfRotXRad);
    float S2 = sin(halfRotYRad);
    float S3 = sin(halfRotZRad);

    //Create the final values
    this->w = ((C1 * C2 * C3) - (S1 * S2 * S3));
    this->x = (S1 * S2 * C3) + (C1 * C2 * S3);
    this->y = (S1 * C2 * C3) + (C1 * S2 * S3);
    this->z = (C1 * S2 * C3) - (S1 * C2 * S3);
}

Quaternion Quaternion::operator* (Quaternion otherQuat){
    float newW = this->w * otherQuat.w - this->x * otherQuat.x - this->y * otherQuat.y - this->z * otherQuat.z;
    float newX = this->w * otherQuat.x + this->x * otherQuat.w + this->y * otherQuat.z - this->z * otherQuat.y;
    float newY = this->w * otherQuat.y - this->x * otherQuat.z + this->y * otherQuat.w + this->z * otherQuat.x;
    float newZ = this->w * otherQuat.z + this->x * otherQuat.y - this->y * otherQuat.x + this->z * otherQuat.w;
    // float newW = otherQuat.w * this->w - otherQuat.x * this->x - otherQuat.y * this->y - otherQuat.z * this->z;
    // float newX = otherQuat.w * this->x + otherQuat.x * this->w + otherQuat.y * this->z - otherQuat.z * this->y;
    // float newY = otherQuat.w * this->y - otherQuat.x * this->z + otherQuat.y * this->w + otherQuat.z * this->x;
    // float newZ = otherQuat.w * this->z + otherQuat.x * this->y - otherQuat.y * this->x + otherQuat.z * this->w;

    Quaternion prod;
    prod.w = newW;
    prod.x = newX;
    prod.y = newY;
    prod.z = newZ;

    prod.normalize();
    return prod;
}

void Quaternion::populateRotationMatrix(GLfloat* matrix){
    //column 1
    matrix[0] = (GLfloat) ((this->w * this->w) + (this->x * this->x) - (this->y * this->y) - (this->z * this->z));
    matrix[1] = (GLfloat) ((2 * this->x * this->y) + (2 * this->w * this->z)); 
    matrix[2] = (GLfloat) ((2 * this->x * this->z) - (2 * this->w * this->y));
    matrix[3] = 0;

    //column 2
    matrix[4] = (GLfloat) ((2 * this->x * this->y) - (2 * this->w * this->z)); 
    matrix[5] = (GLfloat) ((this->w * this->w) - (this->x * this->x) + (this->y * this->y) - (this->z * this->z));
    matrix[6] = (GLfloat) ((2 * this->y * this->z) + (2 * this->w * this->x));
    matrix[7] = 0;

    //column 3
    matrix[8] = (GLfloat) ((2 * this->x * this->z) + (2 * this->w * this->y));
    matrix[9] = (GLfloat) ((2 * this->y * this->z) - (2 * this->w * this->x));
    matrix[10] = (GLfloat) ((this->w * this->w) - (this->x * this->x) - (this->y * this->y) + (this->z * this->z));
    matrix[11] = 0;

    //column 4
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

void Quaternion::normalize(){
    float length = sqrt(this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z);

    this->w = this->w / length;
    this->x = this->x / length;
    this->y = this->y / length;
    this->z = this->z / length;
}

Quaternion slerp(Quaternion qa, Quaternion qb, float t) {
	// quaternion to return
	Quaternion qm;
	// Calculate angle between them.
	float cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
    if (cosHalfTheta < 0) {
        qb.w = -qb.w; qb.x = -qb.x; qb.y = -qb.y; qb.z = qb.z;
        cosHalfTheta = -cosHalfTheta;
    }
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (abs(cosHalfTheta) >= 1.0){
		qm.w = qa.w;qm.x = qa.x;qm.y = qa.y;qm.z = qa.z;
		return qm;
	}
	// Calculate temporary values.
	float halfTheta = acos(cosHalfTheta);
	float sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	if (fabs(sinHalfTheta) < 0.001){ // fabs is floating point absolute
		qm.w = (qa.w * 0.5 + qb.w * 0.5);
		qm.x = (qa.x * 0.5 + qb.x * 0.5);
		qm.y = (qa.y * 0.5 + qb.y * 0.5);
		qm.z = (qa.z * 0.5 + qb.z * 0.5);
		return qm;
	}
	float ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
	float ratioB = sin(t * halfTheta) / sinHalfTheta; 
	//calculate Quaternion.
	qm.w = (qa.w * ratioA + qb.w * ratioB);
	qm.x = (qa.x * ratioA + qb.x * ratioB);
	qm.y = (qa.y * ratioA + qb.y * ratioB);
	qm.z = (qa.z * ratioA + qb.z * ratioB);
	return qm;
}