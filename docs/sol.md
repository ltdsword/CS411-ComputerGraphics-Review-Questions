



Based on the exam images and the provided image for Question 3, here are the detailed solutions for the three essay questions.

### **1. Compare Phong shading and Gouraud shading (Concept, Equations, Performance)**

**Concept:**
*   **Gouraud Shading (Intensity Interpolation):** This method calculates the full lighting model (like the Phong reflection model) only at the *vertices* of the polygon using the vertex normals. For the pixels inside the polygon, it linearly interpolates the resulting *color/intensity values* calculated at the vertices.
*   **Phong Shading (Normal Interpolation):** This method interpolates the *normal vectors* themselves across the face of the polygon. The full lighting model equation is then evaluated per-pixel (at every fragment) using the newly interpolated normal for that specific point.

**Equations:**
*   **Gouraud:** 
    1. Calculate lighting at vertex $V_1, V_2, V_3$ to get intensities $I_1, I_2, I_3$. (Using $I = I_{ambient} + I_{diffuse} + I_{specular}$)
    2. Interpolate intensity for a pixel $p$ inside the polygon: 
       $I_p = \alpha I_1 + \beta I_2 + \gamma I_3$ (where $\alpha, \beta, \gamma$ are barycentric coordinates, or linearly interpolated along scanlines).
*   **Phong:**
    1. Interpolate normal vectors for a pixel $p$: 
       $N_p = \alpha N_1 + \beta N_2 + \gamma N_3$ (and then normalize $N_p$).
    2. Calculate lighting equation per-pixel using $N_p$:
       $I_p = k_a i_a + k_d i_d (\vec{L} \cdot \vec{N}_p) + k_s i_s (\vec{R} \cdot \vec{V})^n$

**Performance:**
*   **Gouraud:** Faster and less computationally expensive because the heavy lighting equations are only computed at the vertices. However, it can suffer from visual artifacts like the "Mach band" effect and can entirely miss specular highlights (bright shiny spots) if they happen to fall in the middle of a large polygon and not near a vertex.
*   **Phong:** Slower and much more computationally intensive because vector normalization and lighting calculations are done for every single pixel. However, it yields significantly higher quality, smoother shading, and accurately captures specular highlights regardless of polygon size.

---

### **2. Compare Z-buffering and Backface culling methods (Concepts, Performance)**

**Concepts:**
*   **Backface Culling:** An *object-space* operation typically performed early in the rendering pipeline. It determines whether a polygon is facing toward or away from the camera. If the dot product of the polygon's normal vector and the camera's view vector is positive (facing away), the entire polygon is discarded before rasterization.
*   **Z-buffering (Depth Buffering):** An *image-space* operation performed at the end of the pipeline during rasterization. It uses a 2D array (the Z-buffer) of the same resolution as the screen to store the depth (Z-value) of the closest pixel processed so far. When drawing a new pixel, its depth is compared to the Z-buffer. If it is closer to the camera, it overwrites the screen color and updates the Z-buffer; otherwise, it is ignored.

**Performance (Advantages & Disadvantages):**
*   **Backface Culling:**
    *   *Advantages:* Highly performant for closed, convex objects. It halves the number of polygons that need to be processed (transformed, shaded, rasterized), saving significant computational time. Requires no extra memory.
    *   *Disadvantages:* It cannot handle occlusions between *different* objects, nor can it handle self-occlusions in concave objects. It is only a partial Hidden Surface Removal (HSR) solution.
*   **Z-buffering:**
    *   *Advantages:* Extremely robust. It accurately handles complex scenes with multiple intersecting objects, concave shapes, and arbitrary drawing orders. It is simple to implement in hardware.
    *   *Disadvantages:* Requires significant extra memory to store the depth buffer. It can suffer from "overdraw" (wasting processing power shading pixels that are later overwritten by closer objects) if the scene isn't pre-sorted front-to-back. Can have precision issues (Z-fighting) for objects very close together.

---

### **3. Sutherland-Hodgman Polygon Clipping**

First, let's establish a clear naming convention for the intersection points between the polygon edges and the viewport lines $\{L_1, L_2, L_3, L_4\}$.

**Assigned Intersection Names:**
*   $T_{56}$: Intersection of edge $P_5-P_6$ with Top line ($L_1$)
*   $T_{61}$: Intersection of edge $P_6-P_1$ with Top line ($L_1$)
*   $L_{61}$: Intersection of edge $P_6-P_1$ with Left line ($L_2$)
*   $L_{12}$: Intersection of edge $P_1-P_2$ with Left line ($L_2$)
*   $B_{12}$: Intersection of edge $P_1-P_2$ with Bottom line ($L_3$)
*   $B_{34}$: Intersection of edge $P_3-P_4$ with Bottom line ($L_3$)
*   $R_{45}$: Intersection of edge $P_4-P_5$ with Right line ($L_4$)
*   $R_{56}$: Intersection of edge $P_5-P_6$ with Right line ($L_4$)

Here is the step-by-step output vertex list of the Sutherland-Hodgman algorithm. We will apply the clipping planes in the order: **Top ($L_1$), Left ($L_2$), Bottom ($L_3$), Right ($L_4$)**.

**Initial List:** $\{P_1, P_2, P_3, P_4, P_5, P_6\}$

*   **Step 1: Clip against $L_1$ (Top)** - *Inside is below $L_1$*
    *   $P_1 \rightarrow P_2$ (In $\rightarrow$ In): output $P_2$
    *   $P_2 \rightarrow P_3$ (In $\rightarrow$ In): output $P_3$
    *   $P_3 \rightarrow P_4$ (In $\rightarrow$ In): output $P_4$
    *   $P_4 \rightarrow P_5$ (In $\rightarrow$ In): output $P_5$
    *   $P_5 \rightarrow P_6$ (In $\rightarrow$ Out): output $T_{56}$
    *   $P_6 \rightarrow P_1$ (Out $\rightarrow$ In): output $T_{61}, P_1$
    *   **Output List 1:** $\{P_2, P_3, P_4, P_5, T_{56}, T_{61}, P_1\}$

*   **Step 2: Clip against $L_2$ (Left)** - *Inside is right of $L_2$*
    *   $P_2 \rightarrow P_3$ (In $\rightarrow$ In): output $P_3$
    *   $P_3 \rightarrow P_4$ (In $\rightarrow$ In): output $P_4$
    *   $P_4 \rightarrow P_5$ (In $\rightarrow$ In): output $P_5$
    *   $P_5 \rightarrow T_{56}$ (In $\rightarrow$ In): output $T_{56}$
    *   $T_{56} \rightarrow T_{61}$ (In $\rightarrow$ In): output $T_{61}$
    *   $T_{61} \rightarrow P_1$ (In $\rightarrow$ Out): output $L_{61}$
    *   $P_1 \rightarrow P_2$ (Out $\rightarrow$ In): output $L_{12}, P_2$
    *   **Output List 2:** $\{P_3, P_4, P_5, T_{56}, T_{61}, L_{61}, L_{12}, P_2\}$

*   **Step 3: Clip against $L_3$ (Bottom)** - *Inside is above $L_3$*
    *   $P_3 \rightarrow P_4$ (Out $\rightarrow$ In): output $B_{34}, P_4$
    *   $P_4 \rightarrow P_5$ (In $\rightarrow$ In): output $P_5$
    *   $P_5 \rightarrow T_{56}$ (In $\rightarrow$ In): output $T_{56}$
    *   $T_{56} \rightarrow T_{61}$ (In $\rightarrow$ In): output $T_{61}$
    *   $T_{61} \rightarrow L_{61}$ (In $\rightarrow$ In): output $L_{61}$
    *   $L_{61} \rightarrow L_{12}$ (In $\rightarrow$ In): output $L_{12}$
    *   $L_{12} \rightarrow P_2$ (In $\rightarrow$ Out): output $B_{12}$
    *   $P_2 \rightarrow P_3$ (Out $\rightarrow$ Out): output nothing
    *   **Output List 3:** $\{B_{34}, P_4, P_5, T_{56}, T_{61}, L_{61}, L_{12}, B_{12}\}$

*   **Step 4: Clip against $L_4$ (Right)** - *Inside is left of $L_4$*
    *   $B_{34} \rightarrow P_4$ (In $\rightarrow$ In): output $P_4$
    *   $P_4 \rightarrow P_5$ (In $\rightarrow$ Out): output $R_{45}$
    *   $P_5 \rightarrow T_{56}$ (Out $\rightarrow$ In): output $R_{56}, T_{56}$
    *   $T_{56} \rightarrow T_{61}$ (In $\rightarrow$ In): output $T_{61}$
    *   $T_{61} \rightarrow L_{61}$ (In $\rightarrow$ In): output $L_{61}$
    *   $L_{61} \rightarrow L_{12}$ (In $\rightarrow$ In): output $L_{12}$
    *   $L_{12} \rightarrow B_{12}$ (In $\rightarrow$ In): output $B_{12}$
    *   $B_{12} \rightarrow B_{34}$ (In $\rightarrow$ In): output $B_{34}$
    *   **Final Output List:** **$\{P_4, R_{45}, R_{56}, T_{56}, T_{61}, L_{61}, L_{12}, B_{12}, B_{34}\}$**