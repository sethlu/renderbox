#ifndef RENDERBOX_UPDATETRACKEDOBJECT_H
#define RENDERBOX_UPDATETRACKEDOBJECT_H


namespace renderbox {

    class VersionTrackedObject {
    public:

        typedef unsigned int version_type;

        void didUpdate() {
            version_++;
        }

        version_type getVersion() const {
            return version_;
        }

    private:

        version_type version_ = 0;

    };

}


#endif //RENDERBOX_UPDATETRACKEDOBJECT_H
