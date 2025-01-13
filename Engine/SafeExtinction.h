#pragma once

template <typename T>
concept HasRelease = requires(T t) 
{
    { t.Release() } -> std::same_as<void>;  //검사 표현식 Cpp 20버전 스타일
};

template <typename T>
concept ContainerType = requires(T t)
{ // 일반적인 컨테이너 확인
    t.begin();
    t.end();
};

template <typename T>
concept KeyValueContainer = requires(T t)
{ 
    typename T::key_type;
    typename T::mapped_type;
    requires std::same_as<T, std::unordered_map<typename T::key_type, typename T::mapped_type>> ||
             std::same_as<T, std::map<typename T::key_type, typename T::mapped_type>>;
};

template <typename T>
concept ElementContainer = requires(T t)
{ // T<value> 타입이 포인터인지 확인
    requires std::is_pointer_v<typename T::value_type>;
};

class SafeExtinction // 셀프 소멸
{
public:
    template <HasRelease T>
    static void SAFE_RELEASE(T* p)
    {
        if (nullptr != p)
        {
            p->Release();
            p = nullptr;
        }
    }

    template <typename T> requires std::is_pointer_v<T>
    static void SAFE_DELETE(T p) 
    {
        if (nullptr != p)
        {
            delete p;
            p = nullptr;
        }
    }

    // 튜플 같은 자료형은 못 지움
    // 벡터<value*> 이거나 맵 종류만 삭제해줌.
    template <ContainerType T> 
    static void SAFE_CLEAR_CONTAINER(T& container)
    {
        for (auto& element : container)
        {
            if constexpr (ElementContainer<T>)
            {   // 포인터 타입인 경우 삭제
                SAFE_DELETE(element);
            }
            else if constexpr (KeyValueContainer<T>)
            {   // KeyValueContainer일 경우 pair.second 처리
                if constexpr (std::is_pointer_v<typename T::mapped_type>)  // mapped_type이 포인터인 경우
                {
                    SAFE_DELETE(element.second);  // 포인터 삭제
                }
                else
                {
                    SAFE_CLEAR_CONTAINER(element.second);  // 재귀적으로 처리
                }
            }
            else if constexpr (ContainerType<decltype(element)>)
            {   // 중첩된 컨테이너일 경우 재귀 호출
                SAFE_CLEAR_CONTAINER(element);
            }
        }
        container.clear(); // 컨테이너 비우기
    }
};