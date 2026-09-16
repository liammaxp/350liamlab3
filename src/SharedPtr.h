#ifndef SHARED_PTR_HEADER
#define SHARED_PTR_HEADER
#include <cassert>
#include <utility>
// The following class name and funciton names are designed by ChatGPT.
class ControlBlockBase {
public:
    ControlBlockBase(){
        refcount = 1;
    }; // TODO: implement the default constructor.

    // dtor is virtual, so that we can call derived class's dtor from a ptr to this base class.
    virtual ~ControlBlockBase(){

    }; // TODO: implement the destructor.

    // pure virtual function; must be overriden by derived classes
    virtual void* managedAddress() = 0;

    // Delete copies, which also implicitly deletes moves.
    ControlBlockBase(const ControlBlockBase&) = delete;
    ControlBlockBase& operator=(const ControlBlockBase&) = delete;

    long increment()
    {
        assert(refcount > 0);
        refcount++;
        return refcount;
        // TODO: increment refcount by 1 and return result.
    }

    long decrement()
    {
        assert(refcount > 0);
        refcount--;
        return refcount;
        // TODO: decrement refcount by 1 and return result.
    }

    long refCount() const
    {
        return refcount;
        // TODO: just return the refcount.
    }

private:
    // TODO: add field(s) which both control block types need to have
    long refcount;
};


template <typename T>
class ControlBlock : public ControlBlockBase {
    private:
        T* ptr;
    public:
        ControlBlock(T* ptr){
            this->ptr = ptr;
        }
        ~ControlBlock(){
            delete ptr;
        }
        void* managedAddress() override{
            return ptr;
        }
};
template <typename T>
class SharedPtr{
    private:
        ControlBlockBase* controlBlock;
        T* storedPtr;
    public:
        SharedPtr(){
            storedPtr = nullptr;
            controlBlock = nullptr;
        }
        SharedPtr(T* ptr){
            storedPtr = ptr;
            controlBlock = new ControlBlock<T>(ptr);
        }
        ~SharedPtr() {
            if (controlBlock != nullptr) {
                if (controlBlock->decrement() == 0) {
                    delete controlBlock;
                }
            }
        }
        SharedPtr(const SharedPtr& other) {
            storedPtr = other.storedPtr;
            controlBlock = other.controlBlock;
            if (controlBlock != nullptr) {
                controlBlock->increment();
            }
        }
        SharedPtr(SharedPtr&& other){
            storedPtr = other.storedPtr;
            controlBlock = other.controlBlock;
            other.storedPtr = nullptr;
            other.controlBlock = nullptr;
        }
        void swap(SharedPtr<T>& other)
        {
            T* tempPtr = storedPtr;
            storedPtr = other.storedPtr;
            other.storedPtr  = tempPtr;
            ControlBlockBase* tempBlock = controlBlock;
            controlBlock = other.controlBlock;
            other.controlBlock = tempBlock;
        }
        SharedPtr& operator=(const SharedPtr& other)
        {
            SharedPtr temp(other);
            swap(temp);
            return *this;
        }
        SharedPtr& operator=(SharedPtr&& other)
        {
            if (this != &other) {
                reset();
                storedPtr = other.storedPtr;
                controlBlock = other.controlBlock;
                other.storedPtr = nullptr;
                other.controlBlock = nullptr;
            }
            return *this;
        }
        T& operator*() const
        {
            assert(storedPtr != nullptr);
            return *storedPtr;
        }
        T* operator->() const
        {
            assert(storedPtr != nullptr);
            return storedPtr;
        }
        T* get() const
        {
            return storedPtr;
        }
        bool operator==(const SharedPtr<T>& other) const
        {
            return storedPtr == other.storedPtr;
        }
        operator bool() const
        {
            return storedPtr != nullptr;
        }
        void reset()
        {
            SharedPtr<T> temp;
            swap(temp);
        }

        void reset(T* other)
        {
            if (storedPtr != other) {
                SharedPtr<T> temp(other);
                swap(temp);
            }
        }
        long useCount() const
        {
            if (controlBlock == nullptr) {
                return 0;
            }
            return controlBlock->refCount();
        }
};
template <typename T, typename... Args>
SharedPtr<T> makeSharedBasic(Args&&... args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
#endif
