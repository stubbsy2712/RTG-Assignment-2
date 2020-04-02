////////////////////////////////////////////////////////////////////////////////
// Filename: bumpmap.vs
////////////////////////////////////////////////////////////////////////////////

vs.1.1

dcl_position v0
dcl_normal   v1
dcl_texcoord v2
dcl_tangent  v3

// c0   - { 0.0,  0.5, 1.0, 2.0}
// c1   - { 4.0, .5pi, pi, 2pi}  
// c2   - {1, -1/3!, 1/5!, -1/7!} for sin    (1.0f,      -1.0f/6.0f,  1.0f/120.0f, -1.0f/5040.0f)
// c3   - {1/2!, -1/4!, 1/6!, -1/8!} for cos (1.0f/2.0f, -1.0f/24.0f, 1.0f/720.0f, -1.0f/40320.0f)
// c4-7 - Composite World-View-Projection Matrix
// c8   - Model Space Camera Position
// c10  - {1.02, 0.04, 0, 0} fixup factor for Taylor series imprecision
// c11  - {0.5, 0.5, 0.25, 0.25} waveHeight0, waveHeight1, waveHeight2, waveHeight3
// c12  - {0.0, 0.0, 0.0, 0.0} waveOffset0, waveOffset1, waveOffset2, waveOffset3
// c13  - {0.6, 0.7, 1.2, 1.4} waveSpeed0, waveSpeed1, waveSpeed2, waveSpeed3
// c14  - {0.0, 2.0, 0.0, 4.0} waveDirX0, waveDirX1, waveDirX2, waveDirX3
// c15  - {2.0, 0.0, 4.0, 0.0} waveDirY0, waveDirY1, waveDirY2, waveDirY3
// c16  - { time }
// c17  - {-0.00015, 1.0, 0.0, 0.0} base texcoord distortion x0, y0, x1, y1
// c18  - World Martix

mul r0, c14, v2.x      // use tex coords as inputs to sinusoidal warp 
mad r0, c15, v2.y, r0  // use tex coords as inputs to sinusoidal warp 
         
mov r1, c16.x          // time...
mad r0, r1, c13, r0    // add scaled time to move bumps according to frequency
add r0, r0, c12
frc r0.xy, r0          // take frac of all 4 components
frc r1.xy, r0.zwzw
mov r0.zw, r1.xyxy
         
mul r0, r0, c10.x      // multiply by fixup factor (due to inaccuracy of taylor series)
sub r0, r0, c0.y       // subtract .5
mul r0, r0, c1.w       // mult tex coords by 2pi  coords range from(-pi to pi)
         
mul r5, r0, r0         // (wave vec)^2
mul r1, r5, r0         // (wave vec)^3 
mul r6, r1, r0         // (wave vec)^4 
mul r2, r6, r0         // (wave vec)^5
mul r7, r2, r0         // (wave vec)^6
mul r3, r7, r0         // (wave vec)^7 
mul r8, r3, r0         // (wave vec)^8
         
mad r4, r1, c2.y, r0   // (wave vec) - ((wave vec)^3)/3! 
mad r4, r2, c2.z, r4   //  + ((wave vec)^5)/5! 
mad r4, r3, c2.w, r4   //  - ((wave vec)^7)/7! 
         
mov r0, c0.z           // 1
mad r5, r5, c3.x ,r0   // -(wave vec)^2/2!
mad r5, r6, c3.y, r5   // +(wave vec)^4/4!
mad r5, r7, c3.z, r5   // -(wave vec)^6/6!
mad r5, r8, c3.w, r5   // +(wave vec)^8/8!
         
dp4 r0, r4, c11        // multiply wave heights by waves
         
mul r0, r0, v1         // apply deformation in direction of normal

add r0.xyz, r0, v0     // add to position
mov r0.w, c0.z         // homogenous component

m4x4    oPos, r0, c4   // OutPos = WorldSpacePos * Composite View-Projection Matrix
mov     oT0, v2        // Pass along texture coordinates

;This is where the shader starts to diverge a bit from the Ocean shader.  First the binormal is computed

mov     r3, v1
mul     r4, v3.yzxw, r3.zxyw
mad     r4, v3.zxyw, -r3.yzxw, r4   // cross product to find binormal

;Then the normal is warped based on the tangent space basis vectors (tangent and binormal).

mul     r1, r5, c11        // cos * waveheight
dp4     r9.x, -r1, c14     // amount of normal warping in direction of binormal
dp4     r9.y, -r1, c15     // amount of normal warping in direction of tangent
mul     r1, r4, r9.x       // normal warping in direction of binormal
mad     r1, v3, r9.y, r1   // normal warping in direction of tangent
mad     r5, r1, c10.y, v1  // warped normal move nx, ny: cos * wavedir * waveheight

;The normal is then renormalized.

mov     r10, r5
m3x3    r5, r10, c18       // transform normal
dp3     r10.x, r5, r5
rsq     r10.y, r10.x
mul     r5, r5, r10.y      // normalize warped normal

; Next the view vector is computed:
mov     r10, r0
m4x4    r0, r10, c18       // transform vertex position

sub     r2, c8, r0         // view vector
dp3     r10.x, r2, r2
rsq     r10.y, r10.x
mul     r2, r2, r10.y      // normalized view vector

; Then the dot product of the view vector and the warped normal is computed:

dp3     r7, r5, r2         // N.V 
mov     oT2, r7            // Pass along N.V

; This is used to compute the reflection vector.

add     r6, r7, r7         // 2N.V
mad     r6, r6, r5, -r2    // 2N(N.V)-V
mov     oT1, r6            // reflection vector
