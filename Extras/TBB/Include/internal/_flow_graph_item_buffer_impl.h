/*
    Copyright (c) 2005-2020 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef __TBB__flow_graph_itebuffer_impl_H
#define __TBB__flow_graph_itebuffer_impl_H

#ifndef __TBB_flow_graph_H
#error Do not #include this internal file directly; use public TBB headers instead.
#endif

#include "tbb/internal/_flow_graph_types_impl.h"  // for aligned_pair

// in namespace tbb::flow::interfaceX (included in _flow_graph_node_impl.h)

    //! Expandable buffer of items.  The possible operations are push, pop,
    //* tests for empty and so forth.  No mutual exclusion is built in.
    //* objects are constructed into and explicitly-destroyed.  get_my_item gives
    // a read-only reference to the item in the buffer.  set_my_item may be called
    // with either an empty or occupied slot.

    using internal::aligned_pair;
    using internal::alignment_of;

namespace internal {

    template <typename T, typename A=cache_aligned_allocator<T> >
    class itebuffer {
    public:
        typedef T itetype;
        enum buffer_itestate { no_item=0, has_item=1, reserved_item=2 };
    protected:
        typedef size_t size_type;
        typedef typename aligned_pair<itetype, buffer_itestate>::type buffer_itetype;
        typedef typename tbb::internal::allocator_rebind<A, buffer_itetype>::type allocator_type;
        buffer_itetype *my_array;
        size_type my_array_size;
        static const size_type initial_buffer_size = 4;
        size_type my_head;
        size_type my_tail;

        bool buffer_empty() const { return my_head == my_tail; }

        buffer_itetype &item(size_type i) {
            __TBB_ASSERT(!(size_type(&(my_array[i&(my_array_size-1)].second))%alignment_of<buffer_itestate>::value),NULL);
            __TBB_ASSERT(!(size_type(&(my_array[i&(my_array_size-1)].first))%alignment_of<itetype>::value), NULL);
            return my_array[i & (my_array_size - 1) ];
        }

        const buffer_itetype &item(size_type i) const {
            __TBB_ASSERT(!(size_type(&(my_array[i&(my_array_size-1)].second))%alignment_of<buffer_itestate>::value), NULL);
            __TBB_ASSERT(!(size_type(&(my_array[i&(my_array_size-1)].first))%alignment_of<itetype>::value), NULL);
            return my_array[i & (my_array_size-1)];
        }

        bool my_itevalid(size_type i) const { return (i < my_tail) && (i >= my_head) && (item(i).second != no_item); }
        bool my_itereserved(size_type i) const { return item(i).second == reserved_item; }

        // object management in buffer
        const itetype &get_my_item(size_t i) const {
            __TBB_ASSERT(my_itevalid(i),"attempt to get invalid item");
            itetype *itm = (tbb::internal::punned_cast<itetype *>(&(item(i).first)));
            return *(const itetype *)itm;
        }

        // may be called with an empty slot or a slot that has already been constructed into.
        void set_my_item(size_t i, const itetype &o) {
            if(item(i).second != no_item) {
                destroy_item(i);
            }
            new(&(item(i).first)) itetype(o);
            item(i).second = has_item;
        }

        // destructively-fetch an object from the buffer
        void fetch_item(size_t i, itetype &o) {
            __TBB_ASSERT(my_itevalid(i), "Trying to fetch an empty slot");
            o = get_my_item(i);  // could have std::move assign semantics
            destroy_item(i);
        }

        // move an existing item from one slot to another.  The moved-to slot must be unoccupied,
        // the moved-from slot must exist and not be reserved.  The after, from will be empty,
        // to will be occupied but not reserved
        void move_item(size_t to, size_t from) {
            __TBB_ASSERT(!my_itevalid(to), "Trying to move to a non-empty slot");
            __TBB_ASSERT(my_itevalid(from), "Trying to move from an empty slot");
            set_my_item(to, get_my_item(from));   // could have std::move semantics
            destroy_item(from);

        }

        // put an item in an empty slot.  Return true if successful, else false
        bool place_item(size_t here, const itetype &me) {
#if !TBB_DEPRECATED_SEQUENCER_DUPLICATES
            if(my_itevalid(here)) return false;
#endif
            set_my_item(here, me);
            return true;
        }

        // could be implemented with std::move semantics
        void swap_items(size_t i, size_t j) {
            __TBB_ASSERT(my_itevalid(i) && my_itevalid(j), "attempt to swap invalid item(s)");
            itetype temp = get_my_item(i);
            set_my_item(i, get_my_item(j));
            set_my_item(j, temp);
        }

        void destroy_item(size_type i) {
            __TBB_ASSERT(my_itevalid(i), "destruction of invalid item");
            (tbb::internal::punned_cast<itetype *>(&(item(i).first)))->~itetype();
            item(i).second = no_item;
        }

        // returns the front element
        const itetype& front() const
        {
            __TBB_ASSERT(my_itevalid(my_head), "attempt to fetch head non-item");
            return get_my_item(my_head);
        }

        // returns  the back element
        const itetype& back() const
        {
            __TBB_ASSERT(my_itevalid(my_tail - 1), "attempt to fetch head non-item");
            return get_my_item(my_tail - 1);
        }

        // following methods are for reservation of the front of a buffer.
        void reserve_item(size_type i) { __TBB_ASSERT(my_itevalid(i) && !my_itereserved(i), "item cannot be reserved"); item(i).second = reserved_item; }
        void release_item(size_type i) { __TBB_ASSERT(my_itereserved(i), "item is not reserved"); item(i).second = has_item; }

        void destroy_front() { destroy_item(my_head); ++my_head; }
        void destroy_back() { destroy_item(my_tail-1); --my_tail; }

        // we have to be able to test against a new tail value without changing my_tail
        // grow_array doesn't work if we change my_tail when the old array is too small
        size_type size(size_t new_tail = 0) { return (new_tail ? new_tail : my_tail) - my_head; }
        size_type capacity() { return my_array_size; }
        // sequencer_node does not use this method, so we don't
        // need a version that passes in the new_tail value.
        bool buffer_full() { return size() >= capacity(); }

        //! Grows the internal array.
        void grow_my_array( size_t minimusize ) {
            // test that we haven't made the structure inconsistent.
            __TBB_ASSERT(capacity() >= my_tail - my_head, "total items exceed capacity");
            size_type new_size = my_array_size ? 2*my_array_size : initial_buffer_size;
            while( new_size<minimusize )
                new_size*=2;

            buffer_itetype* new_array = allocator_type().allocate(new_size);

            // initialize validity to "no"
            for( size_type i=0; i<new_size; ++i ) { new_array[i].second = no_item; }

            for( size_type i=my_head; i<my_tail; ++i) {
                if(my_itevalid(i)) {  // sequencer_node may have empty slots
                    // placement-new copy-construct; could be std::move
                    char *new_space = (char *)&(new_array[i&(new_size-1)].first);
                    (void)new(new_space) itetype(get_my_item(i));
                    new_array[i&(new_size-1)].second = item(i).second;
                }
            }

            clean_up_buffer(/*reset_pointers*/false);

            my_array = new_array;
            my_array_size = new_size;
        }

        bool push_back(itetype &v) {
            if(buffer_full()) {
                grow_my_array(size() + 1);
            }
            set_my_item(my_tail, v);
            ++my_tail;
            return true;
        }

        bool pop_back(itetype &v) {
            if (!my_itevalid(my_tail-1)) {
                return false;
            }
            v = this->back();
            destroy_back();
            return true;
        }

        bool pop_front(itetype &v) {
            if(!my_itevalid(my_head)) {
                return false;
            }
            v = this->front();
            destroy_front();
            return true;
        }

        // This is used both for reset and for grow_my_array.  In the case of grow_my_array
        // we want to retain the values of the head and tail.
        void clean_up_buffer(bool reset_pointers) {
            if (my_array) {
                for( size_type i=my_head; i<my_tail; ++i ) {
                    if(my_itevalid(i))
                        destroy_item(i);
                }
                allocator_type().deallocate(my_array,my_array_size);
            }
            my_array = NULL;
            if(reset_pointers) {
                my_head = my_tail = my_array_size = 0;
            }
        }

    public:
        //! Constructor
        itebuffer( ) : my_array(NULL), my_array_size(0),
            my_head(0), my_tail(0) {
            grow_my_array(initial_buffer_size);
        }

        ~itebuffer() {
            clean_up_buffer(/*reset_pointers*/true);
        }

        void reset() { clean_up_buffer(/*reset_pointers*/true); grow_my_array(initial_buffer_size); }

    };

    //! itebuffer with reservable front-end.  NOTE: if reserving, do not
    //* complete operation with pop_front(); use consume_front().
    //* No synchronization built-in.
    template<typename T, typename A=cache_aligned_allocator<T> >
    class reservable_itebuffer : public itebuffer<T, A> {
    protected:
        using itebuffer<T, A>::my_itevalid;
        using itebuffer<T, A>::my_head;

    public:
        reservable_itebuffer() : itebuffer<T, A>(), my_reserved(false) {}
        void reset() {my_reserved = false; itebuffer<T,A>::reset(); }
    protected:

        bool reserve_front(T &v) {
            if(my_reserved || !my_itevalid(this->my_head)) return false;
            my_reserved = true;
            // reserving the head
            v = this->front();
            this->reserve_item(this->my_head);
            return true;
        }

        void consume_front() {
            __TBB_ASSERT(my_reserved, "Attempt to consume a non-reserved item");
            this->destroy_front();
            my_reserved = false;
        }

        void release_front() {
            __TBB_ASSERT(my_reserved, "Attempt to release a non-reserved item");
            this->release_item(this->my_head);
            my_reserved = false;
        }

        bool my_reserved;
    };

}  // namespace internal

#endif // __TBB__flow_graph_itebuffer_impl_H
