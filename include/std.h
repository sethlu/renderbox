#ifndef RENDERBOX_STD_H
#define RENDERBOX_STD_H

namespace std {

    template<typename T1, typename T2>
    struct hash<std::pair<T1 *, T2 *>> {
    public:
        size_t operator()(std::pair<T1 *, T2 *> const &x) const {
            size_t h = reinterpret_cast<size_t>(x.first) + reinterpret_cast<size_t>(x.second);
            return h;
        }
    };

}

#endif //RENDERBOX_STD_H
