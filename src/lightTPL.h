#ifndef __LIGHT_TPL_H__
#define __LIGHT_TPL_H__

#ifndef LTPL_MAX_VALUES
#define LTPL_MAX_VALUES 32
#endif

#ifndef LTPL_MAX_KEY_LNG
#define LTPL_MAX_KEY_LNG 32
#endif

#include <functional>
#include "lightTPL-result.h"

class LightTpl
{
private:
  const char *keys[LTPL_MAX_VALUES];
  bool owns[LTPL_MAX_VALUES];
  const char *values[LTPL_MAX_VALUES];
  int_fast16_t counter;
  const char *tpl;

public:
  LightTpl(const char *tpl);
  ~LightTpl();

  void setValue(const char *key, const char *value);
  void setValue(const char *key, int value);
  void setValue(const char *key, unsigned int value);
  void setValue(const char *key, short value);
  void setValue(const char *key, unsigned short value);
  void setValue(const char *key, float value, char dec);
  void setValue(const char *key, double value, char dec);
  void render(std::function<void(const char)> write);

private:
  Result<int_fast16_t> findKeys(const char *key, size_t lng);
};

#endif
