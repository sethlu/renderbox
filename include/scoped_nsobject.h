#ifndef RENDERBOX_SCOPEDNSOBJECT_H
#define RENDERBOX_SCOPEDNSOBJECT_H


// Based on: https://cs.chromium.org/chromium/src/base/mac/scoped_nsobject.h

namespace renderbox {

    template<typename NST>
    class scoped_nsprotocol {
    public:

        explicit scoped_nsprotocol(NST object = nil)
                : object_(object) {};

        scoped_nsprotocol(const scoped_nsprotocol<NST> &that)
                : object_(/*[*/that.object_/* retain]*/) {};

        ~scoped_nsprotocol() {
            // [object_ release];
        };

        void reset(NST object = nil) {
            // [object_ release];
            object_ = object;
        };

        scoped_nsprotocol &operator=(const scoped_nsprotocol<NST> &that) {
            reset(that.object_);
            return *this;
        };

        bool operator==(NST that) const {
            return object_ == that;
        };

        bool operator!=(NST that) const {
            return object_ != that;
        };

        operator NST() const {
            return object_;
        };

        NST get() const {
            return object_;
        };

        void swap(scoped_nsprotocol &that) {
            NST temp = that.object_;
            that.object_ = object_;
            object_ = temp;
        };

    private:

        NST object_;

    };

    template<typename NST>
    class scoped_nsobject : public scoped_nsprotocol<NST *> {
    public:

        explicit scoped_nsobject(NST *object = nil)
                : scoped_nsprotocol<NST *>(object) {};

        scoped_nsobject(const scoped_nsobject<NST> &that)
                : scoped_nsprotocol<NST *>(that) {};

        scoped_nsobject &operator=(const scoped_nsobject<NST> &that) {
            scoped_nsprotocol<NST *>::operator=(that);
            return *this;
        };

    };

    template<>
    class scoped_nsobject<id> : public scoped_nsprotocol<id> {
    public:

        explicit scoped_nsobject(id object = nil)
                : scoped_nsprotocol<id>(object) {};

        scoped_nsobject(const scoped_nsobject<id> &that)
                : scoped_nsprotocol<id>(that) {};

        scoped_nsobject &operator=(const scoped_nsobject<id> &that) {
            scoped_nsprotocol<id>::operator=(that);
            return *this;
        };

    };

}


#endif //RENDERBOX_SCOPEDNSOBJECT_H
