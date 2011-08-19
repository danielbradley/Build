#include <openocl/base/ErrorPSA.h>

#include <openocl/base/Exception.h>
#include <openocl/base/String.h>

#include <errno.h>

using namespace openocl::base;

void ErrorPSA::ThrowException() throw (Exception*)
{
  throw new Exception( new String( errno ) );
}
