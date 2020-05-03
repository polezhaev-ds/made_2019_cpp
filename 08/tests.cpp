//
// Created by admin2 on 17.11.2019.
//

#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <iterator>
#include <cstdint>
#include <string>
#include "catch2/catch.hpp"
#include "Vector.h"

template <class T>
class DummyClass {
public:
    explicit DummyClass(const T& value = 0):
        value_(value),
        moves_count_(0),
        is_empty_(false)
    {
    }

    DummyClass(const DummyClass& obj) = default;

    DummyClass(DummyClass&& obj) noexcept:
        value_(std::move(obj.value_)),
        moves_count_(obj.moves_count_ + 1),
        is_empty_(false)
    {
        obj.is_empty_ = true;
    }

    DummyClass& operator = (const DummyClass& obj) = default;

    DummyClass& operator = (DummyClass&& obj) noexcept {
        value_ = std::move(obj.value_);
        moves_count_ = obj.moves_count_ + 1;
        obj.is_empty_ = true;
    }

    [[nodiscard]] std::size_t moves_count() const {
        return moves_count_;
    }

    [[nodiscard]] bool is_empty() const {
        return is_empty_;
    }

    [[nodiscard]] const T& value() const {
        return value_;
    }

private:
    T value_;
    std::size_t moves_count_;
    bool is_empty_;
};

template <class Iterator>
void TestArrowOperatorForIterator() {
    using size_type = typename Iterator::size_type;
    using pair = typename Iterator::value_type;

    const size_type size = 3;
    pair* buffer = new pair[size] { {1, 'a'}, {2, 'b'}, {3, 'c'} };

    Iterator it(buffer);
    CHECK( (it -> first == 1 && it -> second == 'a' &&
            (it + 1) -> first == 2 && (it + 1) -> second == 'b' &&
            (it + 2) -> first == 3 && (it + 2) -> second == 'c') );

    delete[] buffer;
}

template <class T>
bool AreComparisonOperatorsCorrectForDifferentIterators(const Iterator<T>& lessIt, const Iterator<T>& greaterIt) {
    return lessIt < greaterIt && greaterIt > lessIt && lessIt <= greaterIt && greaterIt >= lessIt &&
        lessIt != greaterIt && !(lessIt == greaterIt)  && !(lessIt > greaterIt) && !(lessIt >= greaterIt);
}

template <class T>
bool AreComparisonOperatorsCorrectForEqualIterators(const Iterator<T>& it1, const Iterator<T>& it2) {
    return it1 == it2 && !(it1 != it2) && !(it1 < it2) && !(it1 > it2) && it1 <= it2 && it1 >= it2;
}

template <class Iterator>
void TestAdditionAndSubtractionOperatorsForIterator() {
    using T = typename Iterator::value_type;
    using size_type = typename Iterator::size_type;

    const size_type size = 5;
    T* buffer = new T[size];

    Iterator it0(buffer);
    Iterator it1 = it0 + 1;
    Iterator it2 = it0 + 2;
    Iterator it3 = it0 + 3;
    Iterator it4 = it3 + 1;
    CHECK( (it0.data_pointer() == buffer && it1.data_pointer() == buffer + 1 &&
            it2.data_pointer() == buffer + 2 && it3.data_pointer() == buffer + 3 &&
            it4.data_pointer() == buffer + 4) );
    CHECK( (it4 - it3 == 1 && it3 - it4 == -1 && it4 - it2 == 2 && it2 - it4 == -2 &&
            it4 - it1 == 3 && it1 - it4 == -3) );
    CHECK( (it4 - 1 == it3 && it4 - 2 == it2 && it4 - 3 == it1 && it3 - 1 == it2 && it3 - 2 == it1) );

    delete[] buffer;
}

template <class Iterator, class T = typename Iterator::value_type>
void TestAIndexingOperatorsForIterator(std::initializer_list<T> init) {
    using index_type = typename Iterator::difference_type;

    std::size_t size = init.size();
    T *buffer = new T[size];
    auto init_it = init.begin();
    for (index_type i = 0; i < size; ++i, ++init_it)
        buffer[i] = *init_it;

    bool is_correct = true;
    Iterator it(buffer);
    init_it = init.begin();
    for (index_type i = 0; i < size; ++i, ++init_it)
        if (it[i] != *init_it) {
            is_correct = false;
            break;
        }
    if (is_correct) {
        Iterator it2(buffer + size - 1);
        init_it = init.begin();
        for (index_type i = -size + 1; i <= 0; ++i, ++init_it)
            if (it2[i] != *init_it) {
                is_correct = false;
                break;
            }
    }
    CHECK( is_correct );

    delete[] buffer;
}

template <class T, class size_type = typename Vector<T>::size_type>
void TestSizeBasedVectorConstructors(size_type size, const T& value) {
    Vector<T> vec1(size);
    Vector<T> vec2(size, value);
    CHECK( vec1.size() == size );
    CHECK( vec1.capacity() >= size );
    CHECK( vec2.size() == size );
    CHECK( vec2.capacity() >= size );

    T default_value = T();
    bool is_correct = true;
    for (size_type i = 0; i < size; ++i)
        if (vec1[i] != default_value || vec2[i] != value ) {
            is_correct = false;
            break;
        }
    CHECK( is_correct );
}

template <class T>
void TestVectorConstructorForInitializerList(const std::initializer_list<T>& init) {
    using size_type = typename Vector<T>::size_type;

    Vector<T> vec(init);

    CHECK( vec.size() == init.size() );
    CHECK( vec.capacity() >= init.size() );

    bool is_correct = true;
    auto it = init.begin();
    for (size_type i = 0; i < vec.size(); ++i, ++it)
        if (vec[i] != *it) {
            is_correct = false;
            break;
        }

    CHECK( is_correct );
}

template <class T>
void TestVectorCopyAndMoveConstructors(const std::initializer_list<T>& init) {
    Vector<T> vec(init);
    CHECK( vec.size() == init.size() );
    CHECK( vec.capacity() >= init.size() );

    Vector<T> vec1 = vec;
    if (init.size() > 0) {
        CHECK((vec.buffer_pointer() != vec1.buffer_pointer()));
    } else {
        CHECK( vec1.size() == 0 );
        CHECK( vec1.capacity() >= 0 );
    }
    CHECK(vec1 == vec);

    Vector<T> vec2 = std::move(vec);
    CHECK( (vec.buffer_pointer() == nullptr && vec.capacity() == 0 && vec.size() == 0) );
    CHECK( vec2 == vec1 );
}

template <class T>
void TestVectorDestructor(const std::initializer_list<T>& init) {
    Vector<T> vec(init);

    vec.~Vector();

    CHECK( vec.size() == 0 );
    CHECK( vec.capacity() == 0 );
    CHECK( vec.buffer_pointer() == nullptr );
}

template <class T>
void TestVectorAssignmentOperators(const std::initializer_list<T>& init) {
    Vector<T> vec1(init);

    Vector<T> vec2;
    Vector<T>& assignment_result = (vec2 = vec1);
    CHECK( &assignment_result == &vec2 );
    CHECK( vec2.buffer_pointer() != vec1.buffer_pointer() );
    CHECK( vec2 == vec1 );

    Vector<T> vec3;
    Vector<T>& assignment_result2 = (vec3 = std::move(vec1));
    CHECK( &assignment_result2 == &vec3 );
    CHECK( (vec1.buffer_pointer() == nullptr && vec1.size() == 0 && vec1.capacity() == 0) );
    CHECK( vec3 == vec2 );

    Vector<T>& assignment_result3 = (vec3 = vec3);
    CHECK( &assignment_result3 == &vec3 );
    CHECK( vec3 == vec2 );

    Vector<T>& assignment_result4 = (vec3 = std::move(vec3));
    CHECK( &assignment_result4 == &vec3 );
    CHECK( vec3 == vec2 );
}

template <class T>
void TestVectorEqualAndNotEqualOperators(const std::initializer_list<T>& init) {
    Vector<T> vec1(init);
    Vector<T> vec2(init);
    CHECK( vec1 == vec2 );
    CHECK( !(vec1 != vec2 ));

    vec1.push_back(T());
    CHECK( vec1 != vec2 );
    CHECK( !(vec1 == vec2 ));

    vec2.push_back(T());
    CHECK( vec1 == vec2 );
    CHECK( !(vec1 != vec2 ));
}

template <class T>
void TestVectorMethodsThatReturnIterators(const std::initializer_list<T>& init) {
    Vector<T> vec(init);

    Iterator<T> it1 = vec.begin();
    CHECK( it1.data_pointer() == vec.buffer_pointer() );

    Iterator<T> it2 = vec.end();
    CHECK( it2.data_pointer() == vec.buffer_pointer() + vec.size() );

    std::reverse_iterator<Iterator<T>> it3 = vec.rbegin();
    if (init.size() > 0)
        CHECK( it3.base().data_pointer() == vec.buffer_pointer() + vec.size() );
    else
        CHECK( (it3.base().data_pointer() == nullptr && vec.buffer_pointer() == nullptr) );

    std::reverse_iterator<Iterator<T>> it4 = vec.rend();
    if (init.size() > 0)
        CHECK( it4.base().data_pointer() == vec.buffer_pointer() );
    else
        CHECK( (it4.base().data_pointer() == nullptr && vec.buffer_pointer() == nullptr) );
}

template <class Vector, class T = typename Vector::value_type>
void TestVectorIndexingOperatorAndMethod(const std::initializer_list<T>& init) {
    using size_type = typename Vector::size_type;

    Vector vec(init);

    bool is_correct = true;
    auto it = init.begin();
    for (size_type i = 0; i < vec.size(); ++i, ++it)
    {
        if (vec[i] != *it || vec.at(i) != *it) {
            is_correct = false;
            break;
        }
    }
    CHECK( is_correct );
    CHECK_THROWS_AS( vec.at(vec.size()), std::out_of_range );
    CHECK_THROWS_AS( vec.at(vec.size() + 1), std::out_of_range );
}

template <class T>
void TestVectorPushAndPopBackMethods(const std::initializer_list<T>& init) {
    using size_type = typename Vector<T>::size_type;

    Vector<T> vec;
    Vector<T> vec2(init);

    auto it = init.begin();
    for (size_type i = 0; i < init.size(); ++i, ++it)
        vec.push_back(*it);
    CHECK( vec == vec2 );


    vec.push_back(std::move(T()));
    CHECK( vec[vec.size() - 1] ==  T() );

    vec.pop_back();
    CHECK( vec == vec2 );

    bool is_correct = true;
    while (vec.size() > 0) {
        vec.pop_back();
        it = init.begin();
        for (size_type i = 0; i < vec.size(); ++i, ++it)
            if (vec[i] != *it) {
                is_correct = false;
                break;
            }
    }
    CHECK( is_correct );
    CHECK_THROWS_AS( vec.pop_back(), std::underflow_error );

    Vector<DummyClass<int>> vec3;
    DummyClass<int> dummy_value {20};
    vec3.push_back(std::move(dummy_value));
    CHECK( dummy_value.is_empty() );
    CHECK( vec3[vec3.size() - 1].moves_count() == 1 );
    CHECK( vec3[vec3.size() - 1].value() == 20 );
    vec3.push_back(DummyClass<int> {30});
    CHECK( vec3[vec3.size() - 1].moves_count() == 1 );
    CHECK( vec3[vec3.size() - 1].value() == 30 );
}

template <class T>
void TestVectorSizeEmptyAndCapacityMethods(const std::initializer_list<T>& init) {
    using size_type = typename Vector<T>::size_type;

    Vector<T> vec;
    CHECK( vec.size() == 0 );
    CHECK( vec.empty() );
    CHECK( vec.capacity() == 0);

    bool is_correct = true;
    auto it = init.begin();
    for (size_type i = 0; i < init.size(); ++i, ++it) {
        vec.push_back(*it);
        if ( vec.size() != i + 1 || vec.empty() || vec.capacity() < i + 1 ) {
            is_correct = false;
            break;
        }
    }
    CHECK ( is_correct );
    CHECK( vec.size() == init.size() );
    CHECK( vec.empty() == (init.size() == 0) );
    CHECK( vec.capacity() >= init.size() );
}

template <class T>
void TestVectorResizeMethods(const T& value) {
    using size_type = typename Vector<T>::size_type;

    std::size_t size = 5;
    Vector<T> vec;
    CHECK( vec.size() == 0 );
    CHECK( vec.empty() );
    CHECK( vec.capacity() == 0);

    vec.resize(size);
    CHECK( vec.size() == size );
    CHECK( !vec.empty() );
    CHECK( vec.capacity() == size );

    T default_value = T();
    bool is_correct = true;
    for (size_type i = 0; i < size; ++i)
        if (vec[i] != default_value) {
            is_correct = false;
            break;
        }
    CHECK( is_correct );

    vec.resize(2 * size, value);
    is_correct = true;
    for (size_type i = 0; i < size; ++i)
        if (vec[i] != default_value) {
            is_correct = false;
            break;
        }
    CHECK( is_correct );
    for (size_type i = size; i < 2 * size; ++i)
        if (vec[i] != value) {
            is_correct = false;
            break;
        }
    CHECK( is_correct );
}

template <class T>
void TestVectorReserveMethod() {
    std::size_t size = 5;
    Vector<T> vec;
    CHECK( vec.size() == 0 );
    CHECK( vec.empty() );
    CHECK( vec.capacity() == 0);

    vec.reserve(size);
    CHECK( vec.size() == 0 );
    CHECK( vec.empty() );
    CHECK( vec.capacity() == size );

    vec.reserve(1);
    CHECK( vec.size() == 0 );
    CHECK( vec.empty() );
    CHECK( vec.capacity() == size );

    vec.reserve(2 * size);
    CHECK( vec.size() == 0 );
    CHECK( vec.empty() );
    CHECK( vec.capacity() == 2 * size );
}

template <class T>
void TestVectorClearMethod(const std::initializer_list<T>& init) {
    using size_type = typename Vector<T>::size_type;

    Vector<T> vec(init);
    CHECK( vec.size() == init.size() );
    CHECK( vec.empty() == (init.size() == 0) );
    CHECK( vec.capacity() >= init.size() );

    vec.clear();
    CHECK( vec.size() == 0 );
    CHECK( vec.empty() );
    CHECK( vec.capacity() >= init.size() );
}

TEST_CASE( "Test class Allocator" ) {

    SECTION( "Test traits of class Allocator" ) {
        using T = int;
        CHECK( std::is_same<Allocator<T>::value_type, T>::value );
        CHECK( std::is_same<Allocator<T>::pointer, T*>::value );
        CHECK( std::is_same<Allocator<T>::const_pointer, const T*>::value );
        CHECK( std::is_same<Allocator<T>::reference, T&>::value );
        CHECK( std::is_same<Allocator<T>::const_reference, const T&>::value );
        CHECK( std::is_same<Allocator<T>::size_type, std::size_t>::value );
        CHECK( std::is_same<Allocator<T>::difference_type, std::ptrdiff_t>::value );
        CHECK( std::is_same<Allocator<T>::propagate_on_container_move_assignment, std::true_type>::value );
        CHECK( std::is_same<Allocator<T>::is_always_equal, std::true_type>::value );
    }

    SECTION( "Test allocation and deallocation methods of class Allocator" ) {
        Allocator<std::size_t> alloc;
        const std::size_t size = 5;
        std::size_t* pointer;
        CHECK_NOTHROW( pointer = alloc.allocate(size) );
        CHECK( pointer != nullptr );

        for (std::size_t i = 0; i < size; ++i)
            pointer[i] = i;
        bool is_correct = true;
        for (std::size_t i = 0; i < size; ++i)
            if (pointer[i] != i) {
                is_correct = false;
                break;
            }
        CHECK( is_correct );
        CHECK_NOTHROW( alloc.deallocate(pointer, size) );
    }

    SECTION( "Test construct and destroy methods of class Allocator ") {
        Allocator<std::size_t> alloc;
        const std::size_t size = 5;
        std::size_t* pointer = alloc.allocate(size);
        for (std::size_t i = 0; i < size; ++i)
            alloc.construct(pointer + i, i);
        bool is_correct = true;
        for (std::size_t i = 0; i < size; ++i)
            if (pointer[i] != i) {
                is_correct = false;
                break;
            }
        CHECK( is_correct );
        for (std::size_t i = 0; i < size; ++i)
            alloc.destroy(pointer + i);
        alloc.deallocate(pointer, size);
    }

    SECTION( "Test max_size value of class Allocator" ) {
        Allocator<int> alloc_int;
        Allocator<double> alloc_double;
        CHECK( (alloc_int.max_size() == std::numeric_limits<std::size_t>::max() / sizeof(int)) );
        CHECK( (alloc_double.max_size() == std::numeric_limits<std::size_t>::max() / sizeof(double)) );
    }

    SECTION( "Test == and != operators of class Allocator") {
        Allocator<int> x;
        Allocator<double> y;
        CHECK( x == y );
        CHECK( !(x != y) );
    }
}

TEST_CASE( "Test class Iterator" ) {

    SECTION( "Test traits of class Iterator" ) {
        using T = int;
        CHECK( std::is_same<Iterator<T>::iterator_category, std::random_access_iterator_tag>::value );
        CHECK( std::is_same<Iterator<T>::value_type, T>::value );
        CHECK( std::is_same<Iterator<T>::reference, T&>::value );
        CHECK( std::is_same<Iterator<T>::const_reference, const T&>::value );
        CHECK( std::is_same<Iterator<T>::pointer, T*>::value );
        CHECK( std::is_same<Iterator<T>::const_pointer, const T*>::value );
        CHECK( std::is_same<Iterator<T>::size_type, std::size_t>::value );
        CHECK( std::is_same<Iterator<T>::difference_type, std::ptrdiff_t>::value );
    }

    SECTION( "Test constructors of class Iterator" ) {
        using T = int;
        using size_type = typename Iterator<T>::size_type;

        const size_type size = 1;
        T* buffer = new T[size];

        Iterator<T> it1(buffer);
        CHECK( it1.data_pointer() == buffer );

        Iterator<T> it2 = it1;
        CHECK( it2.data_pointer() == it1.data_pointer() );

        Iterator<T> it3 = std::move(it1);
        CHECK( (it3.data_pointer() == it2.data_pointer() && it1.data_pointer() == nullptr) );

        delete[] buffer;
    }

    SECTION( "Test assignment operators of class Iterator" ) {
        using T = int;
        using size_type = typename Iterator<T>::size_type;

        const size_type size = 5;
        T* buffer = new T[size];

        Iterator<T> it1(buffer);
        Iterator<T> it2(nullptr);
        it2 = it1;
        CHECK( it2.data_pointer() == it1.data_pointer() );

        Iterator<T> it3(nullptr);
        it3 = std::move(it1);
        CHECK( (it3.data_pointer() == it2.data_pointer() && it1.data_pointer() == nullptr) );

        Iterator<T> it4(buffer);
        it4 += 4;
        CHECK( it4.data_pointer() == buffer + 4 );

        it4 += -1;
        CHECK( it4.data_pointer() == buffer + 3 );

        it4 -= 3;
        CHECK( it4.data_pointer() == buffer );

        it4 -= -1;
        CHECK( it4.data_pointer() == buffer + 1 );

        delete[] buffer;
    }

    SECTION( "Test dereference operators of class Iterator" ) {
        using T = int;
        using size_type = typename Iterator<T>::size_type;

        const size_type size = 5;
        T* buffer = new T[size] {1, 2, 3, 4, 5};

        Iterator<T> it(buffer);
        CHECK( (*it == 1 && *(it + 1) == 2 && *(it + 2) == 3 && *(it + 3) == 4 && *(it + 4) == 5) );

        const Iterator<T> const_it(buffer);
        CHECK( (*const_it == 1 && *(const_it + 1) == 2 && *(const_it + 2) == 3 && *(const_it + 3) == 4 &&
            *(const_it + 4) == 5) );

        delete[] buffer;
    }

    SECTION( "Test -> operators of class Iterator" ) {
        TestArrowOperatorForIterator<Iterator<std::pair<int, char>>>();
        TestArrowOperatorForIterator<const Iterator<std::pair<int, char>>>();
    }

    SECTION( "Test increment and decrement operators of class Iterator" ) {
        using T = int;
        using size_type = typename Iterator<T>::size_type;

        const size_type size = 5;
        T* buffer = new T[size];

        Iterator<T> it1(buffer);
        Iterator<T> it2 = ++it1;
        CHECK( (it2.data_pointer() == buffer + 1 && it2.data_pointer() == it1.data_pointer()) );

        Iterator<T> it3 = it1++;
        CHECK( (it3.data_pointer() == buffer + 1 && it1.data_pointer() == buffer + 2) );

        Iterator<T> it4 = --it1;
        CHECK( (it4.data_pointer() == buffer + 1 && it4.data_pointer() == it1.data_pointer()) );

        Iterator<T> it5 = it1--;
        CHECK( (it5.data_pointer() == buffer + 1 && it1.data_pointer() == buffer) );

        delete[] buffer;
    }

    SECTION( "Test comparison operators of class Iterator" ) {
        using T = int;
        using size_type = typename Iterator<T>::size_type;

        const size_type size = 5;
        T* buffer = new T[size];

        Iterator<T> it0(buffer);
        Iterator<T> it1 = it0 + 1;
        Iterator<T> it2 = it0 + 2;
        Iterator<T> it3 = it0 + 3;
        Iterator<T> it4 = it3 + 1;

        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it0, it1) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it0, it2) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it0, it3) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it0, it4) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it1, it2) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it1, it3) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it1, it4) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it2, it3) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it2, it4) );
        CHECK( AreComparisonOperatorsCorrectForDifferentIterators(it3, it4) );

        it4 = it4 - 1;
        CHECK( AreComparisonOperatorsCorrectForEqualIterators(it3, it4) );

        it3 = it3 - 1;
        CHECK( AreComparisonOperatorsCorrectForEqualIterators(it2, it3) );

        delete[] buffer;
    }

    SECTION( "Test arithmetic operators + and - of class Iterator" ) {
        TestAdditionAndSubtractionOperatorsForIterator<Iterator<int>>();
        TestAdditionAndSubtractionOperatorsForIterator<Iterator<double>>();
        TestAdditionAndSubtractionOperatorsForIterator<const Iterator<int>>();
        TestAdditionAndSubtractionOperatorsForIterator<const Iterator<double>>();
    }

    SECTION( "Test indexing operators of class Iterator" ) {
        TestAIndexingOperatorsForIterator<Iterator<int>>({1, 2, 3, 4, 5});
        TestAIndexingOperatorsForIterator<const Iterator<int>>({1, 2, 3, 4, 5});
        TestAIndexingOperatorsForIterator<Iterator<char>>({'1', '2', '3', '4', '5'});
        TestAIndexingOperatorsForIterator<const Iterator<char>>({'1', '2', '3', '4', '5'});
    }

    SECTION( "Test swap method and function for class Iterator") {
        using T = int;
        using size_type = typename Iterator<T>::size_type;

        const size_type size = 5;
        T* buffer = new T[size];

        Iterator<T> it1(buffer);
        Iterator<T> it2(buffer + size - 1);

        swap(it1, it2);
        CHECK( (it1.data_pointer() == buffer + size - 1 &&  it2.data_pointer() == buffer) );

        it1.swap(it2);
        CHECK( (it1.data_pointer() == buffer &&  it2.data_pointer() == buffer + size - 1) );

        std::swap(it1, it2);
        CHECK( (it1.data_pointer() == buffer + size - 1 &&  it2.data_pointer() == buffer) );

        delete[] buffer;
    }

    SECTION( "Test data_pointer() for class Iterator") {
        using T = int;
        using size_type = typename Iterator<T>::size_type;

        const size_type size = 5;
        T* buffer = new T[size];

        Iterator<T> it1(buffer);
        Iterator<T> it2(buffer + size - 1);
        CHECK( (it1.data_pointer() == buffer &&  it2.data_pointer() == buffer + size - 1) );

        delete[] buffer;
    }

}

TEST_CASE( "Test class Vector" ) {

    SECTION("Test traits of class Vector") {
        using T = int;
        CHECK( std::is_same<Vector<T>::iterator, Iterator<T>>::value );
        CHECK( std::is_same<Vector<T>::reverse_iterator, std::reverse_iterator<Iterator<T>>>::value );
        CHECK( std::is_same<Vector<T>::size_type, std::size_t>::value );
        CHECK( std::is_same<Vector<T>::value_type, T>::value );
        CHECK( std::is_same<Vector<T>::reference, T&>::value );
        CHECK( std::is_same<Vector<T>::const_reference, const T&>::value );
        CHECK( std::is_same<Vector<T>::allocator_type, Allocator<T>>::value );
        CHECK( std::is_same<Vector<T>::pointer, T*>::value );
        CHECK( std::is_same<Vector<T>::const_pointer, const T*>::value );
    }

    SECTION("Test constructors of class Vector") {
        SECTION("Test size-based constructors of class Vector") {
            TestSizeBasedVectorConstructors<int>(0, 12345670);
            TestSizeBasedVectorConstructors<int>(10, 12345670);
            TestSizeBasedVectorConstructors<double>(20, -1.23456789);
        }

        SECTION("Test constructor for initializer lists of class Vector") {
            TestVectorConstructorForInitializerList<int>({});
            TestVectorConstructorForInitializerList({1, 2, 3, 4, -5, 6});
            TestVectorConstructorForInitializerList({"abc", "def", "pppp"});
        }

        SECTION("Test copy and move constructors of class Vector") {
            TestVectorCopyAndMoveConstructors<int>({});
            TestVectorCopyAndMoveConstructors({1, 2, 3, 4, -5, 6});
            TestVectorCopyAndMoveConstructors(std::initializer_list<std::string>{"abc", "def", "pppp"});
        }

        SECTION("Test destructor of class Vector") {
            TestVectorDestructor<int>({});
            TestVectorDestructor({1, 2, 3, 4, -5, 6});
            TestVectorDestructor<std::string>({"abc", "def", "pppp"});
        }

        SECTION("Test assignment operators of class Vector") {
            TestVectorAssignmentOperators<int>({});
            TestVectorAssignmentOperators({1, 2, 3, 4, -5, 6});
            TestVectorAssignmentOperators<std::string>({"abc", "def", "pppp"});
        }

        SECTION("Test == and != operators of class Vector") {
            TestVectorEqualAndNotEqualOperators<int>({});
            TestVectorEqualAndNotEqualOperators({1, 2, 3, 4, -5, 6});
            TestVectorEqualAndNotEqualOperators<std::string>({"abc", "def", "pppp"});
        }

        SECTION("Test methods of class Vector that return iterators") {
            TestVectorMethodsThatReturnIterators<int>({});
            TestVectorMethodsThatReturnIterators({1, 2, 3, 4, -5, 6});
            TestVectorMethodsThatReturnIterators<std::string>({"abc", "def", "pppp"});
        }

        SECTION("Test indexing operator and methods of class Vector") {
            TestVectorIndexingOperatorAndMethod<Vector<int>>({});
            TestVectorIndexingOperatorAndMethod<const Vector<int>>({});
            TestVectorIndexingOperatorAndMethod<Vector<int>>({1, 2, 3, 4, -5, 6});
            TestVectorIndexingOperatorAndMethod<const Vector<int>>({1, 2, 3, 4, -5, 6});
            TestVectorIndexingOperatorAndMethod<Vector<std::string>>(
                    std::initializer_list<std::string>{"abc", "def", "pppp"});
            TestVectorIndexingOperatorAndMethod<const Vector<std::string>>(
                    std::initializer_list<std::string>{"abc", "def", "pppp"});
        }

        SECTION("Test push_back and pop_back methods of class Vector") {
            TestVectorPushAndPopBackMethods<int>({});
            TestVectorPushAndPopBackMethods({1, 2, 3, 4, -5, 6});
            TestVectorPushAndPopBackMethods({"abc", "def", "pppp"});
        }

        SECTION("Test size, empty and capacity methods of class Vector") {
            TestVectorSizeEmptyAndCapacityMethods<int>({});
            TestVectorSizeEmptyAndCapacityMethods({1, 2, 3, 4, -5, 6});
            TestVectorSizeEmptyAndCapacityMethods<std::string>({"abc", "def", "pppp"});
        }

        SECTION("Test reserve method of class Vector") {
            TestVectorReserveMethod<int>();
            TestVectorReserveMethod<int>();
            TestVectorReserveMethod<double>();
        }

        SECTION("Test resize methods of class Vector") {
            TestVectorResizeMethods<int>(0);
            TestVectorResizeMethods<int>(-12345678);
            TestVectorResizeMethods<double>(-2123.5555112);
        }

        SECTION("Test clear method of class Vector") {
            TestVectorClearMethod<int>({});
            TestVectorClearMethod({1, 2, 3, 4, -5, 6});
            TestVectorClearMethod<std::string>({"abc", "def", "pppp"});
        }

        SECTION("STL based tests of class Vector") {
            Vector<int> vec = {1, 22, -4, -3, -4, 1, 0};

            std::sort(vec.begin(), vec.end());
            std::nth_element(vec.begin(), vec.begin() + 2, vec.end());
            CHECK( *(vec.begin() + 2) == -3 );
            CHECK( std::is_sorted(vec.begin(), vec.end()) );
            CHECK( vec == Vector<int>({-4, -4, -3, 0, 1, 1, 22}) );
            CHECK( !std::binary_search(vec.begin(), vec.end(), 7) );
            CHECK( std::lower_bound(vec.begin(), vec.end(), 7) == vec.end() - 1  );
            CHECK( std::adjacent_find(vec.begin(), vec.end()) == vec.begin() );
            CHECK( std::adjacent_find(vec.begin() + 1, vec.end()) == vec.end() - 3 );
            CHECK( std::any_of(vec.begin(), vec.end(), [](int x) { return x % 3 == 0; }) );
            CHECK( std::all_of(vec.begin(), vec.end(), [](int x) { return x < 100; }) );
            CHECK( std::none_of(vec.begin(), vec.end(), [](int x) { return x > 100; }) );
            CHECK( std::count(vec.begin(), vec.end(), 1) == 2 );
            CHECK( std::count_if(vec.begin(), vec.end(), [](int x) { return x % 2 == 0; }) == 4 );
            CHECK( std::equal(vec.begin(), vec.end(), Vector<int>({-4, -4, -3, 0, 1, 1, 22}).begin()) );
        }

    }

}

