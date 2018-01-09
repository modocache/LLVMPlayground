#include <iostream>

// The foo() method isn't declared or defined at this point. However,
// by using CRTP
// (https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern),
// I can define behavior that uses the foo() method.
template <typename T> struct FooOnDestroy {
  ~FooOnDestroy() { static_cast<T *>(this)->foo(); }
};

// A struct that defines a method foo(). This also inherits from FooOnDestroy,
// and so inherits the behavior that calls foo() in its destructor.
struct DoesFoo : public FooOnDestroy<DoesFoo> {
  void foo() const { std::cout << "Foo!" << std::endl; }
};

// A struct that does not define foo(), but inherits from CRTP class
// FooOnDestroy. Declaring such a class is not an error, but using it is --
// see the commented-out line in main below.
struct DoesNotDoFoo : public FooOnDestroy<DoesNotDoFoo> {
  void bar() const {}
};

// This struct causes a compilation error if Fooer::doIt is called for any
// type T which does not implement the foo() method. Not technically part of
// CRTP but this is the pattern that IntrusiveRefCntPtrInfo uses.
template <typename T> struct Fooer {
  static void doIt(const T &obj) { obj.foo(); }
};

// This struct calls foo() via the intermediate struct Fooer. This allows
// developers to specialize Fooer<T> for their type T, implementing logic
// that does more than simply call through to foo(). Not technically part of
// CRTP but this is the pattern that IntrusiveRefCntPtr uses.
template <typename T> struct Fooest {
  void doItEvenMore(const T &obj) { Fooer<T>::doIt(obj); }
};

template <> struct Fooer<int> {
  static void doIt(const int &obj) {
#pragma unused(obj)
    std::cout << "int doesn't implement foo(), but I can specialize Fooer<int> "
              << "and thus avoid the call, thereby allowing compilation to "
              << "succeed." << std::endl;
  }
};

int main() {
  Fooest<DoesFoo> F;
  DoesFoo D;
  F.doItEvenMore(D);

  Fooest<int> FI;
  FI.doItEvenMore(10);

  // Uncomment this line for a compilation error: no member named 'foo' in
  // 'DoesNotDoFoo'
  // DoesNotDoFoo DF;
  return 0;
}
