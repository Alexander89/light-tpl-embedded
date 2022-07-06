# 🖨️ Simple template rendering engine

Simple, flexible and light weight template engine to replace {{tags}} with variables.

Very handy to render HTML Templates or console messages

## 🚀 HOW TO USE

```cpp
#include "lightTPL.h"

const char *tmp = "Hello!\n\
First test number: {{number}}\n\
Test a string {{string}}\n\
Test a string {{float}}\n\
This key doesn't exist {{key}}\n\
no key set {{}}\n\
open key {{this as a very long key and should not exist}}";

const char *value = "test";

void setup()
{
  Serial.begin(115200);
  delay(3000);

  LightTpl test(tmp);
  test.setValue("number", 1);
  test.setValue("string", value);
  test.setValue("float", 3.141592653589793f);

  test.render(
      [](char c)
      {
        Serial.write(c);
        delay(80);
      });
}
void loop(){}
```

## ⚙️ Config

Override by global defines or define it before you include the *"lightTPL.h"*

| define           | default | usage                               | hint                |
| ---------------- | ------- | ----------------------------------- | ------------------- |
| LTPL_MAX_VALUES  | 32      | Max number of keys in the templates | memory optimization |
| LTPL_MAX_KEY_LNG | 32      | Max allowed length of a key         | memory optimization |
