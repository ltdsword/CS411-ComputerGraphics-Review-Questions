


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