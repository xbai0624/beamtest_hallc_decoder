#ifndef TRACKING_H
#define TRACKING_H

#include <unordered_map>
#include <vector>
#include <iomanip>
#include "tracking_struct.h"
#include "Cuts.h"

namespace tracking_dev {

    class TrackingUtility;
    class AbstractDetector;

#define LARGE_VALUE 999999999.

class Tracking
{
public:
    Tracking();
    ~Tracking();

    void AddDetector(int index, AbstractDetector*);
    void CompleteSetup();
    void FindTracks();
    void ClearPreviousEvent();

    // unit test
    void UnitTest();
    void Print(const std::vector<int> &v);
    void PrintHitStatus();
    void PrintLayerGroups();

    // getters
    bool GetBestTrack(double &xt, double &yt, double &xp, double &yp, double &chi);
    int GetNHitsonBestTrack(){return nhits_on_best_track;}
    const std::vector<int> &GetBestTrackLayerIndex(){return best_track_layer_index;}
    const std::vector<int> &GetBestTrackHitIndex(){return best_track_hit_index;}
    TrackingUtility* GetTrackingUtility() {return tracking_utility;}

private:
    void initHitStatus();
    void initLayerGroups();
    void loopAllLayerGroups();
    void nextLayerGroup(const std::vector<int> &group);
    void nextTrackCandidate(const std::vector<std::pair<int, int>> &combination);
    void nextTrackCandidate(const std::vector<point_t> &combination);
    void nextTrackCandidate(const std::vector<int> &layer_index, const std::vector<int> &hit_index);
    void scanCandidate(const std::vector<int> &nhit_by_layer,
            const std::vector<int> &layer_index,
            std::vector<int> &hit_comb);
    bool found_tracks_with_nlayer(int nlayer);

private:
    void getCombinationList(const std::vector<int> &layers, const int &m,
            std::vector<std::vector<int>>& res);

private:
    TrackingUtility *tracking_utility;
    Cuts *tracking_cuts;

    std::unordered_map<int, AbstractDetector*> detector; // layer_id <-> detector
    std::vector<int> layer_index; // vector of layer_id

    std::unordered_map<int, std::vector<bool>> hit_used; // layer_index <-> detector hit status

    int minimum_hits_on_track = 3;
    double chi2_cut = 10;

    int nhits_on_best_track;

    // all possible groups
    std::unordered_map<int, std::vector<std::vector<int>>> group_nlayer;

    // cache current working combination
    std::vector<int> current_layer_comb; // optional, as (xtrack, ytrack), (xptrack, yptrack) is enough
    std::vector<int> current_hit_comb;   // optional, as (xtrack, ytrack), (xptrack, yptrack) is enough

    // tracking result
    std::vector<int> best_track_layer_index; // optional, as (xtrack, ytrack), (xptrack, yptrack) is enough
    std::vector<int> best_track_hit_index;   // optional, as (xtrack, ytrack), (xptrack, yptrack) is enough
    double best_track_chi2ndf = LARGE_VALUE;
    double best_xtrack = LARGE_VALUE, best_ytrack = LARGE_VALUE;
    double best_xptrack = LARGE_VALUE, best_yptrack = LARGE_VALUE;
    //
    std::unordered_map<int, double> best_track_chi2ndf_by_nlayer;

    // debug
    //std::vector<point_t> best_hits_on_track;
};

};

#endif
