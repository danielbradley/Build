#include <openapi/base/ErrorPSA.h>
#include <openapi/base/Exception.h>
#include <openapi/base/String.h>

#include <errno.h>

using namespace openapi::base;

void ErrorPSA::ThrowException() throw (Exception*)
{
  String* s = new String( errno );
  Exception* e = new Exception( s );
  s->destroy();
  throw e;
}
