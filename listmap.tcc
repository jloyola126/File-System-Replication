// $Id: listmap.tcc,v 1.11 2018-01-25 14:19:14-08 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   while(anchor_.next != anchor())
     erase(begin());
}

//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
    iterator iter;
    for(iter = begin(); (iter != end()) && (less(iter->first, pair.first)); ++iter);
    if (iter != end() && !(less(pair.first, iter->first))) {
        // Key found!!
        iter->second = pair.second;
        return iter;
    }
    // Key not found; make new using the new keyword...
    node* n = new node(iter.where, iter.where->prev, pair);
    iter.where->prev->next = n;
    iter.where->prev = n;
    return iterator(n); //return the iterator at the node...
   DEBUGF ('l', &pair << "->" << pair);
}
//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   iterator iter;
   for(iter = begin(); iter != end() ;++iter)
   {
     if (less(iter->first,that)) //if iter key is less than the key type, continue...
       continue;
     else if (!less(that,iter->first)) //key found!!
       return iter;
   }
   return end();

   DEBUGF ('l', that);
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   DEBUGF ('l', &*position);
   iterator iter;
   for(iter = begin(); iter != end() ;++iter)
   {
     if(iter == position) //If selected key is found... 
     {
       position.where->prev->next = position.where->next; //update position
       position.where->next->prev = position.where->prev;
       position.erase(); //erase at that position...
       break; //break and return...
      }
   }
   return iterator(position.where->next); //return iterator for the position's next value....'
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   DEBUGF ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   DEBUGF ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   DEBUGF ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   DEBUGF ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}
