#pragma once

// Pure geometry shared by the native layout helper and its boundary checks.
struct HSJRect { int x, y, width, height; };
struct HSJLayoutGeometry {
    int width, height;
    HSJRect first, second, right, table, title;
};
inline int HSJMin(int a, int b) { return a < b ? a : b; }
inline int HSJMax(int a, int b) { return a > b ? a : b; }

inline HSJLayoutGeometry HSJComputeLayout(int kind, int width, int height, int dpi) {
    HSJLayoutGeometry g = {};
    const int scaleDpi = HSJMax(96, dpi);
    const int gap = HSJMax(2, 4 * scaleDpi / 96);
    g.width = HSJMax(width, (kind == 0 ? 1180 : 820) * scaleDpi / 96);
    g.height = HSJMax(height, (kind == 0 ? 560 : 480) * scaleDpi / 96);
    const int top = (kind == 0 ? 32 : 48) * scaleDpi / 96;
    const int table = kind == 0 ? 112 * scaleDpi / 96 : 0;
    const int graphHeight = g.height - top - table;
    // Retain a square polar/orbit plot, but bound it by BOTH available dimensions.
    const int rightSize = HSJMin(g.width * 42 / 100, graphHeight);
    const int leftWidth = g.width - rightSize - gap;
    const int firstHeight = (graphHeight - gap) / 2;
    HSJRect first = {0, top, leftWidth, firstHeight};
    HSJRect second = {0, top + firstHeight + gap, leftWidth, graphHeight - firstHeight - gap};
    HSJRect right = {leftWidth + gap, top + (graphHeight - rightSize) / 2, rightSize, rightSize};
    HSJRect grid = {0, g.height - table, g.width, table};
    HSJRect title = {0, 0, g.width, top};
    g.first = first; g.second = second; g.right = right;
    g.table = grid; g.title = title;
    return g;
}
