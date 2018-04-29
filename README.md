# CppSimpleKeyValuePropertyParser
Simple header only property file parser (key=value)

# Simple usage
### file.property
```
option1 = value
option2 = $(value)/value2
```

### C++ usage
```
PropertyParser property("file.property");
```
