#ifndef RENDERBOX_KEYFRAMETRACK_H
#define RENDERBOX_KEYFRAMETRACK_H


#include <vector>

#include "Vector.h"
#include "logging.h"


namespace renderbox {

    struct KeyframeTrack {
    public:

        virtual ~KeyframeTrack();

        std::vector<float> times;

    };

    template<typename V>
    class TypedKeyframeTrack : public KeyframeTrack {
    public:

        std::vector<V> values;

        virtual V interpolate(float t);

    };

    template<typename V>
    V TypedKeyframeTrack<V>::interpolate(float t) {
        auto i = std::lower_bound(times.begin(), times.end(), t) - times.begin();
        auto n = values.size();

        if (i == 0) {
            return values[0];
        } else if (i == n - 1 && t >= times[i]) {
            return values[n - 1];
        }
        auto w = (t - times[i - 1]) / (times[i] - times[i - 1]);
        return (1 - w) * values[i - 1] + w * values[i];
    }

    typedef TypedKeyframeTrack<vec3> Vector3KeyframeTrack;

}


#endif //RENDERBOX_KEYFRAMETRACK_H
