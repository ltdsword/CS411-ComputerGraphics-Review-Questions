


Based on the provided images, here are the transcribed questions that are clearly legible along with their correct answers and brief explanations. 

### **From Image 2**

**9) The parallel projection is different from the perspective projection by the following points:**
*   a) The size of object in the 2D display scene is not dependent on the distance from the object to eye point.
*   b) The parallel lines in the real world are projected to non parallel lines in the 2D display scene.
*   c) Its view volume is a truncated 4-sided pyramid.
*   d) a), b), and c) are correct.
> **Answer: a**
> **Explanation:** In an orthographic/parallel projection, parallel lines remain parallel, and distance from the viewer does not affect the size of the projected object. In contrast, perspective projection causes distant objects to appear smaller (size depends on distance), projects parallel lines to converging points (vanishing points), and uses a truncated pyramid (frustum) as its view volume.

**10) In the Sutherland – Hodgeman algorithm of clipping one polygon, the final result will be**
*   a) only one polygon
*   b) only two polygons
*   c) always concave polygons
*   d) always convex polygons
> **Answer: a**
> **Explanation:** The standard Sutherland-Hodgman algorithm takes a single polygon (represented as a single list of vertices) and outputs a single list of vertices. A well-known limitation of the basic algorithm is that if it clips a concave polygon into pieces that should visually be separate, it will still connect them with a degenerate edge, returning "only one polygon".

**11) In the Cyrus-Beck algorithm of clipping a line defined by two points $P_0$ and $P_1$, what is the meaning of D in the condition $N_i \cdot D == 0$, where $N_i$ is the normal vector of line polygon edge and $D = (P_1 - P_0)$?**
*   a) D is a point positioning on the line $P_0P_1$
*   b) D is a scalar value.
*   c) D is a vector with its head and tail positioning at $P_1$ and $P_0$, respectively
*   d) D is a vector with its head and tail positioning at $P_0$ and $P_1$, respectively
> **Answer: c**
> **Explanation:** The equation $D = P_1 - P_0$ calculates the direction vector of the line segment pointing from $P_0$ to $P_1$. By definition of vector subtraction, this means the tail (origin) of the vector is at $P_0$ and the head (tip) is at $P_1$.

**12) The efficiency of Sutherland-Hodgeman polygon clipping method comes from**
*   a) Pipelining
*   b) Integer computations
*   c) Logic computations
*   d) None of the above
> **Answer: a**
> **Explanation:** The algorithm clips the polygon sequentially against each plane (or edge) of the clipping window. The output vertex list from one clip plane is immediately passed as the input to the next clip plane, making it highly suitable for hardware or software pipelining.

***

### **From Image 3**

**3) Why do we need homogenous coordinate in computer graphic?**
*   a) Because it is useful for composite transformation.
*   b) Because it is useful for clipping algorithms.
*   c) Because it is useful for scaling transformation.
*   d) Because it is useful for filling algorithms.
> **Answer: a**
> **Explanation:** Homogeneous coordinates add an extra dimension (e.g., using a $3 \times 3$ matrix for 2D geometry) which allows translations to be represented as matrix multiplications rather than additions. This means multiple transformations (translation, rotation, scaling) can be multiplied together into a single composite transformation matrix.

**4) The inverse transformation of a rotation denoted by matrix A in 2D is**
*   a) a square matrix
*   b) a transpose matrix of A
*   c) an inverse matrix of A
*   d) All above answers are correct
> **Answer: d**
> **Explanation:** A 2D rotation matrix $A$ is a square ($2 \times 2$ or $3 \times 3$) orthogonal matrix. By mathematical definition, the inverse operation is represented by the inverse matrix ($A^{-1}$). Because it is orthogonal, its inverse is exactly equal to its transpose ($A^{-1} = A^T$). Therefore, all statements are true.

**5) Explain in details how this 2D transform matrix (in formula of row matrix) does**
$M = \begin{bmatrix} 0 & -1 & 0 \\ 1 & 0 & 0 \\ 1 & 1 & 1 \end{bmatrix}$
*   a) scaling, then translation
*   b) translation, then rotation
*   c) rotation, then scaling
*   d) rotation, then translation
> **Answer: d**
> **Explanation:** Following row-major convention, a point $P =[x, y, 1]$ is transformed by $P_{new} = P \times M$. 
> The matrix $M$ can be decomposed into a rotation matrix $R$ and a translation matrix $T$:
> $R = \begin{bmatrix} 0 & -1 & 0 \\ 1 & 0 & 0 \\ 0 & 0 & 1 \end{bmatrix}$ (Rotation)
> $T = \begin{bmatrix} 1 & 0 & 0 \\ 0 & 1 & 0 \\ 1 & 1 & 1 \end{bmatrix}$ (Translation)
> If you multiply $R \times T$, you get exactly matrix $M$. Because matrix multiplication associates left-to-right for row vectors, $P \times (R \times T) = (P \times R) \times T$. This means the rotation is applied first, followed by the translation.

**6) Which 2D transformations are affine transformations?**
*   a) rotation, scaling, shear, translation
*   b) scaling, shear, reflection, translation
*   c) translation, rotation, scaling, reflection
*   d) a), b), and c) are correct.
> **Answer: d**
> **Explanation:** Affine transformations include any combination of linear transformations (rotation, scaling, shear, reflection) and translations. They preserve collinearity and ratios of distances. All the individual transformations listed in options a, b, and c are valid affine transformations.

*(Note: Question 7 on Image 3 is partially cut off. However, based on standard computer graphics curriculum, the distinguishing factor the question is looking for is that Bresenham's algorithm utilizes only integer arithmetic, whereas the DDA algorithm requires floating-point operations.)*





Based on the clear portions of the provided images, here are the transcriptions of the questions along with their correct answers and brief explanations.

### **Hidden Surface Removal & Shading (Images 1, 2 & 4)**

**16) Which color system has the largest range of color expression?**
*   a) RGB
*   b) CMY
*   c) Grayscale
*   d) CIE
> **Answer: d**
> **Explanation:** The CIE color models (such as CIE 1931 XYZ) are mathematical models based on human perception and represent the entire gamut of human vision. RGB and CMY are device-dependent color spaces that can only reproduce a subset (a limited gamut) of visible colors.

**17) The following figure describes a result of one Hidden Surface Removal method. In the HSR result, we can see that there are some surfaces which can not be hidden correctly. Give the name of the HSR method that has been applied**
*(Image shows a cat wireframe and a resulting mesh where occluded internal/back geometry is incorrectly visible)*
*   a) Z-buffering
*   b) Back-face culling
*   c) Ray tracing
*   d) All above answers are incorrect.
> **Answer: b**
> **Explanation:** The object (a cat) is highly non-convex. Back-face culling only removes polygons facing away from the camera. If a non-convex/concave object is rendered using *only* back-face culling, polygons that face the camera but are physically occluded by other parts of the object (like the inside of a fold) will still be drawn, resulting in the messy, incorrect visibility seen in the image. Z-buffering and Ray tracing would render this correctly.

**18) What is the similar point between Phong shading and Gouraud shading?**
*   a) Two methods apply interpolation of intensity values.
*   b) Two methods apply interpolation of normal vectors.
*   c) Two methods calculate normal vector at each vertex firstly.
*   d) Two methods calculate normal vector at the center point of patch firstly.
> **Answer: c**
> **Explanation:** Both Gouraud and Phong shading rely on knowing the surface normals at the vertices of the polygons to create a smooth appearance. Gouraud calculates lighting at these vertices and interpolates the resulting *colors/intensities*, while Phong interpolates the *normal vectors* themselves across the polygon and calculates lighting per-pixel.

**19) To fill color in a 3D non-convex object, which method of HL/HSR can apply to solve this problem without a mismatch.**
*   a) Scan line method
*   b) Back-face culling method
*   c) Painter method
*   d) Ray tracing method
> **Answer: d**
> **Explanation:** For a non-convex object, back-face culling fails (as seen in Q17), and the Painter's method can easily fail if polygons intersect or have cyclic overlaps. While a Scan-line method with a Z-buffer works, standard basic Scan-line can be complex with self-occlusions without depth tracking. Ray tracing inherently and mathematically solves arbitrary visibility and self-occlusion perfectly by tracing light paths per pixel.

**20) In the Hidden Line / Hidden Surface Removal (HL/HSR) methods, which method returns incorrect result for one 3D completely convex object?**
*   a) Z-buffer
*   b) Back-face culling method
*   c) Painter method
*   d) All above answers are incorrect.
> **Answer: d**
> **Explanation:** For a *completely convex* object, there is no self-occlusion by front-facing polygons. Therefore, Back-face culling works perfectly, Painter's method works perfectly (sorting is unambiguous), and Z-buffering works perfectly. Since none of methods a, b, or c return an *incorrect* result, the answer is "All above answers are incorrect".

### **Projections & Pipelines (Images 2 & 3)**

**21) This projection technique does NOT have the direction of projection perpendicular to the viewing plane.**
*   a) Orthographic Parallel Projection
*   b) Axonometric Parallel Projection
*   c) Oblique Parallel Projection
*   d) All above answers are incorrect
> **Answer: c**
> **Explanation:** By definition, oblique projection lines intersect the projection (viewing) plane at an oblique angle (an angle other than 90 degrees). Orthographic and axonometric projections have parallel lines that are strictly perpendicular to the viewing plane.

**22) The order of processes to build one image of 3D object is that**
*   a) Cloud points -> Wireframe -> HL/HSR -> Shading -> 3D object image
*   b) Wireframe -> Cloud points -> HL/HSR -> Shading -> 3D object image
*   c) Wireframe -> Cloud points -> Shading -> HL/HSR -> 3D object image
*   d) Cloud points -> Wireframe -> Shading -> HL/HSR -> 3D object image
> **Answer: a**
> **Explanation:** In a standard 3D pipeline: First, geometry is defined by vertices (cloud points). These are connected to form polygons (wireframe). Then, hidden surfaces are removed (HL/HSR) so we only calculate light for what is visible. Finally, lighting and colors are applied (Shading) to produce the final 2D image.

### **Phong Illumination Model (Image 3)**

**23) Phong illumination and Phong shading can be explained as the following:**
*   a) Phong illumination is a model of illumination to calculate the intensity of reflection light.
*   b) Phong shading is an application of Phong illumination into a specific surface in 3D model
*   c) Phong shading utilizes an interpolation technique for surface shading in 3D computer graphic, where it utilizes Phong illumination model with the normal vector interpolated at each point in the 3D surface.
*   d) All above answers are correct
> **Answer: d**
> **Explanation:** All statements are accurate descriptions. Statement A defines the mathematical lighting model, Statement B describes its usage, and Statement C provides the exact technical definition of how Phong shading interpolates normals to evaluate the Phong lighting model per-pixel.

**24) Which portion of the Phong illumination model is responsible for giving spheres their bright white spots?**
*   a) Ambient
*   b) Diffuse
*   c) Specular
*   d) None of the above
> **Answer: c**
> **Explanation:** The specular reflection component models the shiny, mirror-like highlight (bright spots) seen on glossy surfaces when the viewing angle aligns with the angle of light reflection.

**25) Which of the following is NOT a vector needed for the Phong illumination model?**
*   a) Surface Normal
*   b) Direction to Viewer
*   c) Direction to the Center of object
*   d) Direction to Light
> **Answer: c**
> **Explanation:** The Phong reflection model equation uses the Light vector ($L$), Normal vector ($N$), Viewer/Camera vector ($V$), and the computed Reflection vector ($R$). The center of the object is irrelevant to per-pixel surface lighting calculations.

### **Advanced Rendering & Computer Vision (Images 6 & 7)**

**26) What is the main difference between traditional rendering (mesh-based) and 3D Gaussian Splatting?**
*   a) Traditional rendering uses Gaussians to represent surfaces
*   b) Gaussian Splatting represents scenes using Gaussian distributions instead of triangle meshes
*   c) Traditional rendering does not require geometric information
*   d) Gaussian Splatting only works with 2D images
> **Answer: b**
> **Explanation:** 3D Gaussian Splatting is a novel rendering technique that represents a 3D scene as a collection of 3D Gaussian ellipsoids (volumes) rather than explicit geometric surfaces like polygons/triangle meshes used in traditional rendering.

**27) In stereo correspondence, what does “disparity” represent?**
*   a) Image brightness
*   b) The positional difference of a point between two images
*   c) The color of a pixel
*   d) Image processing speed
> **Answer: b**
> **Explanation:** In computer vision (stereo vision), disparity refers to the shift in the 2D pixel coordinates of a single 3D world point when viewed from two slightly different camera positions (like left and right eyes). Depth is inversely proportional to this disparity.

**28) In Structure from Motion (SfM), what is the main objective?**
*   a) Increase image resolution
*   b) Reconstruct 3D structure and motion from multiple images
*   c) Reduce image noise
*   d) Compress image data
> **Answer: b**
> **Explanation:** SfM is a photogrammetry technique used to estimate 3D structures and the 3D camera poses (motion) simultaneously from a series of 2D overlapping images.

**29) In 3D Gaussian Splatting, which of the following is NOT a parameter describing a Gaussian?**
*   a) Mean (3D position)
*   b) Covariance
*   c) Opacity
*   d) Gradient descent rate
> **Answer: d**
> **Explanation:** An individual 3D Gaussian is defined by its Mean (position in space), Covariance (its shape/rotation/scale), Opacity (how opaque it is), and Color (usually via Spherical Harmonics). The "gradient descent rate" is a hyperparameter of the machine learning training algorithm, not an intrinsic property of the Gaussian itself.

**30) Why must the covariance matrix be positive semi-definite in 3D Gaussian Splatting?**
*   a) To increase computational speed
*   b) To ensure the Gaussian has a valid shape
*   c) To reduce memory usage
*   d) To increase image brightness
> **Answer: b**
> **Explanation:** In mathematics, a covariance matrix defines the shape of a multi-dimensional Gaussian distribution. It must be positive semi-definite to ensure that the distribution is physically valid (i.e., you cannot have negative variance/scale in any direction, which would break the ellipsoid shape).

***

### **Essay Questions (For Reference)**
1. For shading methods, compare Phong shading and Gouraud shading (concept, equations, performance) (2 marks)
2. For HLHSR methods, compare Z-buffering and Backface culling methods in concepts, performance (advantages and disadvantages of each method) (2 marks)
3. Write the list of vertices in the output of the clipping Sutherland-Hodgeman algorithm at each step when we clip the polygon defined by 6 vertices ($P_1, P_2, P_3, P_4, P_5, P_6$) with the viewport