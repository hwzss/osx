/* Test that -fvisibility does not override class member specific settings. */
/* { dg-do compile } */
/* { dg-require-visibility "" } */
/* { dg-options "-fvisibility=hidden" } */
/* { dg-final { scan-assembler "\\.internal.*Foo.methodEv" } } */

class __attribute__ ((visibility ("internal"))) Foo
{
  void method();
};

void Foo::method() { }
