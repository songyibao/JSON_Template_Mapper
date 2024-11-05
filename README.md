# 使用模板对 JSON 进行转换
> 基于 ```JSON Pointer``` [RFC 6901](https://datatracker.ietf.org/doc/html/rfc6901)  

## 转换示例

**源 json**
```json
{
  "name": "Alice",
  "details": {
    "age": 30,
    "city": "Wonderland"
  },
  "skills": [
    "Magic",
    "Swordsmanship"
  ]
}
```

**模板 json**

```json
{
  "user_name": "/name",
  "user_age": "/details/age",
  "location": {
    "city": "/details/city"
  },
  "abilities": "/skills"
}
```

**转换结果**

```json
{
  "user_name": "Alice",
  "user_age": 30,
  "location": {
    "city": "Wonderland"
  },
  "abilities": [
    "Magic",
    "Swordsmanship"
  ]
}
```

## 使用的开源库
感谢开源的 [YYJSON](https://github.com/ibireme/yyjson/tree/master) 库，这是一个快速且轻量级的JSON解析和生成库。


