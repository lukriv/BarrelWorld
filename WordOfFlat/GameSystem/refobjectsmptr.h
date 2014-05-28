#ifndef __REFERENCED_OBJECT_SMART_POINTER_H__
#define __REFERENCED_OBJECT_SMART_POINTER_H__

#include "refobject.h"

// to prevent calling addRef and Release methods through the smart pointer
template <class T> class _NoAddRefRelease : public T
{
private:
    _NoAddRefRelease(); // Make compiler happy
    virtual void addRef() = 0;
    virtual wxInt32 release() = 0;   
};

template <class T> class RefObjSmPtr {
private:
    T *m_pIF;
public:
    RefObjSmPtr(): m_pIF(nullptr) {}
    
	~RefObjSmPtr()
	{
        Release();
    }
	
    RefObjSmPtr(const RefObjSmPtr &orig)
    {
        m_pIF = orig.m_pIF;
        if (m_pIF) {
            m_pIF->addRef();
        }
    }
	
    RefObjSmPtr(T* p) : m_pIF(p) {
        if (m_pIF) {
            m_pIF->addRef();
        }
    }
    
	RefObjSmPtr & operator= (const RefObjSmPtr &rhs)
    {
        return operator=(rhs.m_pIF);
    }
	
    RefObjSmPtr & operator=(const T *p)
    {
        if (*this != p) {
            Release();
            m_pIF = const_cast<T*>(p);
            if (m_pIF) {
                m_pIF->addRef();
            }
        }
        return *this;
    }
	
    inline void Release()
    {
        if (0 != m_pIF)
        {
            m_pIF->release();
            m_pIF = 0;
        }
    }
	
    _NoAddRefRelease<T>* operator->() const
    {
        return (_NoAddRefRelease<T>*)m_pIF;
    }
	
    void Zap()
    {
        m_pIF = 0;
    }
    
	operator T*() const
    {
        return m_pIF;
    }
	
    T* In() const
    {
        return m_pIF;
    }
	
    T*& Ref()
    {
        return m_pIF;
    }
	
    T* const & Ref() const
    {
        return m_pIF;
    }
    T** Out()
    {
        Release();
        return &m_pIF;
    }
	
    T*& OutRef()
    {
        return *Out();
    }
    
	void** OutVoid()
    {       
        return (void**)Out();
    }
	
    void Attach(T* p)
    {
        if (p!=m_pIF) {
            Release();
            m_pIF = p;
        }
    }
	
    T *Detach()
    {
        T *retIF = m_pIF;
        m_pIF = 0;
        return retIF;
    }
	
    bool IsEmpty() const
    {
        return m_pIF==0?true:false;
    }
};

#endif //__REFERENCED_OBJECT_SMART_POINTER_H__