# Treap

## ???
Treap = tree + heap!

Реализация структуры данных "декартово дерево". Узлы дерева представляют собой пары "ключ - приоритет". При этом дерево является бинарным деревом поиска по ключам, и кучей по приоритетам. 

## Th!
При случайном наборе ключей получается в среднем достаточно хорошо балансированное дерево, то есть с логарифмическим временем операций поиска, удаления, вставки элементов. 

## Installation / Установка

Run ```install.sh``` as sudo/root (need copy header to /usr/include and create dir).

Запустите скрипт ```install.sh``` из-под рута или суперпользователя (для возможности создания директории в /usr/include/ и копирования туда).

# Interface

 Defined in header <treap.hpp>

```cpp
namespace bst { 
   template<typename T> class treap;
 }
 ```

## Member functions:

1. ``` bool find(T key) ``` -- поиск по ключу. Помеченные как удаленные при удалении ```remove``` элементы не ищутся этой функцией

2. ```bool was(T key)``` -- поиск по ключу. Находит и удалённые при помощи ```remove``` элементы

3. ```T upper_bound(T key) ``` -- поиск точной верхней границы по ключу (минимальный элемент в дереве, больший ключа)

4. ```T previous(T key)``` -- поиск максимального элемента, меньшего чем ключ

5. ```void insert(T key)``` -- вставка ключа в дерево

6. ```void erase(T key)``` -- физическое удаление по ключу из дерева (затираются ссылки и пр.)

7. ```bool remove(T key)``` -- удаление без фактического изъятия из структуры. Элемент помечается как удаленный и недоступен для процедур ```find```, ```upper_bound```, ```previuos```

8. ```bool get_success()``` -- позволяет узнать, корректно ли завершилась операция  ```upper_bound``` и ```previous``` -- был ли искомый элемент найден

9. ```size_t size()``` -- размер деревa

10. ```T k_max(T key)``` --- возвращает k-й максимум в дереве

# Usage 

```cpp
#include <my_dev/treap.hpp>
```
