#ifndef __LIGHT_TPL_RESULTH__
#define __LIGHT_TPL_RESULTH__

template <typename T>
struct Result
{
  bool ok;
  union
  {
    T result;
    const char *error;
  };

  inline bool isOk() { return ok; }
  inline bool isErr() { return !ok; }
};

#define ok(T, value) Result<T>{.ok = true, .result = value};
#define err(T, err) Result<T>{.ok = false, .error = err};

#endif
