Continuing from your transcription, here are the detailed solutions for the three essay questions from your Advanced Program in Computer Science exam.

---

## 1. Comparison of Shading Methods
| Feature | **Gouraud Shading** | **Phong Shading** |
| :--- | :--- | :--- |
| **Basic Concept** | Interpolates **color (intensity)** across the polygon surface using the calculated colors at the vertices. | Interpolates the **surface normal vectors** across the polygon and applies the illumination model at every pixel. |
| **Equation (Linear)** | $C_p = (1-t)C_1 + tC_2$ (where $C$ is color/intensity). | $N_p = \frac{(1-t)N_1 + tN_2}{\|(1-t)N_1 + tN_2\|}$ (where $N$ is the normal vector). |
| **Performance** | Fast and computationally inexpensive; requires only one illumination calculation per vertex. | Slower and more expensive; requires an illumination calculation for every pixel. |
| **Visual Quality** | Suffers from "Mach banding" and often fails to render specular highlights if they fall in the middle of a polygon. | Produces much smoother surfaces and accurate, sharp specular highlights. |

---

## 2. Comparison of HL/HSR Methods
### **Z-buffering (Image-Space)**
*   **Concept:** Uses a 2D array (depth buffer) to store the $z$-coordinate of the object closest to the viewer for every pixel.
*   **Advantages:** Handles any geometric complexity (non-convex objects, intersecting surfaces) and is easy to implement in hardware.
*   **Disadvantages:** Requires a significant amount of memory for the $z$-buffer and can suffer from precision issues (z-fighting).

### **Back-face Culling (Object-Space)**
*   **Concept:** Identifies and discards polygons that face away from the camera by checking if the dot product of the surface normal and the view vector is negative ($N \cdot V < 0$).
*   **Advantages:** Extremely fast; it reduces the number of polygons to be processed by approximately **50%** before they reach the rendering pipeline.
*   **Disadvantages:** Cannot handle visibility between multiple objects or resolve hidden surfaces within a single non-convex object.

---

## 3. Sutherland-Hodgeman Clipping Process
Based on the provided diagram, the clipping order is: **Top ($L_1$), Left ($L_2$), Bottom ($L_3$), Right ($L_4$)**.

**Intersection Labels:**
*   **$I_{T1}, I_{T2}$:** Intersections on the Top boundary ($L_1$).
*   **$I_{L1}, I_{L2}$:** Intersections on the Left boundary ($L_2$).
*   **$I_{B1}, I_{B2}$:** Intersections on the Bottom boundary ($L_3$).
*   **$I_{R1}, I_{R2}$:** Intersections on the Right boundary ($L_4$).
*   **$C_{BL}$:** The Bottom-Left corner vertex of the viewport.

### **Step-by-Step Vertex Lists:**
1.  **Initial Input:** $\{P_1, P_2, P_3, P_4, P_5, P_6\}$
2.  **After Clipping Top ($L_1$):**
    *   $P_6$ is outside. Edge $P_5 \to P_6$ produces $I_{T1}$; edge $P_6 \to P_1$ produces $I_{T2}$ then $P_1$.
    *   **List:** $\{P_1, P_2, P_3, P_4, P_5, I_{T1}, I_{T2}\}$
3.  **After Clipping Left ($L_2$):**
    *   $P_1, P_2$ are outside. Edge $I_{T2} \to P_1$ produces $I_{L2}$; edge $P_2 \to P_3$ produces $I_{L1}$ then $P_3$.
    *   **List:** $\{P_3, P_4, P_5, I_{T1}, I_{T2}, I_{L2}, I_{L1}\}$
4.  **After Clipping Bottom ($L_3$):**
    *   $P_3$ and $I_{L1}$ are outside. Edge $I_{L1} \to P_3$ is ignored; edge $P_3 \to P_4$ produces $I_{B1}$ then $P_4$; edge $I_{L2} \to I_{L1}$ produces $C_{BL}$.
    *   **List:** $\{P_4, P_5, I_{T1}, I_{T2}, I_{L2}, C_{BL}, I_{B1}\}$
5.  **After Clipping Right ($L_4$):**
    *   $P_5$ is outside. Edge $P_4 \to P_5$ produces $I_{R1}$; edge $P_5 \to I_{T1}$ produces $I_{R2}$ then $I_{T1}$.
    *   **Final Output:** $\{I_{T1}, I_{T2}, I_{L2}, C_{BL}, I_{B1}, P_4, I_{R1}, I_{R2}\}$