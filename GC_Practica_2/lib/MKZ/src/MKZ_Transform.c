#include "MKZ_Arithmetic.h"

#include <stdio.h>


void MKZ_TRANSFORM_matrix_global(float * modMat, float * transformMat);

void MKZ_TRANSFORM_to_cameraMatrix(float* transform_mat , float* camera_mat){

	MKZ_ARITHMETIC_identityMatrix(camera_mat);

	int i = 0;
	for (i = 0; i < 9 ; i++){
		camera_mat[((i%3)*4) + i/3] = transform_mat[ i%3 + 4*(i/3)];
	}

	for (i = 0 ; i < 3 ; i++ ){
		camera_mat[12 + i] = -MKZ_ARITHMETIC_dotProduct(transform_mat+i*4,transform_mat+12);
	}

}

void MKZ_TRANSFORM_matrix_global(float * modMat, float * transformMat){

	float * temp = MKZ_ARITHMETIC_create_matrix();

	MKZ_ARITHMETIC_matMul(modMat, transformMat, temp);

	MKZ_ARITHMETIC_copy_matrix(temp, transformMat);

	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_translate_global(float * tramat, MKZ_vector3 * v3){

	tramat[12] += v3->x;
	tramat[13] += v3->y;
	tramat[14] += v3->z;

}

void MKZ_TRANSFORM_rotate_global(float * tramat, MKZ_line * axis,  float rads){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_rotate(axis, rads, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_rotateX_global(float * tramat, float rads){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_rotateX(rads, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_rotateY_global(float * tramat, float rads){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_rotateY(rads, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_rotateZ_global(float * tramat, float rads){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_rotateZ(rads, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scale_global(float * tramat, MKZ_vector3 * along, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scale(along, scalar, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scaleX_global(float * tramat, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scaleX(scalar, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scaleY_global(float * tramat, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scaleY(scalar, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scaleZ_global(float * tramat, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scaleZ(scalar, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scaleUniform_global(float * tramat, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scaleUniform(scalar, temp);
	MKZ_TRANSFORM_matrix_global(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_set_position_global(float * modMat, MKZ_point3 * position){

	modMat[12] = position->x;
	modMat[13] = position->y;
	modMat[14] = position->z;

}


void MKZ_TRANSFORM_set_absolute_scale(float* modMat, MKZ_vector3 * scaleVector){

	float f = MKZ_ARITHMETIC_eulidean_norm(modMat);
	float f2 = scaleVector->x/f;
	modMat[0] *= f2;
	modMat[1] *= f2;
	modMat[2] *= f2;

	f = MKZ_ARITHMETIC_eulidean_norm(modMat+4);
	f2 = scaleVector->y/f;
	modMat[4] *= f2;
	modMat[5] *= f2;
	modMat[6] *= f2;

	f = MKZ_ARITHMETIC_eulidean_norm(modMat+8);
	f2 = scaleVector->z/f;
	modMat[8] *= f2;
	modMat[9] *= f2;
	modMat[10] *= f2;
}

void MKZ_TRANSFORM_look_at(float * modMat, MKZ_point3 * p3){

	//direct es el vector dirección de la camaŕa al objeto.
		float direct_x = p3->x - modMat[12];
		float direct_y = p3->y - modMat[13];
		float direct_z = p3->z - modMat[14];

		MKZ_vector3 direction;
		direction.x = direct_x;
		direction.y = direct_y;
		direction.z = direct_z;

		MKZ_vector3 vup;
		vup.x = modMat[4];
		vup.y = modMat[5];
		vup.z = modMat[6];

		MKZ_vector3 newX;

		MKZ_ARITHMETIC_normalize_vector(&direction);
		MKZ_ARITHMETIC_normalize_vector(&vup);
		MKZ_ARITHMETIC_corssProduct_vector( &direction, &vup, &newX);
		MKZ_ARITHMETIC_corssProduct_vector( &newX , &direction, &vup);


		float  mod = MKZ_ARITHMETIC_eulidean_norm(modMat);
		//mod = 1;
		modMat[0] = newX.x*mod;
		modMat[1] = newX.y*mod;
		modMat[2] = newX.z*mod;

		mod = MKZ_ARITHMETIC_eulidean_norm(modMat+4);
		//mod = 1;
		modMat[4]= vup.x*mod;
		modMat[5]= vup.y*mod;
		modMat[6]= vup.z*mod;

		mod = MKZ_ARITHMETIC_eulidean_norm(modMat+8);
		//mod = 1;
		modMat[8] = -direction.x*mod;
		modMat[9] = -direction.y*mod;
		modMat[10] = -direction.z*mod;
}

MKZ_vector3 * MKZ_TRANSFORM_get_absolute_scale(float* tramat){

	MKZ_vector3 * v3 = MKZ_GEOMETRY_create_vector3();

	v3->x = MKZ_ARITHMETIC_eulidean_norm(tramat);
	v3->y = MKZ_ARITHMETIC_eulidean_norm(tramat+4);
	v3->z = MKZ_ARITHMETIC_eulidean_norm(tramat+8);

	return v3;

}

/* local transform */
void MKZ_TRANSFORM_matrix_local(float * modMat, float * transformMat){

	float * temp = MKZ_ARITHMETIC_create_matrix();

	MKZ_ARITHMETIC_matMul( transformMat, modMat, temp);

	MKZ_ARITHMETIC_copy_matrix(temp, transformMat);

	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_translate_local(float * tramat, MKZ_vector3 * v3){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_translate(v3, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);


}

void MKZ_TRANSFORM_rotate_local(float * tramat, MKZ_line * axis, float rads){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_rotate(axis, rads, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_rotateX_local(float * tramat, float rads){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_rotateX(rads, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_rotateY_local(float * tramat, float rads){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_rotateY(rads, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_rotateZ_local(float * tramat, float rads){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_rotateZ(rads, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scale_local(float * tramat, MKZ_vector3 * along, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scale(along, scalar, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scaleX_local(float * tramat, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scaleX( scalar, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scaleY_local(float * tramat, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scaleY( scalar, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scaleZ_local(float * tramat, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scaleZ( scalar, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}

void MKZ_TRANSFORM_scaleUniform_local(float * tramat, float scalar){

	float * temp = MKZ_ARITHMETIC_create_matrix();
	MKZ_ARITHMETIC_matrix_scaleUniform(scalar, temp);
	MKZ_TRANSFORM_matrix_local(temp, tramat);
	MKZ_ARITHMETIC_free_matrix(temp);

}


