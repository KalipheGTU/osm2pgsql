#ifndef OSMIUM_OSM_NODE_REF_LIST_HPP
#define OSMIUM_OSM_NODE_REF_LIST_HPP

/*

This file is part of Osmium (http://osmcode.org/libosmium).

Copyright 2013-2016 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <cassert>
#include <cstddef>
#include <iterator>

#include <osmium/memory/item.hpp>
#include <osmium/osm/item_type.hpp>
#include <osmium/osm/location.hpp>
#include <osmium/osm/node_ref.hpp>

namespace osmium {

    /**
     * An ordered collection of NodeRef objects. Usually this is not
     * instantiated directly, but one of its subclasses are used.
     */
    class NodeRefList : public osmium::memory::Item {

    public:

        using value_type             = NodeRef;
        using reference              = NodeRef&;
        using const_reference        = const NodeRef&;
        using iterator               = NodeRef*;
        using const_iterator         = const NodeRef*;
        using const_reverse_iterator = std::reverse_iterator<const NodeRef*>;
        using difference_type        = std::ptrdiff_t;
        using size_type              = std::size_t;

        explicit NodeRefList(osmium::item_type itemtype) noexcept :
            osmium::memory::Item(sizeof(NodeRefList), itemtype) {
        }

        /**
         * Checks whether the collection is empty.
         *
         * Complexity: Constant.
         */
        bool empty() const noexcept {
            return sizeof(NodeRefList) == byte_size();
        }

        /**
         * Returns the number of NodeRefs in the collection.
         *
         * Complexity: Constant.
         */
        size_type size() const noexcept {
            const auto size_node_refs = byte_size() - sizeof(NodeRefList);
            assert(size_node_refs % sizeof(NodeRef) == 0);
            return size_node_refs / sizeof(NodeRef);
        }

        /**
         * Access specified element.
         *
         * Complexity: Constant.
         *
         * @pre @code n < size() @endcode
         *
         * @param n Get the n-th element of the collection.
         */
        const NodeRef& operator[](size_type n) const noexcept {
            assert(n < size());
            const NodeRef* node_ref = &*(cbegin());
            return node_ref[n];
        }

        /**
         * Access the first element.
         *
         * Complexity: Constant.
         *
         * @pre @code !empty() @endcode
         */
        const NodeRef& front() const noexcept {
            assert(!empty());
            return operator[](0);
        }

        /**
         * Access the last element.
         *
         * Complexity: Constant.
         *
         * @pre @code !empty() @endcode
         */
        const NodeRef& back() const noexcept {
            assert(!empty());
            return operator[](size()-1);
        }

        /**
         * Checks whether the first and last node in the collection have the
         * same ID. The locations are not checked.
         *
         * Complexity: Constant.
         *
         * @pre @code !empty() @endcode
         */
        bool is_closed() const noexcept {
            return ends_have_same_id();
        }

        /**
         * Checks whether the first and last node in the collection have the
         * same ID. The locations are not checked.
         *
         * Complexity: Constant.
         *
         * @pre @code !empty() @endcode
         */
        bool ends_have_same_id() const noexcept {
            return front().ref() == back().ref();
        }

        /**
         * Checks whether the first and last node in the collection have the
         * same location. The IDs are not checked.
         *
         * Complexity: Constant.
         *
         * @pre @code !empty() @endcode
         * @pre @code front().location() && back().location() @endcode
         */
        bool ends_have_same_location() const {
            assert(front().location() && back().location());
            return front().location() == back().location();
        }

        /// Returns an iterator to the beginning.
        iterator begin() noexcept {
            return iterator(data() + sizeof(NodeRefList));
        }

        /// Returns an iterator to the end.
        iterator end() noexcept {
            return iterator(data() + byte_size());
        }

        /// Returns an iterator to the beginning.
        const_iterator cbegin() const noexcept {
            return const_iterator(data() + sizeof(NodeRefList));
        }

        /// Returns an iterator to the end.
        const_iterator cend() const noexcept {
            return const_iterator(data() + byte_size());
        }

        /// Returns an iterator to the beginning.
        const_iterator begin() const noexcept {
            return cbegin();
        }

        /// Returns an iterator to the end.
        const_iterator end() const noexcept {
            return cend();
        }

        /// Returns a reverse_iterator to the beginning.
        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(cend());
        }

        /// Returns a reverse_iterator to the end.
        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(cbegin());
        }

    }; // class NodeRefList

} // namespace osmium

#endif // OSMIUM_OSM_NODE_REF_LIST_HPP
