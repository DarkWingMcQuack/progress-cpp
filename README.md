Progress-CPP
===

A flexible ASCII progress bar for your console based C++ projects.
![](example/overview.gif)


### Usage
Progress is a header-only library and can be used by simply including the `ProgressBar.hpp` header file.

The bar takes the following options at initialization
- Limit: the total number of ticks that need to be completed
- Width: width of the bar
- Complete Char: the character to indicate completion (defaults to `=`)
- Incomplete Char: the character to indicate pending. (defaults to ' ')
- Arrow Char: the rightmost complete char. (defaults to `>`)

```c++
#include "ProgressBar.hpp"

int main() {

    const int limit = 10000;

    // initialize the bar
    ProgressBar progressBar(limit, 70);

    for (int i = 0; i < limit; i++) {
        // record the tick
        ++progressBar;

        // display the bar
        progressBar.display();
    }

    // tell the bar to finish
    progressBar.done();
}
```
The above code results in the following output

![](example/success.gif)


Instead of successfully being done, a `ProgessBar` can also fail
```c++
#include "ProgressBar.hpp"

int main() {

    const int limit = 10000;

    // initialize the bar
    ProgressBar progressBar(limit, 70);

    for (int i = 0; i < limit / 1.2; i++) {
        // record the tick
        ++progressBar;

        // display the bar
        progressBar.display();
    }

    // tell the bar to finish
    progressBar.failure();
}
```
The above code results in the following output

![](example/failure.gif)

The `ProgressBar::failure` can also be called with a reason of the failure, which then will be displayed.

### Performance
Everytime the `ProgressBar::display` method is called `std::cout << std::flush` is called, which can be quite slow.
To avoid this, use the `ProgressBar::displayIfChangedAtLeast` method:
```c++
auto displayIfChangedAtLeast(double percent) noexcept -> void;
```
This method can be used to avoid redisplaying and therefore flushing.
It expects a `double` which should be between 0 and 1.
This method only redisplays the bar if progress has increased at least by the given double since the
last time it was displayed.

### Example
Refer to [example.cpp](example/src/example.cpp) file for an example usage. To run it,

```
$ mkdir build && cd build
$ cmake ..
$ make
$ ./ProgressBar
```

Or without `cmake`
```
$ g++ -O3 -I. main.cpp -Wall -std=c++11 -o ProgressBar
$ ./ProgressBar
```

### CMake configuration
Cmake and project layout is inspired by [github.com/TheLartians/ModernCppStarter](https://github.com/TheLartians/ModernCppStarter).
