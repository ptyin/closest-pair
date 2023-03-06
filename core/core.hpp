#ifndef QT_CLOSEST_PAIR_FUNC_HPP
#define QT_CLOSEST_PAIR_FUNC_HPP

#include <algorithm>
#include <vector>
#include <functional>
#include <cmath>

using namespace std;

static pair<int, int> _run(const vector<pair<int, int>> &points) {
    auto get_dis = [](auto &a, auto &b) {
        int dx = a.first - b.first;
        int dy = a.second - b.second;
        return sqrt((long long) dx * dx + (long long) dy * dy);
    };
    size_t n = points.size();
    if (n <= 1)
        return {-1, -1};

    int L = floor(1e9 + 7), R = 0;
    for (auto p: points) {
        L = min(L, p.first);
        R = max(R, p.first);
    }
    int mid = (L + R) / 2;


    vector<pair<int, int>> left, right;
    vector<int> id1, id2;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].first <= mid && left.size() < n / 2) {
            left.push_back(points[i]);
            id1.push_back(i);
        } else {
            right.push_back(points[i]);
            id2.push_back(i);
        }
    }
    pair<int, int> ans1 = _run(left);
    pair<int, int> ans2 = _run(right);
    pair<int, int> ans = make_pair(0, 0);
    double d = 1e18;
    if (ans1.first != -1 && ans1.second != -1) {
        double d1 = get_dis(left[ans1.first], left[ans1.second]);
        if (d1 < d) {
            d = d1;
            ans = make_pair(id1[ans1.first], id1[ans1.second]);
        }
    }
    if (ans2.first != -1 && ans2.second != -1) {
        double d2 = get_dis(right[ans2.first], right[ans2.second]);
        if (d2 < d) {
            d = d2;
            ans = make_pair(id2[ans2.first], id2[ans2.second]);
        }
    }

    int head = 0, tail = 0;
    for (int i = 0; i < left.size(); i++) {
        int y1 = left[i].second;
        while (head < right.size() && right[head].second - y1 < d)
            head++;
        while (tail < head && y1 - right[tail].second > d)
            tail++;
        for (int j = tail; j < head; j++) {
            double d3 = get_dis(left[i], right[j]);
            if (d3 < d) {
                d = d3;
                ans = make_pair(id1[i], id2[j]);
            }
        }
    }
    return ans;
}

template<template<typename, typename...> class C, typename P, typename X, typename Y, typename... Args>
pair<int, int> run(C<P, Args...> &points, X &x, Y &y) {
    sort(begin(points), end(points), [&y](auto &a, auto &b) { return y(a) < y(b); });
    vector<pair<int, int>> pointsStd(points.size());
    transform(cbegin(points), cend(points), pointsStd.begin(), [&x, &y](const auto &p){
        return pair<int, int>(x(p), y(p));
    });
    return _run(pointsStd);
}

#endif //QT_CLOSEST_PAIR_FUNC_HPP