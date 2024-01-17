//#pragma once
//#include <exception>
//#include <iostream>
//#include <map>
//#include <memory>
//
//template <typename T>
//struct track_alloc : std::allocator<T> {
//  using Base = std::allocator<T>;
//  using pointer = std::allocator_traits<Base>::pointer;
//  using size_type = std::allocator_traits<Base>::size_type;
//
//  template <typename U>
//  struct rebind { typedef track_alloc<U> other; };
//
//  track_alloc() {}
//
//  template <typename U>
//  track_alloc(track_alloc<U> const& u) : std::allocator<T>(u) {}
//
//  pointer allocate(size_type size) {
//    void* p = std::malloc(size * sizeof(T));
//    if (p == nullptr) {
//      throw std::bad_alloc();
//    }
//    return static_cast<pointer>(p);
//  }
//
//  // todo for tolsta: make it use size_type properly
//  void deallocate(pointer p, size_type) { std::free(p); }
//};
//
//typedef std::map<void*, std::size_t, std::less<void*>,
//                 track_alloc<std::pair<void* const, std::size_t> > >
//    track_type;
//
//struct track_printer {
//  track_type* track;
//  track_printer(track_type* track) : track(track) {}
//  ~track_printer() {
//    track_type::const_iterator it = track->begin();
//    while (it != track->end()) {
//      std::cerr << "TRACK: leaked at " << it->first << ", " << it->second << " bytes\n";
//      ++it;
//    }
//  }
//};
//
//track_type* get_map() {
//  // don't use normal new to avoid infinite recursion.
//  static track_type* track = new (std::malloc(sizeof *track)) track_type;
//  static track_printer printer(track);
//  return track;
//}
//
//void* operator new(std::size_t size) noexcept(false) {
//  // we are required to return non-null
//  void* mem = std::malloc(size == 0 ? 1 : size);
//  if (mem == 0) {
//    throw std::bad_alloc();
//  }
//  (*get_map())[mem] = size;
//  return mem;
//}
//
//void operator delete(void* mem) noexcept {
//  if (get_map()->erase(mem) == 0) {
//    // this indicates a serious bug
//    std::cerr << "bug: memory at " << mem << " wasn't allocated by us\n";
//  }
//  std::free(mem);
//}