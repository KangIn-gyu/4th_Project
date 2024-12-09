#pragma once
#pragma once
#include <mutex>
#include <iostream>

template<typename T>
class SingletonBase
{
public:
    inline static const std::shared_ptr<T>& GetInstance()
    {
        std::call_once(_initFlag, []()
            {
                T* prim = new T();
                _instance = std::shared_ptr<T>(prim, Deleter());
            });

        return _instance;
    }

    struct Deleter
    {
        void operator()(T* instance)
        {
            delete instance;
        }
    };

protected:
    SingletonBase() = default;
    virtual ~SingletonBase() = default;

private:
    static std::shared_ptr<T> _instance;
    static std::once_flag _initFlag;
    // Delete the copy constructor and assignment operator to prevent copies.
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;
};

template <typename T>
std::shared_ptr<T> SingletonBase<T>::_instance = std::shared_ptr<T>();

template <typename T>
std::once_flag SingletonBase<T>::_initFlag;

/**
@file      SingletonBase
@brief     Template C++ Singleton Base class
@author    SKYFISH
@date      JUNE.2023
@details
    - std::once_flag를 사용하여 일회성 초기화가 보장되는 싱글톤 패턴을 구현
    - 여러 스레드가 있는 경우에도 싱글톤 인스턴스의 초기화가 한 번만 발생하도록 하기 위해 std::call_once와 함께 사용

**/

/**
* \brief Singleton Pattern을 적용할 수 있는 템플릿 기본 클래스
* \tparam TClass Singleton Pattern을 적용하고 싶은 클래스
*/