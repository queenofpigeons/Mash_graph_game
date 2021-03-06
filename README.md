# Mash_graph_game

## Зависимости
* cmake
* glfw

## Реализованные функции
* Базовая часть варианта А2
  * Дополнительно был реализован тайл сундука, которые можно открыть, встав перед ним и нажав кноку E. В отличие от дверей, закрыть сундук нельзя и игрок может свободно проходить по нему
  * Пустые тайлы были заменены на лаву для красоты
* Анимация при беге персонажа
  * Для лучшей читаемости персонаж меняет цвет, пока идет вниз
* Плавный переход к сообщению о переходе на другой уровень
* Диско эффект при звершении игры

## Написание уровней
* Уровни пишутся в файлах resources/lvl1.txt и resources/lvl2.txt
* Каждый уровень имеет формат 40 * 40 тайлов
* С края уровня должна располагаться либо стена, либо пустое пространство
* Тайлы в файле при отображении отражаются относительно оси OX
* Допускается только один тайл со стартовой позицией игрока; количество выходов с уровня не ограничено
* Список допустимых тайлов
  * пустое пространство: ‘ ‘ (пробел)
  * стена: ‘#’
  * пол: ‘.’
  * игрок: ‘@’
  * выход с уровня: ‘x’
  * сундук: 'С'
  * дверь: 'D'

## Запуск на macOS
Изначально игра писалась для macOS, поэтому тестирование проводилось на данной ОС. 
Для запуска игры на macOS необходимо заменить строку `<target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw rt dl)>` на строку `<target_link_libraries(main LINK_PUBLIC ${OPENGL_gl_LIBRARY} glfw dl)>` в файле CMakeLists.txt
В зависимости от монитора, окно с уровнем может быть в два раза больше, чем сам уровень.
