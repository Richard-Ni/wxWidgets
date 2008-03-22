/////////////////////////////////////////////////////////////////////////////
// Name:        vector.h
// Purpose:     interface of wxVector<T>
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @wxheader{vector.h}

    wxVector<T> is a template class which implements most of the @c std::vector
    class and can be used like it.

    If wxWidgets is compiled in STL mode, wxVector will just be a typedef to @c
    std::vector. Just like for @c std::vector, objects stored in wxVector<T>
    need to be @e assignable but don't have to be @e default constructible.

    Please refer to the STL documentation for further information.

    @nolibrary
    @category{containers}

    @see @ref overview_container, wxListT, wxArrayT
*/
template<typename T>
class wxVector<T>
{
public:
    typedef size_t size_type;
    typedef T value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef value_type& reference;

    /**
        Constructor.
    */
    wxVector();

    /**
        Copy onstructor.
    */
    wxVector(const wxVector<T>& c);

    /**
        Destructor.
    */
    ~wxVector();

    /**
        Returns item at position @e idx.
    */
    const value_type& at(size_type idx) const;

    /**
        Returns item at position @e idx.
    */
    value_type& at(size_type idx);

    /**
        Return the last item.
    */
    const value_type& back() const;

    /**
        Return the last item.
    */
    value_type& back();

    /**
        Return iterator to beginning of the vector.
    */
    const_iterator begin() const;

    /**
        Return iterator to beginning of the vector.
    */
    iterator begin();

    /**
        Returns vector's current capacity, i.e. how much memory is allocated.

        @see reserve()
    */
    size_type capacity() const;

    /**
        Clears the vector.
    */
    void clear();

    /**
        Returns @true if the vector is empty.
    */
    bool empty() const;

    /**
        Returns iterator to the end of the vector.
    */
    const_iterator end() const;

    /**
        Returns iterator to the end of the vector.
    */
    iterator end();

    /**
        Erase item pointed to by iterator @a it.

        @return Iterator pointing to the item immediately after the erased one.
    */
    iterator erase(iterator it);

    /**
        Erase items in the range @a first to @a last (@a last is not erased).

        @return Iterator pointing to the item immediately after the
                erased range.
    */
    iterator erase(iterator first, iterator last);

    /**
        Returns the first item.
    */
    const value_type& front() const;

    /**
        Returns the first item.
    */
    value_type& front();

    /**
        Insert item @a v at given position @a it.

        @return Iterator for the inserted item.
    */
    iterator insert(iterator it, const value_type& v = value_type());

    /**
        Assignment operator.
    */
    wxVectorT& operator operator=(const wxVector<T>& vb);

    /**
        Returns item at position @e idx.
    */
    const value_type& operator[](size_type idx) const;

    /**
        Returns item at position @e idx.
    */
    value_type& operator[](size_type idx);

    /**
        Removes the last item.
    */
    void pop_back();

    /**
        Adds an item to the end of the vector.
    */
    void push_back(const value_type& v);

    /**
        Reserves memory for at least @a n items.

        @see capacity()
    */
    void reserve(size_type n);
};

