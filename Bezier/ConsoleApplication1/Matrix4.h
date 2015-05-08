//
//  Matrix3.h
//  SimpleFPS
//
//  Created by Dimitriy Dounaev on 22/12/13.
//  Copyright (c) 2013 Dimitriy Dounaev. All rights reserved.
//


#ifndef __SimpleFPS__Matrix4__
#define __SimpleFPS__Matrix4__

#include <iostream>


typedef struct
{
    float m00;
    float m01;
    float m02;
    float m03;
    float m10;
    float m11;
    float m12;
    float m13;
	float m20;
	float m21;
	float m22;
	float m23;
	float m30;
	float m31;
	float m32;
	float m33;
} Matrix4;

/*Matrix3 scalerMultiplyMatrix3(Matrix3 matrix, GLfloat scalerValue);
Matrix3 addMatrix3(Matrix3 matrixA, Matrix3 matrixB);
Matrix3 makeRotationMatrix3(GLfloat angle, GLfloat xAxis, GLfloat yAxis, GLfloat zAxis);*/

const Matrix4 identityMatrix4 = {  1.0f, 0.0f, 0.0f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f,
0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 0.0f, 1.0f };

#endif /* defined(__SimpleFPS__Matrix3__) */
