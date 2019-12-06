// https://github.com/Flix01/Tiny-OpenGL-Shadow-Mapping-Examples
/** License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_
#include <math.h>
#include <stdio.h>	// printf

#ifndef HELPER_FUNCTIONS_VERSION
#   define HELPER_FUNCTIONS_VERSION 1.0
#endif //VERSION

#ifdef __cplusplus
extern "C" {
#endif


/* The __restrict and __restrict__ keywords are recognized in both C, at all language levels, and C++, at LANGLVL(EXTENDED).*/
//#ifdef NO_RESTRICT  // please define it globally if the keyword __restrict is not present
#   ifndef __restrict   
#       define __restrict /*no-op*/
#   endif
//#endif //NO_RESTRICT


#ifndef MATRIX_USE_DOUBLE_PRECISION
typedef float  hloat;       // short form of hELPER_FUNCTIONS_FLoat
#else
typedef double hloat;
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_HALF_PI
#define M_HALF_PI (M_PI/2.0)
#endif
#ifndef M_PIOVER180
#define M_PIOVER180 (3.14159265358979323846/180.0)
#endif
#ifndef M_180OVERPI
#define M_180OVERPI (180.0/3.14159265358979323846)
#endif


hloat Helper_Round(hloat number);
void Helper_IdentityMatrix(hloat* __restrict result16);
void Helper_CopyMatrix(hloat* __restrict dst16,const hloat* __restrict src16);
void Helper_MultMatrix(hloat* __restrict result16,const hloat* __restrict ml16,const hloat* __restrict mr16);
void Helper_MatrixMulDir(const hloat* __restrict m16,hloat* __restrict dirOut3,const hloat dirX,hloat dirY,hloat dirZ);
void Helper_MatrixMulPos(const hloat* __restrict m16,hloat* __restrict posOut3,const hloat posX,hloat posY,hloat posZ);
hloat Helper_Vector3Dot(const hloat* a3,const hloat* b3);
void Helper_Vector3Normalize(hloat* __restrict v3);
void Helper_Vector3Cross(hloat* __restrict vOut3,const hloat* __restrict a3,const hloat* __restrict b3);
hloat Helper_Vector3DistSquared(const hloat* __restrict a3,const hloat* __restrict b3);
hloat Helper_Vector3Dist(const hloat* __restrict a3,const hloat* __restrict b3);
void Helper_ConvertMatrixd2f16(float* __restrict result16,const double* __restrict m16);
void Helper_ConvertMatrixf2d16(double* __restrict result16,const float* __restrict m16);
void Helper_ConvertMatrixd2f9(float* __restrict result9,const double* __restrict m9);
void Helper_ConvertMatrixf2d9(double* __restrict result9,const float* __restrict m9);
void Helper_GlUniformMatrix4v(GLint location,GLsizei count,GLboolean transpose,const hloat* value);
void Helper_GlUniformMatrix3v(GLint location,GLsizei count,GLboolean transpose,const hloat* value);
void Helper_LookAt(hloat* __restrict mOut16,hloat eyeX,hloat eyeY,hloat eyeZ,hloat centerX,hloat centerY,hloat centerZ,hloat upX,hloat upY,hloat upZ);
void Helper_Perspective(hloat* __restrict mOut16,hloat degfovy,hloat aspect, hloat zNear, hloat zFar);
void Helper_Ortho(hloat* __restrict mOut16,hloat left,hloat right, hloat bottom, hloat top,hloat nearVal,hloat farVal);
void Helper_Ortho3D(hloat* __restrict mOut16,hloat cameraTargetDistance,hloat degfovy,hloat aspect,hloat znear,hloat zfar);
hloat Helper_Perspective_LinearizeDepth(hloat depth,hloat nearClippingPlane,hloat farClippingPlane);
hloat Helper_Perspective_DelinearizeDepth(hloat depth,hloat nearClippingPlane,hloat farClippingPlane);
hloat Helper_Perspective_DepthValueToZ(const hloat* __restrict pMatrix,hloat depthValue);
hloat Helper_Perspective_ZToDepthValue(const hloat* __restrict pMatrix,hloat zEye);
hloat Helper_Ortho_DepthValueToZ(const hloat* __restrict pMatrix,hloat depthValue);
hloat Helper_Ortho_ZToDepthValue(const hloat* __restrict pMatrix,hloat zEye);
int Helper_InvertMatrix(hloat* __restrict mOut16,const hloat* __restrict m16);
void Helper_InvertMatrixFast(hloat* __restrict mOut16,const hloat* __restrict m16);
void Helper_GetFrustumPlaneEquations(hloat planeEquationsOut[6][4],const hloat* __restrict vpMatrix16,int normalizePlanes);
void Helper_GetFrustumPoints(hloat frustumPoints[8][4],const hloat* __restrict vpMatrixInverse16);
void Helper_GetFrustumAabbCenterAndHalfExtents(hloat* __restrict frustumCenterOut3,hloat* __restrict frustumHalfExtentsOut3,const hloat frustumPoints[8][4]);
// 'optionalPMatrixInverse16' is required only if you need to retrieve (one or more of) the arguments that follow it (otherwise their value is untouched).
void Helper_GetLightViewProjectionMatrixExtra(hloat* __restrict lvpMatrixOut16,
                                                          const hloat* __restrict cameraVMatrixInverse16,
                                                          hloat cameraNearClippingPlane,hloat cameraFarClippingPlane,hloat cameraFovyDeg,hloat cameraAspectRatio,hloat cameraTargetDistanceForUnstableOrtho3DModeOnly_or_zero,
                                                          const hloat*  __restrict normalizedLightDirection3, hloat texelIncrement
                                                          ,hloat* __restrict optionalSphereCenterOut,hloat* __restrict optionalSphereRadiiSquaredOut
                                                          ,const hloat* __restrict optionalCameraPMatrixInverse16
                                                          ,hloat* __restrict optionalLightViewportClippingOut4,hloat optionalCameraFrustumPointsInNDCLightSpaceOut[8][4]
                                                          ,hloat* __restrict optionalLVPMatrixForFrustumCullingUsageOut16   // Highly experimental and untested
                                                          );
void Helper_GetLightViewProjectionMatrix(hloat* __restrict lvpMatrixOut16,
                                                          const hloat* __restrict cameraVMatrixInverse16,
                                                          hloat cameraNearClippingPlane,hloat cameraFarClippingPlane,hloat cameraFovyDeg,hloat cameraAspectRatio,
                                                          const hloat*  __restrict normalizedLightDirection3, hloat texelIncrement);
// Warning: 'cascadeNearAndFarPlanesArrayOut' (and 'optionalCascadeNearAndFarPlanesInClipSpaceArrayOut' if used) must contain 'numCascades+1' elements
// (returned cascadeNearAndFarPlanesArrayOut[0]==cameraNearClippingPlane and cascadeNearAndFarPlanesArrayOut[numCascades]==cameraFarClippingPlane)
 void Helper_GetCascadeNearAndFarClippingPlaneArray(hloat*  __restrict cascadeNearAndFarPlanesArrayOut,int numCascades,hloat lambda,hloat cameraNearClippingPlane,hloat cameraFarClippingPlane
                                                                    //,hloat* __restrict optionalCascadeNearAndFarPlanesInClipSpaceArrayOut
                                                                    );
/* 'cascadeNearAndFarPlanesArray' must be an array of numCascades+1 hloats
* 'optionalCameraPMatrixInverseArray' is required only if you need to retrieve (one or more of) the arguments that follow it (otherwise their value is untouched).
* 'optionalCameraPMatrixInverseArray'  (if used) must be an array of 16*numCascades hloats (basically one camera pMatrixInverse per cascade, because the near and far planes change)
* 'optionalLightViewportClippingArrayOut' (if used) must be an array of 4*numCascades hloats
* 'optionalCameraFrustumPointsInNDCLightSpaceArrayOut' (if used) must be an array of [8*numCascades][4] hloat
* 'optionalLVPMatrixForFrustumCullingUsageArrayOut' (if used must contain 16*numCascades hloats
*/
 void Helper_GetLightViewProjectionMatricesExtra(hloat* __restrict lvpMatricesOut16,const hloat*  __restrict cascadeNearAndFarPlanesArray,int numCascades,
                                                          const hloat* __restrict cameraVMatrixInverse16,
                                                          hloat cameraFovyDeg,hloat cameraAspectRatio,hloat cameraTargetDistanceForOrtho3DModeOnly_or_zero,
                                                          const hloat*  __restrict normalizedLightDirection3, hloat texelIncrement
                                                          ,hloat* __restrict optionalCascadeSphereCenterPlanesOut,hloat* __restrict optionalCascadeSphereRadiiSquaredOut
                                                          ,const hloat* __restrict optionalCameraPMatrixInverseArray
                                                          ,hloat* __restrict optionalLightViewportClippingArrayOut,hloat optionalCameraFrustumPointsInNDCLightSpaceArrayOut[][4]
                                                          ,hloat* __restrict optionalLVPMatrixForFrustumCullingUsageArrayOut   // Highly experimental and untested
                                                          );
 void Helper_GetLightViewProjectionMatrices(hloat* __restrict lvpMatricesOut16,const hloat*  __restrict cascadeNearAndFarPlanesArray,int numCascades,
                                                          const hloat* __restrict cameraVMatrixInverse16,
                                                          hloat cameraFovyDeg,hloat cameraAspectRatio,hloat cameraTargetDistanceForOrtho3DModeOnly_or_zero,
                                                          const hloat*  __restrict normalizedLightDirection3, hloat texelIncrement);
 void Helper_GetLightViewProjectionMatricesHorizontal(hloat* __restrict lvpMatrixRightOut16,hloat* __restrict lvpMatrixLeftOut16,hloat cameraNearPlane,hloat cameraFarPlane,
                                                                      const hloat* __restrict cameraVMatrixInverse16,
                                                                      hloat cameraFovyDeg,hloat cameraAspectRatio,hloat cameraTargetDistanceForOrtho3DModeOnly_or_zero,
                                                                      const hloat*  __restrict normalizedLightDirection3, hloat texelIncrement);
 void Helper_GetLightViewProjectionMatricesHorizontalAndVertical(hloat* __restrict lvpMatricesOut64,hloat cameraNearPlane,hloat cameraFarPlane,
                                                                      const hloat* __restrict cameraVMatrixInverse16,
                                                                      hloat cameraFovyDeg,hloat cameraAspectRatio,
                                                                      const hloat*  __restrict normalizedLightDirection3, hloat texelIncrement);
// 'optionalPMatrixInverse16' is required only if you need to retrieve (one or more of) the arguments that follow it (otherwise their value is untouched).
void Helper_GetLightViewProjectionMatrixTextureWidthAlignedExtra(hloat* __restrict lvpMatrixOut16,
                                                          const hloat* __restrict cameraVMatrixInverse16,
                                                          hloat cameraNearClippingPlane,hloat cameraFarClippingPlane,hloat cameraFovyDeg,hloat cameraAspectRatio,hloat cameraTargetDistanceForUnstableOrtho3DModeOnly_or_zero,
                                                          const hloat*  __restrict normalizedLightDirection3, hloat texelIncrement
                                                          ,hloat* __restrict optionalSphereCenterOut,hloat* __restrict optionalSphereRadiiSquaredOut
                                                          ,const hloat* __restrict optionalCameraPMatrixInverse16
                                                          ,hloat* __restrict optionalLightViewportClippingOut4,hloat optionalCameraFrustumPointsInNDCLightSpaceOut[8][4]
                                                          ,hloat* __restrict optionalLVPMatrixForFrustumCullingUsageOut16   // Highly experimental and untested
                                                          );
void Helper_GetLightViewProjectionMatrixTextureWidthAligned(hloat* __restrict lvpMatrixOut16,
                                                          const hloat* __restrict cameraVMatrixInverse16,
                                                          hloat cameraNearClippingPlane,hloat cameraFarClippingPlane,hloat cameraFovyDeg,hloat cameraAspectRatio,
                                                          const hloat*  __restrict normalizedLightDirection3, hloat texelIncrement);

void Helper_Min3(hloat* __restrict res3,const hloat* a3,const hloat* b3);
void Helper_Max3(hloat* __restrict res3,const hloat* a3,const hloat* b3);
// It "should" performs AABB test. mfMatrix16 is the matrix M so that: F*M = mvpMatrix (F being the matrix used to extract the frustum planes). We can use: F=pMatrix and M=mvMatrix, or: F=vpMatrix and M=mMatrix.
int Helper_IsVisible(const hloat frustumPlanes[6][4],const hloat*__restrict mfMatrix16,hloat aabbMinX,hloat aabbMinY,hloat aabbMinZ,hloat aabbMaxX,hloat aabbMaxY,hloat aabbMaxZ);

// Loading shader function
GLhandleARB Helper_LoadShader(const char* buffer, const unsigned int type);
GLuint Helper_LoadShaderProgramFromSourceWithGeometryShader(const char* vs,const char* gs,const char* fs);
GLuint Helper_LoadShaderProgramFromSource(const char* vs,const char* fs);

// elapsedMs: from the start of the application
// cosAlpha,sinAlpha: some changing values used to move two objects
// cameraTargetPosition3 is an optional array of 3 floats containing the camera target position (to display a 3D pivot)
// pOptionalDisplayListBase: when not 0, 40 display lists are generated, created and used
//                          please free them at cleanup using glDeleteLists(*pOptionalDisplayListBase,40);
void Helper_GlutDrawGeometry(float elapsedMs, float cosAlpha, float sinAlpha,const float* cameraTargetPosition3,GLuint* pOptionalDisplayListBase);

#ifdef __cplusplus
}   // extern "C"
#endif

#endif //HELPER_FUNCTIONS_H_