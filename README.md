# dominichess
2-D игра Domini Chess

## Принцип игры

После расстановки фигур на шахматной доске по очереди ходит игрок и AI. Во время хода каждый может переместить одну фигуру. Фигуры можно перемещать только на одну клетку (вверх, вниз, влево, вправо). В одну клетку можно поставить только одну фигуру. Побеждает тот, кто первый расставит все свои фигуры на место фигур противника. Расстановка фигур в начале игры показана на первом скриншоте.

## Скриншоты

![Снимок экрана (104)](https://user-images.githubusercontent.com/71713927/162629994-0b268444-4e2d-435f-9906-4c2dca617c23.png)
____
![Снимок экрана (105)](https://user-images.githubusercontent.com/71713927/162629995-1d5172e7-ed0a-4267-b165-d09e849a48e1.png)

## Для разработчиков

Основные классы для работы: 
- Сцена:
```cpp 
class Scene;
```
- Игровое поле:
```cpp 
class Map;
``` 
- Меню игры:
```cpp 
class Menu;
class Button;
```
- Клетка поля:
```cpp 
class Cell : public ObjectBase;
```
- Фигура:
```cpp 
class Figure : public ObjectBase;
```
- Фигура бота:
```cpp 
class BotFigure : public Figure;
```
