#ifndef CUBE_GEOMETRY_H
#define CUBE_GEOMETRY_H

// Cube face layout (from user's diagram):
//         [5̄]         (top, inverted)
// [2][3][1][4]        (back-left-front-right)
//         [6̄]         (bottom, inverted)
//
// Face indices in code:
// 0 = front (face 1)
// 1 = back (face 2)
// 2 = left (face 3)
// 3 = right (face 4)
// 4 = top (face 5, inverted)
// 5 = bottom (face 6, inverted)
//
// Horizontal ring: back(1) ← left(2) ← front(0) ← right(3) ← back(1)
// Top: y=0 near front (top edge connects to front's top edge)
// Bottom: y=7 near front (bottom edge connects to front's bottom edge)

struct CubePos {
    int face;  // 0-5
    int x, y;  // 0-7
};

// Get neighbor position when moving in a direction
// Returns new position after crossing edge (if needed)
inline CubePos getNeighbor(int face, int x, int y, int dx, int dy) {
    const int W = 8;
    const int H = 8;

    int newX = x + dx;
    int newY = y + dy;

    // Check if still on same face
    if (newX >= 0 && newX < W && newY >= 0 && newY < H) {
        return {face, newX, newY};
    }

    CubePos result = {face, newX, newY};

    // Coordinate system: y=0 is TOP of screen, y=7 is BOTTOM
    // So y < 0 means going UP (to top face), y >= H means going DOWN (to bottom face)
    // Top and Bottom are rotated 180° relative to front

    if (face == 0) { // FRONT
        if (newX < 0) { result = {2, W-1, y}; }           // → left
        else if (newX >= W) { result = {3, 0, y}; }       // → right
        else if (newY < 0) { result = {4, W-1-x, 0}; }    // → top (y=0 is near front)
        else if (newY >= H) { result = {5, W-1-x, H-1}; } // → bottom (y=7 is near front)
    }
    else if (face == 2) { // LEFT
        if (newX < 0) { result = {1, W-1, y}; }           // → back
        else if (newX >= W) { result = {0, 0, y}; }       // → front
        else if (newY < 0) { result = {4, W-1, H-1-x}; }  // → top (left x=0→back→y=7, x=7→front→y=0)
        else if (newY >= H) { result = {5, W-1, x}; }     // → bottom (left x=0→back→y=0, x=7→front→y=7)
    }
    else if (face == 1) { // BACK
        if (newX < 0) { result = {3, W-1, y}; }           // → right
        else if (newX >= W) { result = {2, 0, y}; }       // → left
        else if (newY < 0) { result = {4, x, H-1}; }      // → top (y=7 is near back)
        else if (newY >= H) { result = {5, x, 0}; }       // → bottom (y=0 is near back)
    }
    else if (face == 3) { // RIGHT
        if (newX < 0) { result = {0, W-1, y}; }           // → front
        else if (newX >= W) { result = {1, 0, y}; }       // → back
        else if (newY < 0) { result = {4, 0, x}; }        // → top (right x=0→front→y=0, x=7→back→y=7)
        else if (newY >= H) { result = {5, 0, H-1-x}; }   // → bottom (right x=0→front→y=7, x=7→back→y=0)
    }
    else if (face == 4) { // TOP (180° rotated: y=0 near front, y=7 near back, x=0 near right, x=7 near left)
        if (newX < 0) { result = {3, y, 0}; }             // → right (top y=0→front→right x=0, y=7→back→right x=7)
        else if (newX >= W) { result = {2, H-1-y, 0}; }   // → left (top y=0→front→left x=7, y=7→back→left x=0)
        else if (newY < 0) { result = {0, W-1-x, 0}; }    // → front (y=0 is near front)
        else if (newY >= H) { result = {1, x, 0}; }       // → back (y=7 is near back)
    }
    else if (face == 5) { // BOTTOM (y=7 near front, y=0 near back, x=0 near right, x=7 near left)
        if (newX < 0) { result = {3, H-1-y, H-1}; }       // → right (bottom y=7→front→right x=0, bottom y=0→back→right x=7)
        else if (newX >= W) { result = {2, y, H-1}; }     // → left (bottom y=7→front→left x=7, bottom y=0→back→left x=0)
        else if (newY < 0) { result = {1, x, H-1}; }      // → back (y=0 is near back)
        else if (newY >= H) { result = {0, W-1-x, H-1}; } // → front (y=7 is near front)
    }

    return result;
}

// Move position in direction, handling face transitions
inline void moveOnCube(CubePos& pos, int dx, int dy) {
    pos = getNeighbor(pos.face, pos.x, pos.y, dx, dy);
}

// Get all 4 neighbors of a position
inline void getNeighbors(const CubePos& pos, CubePos neighbors[4]) {
    neighbors[0] = getNeighbor(pos.face, pos.x, pos.y, 1, 0);   // right
    neighbors[1] = getNeighbor(pos.face, pos.x, pos.y, -1, 0);  // left
    neighbors[2] = getNeighbor(pos.face, pos.x, pos.y, 0, 1);   // up
    neighbors[3] = getNeighbor(pos.face, pos.x, pos.y, 0, -1);  // down
}

// Calculate "global" distance between two positions on cube surface
inline float cubeDistance(const CubePos& a, const CubePos& b) {
    if (a.face == b.face) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return sqrt(dx*dx + dy*dy);
    }
    // Simplified: if on different faces, estimate distance
    return 8.0f + abs(a.x - b.x) + abs(a.y - b.y);
}

#endif
